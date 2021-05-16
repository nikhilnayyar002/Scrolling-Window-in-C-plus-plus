#pragma once
/*******************************************************************************************************************************************
 * headers
 * *****************************************************************************************************************************************/
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <stdexcept>
#include "conio.cpp"

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
    enum BorderChars
    {
        horizontalBottom = 223,
        horizontalTop = 220,
        vertical = 219
    };

    enum ScrollBarChars
    {
        scrollBar = 177,
        scrollBarThumb = 178,
        scrollButtonTop = 30,
        scrollButtonDown = 31,
    };

    enum Position
    {
        top = 1,
        bottom = 2,
        left = 3,
        right = 4
    };

    const short BOX_BORDER_HIGHLIGHTED_COLOR = winConio::YELLOW;

    // padding inside box (>=0)
    //
    // 1. padding = 0
    // 000000000000
    // 0nik       0
    // 0          0
    // 0          0
    // 000000000000
    //
    // 2. padding = 1
    // 000000000000
    // 0          0
    // 0  nik     0
    // 0          0
    // 000000000000
    const short BOX_INNER_PADDING = 1;

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

        // outer (boundary) parameters

        short y1, y2, x1, x2;
        int horSize, verSize;

        // is current box in focus
        bool _hasFocus;

        // inner box output region parameters

        short bgColor, txtColor;
        int innerHorSize, innerVerSize;
        COORD innerTopLeftCoord, innerBottomRightCoord;

        // inner box output region parameters (with padding)

        int innerHorSizePadded, innerVerSizePadded;
        COORD innerTopLeftCoordPadded, innerBottomRightCoordPadded;

        // default box border colors

        short borderTxtColor = winConio::WHITE;
        short borderBgColor = winConio::BLACK;

        // scrollbar
        short scrollThumbWidth, scrollThumbPos;

        // extra data members

        std::string renderedTitle;
        short actualOffsetLengthBeforeRenderedTitle;

        // initialize box data members

        void setDimension(short x1, short y1, short x2, short y2);
        void setRenderedTitle();

        // render methods

        void renderVerBorder(Position pc);
        void renderHorBorder(BorderChars bc);
        void renderBorders(int borderTxtColor, int borderBgColor);
        void resetOutput();

    public:
        Box(short x1, short y1, short x2, short y2, std::string title, short bgColor, short txtColor, HANDLE hOut);

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

    Box::Box(short x1, short y1, short x2, short y2, std::string title, short bgColor, short txtColor, HANDLE hOut)
        : _hasFocus(false), hOut(hOut), bgColor(bgColor), txtColor(txtColor), title(title)
    {
        // if (!(bgColor >= 0 && bgColor < winConio::TOTAL_COLORS))
        //     throw std::runtime_error("Box::bgColor is not valid.");

        // if (!(txtColor >= 0 && txtColor < winConio::TOTAL_COLORS))
        //     throw std::runtime_error("Box::txtColor is not valid.");

        // check for dimensions inconsistencies
        if (!(x1 >= 0 && y1 >= 0 && x2 > x1 && y2 > y1))
            throw std::runtime_error("Box dimensions are not correct. Make sure x1 >= 0 && y1 >= 0 && x2 > x1 && y2 > y1.");

        setDimension(x1, y1, x2, y2);
        setRenderedTitle();
        renderBorders(borderTxtColor, borderBgColor);
        resetOutput();
    }

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

        innerHorSizePadded = innerHorSize - BOX_INNER_PADDING * 2;
        innerHorSizePadded = innerVerSize - BOX_INNER_PADDING * 2;
        innerTopLeftCoordPadded = {short(innerTopLeftCoord.X + BOX_INNER_PADDING), short(innerTopLeftCoord.Y + BOX_INNER_PADDING)};
        innerBottomRightCoordPadded = {short(innerBottomRightCoord.X - BOX_INNER_PADDING), short(innerBottomRightCoord.Y - BOX_INNER_PADDING)};

        if (BOX_OFFSET_LENGTH_BEFORE_RENDERED_TITLE > innerHorSize)
            actualOffsetLengthBeforeRenderedTitle = innerHorSize;
        else
            actualOffsetLengthBeforeRenderedTitle = BOX_OFFSET_LENGTH_BEFORE_RENDERED_TITLE;

        scrollThumbWidth = 2;
        scrollThumbPos = 3;
    }

    // render methods ********************************************************************
    void Box::resetOutput()
    {
        winConio::paintBackground(innerTopLeftCoord.X, innerTopLeftCoord.Y, innerBottomRightCoord.X, innerBottomRightCoord.Y, bgColor, hOut);
    }
    void Box::renderBorders(int borderTxtColor, int borderBgColor)
    {
        winConio::setTextAndBackgroundColor(borderTxtColor, borderBgColor, hOut);
        renderVerBorder(Position::left);
        renderVerBorder(Position::right);
        renderHorBorder(BorderChars::horizontalTop);
        renderHorBorder(BorderChars::horizontalBottom);
    }
    void Box::renderVerBorder(Position pc)
    {
        const short x = pc == Position::left ? x1 : x2;
        short y = y1;
        const unsigned char borderChar = BorderChars::vertical;

        if (pc == Position::right && true)
        {
            //  create vertical scroll output string
            std::string temp(1, ScrollBarChars::scrollButtonTop);
            temp += std::string(scrollThumbPos, ScrollBarChars::scrollBar);
            temp += std::string(scrollThumbWidth, ScrollBarChars::scrollBarThumb);
            temp += std::string(innerVerSize - temp.length() - 1, ScrollBarChars::scrollBar);
            temp += std::string(1, ScrollBarChars::scrollButtonDown);

            int i = 0;
            while (++y < y2)
            {
                winConio::gotoxy(x, y, hOut);
                std::cout << temp[i];
                ++i;
            }
        }
        else
            while (++y < y2)
            {
                winConio::gotoxy(x, y, hOut);
                std::cout << borderChar;
            }
    }
    void Box::renderHorBorder(BorderChars bc)
    {
        winConio::gotoxy(x1, bc == BorderChars::horizontalTop ? y1 : y2, hOut);
        const unsigned char borderChar = bc;

        if (bc == BorderChars::horizontalBottom)
            std::cout << std::string(horSize, borderChar);
        else
        {
            // let offset character be #
            // let border character by 0
            // let title = nikhil
            // 0##nikhil000
            // 0          0
            // 000000000000

            // before rendered title
            std::string temp(1 + actualOffsetLengthBeforeRenderedTitle, borderChar);
            // rendered title
            temp += renderedTitle;
            // after rendered title
            temp += std::string(horSize - temp.length(), borderChar);

            // display whole horizontal border with rendered title
            std::cout << temp;
        }
    }
    // ********************************************************************

    void Box::setFocus(bool state = false)
    {
        _hasFocus = state;

        if (state)
            // focus the box
            renderBorders(BOX_BORDER_HIGHLIGHTED_COLOR, borderBgColor);
        else
            renderBorders(borderTxtColor, borderBgColor);
    }
}