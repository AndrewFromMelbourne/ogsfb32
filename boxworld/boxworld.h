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

#include "framebuffer8880.h"
#include "image8880.h"
#include "joystick.h"

#include "images.h"
#include "level.h"
#include "levels.h"

//-------------------------------------------------------------------------

class Boxworld
{
public:

    struct Location
    {
        int x;
        int y;
    };

    //---------------------------------------------------------------------

    enum Pieces
    {
        EMPTY = 0x00,
        PASSAGE = 0x01,
        BOX = 0x02,
        PLAYER = 0x03,
        WALL = 0x04,
        PASSAGE_WITH_TARGET = 0x05,
        BOX_ON_TARGET = 0x06,
        PLAYER_ON_TARGER = 0x07
    };

    //---------------------------------------------------------------------

    static constexpr uint8_t targetMask = 0x04;

    static constexpr int boardYoffset = 10;
    static constexpr int boardYend = boardYoffset + (tileHeight * Level::levelHeight);

    //---------------------------------------------------------------------

    Boxworld();

    void init();
    void update(ogsfb32::Joystick& js);
    void draw(ogsfb32::FrameBuffer8880& fb);

private:

    void findPlayer();
    void swapPieces(const Location& location1, const Location& location2);
    void isLevelSolved();
    void drawBoard(ogsfb32::FrameBuffer8880& fb);
    void drawText(ogsfb32::FrameBuffer8880& fb);

    //---------------------------------------------------------------------

    int m_level;
    bool m_levelSolved;
    bool m_canUndo;

    Location m_player;
    Level::LevelType m_board;
    Level::LevelType m_boardPrevious;
    const Levels m_levels;

    std::array<ogsfb32::Image8880, tileCount> m_tileBuffers;
    ogsfb32::Image8880 m_topTextImage;
    ogsfb32::Image8880 m_bottomTextImage;

    ogsfb32::RGB8880 m_textRGB;
    ogsfb32::RGB8880 m_boldRGB;
    ogsfb32::RGB8880 m_disabledRGB;
    ogsfb32::RGB8880 m_solvedRGB;
    ogsfb32::RGB8880 m_backgroundRGB;
};

