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

#include <cstddef>
#include <cstdint>
#include <string>

#include "image8880.h"
#include "point.h"

//-------------------------------------------------------------------------

namespace ogsfb32
{

//-------------------------------------------------------------------------

using FontPoint = Point<int16_t>;

//-------------------------------------------------------------------------

class RGB8880;

//-------------------------------------------------------------------------

constexpr int16_t sc_fontWidth{8};
constexpr int16_t sc_fontHeight{16};

//-------------------------------------------------------------------------

FontPoint
drawBattery(
    const Image8880Point& p,
    uint16_t percent,
    uint32_t rgb,
    Image8880& image);

FontPoint
drawChar(
    const Image8880Point& p,
    uint8_t c,
    const RGB8880& rgb,
    Image8880& image);

FontPoint
drawChar(
    const Image8880Point& p,
    uint8_t c,
    uint32_t rgb,
    Image8880& image);

FontPoint
drawString(
    const Image8880Point& p,
    const char* string,
    const RGB8880& rgb,
    Image8880& image);

FontPoint
drawString(
    const Image8880Point& p,
    const std::string& string,
    const RGB8880& rgb,
    Image8880& image);

//-------------------------------------------------------------------------

} // namespace ogsfb32

