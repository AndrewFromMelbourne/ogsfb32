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

#pragma once

//-------------------------------------------------------------------------

#include <cstdint>
#include <string>

#include "image8880Font.h"
#include "panel.h"
#include "rgb8880.h"

//-------------------------------------------------------------------------

namespace ogsfb32
{
class FrameBuffer8880;
}

//-------------------------------------------------------------------------

class DynamicInfo
:
    public Panel
{
public:

    DynamicInfo(int16_t width, int16_t yPosition);

    void update(time_t now) override;

private:

    ogsfb32::RGB8880 m_heading;
    ogsfb32::RGB8880 m_foreground;
    ogsfb32::RGB8880 m_warning;
    ogsfb32::RGB8880 m_background;

    static std::string getIpAddress(char& interface);
    void drawIpAddress(ogsfb32::FontPoint& position);

    static std::string getTemperature();
    void drawTemperature(ogsfb32::FontPoint& position);

    static std::string getTime(time_t now);
    void drawTime(ogsfb32::FontPoint& position, time_t now);

    void drawBatteryCharge(ogsfb32::FontPoint& position);
};

