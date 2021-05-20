#pragma once

#include <bits/stdc++.h>
#include <windows.h>
#include <stdexcept>
#include "conio.cpp"
#include "lib.cpp"
#include <sstream>

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
    const short MAX_SCROLL_THUMB_UNIT_DISTANCE_TRAVERSAL = 1, SCROLL_BUTTON_HEIGHT = 1, BORDER_WIDTH = 1;

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

    const short MAX_BOX_DOTS_IN_RENDERED_TITLE = 2;

    class Box
    {
    protected:
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
        short backgroundColor, textColor;
        int innerHorSize, innerVerSize;
        COORD innerTopLeftCoord, innerBottomRightCoord;

        // inner box output region parameters (with padding)

        int innerHorSizePadded, innerVerSizePadded;
        COORD innerTopLeftCoordPadded, innerBottomRightCoordPadded;

        // default box border colors

        short borderTxtColor = winConio::WHITE;
        short borderBgColor = winConio::BLACK;

        // extra data members

        std::string renderedTitle;
        short actualOffsetLengthBeforeRenderedTitle;

        // initialize box data members

        void setDimension(short x1, short y1, short x2, short y2);
        void setRenderedTitle();

        // render methods

        virtual void renderVerBorder(lib::Position pc);
        virtual void renderHorBorder(lib::Chars bc);
        void renderBorders(int borderTxtColor, int borderBgColor);
        void resetOutput(); // clears the content inside the box

    public:
        Box(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        void setFocus(bool state);
        bool hasFocus() { return _hasFocus; }
    };

    class BoxWithScrollBar : public Box
    {
        // scrollbar

        bool hasScrollBar;
        short scrollThumbHeight;
        float scrollThumbPos;
        short scrollBarHeight;
        // how much to move the scrollThumb (at minimum) up/down.
        // Value should be in range: (0,1] bcz it is calculated as: (scrollBarHeight - scrollThumbHeight) / linesNotRendered
        float scrollThumbUnitDistanceTraversal;

        // lines

        int noOfLines, // the no of lines affect the scrollbar.
            linesNotRendered;

        // render methods

        void renderVerBorder(lib::Position pc);
        void reRenderScrollbar();

    public:
        BoxWithScrollBar(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        bool scroll(lib::Direction scrollDirection, int noOfLines);
        void setNoOfLines(int n);

        // friend classes to box

        friend class scrollWin::SwBase;
        friend class scrollWin::SwMain;
        friend class scrollWin::SwSelec;
    };
}

// definations

namespace box
{
    Box::Box(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : _hasFocus(false), hOut(hOut), backgroundColor(backgroundColor), textColor(textColor), title(title)
    {
        // if (!(backgroundColor >= 0 && backgroundColor < winConio::TOTAL_COLORS))
        //     throw std::runtime_error("Box::backgroundColor is not valid.");

        // if (!(textColor >= 0 && textColor < winConio::TOTAL_COLORS))
        //     throw std::runtime_error("Box::textColor is not valid.");

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
        int maxLengthOfRenderedTitle = innerHorSize - actualOffsetLengthBeforeRenderedTitle;

        // if length Of Title is small, then not display it, display it with dots or simply display it. For example
        // title = nikhil
        // max dots = 2 (>=0)
        //
        // first condition :
        //      1. maxLengthOfRenderedTitle = 0 ()
        //      # dont render
        //
        // second condition :
        //      1. maxLengthOfRenderedTitle = 2 (00)
        //      00
        //      ..
        //
        //      1. maxLengthOfRenderedTitle = 1 (0)
        //      0
        //      .
        //
        // third condition :
        //      1. maxLengthOfRenderedTitle = 3 (000)
        //      000
        //      n..
        //
        //      2. maxLengthOfRenderedTitle = 5 (00000)
        //      00000
        //      nik..
        //
        // fourth condition :
        //      1. maxLengthOfRenderedTitle = 6 (00000)
        //      000000
        //      nikhil
        //
        //      1. maxLengthOfRenderedTitle = 7 (00000)
        //      0000000
        //      nikhil
        if (!maxLengthOfRenderedTitle)
            renderedTitle = "";
        else if (maxLengthOfRenderedTitle <= MAX_BOX_DOTS_IN_RENDERED_TITLE)
            renderedTitle = std::string(maxLengthOfRenderedTitle, '.');
        else if (title.length() > maxLengthOfRenderedTitle)
        {
            std::string dots = std::string(MAX_BOX_DOTS_IN_RENDERED_TITLE, '.');
            renderedTitle = title.substr(0, maxLengthOfRenderedTitle - dots.length());
            renderedTitle += dots;
        }
        else
            renderedTitle = title;
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
    }

    void Box::resetOutput()
    {
        winConio::paintBackground(innerTopLeftCoord.X, innerTopLeftCoord.Y, innerBottomRightCoord.X, innerBottomRightCoord.Y, backgroundColor, hOut);
    }
    void Box::renderBorders(int borderTxtColor, int borderBgColor)
    {
        winConio::setTextAndBackgroundColor(borderTxtColor, borderBgColor, hOut);
        renderVerBorder(lib::Position::posLeft);
        renderVerBorder(lib::Position::posRight);
        renderHorBorder(lib::Chars::borderHorizontalTop);
        renderHorBorder(lib::Chars::borderHorizontalBottom);
    }
    void Box::renderVerBorder(lib::Position pc)
    {
        const short x = pc == lib::Position::posLeft ? x1 : x2;
        short y = y1;
        const unsigned char borderChar = lib::Chars::borderVertical;

        while (++y < y2)
        {
            winConio::gotoxy(x, y, hOut);
            std::cout << borderChar;
        }
    }
    void Box::renderHorBorder(lib::Chars bc)
    {
        winConio::gotoxy(x1, bc == lib::Chars::borderHorizontalTop ? y1 : y2, hOut);
        const unsigned char borderChar = bc;

        if (bc == lib::Chars::borderHorizontalBottom)
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

    void Box::setFocus(bool state)
    {
        _hasFocus = state;

        if (state)
            // focus the box
            renderBorders(BOX_BORDER_HIGHLIGHTED_COLOR, borderBgColor);
        else
            renderBorders(borderTxtColor, borderBgColor);
    }

    BoxWithScrollBar::BoxWithScrollBar(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : Box(x1, y1, x2, y2, title, backgroundColor, textColor, hOut)
    {
        // scrollbar
        {
            if (padding)
                scrollBarHeight = innerVerSizePadded;
            else
                scrollBarHeight = innerVerSizePadded - 2 * SCROLL_BUTTON_HEIGHT; // when there is no padding (>=1) the space for scroll Button Top and Bottom should be subtracted

            scrollThumbHeight = scrollThumbPos = 0;
            scrollThumbUnitDistanceTraversal = MAX_SCROLL_THUMB_UNIT_DISTANCE_TRAVERSAL;
            hasScrollBar = false;
        }

        // lines

        noOfLines = linesNotRendered = 0;
    }

    void BoxWithScrollBar::renderVerBorder(lib::Position pc)
    {
        if (pc == lib::Position::posRight && hasScrollBar)
        {
            const short x = x2;
            short y = y1;

            int _scrollThumbPos = int(scrollThumbPos);

            //  create vertical scroll output string

            std::string temp(SCROLL_BUTTON_HEIGHT, _scrollThumbPos == 0 ? lib::Chars::borderVertical : lib::Chars::scrollButtonTop);                                            // top botton
            temp += std::string(_scrollThumbPos, lib::Chars::scrollBar);                                                                                                        // scrollBar without thumb
            temp += std::string(scrollThumbHeight, lib::Chars::scrollBarThumb);                                                                                                 // scrollBarThumb
            temp += std::string(innerVerSize - (temp.length() + SCROLL_BUTTON_HEIGHT), lib::Chars::scrollBar);                                                                  // scrollBar without thumb
            temp += std::string(SCROLL_BUTTON_HEIGHT, (_scrollThumbPos + scrollThumbHeight) == innerVerSizePadded ? lib::Chars::borderVertical : lib::Chars::scrollButtonDown); // bottom botton

            // render the scrollbar

            int i = 0;
            while (++y < y2)
            {
                winConio::gotoxy(x, y, hOut);
                std::cout << temp[i++];
            }
        }
        else
            Box::renderVerBorder(pc);
    }

    void BoxWithScrollBar::reRenderScrollbar()
    {
        if (_hasFocus)
            winConio::setTextAndBackgroundColor(BOX_BORDER_HIGHLIGHTED_COLOR, borderBgColor, hOut);
        else
            winConio::setTextAndBackgroundColor(borderTxtColor, borderBgColor, hOut);

        renderVerBorder(lib::Position::posRight);
    }

    void BoxWithScrollBar::setNoOfLines(int n)
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
                    scrollThumbUnitDistanceTraversal = MAX_SCROLL_THUMB_UNIT_DISTANCE_TRAVERSAL;
                }
                else
                {
                    scrollThumbHeight = 1;
                    // remaining steps in scrollbar / linesNotRendered
                    scrollThumbUnitDistanceTraversal = (scrollBarHeight - scrollThumbHeight) / linesNotRendered;
                }
            }
            else
                hasScrollBar = false;
        }

        reRenderScrollbar();
    }

    bool BoxWithScrollBar::scroll(lib::Direction scrollDirection, int noOfLines = 1)
    {
        int _scrollThumbPos = int(scrollThumbPos);
        bool canScroll = false;

        if (scrollDirection == lib::Direction::dirUp && _scrollThumbPos != 0)
        {
            float val = scrollThumbUnitDistanceTraversal * noOfLines;
            scrollThumbPos -= val < 0 ? 0 : val;
            canScroll = true;
        }

        else if (scrollDirection == lib::Direction::dirDown && (_scrollThumbPos + scrollThumbHeight) != innerVerSizePadded)
        {
            float val = scrollThumbUnitDistanceTraversal * noOfLines;
            int _newScrollThumbPos = int(scrollThumbPos + val);

            if ((_newScrollThumbPos + scrollThumbHeight) > innerVerSizePadded)
                scrollThumbPos = innerVerSizePadded;
            else
                scrollThumbPos += val;

            canScroll = true;
        }

        if (canScroll)
            reRenderScrollbar();

        return canScroll;
    }
}