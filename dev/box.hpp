#pragma once
/*******************************************************************************************************************************************
 * headers
 * *****************************************************************************************************************************************/
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "./utility.hpp"
#include "./conio.hpp"
#include <stdexcept>

namespace box
{
    const short BOX_HOR_BORDER_DOWN = 223;
    const short BOX_HOR_BORDER_UP = 220;
    const short BOX_VER_BORDER = 219;

    class Box
    {
        short y1, y2, x1, x2;
        std::string title;
        bool active;
        int hSize, vSize;

        void setDimension(short x1, short y1, short x2, short y2);
        void setTitle(std::string title);

        void drawVer(int y1, int y2, int x1);
        void drawHor(int y1, int x1, int x2, int p);
        void draw();

    public:
        Box(short x1, short y1, short x2, short y2, std::string title)
            : active(false)
        {
            if (x1 >= 0 && y1 >= 0 && x2 > x1 && y2 > y1)
            {
                setDimension(x1, y1, x2, y2);
                setTitle(title);
                draw();
            }
            else
                throw std::runtime_error("Box dimensions are not correct. Make sure x1 >= 0 && y1 >= 0 && x2 > x1 && y2 > y1.");
        }

        int getHorSize() { return hSize; }
        int verHorSize() { return vSize; }
        COORD getTopLeftInnerCoord();
        COORD getBottomRightInnerCoord();
        void setActiveState(bool);
        bool isactive() { return active; }
    };

    inline COORD Box::getTopLeftInnerCoord()
    {
        short x = x1 + 1, y = y1 + 1;
        return {x, y};
    }

    inline COORD Box::getBottomRightInnerCoord()
    {
        short x = x2 - 1, y = y2 - 1;
        return {x, y};
    }

    void Box::setTitle(std::string title)
    {
        short size = title.length();

        if (hSize <= 2) // 2 for  ".."
            title = "";
        else if (size && size > hSize)
            title = title.substr(0, hSize - 2) + "..";

        Box::title = title;
    }

    void Box::setDimension(short x1, short y1, short x2, short y2)
    {
        hSize = x2 - x1 - 2;
        vSize = y2 - y1 - 2;
        Box::y1 = y1;
        Box::x1 = x1;
        Box::x2 = x2;
        Box::y2 = y2;
    }

    void Box::draw()
    {
        drawVer(y1, y2, x1);
        drawVer(y1, y2, x2);
        drawHor(y1, x1, x2, BOX_HOR_BORDER_UP);
        drawHor(y2, x1, x2, BOX_HOR_BORDER_DOWN);

        winConio::paintBackground(x1 + 1, y1 + 1, x2 - 1, y2 - 1, winConio::BLUE);
    }

    void Box::drawVer(int y1, int y2, int x1)
    {
        while (++y1 < y2)
        {
            winConio::gotoxy(x1, y1);
            std::cout << char(BOX_VER_BORDER);
        }
    }
    void Box::drawHor(int y1, int x1, int x2, int p)
    {
        int width = x2 - x1 + 1;
        winConio::gotoxy(x1, y1);

        if (p == BOX_HOR_BORDER_DOWN)
        {
            std::cout << std::string(width, BOX_HOR_BORDER_DOWN);
        }
        else
        {
            const short offset = 2;
            std::string temp(offset, BOX_HOR_BORDER_UP);
            temp += title;
            std::cout << temp << std::string(width - offset - title.length(), BOX_HOR_BORDER_UP);
        }
    }
    void Box::setActiveState(bool state = false)
    {
        active = state;
        if (state)
            winConio::setTextColor(winConio::YELLOW);

        drawVer(y1, y2, x1);
        drawHor(y1, x1, x2, BOX_HOR_BORDER_UP);
        drawHor(y2, x1, x2, BOX_HOR_BORDER_DOWN);

        winConio::setTextColor(); //reset
    }
}