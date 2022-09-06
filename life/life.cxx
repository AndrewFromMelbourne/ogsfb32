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

#include <array>
#include <cstring>

#include "life.h"

//-------------------------------------------------------------------------

using namespace ogsfb32;

//-------------------------------------------------------------------------

Life::Life()
:
    m_display{DISPLAY_CELLS},
    m_populationColours{
        0x000000FF,
        0x000080FF,
        0x0000FFFF,
        0x0000FF80,
        0x0000FF00,
        0x0080FF00,
        0x00FFFF00,
        0x00FF8000,
        0x00FF0000
    },
    m_cellColours{
        0x00000000,
        0x00FFFFFF
    },
    m_cells(),
    m_cellsNext(),
    m_image(WIDTH, HEIGHT)
{
}

//-------------------------------------------------------------------------

void
Life::updateCell(
    int col,
    int row,
    int value)
{
    uint32_t left = (col == 0) ? WIDTH - 1 :  col - 1;
    uint32_t right = (col == WIDTH - 1) ?  0 : col + 1;
    uint32_t above = (row == 0) ? HEIGHT - 1 : row - 1;
    uint32_t below = (row == HEIGHT - 1) ? 0 : row + 1;

    m_cellsNext[left + (above * WIDTH)] += value;
    m_cellsNext[col + (above * WIDTH)] += value;
    m_cellsNext[right + (above * WIDTH)] += value;

    m_cellsNext[left + (row * WIDTH)] += value;
    m_cellsNext[right + (row * WIDTH)] += value;

    m_cellsNext[left + (below * WIDTH)] += value;
    m_cellsNext[col + (below * WIDTH)] += value;
    m_cellsNext[right + (below * WIDTH)] += value;
}

//-------------------------------------------------------------------------

void
Life::setCell(
    int col,
    int row)
{
    updateCell(col, row, 1);
    m_cellsNext[col + (row * WIDTH)] |= aliveCellMask;
}

//-------------------------------------------------------------------------

void
Life::clearCell(
    int col,
    int row)
{
    updateCell(col, row, -1);
    m_cellsNext[col + (row * WIDTH)] &= ~aliveCellMask;
}

//-------------------------------------------------------------------------

void
Life::iterate()
{
    for (int row = 0 ; row < HEIGHT ; ++row)
    {
        for (int col = 0 ; col < WIDTH ; ++col)
        {
            auto cell = m_cells[col + (row * WIDTH)];
            auto neighbours = cell & ~aliveCellMask;
            auto alive = cell & aliveCellMask;

            if (alive)
            {
                if ((neighbours != 2) && (neighbours != 3))
                {
                    clearCell(col, row);
                }
            }
            else
            {
                if (neighbours == 3)
                {
                    setCell(col, row);
                }
            }
        }
    }

    m_cells = m_cellsNext;
}

//-------------------------------------------------------------------------

void
Life::init()
{
    m_cells.fill(0);
    m_cellsNext.fill(0);

    for (int row = 0 ; row < HEIGHT ; ++row)
    {
        for (int col = 0 ; col < WIDTH ; ++col)
        {
            if (std::rand() > (RAND_MAX / 2))
            {
                setCell(col, row);
            }
        }
    }

    m_cells = m_cellsNext;
}

//-------------------------------------------------------------------------

