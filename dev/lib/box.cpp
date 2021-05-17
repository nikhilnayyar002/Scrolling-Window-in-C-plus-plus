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
    enum Chars
    {
        borderHorizontalBottom = 223,
        borderHorizontalTop = 220,
        borderVertical = 219,
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

    const short SCROLL_THUMB_STEP_VALUE = 1, SCROLL_BUTTON_HEIGHT = 1, BORDER_WIDTH = 1, SCROLL_DOWN = 1, SCROLL_UP = 0;

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

        short padding;
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

        bool hasScrollBar;
        short scrollThumbHeight;
        float scrollThumbPos;
        short scrollBarHeight;
        float scrollThumbStepValue; // how much to move the scrollThumb (one at a time) up/down. Value should be in range: (0,1]

        // lines

        int noOfLines, linesNotRendered;

        // extra data members

        std::string renderedTitle;
        short actualOffsetLengthBeforeRenderedTitle;

        // initialize box data members

        void setDimension(short x1, short y1, short x2, short y2);
        void setRenderedTitle();

        // render methods

        void renderVerBorder(Position pc);
        void renderHorBorder(Chars bc);
        void renderBorders(int borderTxtColor, int borderBgColor);
        void resetOutput();
        void reRenderScrollbar();

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

        void setNoOfLines(int n); // should be private
        void scroll(int scrollDirection);

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

        horSize = x2 - x1 + BORDER_WIDTH;
        verSize = y2 - y1 + BORDER_WIDTH;

        // inner

        innerHorSize = x2 - x1 - BORDER_WIDTH;
        innerVerSize = y2 - y1 - BORDER_WIDTH;
        innerTopLeftCoord = {short(x1 + BORDER_WIDTH), short(y1 + BORDER_WIDTH)};
        innerBottomRightCoord = {short(x2 - BORDER_WIDTH), short(y2 - BORDER_WIDTH)};

        // padding
        {
            padding = BOX_INNER_PADDING;

            innerHorSizePadded = innerHorSize - padding * 2;
            innerVerSizePadded = innerVerSize - padding * 2;

            // check for inconsistencies
            if (!(innerHorSizePadded >= 1 && innerVerSizePadded >= 1))
                throw std::runtime_error("innerHorSizePadded, innerVerSizePadded should be >= 1.");

            innerTopLeftCoordPadded = {short(innerTopLeftCoord.X + padding), short(innerTopLeftCoord.Y + padding)};
            innerBottomRightCoordPadded = {short(innerBottomRightCoord.X - padding), short(innerBottomRightCoord.Y - padding)};
        }

        // title offset

        if (BOX_OFFSET_LENGTH_BEFORE_RENDERED_TITLE > innerHorSize)
            actualOffsetLengthBeforeRenderedTitle = innerHorSize;
        else
            actualOffsetLengthBeforeRenderedTitle = BOX_OFFSET_LENGTH_BEFORE_RENDERED_TITLE;

        // scrollbar
        {
            if (padding)
                scrollBarHeight = innerVerSizePadded;
            else
                scrollBarHeight = innerVerSizePadded - 2 * SCROLL_BUTTON_HEIGHT; // when there is no padding (>=1) the space for scroll Button Top and Bottom should be subtracted

            scrollThumbHeight = scrollThumbPos = 0;
            scrollThumbStepValue = SCROLL_THUMB_STEP_VALUE;
            hasScrollBar = false;
        }

        // lines

        noOfLines = linesNotRendered = 0;
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
        renderHorBorder(Chars::borderHorizontalTop);
        renderHorBorder(Chars::borderHorizontalBottom);
    }
    void Box::renderVerBorder(Position pc)
    {
        const short x = pc == Position::left ? x1 : x2;
        short y = y1;
        const unsigned char borderChar = Chars::borderVertical;

        if (pc == Position::right && hasScrollBar)
        {
            //  create vertical scroll output string

            int _scrollThumbPos = int(scrollThumbPos);

            std::string temp(SCROLL_BUTTON_HEIGHT, _scrollThumbPos == 0 ? Chars::borderVertical : Chars::scrollButtonTop);                                            // top botton
            temp += std::string(_scrollThumbPos, Chars::scrollBar);                                                                                                   // scrollBar without thumb
            temp += std::string(scrollThumbHeight, Chars::scrollBarThumb);                                                                                            // scrollBarThumb
            temp += std::string(innerVerSize - (temp.length() + SCROLL_BUTTON_HEIGHT), Chars::scrollBar);                                                             // scrollBar without thumb
            temp += std::string(SCROLL_BUTTON_HEIGHT, (_scrollThumbPos + scrollThumbHeight) == innerVerSizePadded ? Chars::borderVertical : Chars::scrollButtonDown); // bottom botton

            // render the scrollbar

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
    void Box::renderHorBorder(Chars bc)
    {
        winConio::gotoxy(x1, bc == Chars::borderHorizontalTop ? y1 : y2, hOut);
        const unsigned char borderChar = bc;

        if (bc == Chars::borderHorizontalBottom)
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
    void Box::reRenderScrollbar()
    {
        if (_hasFocus)
            winConio::setTextAndBackgroundColor(BOX_BORDER_HIGHLIGHTED_COLOR, borderBgColor, hOut);
        else
            winConio::setTextAndBackgroundColor(borderTxtColor, borderBgColor, hOut);

        renderVerBorder(Position::right);
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

    void Box::setNoOfLines(int n)
    {
        noOfLines = n;

        // scrollbat functionality
        {
            if (noOfLines > innerVerSizePadded)
            {
                hasScrollBar = true;
                linesNotRendered = noOfLines - innerVerSizePadded; // > 0

                if (linesNotRendered < scrollBarHeight)
                {
                    scrollThumbHeight = scrollBarHeight - linesNotRendered;
                    scrollThumbStepValue = SCROLL_THUMB_STEP_VALUE;
                }
                else
                {
                    scrollThumbHeight = 1;
                    // remaining steps in scrollbar / linesNotRendered
                    scrollThumbStepValue = (scrollBarHeight - scrollThumbHeight) / linesNotRendered;
                }
            }
            else
                hasScrollBar = false;
        }

        reRenderScrollbar();
    }

    void Box::scroll(int scrollDirection)
    {
        int _scrollThumbPos = int(scrollThumbPos);

        if (scrollDirection == SCROLL_UP && _scrollThumbPos != 0)
            scrollThumbPos -= scrollThumbStepValue;
        else if (scrollDirection == SCROLL_DOWN && (_scrollThumbPos + scrollThumbHeight) != innerVerSizePadded)
            scrollThumbPos += scrollThumbStepValue;

        reRenderScrollbar();
    }
}