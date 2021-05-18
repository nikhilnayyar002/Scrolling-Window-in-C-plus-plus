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
        short backgroundColor, textColor;
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

        std::vector<std::string> lines;
        int noOfLines, linesNotRendered, topLinePos;

        // extra data members

        std::string renderedTitle;
        short actualOffsetLengthBeforeRenderedTitle;

        // initialize box data members

        void setDimension(short x1, short y1, short x2, short y2);
        void setRenderedTitle();

        // render methods

        void renderVerBorder(lib::Position pc);
        void renderHorBorder(Chars bc);
        void renderBorders(int borderTxtColor, int borderBgColor);
        void resetOutput();
        void reRenderScrollbar();
        void renderContent();

        // other private methods

        void setNoOfLines(int n);

    public:
        // output buffer
        std::ostringstream out;

        Box(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        // methods to get inner box parameters

        int getInnerHorSize() { return innerHorSize; }
        int getInnerVerSize() { return innerVerSize; }

        COORD getInnerTopLeftCoord() { return innerTopLeftCoord; }
        COORD getInnerBottomRightCoord() { return innerBottomRightCoord; }

        // focus methods

        void setFocus(bool);
        bool hasFocus() { return _hasFocus; }

        // other

        void scroll(int scrollDirection);
        void endLine();

        // friend classes to box

        friend class scrollWin::SwBase;
        friend class scrollWin::SwMain;
        friend class scrollWin::SwSelec;
    };

    Box::Box(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : _hasFocus(false), hOut(hOut), backgroundColor(backgroundColor), textColor(textColor), title(title), out(std::ostringstream::ate)
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
        else if (maxLengthOfRenderedTitle <= BOX_MAX_DOTS_IN_RENDERED_TITLE)
            renderedTitle = std::string(maxLengthOfRenderedTitle, '.');
        else if (title.length() > maxLengthOfRenderedTitle)
        {
            std::string dots = std::string(BOX_MAX_DOTS_IN_RENDERED_TITLE, '.');
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

        noOfLines = linesNotRendered = topLinePos = 0;
    }

    // render methods ********************************************************************
    void Box::resetOutput()
    {
        winConio::paintBackground(innerTopLeftCoord.X, innerTopLeftCoord.Y, innerBottomRightCoord.X, innerBottomRightCoord.Y, backgroundColor, hOut);
    }
    void Box::renderBorders(int borderTxtColor, int borderBgColor)
    {
        winConio::setTextAndBackgroundColor(borderTxtColor, borderBgColor, hOut);
        renderVerBorder(lib::Position::left);
        renderVerBorder(lib::Position::right);
        renderHorBorder(Chars::borderHorizontalTop);
        renderHorBorder(Chars::borderHorizontalBottom);
    }
    void Box::renderVerBorder(lib::Position pc)
    {
        const short x = pc == lib::Position::left ? x1 : x2;
        short y = y1;
        const unsigned char borderChar = Chars::borderVertical;

        if (pc == lib::Position::right && hasScrollBar)
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
                std::cout << temp[i++];
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

        renderVerBorder(lib::Position::right);
    }
    void Box::endLine()
    {
        const std::string str = out.str();
        // reset the buffer with set overload of ostringstream::str
        out.str("");
        // split the strings based on newlines ('\n') in it
        std::vector<std::string> newlines = lib::strSpit(str, "\n");

        // let \ = \n
        // string = nikhil\here it is bro
        //
        //      newlines
        //           nikhil
        //           here it is bro
        //
        //      inner box width padded = 5
        //
        //      parts (0 represent border, # represents space added to complete the line width equal to inner box width padded):
        //
        //      0nikhi0
        //      0l####0
        //      0here#0
        //      0it is0
        //      0bro##0
        //
        // string = \
        //
        //      newlines
        //           ''
        //
        for (auto &line : newlines)
        {
            // if line is less or same sized as inner padded width of box then push it as line. Also add space if necc.
            if (line.length() <= innerHorSizePadded)
                lines.push_back(line + std::string(innerHorSizePadded - line.length(), ' '));
            // divide the line into parts and push the parts as lines
            else
            {
                std::vector<std::string> parts = lib::strToEqualSizeParts(line, innerHorSizePadded);
                short lastPartPos = parts.size() - 1;

                // parts before 'last part'
                for (short i = 0; i < lastPartPos; ++i)
                    lines.push_back(parts[i]);

                // last part: its special bcz we may need to add space to it.
                
                std::string &lastPart = parts[lastPartPos];
                lines.push_back(lastPart + std::string(innerHorSizePadded - lastPart.length(), ' '));
            }
        }

        setNoOfLines(lines.size());
    }
    void Box::renderContent()
    {
        const short x = innerTopLeftCoordPadded.X;
        short y = innerTopLeftCoordPadded.Y;
        const short yBottom = innerBottomRightCoordPadded.Y;

        int pos = topLinePos, linesSize = lines.size();

        winConio::setTextAndBackgroundColor(textColor, backgroundColor, hOut);

        // render till y reaches yBottom and also lines are available to render
        while (y <= yBottom && pos != linesSize)
        {
            winConio::gotoxy(x, y, hOut);
            std::cout << lines[pos++];
            ++y;
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

        renderContent();
        reRenderScrollbar();
    }

    void Box::scroll(int scrollDirection)
    {
        int _scrollThumbPos = int(scrollThumbPos);

        if (scrollDirection == SCROLL_UP && _scrollThumbPos != 0)
        {
            scrollThumbPos -= scrollThumbStepValue;
            topLinePos -= SCROLL_THUMB_STEP_VALUE;
        }

        else if (scrollDirection == SCROLL_DOWN && (_scrollThumbPos + scrollThumbHeight) != innerVerSizePadded)
        {
            scrollThumbPos += scrollThumbStepValue;
            topLinePos += SCROLL_THUMB_STEP_VALUE;
        }

        renderContent();
        reRenderScrollbar();
    }
}