void
Life::createGosperGliderGun()
{
    constexpr int x = 222;
    constexpr int y = 236;

    m_cells.fill(0);
    m_cellsNext.fill(0);

    setCell(x + 24, y + 0);

    setCell(x + 22, y + 1);
    setCell(x + 24, y + 1);

    setCell(x + 12, y + 2);
    setCell(x + 13, y + 2);
    setCell(x + 20, y + 2);
    setCell(x + 21, y + 2);
    setCell(x + 34, y + 2);
    setCell(x + 35, y + 2);

    setCell(x + 11, y + 3);
    setCell(x + 15, y + 3);
    setCell(x + 20, y + 3);
    setCell(x + 21, y + 3);
    setCell(x + 34, y + 3);
    setCell(x + 35, y + 3);

    setCell(x + 0, y + 4);
    setCell(x + 1, y + 4);
    setCell(x + 10, y + 4);
    setCell(x + 16, y + 4);
    setCell(x + 20, y + 4);
    setCell(x + 21, y + 4);

    setCell(x + 0, y + 5);
    setCell(x + 1, y + 5);
    setCell(x + 10, y + 5);
    setCell(x + 14, y + 5);
    setCell(x + 16, y + 5);
    setCell(x + 17, y + 5);
    setCell(x + 22, y + 5);
    setCell(x + 24, y + 5);

    setCell(x + 10, y + 6);
    setCell(x + 16, y + 6);
    setCell(x + 24, y + 6);

    setCell(x + 11, y + 7);
    setCell(x + 15, y + 7);

    setCell(x + 12, y + 8);
    setCell(x + 13, y + 8);

    m_cells = m_cellsNext;
}

//-------------------------------------------------------------------------

void
Life::createSimkinGliderGun()
{
    constexpr int x = 225;
    constexpr int y = 230;

    m_cells.fill(0);
    m_cellsNext.fill(0);

    setCell(x + 0, y + 0);
    setCell(x + 1, y + 0);
    setCell(x + 7, y + 0);
    setCell(x + 8, y + 0);

    setCell(x + 0, y + 1);
    setCell(x + 1, y + 1);
    setCell(x + 7, y + 1);
    setCell(x + 8, y + 1);

    setCell(x + 4, y + 3);
    setCell(x + 5, y + 3);

    setCell(x + 4, y + 4);
    setCell(x + 5, y + 4);

    setCell(x + 22, y + 9);
    setCell(x + 23, y + 9);
    setCell(x + 25, y + 9);
    setCell(x + 26, y + 9);

    setCell(x + 21, y + 10);
    setCell(x + 27, y + 10);

    setCell(x + 21, y + 11);
    setCell(x + 28, y + 11);
    setCell(x + 31, y + 11);
    setCell(x + 32, y + 11);

    setCell(x + 21, y + 12);
    setCell(x + 22, y + 12);
    setCell(x + 23, y + 12);
    setCell(x + 27, y + 12);
    setCell(x + 31, y + 12);
    setCell(x + 32, y + 12);

    setCell(x + 26, y + 13);

    setCell(x + 20, y + 17);
    setCell(x + 21, y + 17);

    setCell(x + 20, y + 18);

    setCell(x + 21, y + 19);
    setCell(x + 22, y + 19);
    setCell(x + 23, y + 19);

    setCell(x + 23, y + 20);

    m_cells = m_cellsNext;
}

//-------------------------------------------------------------------------

void
Life::update(
    ogsfb32::Joystick& js)
{
    if (js.buttonPressed(Joystick::BUTTON_A))
    {
        if (m_display == DISPLAY_CELLS)
        {
            m_display = DISPLAY_POPULATION;
        }
        else
        {
            m_display = DISPLAY_CELLS;
        }
    }

    if (js.buttonPressed(Joystick::BUTTON_B))
    {
        init();
    }
    else if (js.buttonPressed(Joystick::BUTTON_X))
    {
        createGosperGliderGun();
    }
    else if (js.buttonPressed(Joystick::BUTTON_Y))
    {
        createSimkinGliderGun();
    }
    else
    {
        iterate();
    }
}

//-------------------------------------------------------------------------

void
Life::draw(
    ogsfb32::FrameBuffer8880& fb)
{
    for (int32_t i = 0 ; i < HEIGHT * WIDTH ; ++i)
    {
        auto cell = m_cells[i];
        auto neighbours = cell & ~aliveCellMask;
        auto state = (cell & aliveCellMask) >> aliveCellShift;

        ogsfb32::Image8880Point p{ int16_t(i % WIDTH), int16_t(i / WIDTH) };

        if (m_display == DISPLAY_CELLS)
        {
            m_image.setPixel(p, m_cellColours[state]);
        }
        else
        {
            m_image.setPixel(p, m_populationColours[neighbours]);
        }
    }

    fb.putImage(ogsfb32::FB8880Point(187, 0), m_image);
}

