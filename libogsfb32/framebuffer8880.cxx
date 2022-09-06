//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2022 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>

#include "framebuffer8880.h"
#include "image8880.h"
#include "point.h"

//-------------------------------------------------------------------------

ogsfb32::FrameBuffer8880:: FrameBuffer8880(
    const std::string& device)
:
    m_finfo{},
    m_vinfo{},
    m_lineLengthPixels{0},
    m_fbp{nullptr}
{
    FileDescriptor fbfd{::open(device.c_str(), O_RDWR)};

    if (fbfd.fd() == -1)
    {
        throw std::system_error{errno,
                                std::system_category(), 
                                "cannot open framebuffer device " + device};
    }

    if (ioctl(fbfd.fd(), FBIOGET_FSCREENINFO, &(m_finfo)) == -1)
    {
        throw std::system_error{errno,
                                std::system_category(), 
                                "reading fixed framebuffer information"};
    }

    if (ioctl(fbfd.fd(), FBIOGET_VSCREENINFO, &(m_vinfo)) == -1)
    {
        throw std::system_error{errno,
                                std::system_category(), 
                                "reading variable framebuffer information"};
    }

    //---------------------------------------------------------------------

    m_lineLengthPixels = m_finfo.line_length / bytesPerPixel;

    //---------------------------------------------------------------------

    void* fbp = ::mmap(nullptr,
                       m_finfo.smem_len,
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED,
                       fbfd.fd(),
                       0);

    if (fbp == MAP_FAILED)
    {
        throw std::system_error(errno,
                                std::system_category(), 
                                "mapping framebuffer device to memory");
    }

    m_fbp = static_cast<uint32_t*>(fbp);
}

//-------------------------------------------------------------------------

ogsfb32::FrameBuffer8880:: ~FrameBuffer8880()
{
    ::munmap(m_fbp, m_finfo.smem_len);
}

//-------------------------------------------------------------------------

bool
ogsfb32::FrameBuffer8880:: cursor(int value)
{
    std::ofstream ofs("/sys/class/graphics/fbcon/cursor_blink");

    bool result = false;

    if (ofs.is_open())
    {
        ofs << value;
        result = true;
    }

    return result;
}

//-------------------------------------------------------------------------

void
ogsfb32::FrameBuffer8880:: clear(
    uint32_t rgb) const
{
    std::fill(m_fbp, m_fbp + (m_finfo.smem_len / bytesPerPixel), rgb);
}

//-------------------------------------------------------------------------

bool
ogsfb32::FrameBuffer8880:: setPixel(
    const FB8880Point& p,
    uint32_t rgb) const
{
    bool isValid{validPixel(p)};

    if (isValid)
    {
        m_fbp[offset(p)] = rgb;
    }

    return isValid;
}

//-------------------------------------------------------------------------

std::pair<bool, ogsfb32::RGB8880>
ogsfb32::FrameBuffer8880:: getPixelRGB(
    const FB8880Point& p) const
{
    bool isValid{validPixel(p)};
    RGB8880 rgb{0, 0, 0};

    if (isValid)
    {
        rgb.set8880(m_fbp[offset(p)]);
    }

    return std::make_pair(isValid, rgb);
}

//-------------------------------------------------------------------------

std::pair<bool, uint32_t>
ogsfb32::FrameBuffer8880:: getPixel(
    const FB8880Point& p) const
{
    bool isValid{validPixel(p)};
    uint32_t rgb{0};

    if (isValid)
    {
        rgb = m_fbp[offset(p)];
    }

    return std::make_pair(isValid, rgb);
}

//-------------------------------------------------------------------------

bool
ogsfb32::FrameBuffer8880:: putImage(
    const FB8880Point& p_left,
    const Image8880& image) const
{
    FB8880Point p{ getWidth() - image.getWidth() - p_left.x(), p_left.y() };

    if ((p.x() < 0) ||
        ((p.x() + image.getWidth()) > static_cast<int32_t>(m_vinfo.yres)))
    {
        return putImagePartial(p, image);
    }

    if ((p.y() < 0) ||
        ((p.y() + image.getHeight()) > static_cast<int32_t>(m_vinfo.xres)))
    {
        return putImagePartial(p, image);
    }

    for (int32_t i = 0 ; i < image.getWidth() ; ++i)
    {
        auto start = image.getColumn(i);

        std::copy(start,
                  start + image.getHeight(), 
                  m_fbp + ((i + p.x()) * m_lineLengthPixels) + p.y());
    }

    return true;
}

//-------------------------------------------------------------------------

bool
ogsfb32::FrameBuffer8880:: putImagePartial(
    const FB8880Point& p,
    const Image8880& image) const
{
    auto x = p.x();
    auto xStart = 0;
    auto xEnd = image.getWidth() - 1;

    auto y = p.y();
    auto yStart = 0;
    auto yEnd = image.getHeight() - 1;

    if (x < 0)
    {
        xStart = -x;
        x = 0;
    }

    if ((x - xStart + image.getWidth()) >
        static_cast<int32_t>(m_vinfo.yres))
    {
        xEnd = m_vinfo.yres - 1 - (x - xStart);
    }

    if (y < 0)
    {
        yStart = -y;
        y = 0;
    }

    if ((y - yStart + image.getHeight()) >
        static_cast<int32_t>(m_vinfo.xres))
    {
        yEnd = m_vinfo.xres - 1 - (y - yStart);
    }

    if ((xEnd - xStart) <= 0)
    {
        return false;
    }

    if ((yEnd - yStart) <= 0)
    {
        return false;
    }

    for (auto i = xStart ; i <= xEnd ; ++i)
    {
        auto start = image.getColumn(i) + yStart;

        std::copy(start,
                  start + (yEnd - yStart + 1),
                  m_fbp + ((i+x) * m_lineLengthPixels) + y);
    }

    return true;
}

//-------------------------------------------------------------------------

size_t
ogsfb32::FrameBuffer8880:: offset(
    const FB8880Point& p) const
{
    return p.y() + (m_vinfo.yres - 1 - p.x()) * m_lineLengthPixels;
}
