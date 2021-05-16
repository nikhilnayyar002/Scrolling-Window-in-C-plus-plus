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

// forward declaration to support in future scrollWin classes as friend classes to class Box
// (in short) Support to declaration:  friend class scrollWin::X;
namespace scrollWin
{
    class SwBase;
    class SwMain;
    class SwSelec;
}

namespace box
{
    const short BOX_HOR_BORDER_DOWN = 223;
    const short BOX_HOR_BORDER_UP = 220;
    const short BOX_VER_BORDER = 219;

    const short BOX_HIGHLIGHTED_COLOR = winConio::YELLOW;

    // let offset character be #
    // let border character by 0
    // let title = nikhil
    //
    // 1. offset length = 2 (>=0)
    // 0##nikhil000
    // 0          0
    // 000000000000
    //
    // 1. offset length = 1
    // 0#nikhil0000
    // 0          0
    // 000000000000
    const short BOX_OFFSET_LENGTH_BEFORE_RENDERED_TITLE = 1;

    // if length Of Rendered Title is too small, then set max dots to display. For example
    // title = nikhil
    // max dots = 2 (>=0)
    //
    // 1. box hor size = 00000
    // 00000
    // nik..
    //
    // 2. box hor size = 000
    // 000
    // n..
    //
    // 3. box hor size = 00
    // 00
    // ..
    //
    // 3. box hor size = 0
    // 0
    // .
    const short BOX_MAX_DOTS_IN_RENDERED_TITLE = 2;

    class Box
    {
        // hold the console buffer screen instance, where box should be rendered
        HANDLE hOut;

        // original title of the box
        std::string title;

        // backgound and text color
        short bgColor, txtColor;

        // outer (boundary) parameters
        short y1, y2, x1, x2;
        int horSize, verSize;

        // is current box in focus
        bool _hasFocus;

        // inner box output region parameters
        int innerHorSize, innerVerSize;
        COORD innerTopLeftCoord, innerBottomRightCoord;

        // extra data members

        std::string renderedTitle;
        short actualOffsetLengthBeforeRenderedTitle;

        // initialize box data members
        void setDimension(short x1, short y1, short x2, short y2);
        void setRenderedTitle();

        // draw methods
        void drawVer(int y1, int y2, int x1);
        void drawHor(int y1, int x1, int x2, int p);
        void draw();

    public:
        Box(short x1, short y1, short x2, short y2, std::string title, short bgColor, short txtColor, HANDLE hOut)
            : _hasFocus(false), hOut(hOut), bgColor(bgColor), txtColor(txtColor), title(title)
        {
            if (!(bgColor >= 0 && bgColor < winConio::TOTAL_COLORS))
                throw std::runtime_error("Box::bgColor is not valid.");

            if (!(txtColor >= 0 && txtColor < winConio::TOTAL_COLORS))
                throw std::runtime_error("Box::txtColor is not valid.");

            // check for dimensions inconsistencies
            if (!(x1 >= 0 && y1 >= 0 && x2 > x1 && y2 > y1))
                throw std::runtime_error("Box dimensions are not correct. Make sure x1 >= 0 && y1 >= 0 && x2 > x1 && y2 > y1.");

            setDimension(x1, y1, x2, y2);
            setRenderedTitle();
            draw();
        }

        // methods to get inner box parameters

        int getInnerHorSize() { return innerHorSize; }
        int getInnerVerSize() { return innerVerSize; }

        COORD getInnerTopLeftCoord() { return innerTopLeftCoord; }
        COORD getInnerBottomRightCoord() { return innerBottomRightCoord; }

        // focus methods
        void setFocus(bool);
        bool hasFocus() { return _hasFocus; }

        // friend classes to box

        friend class scrollWin::SwBase;
        friend class scrollWin::SwMain;
        friend class scrollWin::SwSelec;
    };

    // initialize box data members ********************************************************************

    void Box::setRenderedTitle()
    {
        int lengthOfRenderedTitle = innerHorSize - actualOffsetLengthBeforeRenderedTitle;

        // dont set the title since box is too small
        if (!lengthOfRenderedTitle)
            renderedTitle = "";
        // if lengthOfRenderedTitle is too small, then set max
        else if (lengthOfRenderedTitle <= BOX_MAX_DOTS_IN_RENDERED_TITLE)
            renderedTitle = std::string(lengthOfRenderedTitle, '.');
        else
        {
            // 00000
            // nik..

            std::string dots = std::string(BOX_MAX_DOTS_IN_RENDERED_TITLE, '.');
            renderedTitle = title.substr(0, lengthOfRenderedTitle - dots.length());
            renderedTitle += dots;
        }
    }

    void Box::setDimension(short x1, short y1, short x2, short y2)
    {
        Box::y1 = y1;
        Box::x1 = x1;
        Box::x2 = x2;
        Box::y2 = y2;

        horSize = x2 - x1 + 1;
        verSize = y2 - y1 + 1;

        innerHorSize = x2 - x1 - 1;
        innerVerSize = y2 - y1 - 1;

        innerTopLeftCoord = {short(x1 + 1), short(y1 + 1)};
        innerBottomRightCoord = {short(x2 - 1), short(y2 - 1)};

        if (BOX_OFFSET_LENGTH_BEFORE_RENDERED_TITLE > innerHorSize)
            actualOffsetLengthBeforeRenderedTitle = innerHorSize;
        else
            actualOffsetLengthBeforeRenderedTitle = BOX_OFFSET_LENGTH_BEFORE_RENDERED_TITLE;
    }

    // draw methods ********************************************************************

    void Box::draw()
    {
        drawVer(y1, y2, x1);
        drawVer(y1, y2, x2);
        drawHor(y1, x1, x2, BOX_HOR_BORDER_UP);
        drawHor(y2, x1, x2, BOX_HOR_BORDER_DOWN);

        winConio::paintBackground(x1 + 1, y1 + 1, x2 - 1, y2 - 1, bgColor, hOut);
    }
    void Box::drawVer(int y1, int y2, int x1)
    {
        while (++y1 < y2)
        {
            winConio::gotoxy(x1, y1, hOut);
            std::cout << char(BOX_VER_BORDER);
        }
    }
    void Box::drawHor(int y1, int x1, int x2, int p)
    {
        winConio::gotoxy(x1, y1, hOut);

        if (p == BOX_HOR_BORDER_DOWN)
            std::cout << std::string(horSize, BOX_HOR_BORDER_DOWN);
        else
        {
            // let offset character be #
            // let border character by 0
            // let title = nikhil
            // 0##nikhil000
            // 0          0
            // 000000000000

            // before rendered title
            std::string temp(1 + actualOffsetLengthBeforeRenderedTitle, BOX_HOR_BORDER_UP);
            // rendered title
            temp += renderedTitle;
            // after rendered title
            temp += std::string(horSize - temp.length(), BOX_HOR_BORDER_UP);

            // display whole horizontal border with rendered title
            std::cout << temp;
        }
    }

    // ********************************************************************

    void Box::setFocus(bool state = false)
    {
        _hasFocus = state;

        // focus the box
        if (state)
            // this will make the border color highlighted to illustrate that the box is in focus
            winConio::setTextColor(BOX_HIGHLIGHTED_COLOR, hOut);

        drawVer(y1, y2, x1);
        drawHor(y1, x1, x2, BOX_HOR_BORDER_UP);
        drawHor(y2, x1, x2, BOX_HOR_BORDER_DOWN);

        winConio::setTextColor(txtColor, hOut); // reset to default text color after painting the border
    }
}