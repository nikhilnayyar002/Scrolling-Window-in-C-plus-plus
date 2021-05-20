#pragma once

#include <bits/stdc++.h>
#include <stdexcept>
#include "conio.cpp"
#include "box.cpp"
#include "lib.cpp"

namespace scrollWin
{
    class SwMain
    {

        // box
        box::BoxWithScrollBar box;

        // lines

        std::vector<std::string> lines;
        int topLinePos,

            // example:
            // lines = 5
            // content width = 3
            // :. max topLinePos will be 5 - 3 = 2
            //
            //           0
            // ********* 1
            // *       * 2
            // *       * 3
            // *       * 4
            // *********
            maxTopLinePos;

        void scroll(lib::Direction scrollDirection);

    public:
        // output stream object
        std::ostringstream out;

        SwMain(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        // render the contents inside the box
        void renderContent();

        // add the content from output stream object into lines. Rerender the content inside the window possibly
        void endLine();

        int setActive();
    };
}

namespace scrollWin
{
    SwMain::SwMain(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : box(x1, y1, x2, y2, title, backgroundColor, textColor, hOut), out(std::ostringstream::ate)
    {
        topLinePos = 0;
    }

    void SwMain::endLine()
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
            if (line.length() <= box.innerHorSizePadded)
                lines.push_back(line + std::string(box.innerHorSizePadded - line.length(), ' '));
            // divide the line into parts and push the parts as lines
            else
            {
                std::vector<std::string> parts = lib::strToEqualSizeParts(line, box.innerHorSizePadded);
                short lastPartPos = parts.size() - 1;

                // parts before 'last part'
                for (short i = 0; i < lastPartPos; ++i)
                    lines.push_back(parts[i]);

                // last part: its special bcz we may need to add space to it.

                std::string &lastPart = parts[lastPartPos];
                lines.push_back(lastPart + std::string(box.innerHorSizePadded - lastPart.length(), ' '));
            }
        }

        maxTopLinePos = lines.size() - box.innerVerSizePadded;

        box.setNoOfLines(lines.size());
        renderContent();
    }

    void SwMain::renderContent()
    {
        const short x = box.innerTopLeftCoordPadded.X;
        short y = box.innerTopLeftCoordPadded.Y;
        const short yBottom = box.innerBottomRightCoordPadded.Y;

        int pos = topLinePos, linesSize = lines.size();

        winConio::setTextAndBackgroundColor(box.textColor, box.backgroundColor, box.hOut);

        // render till y reaches yBottom and also lines are available to render
        while (y <= yBottom && pos != linesSize)
        {
            winConio::gotoxy(x, y, box.hOut);
            std::cout << lines[pos++];
            ++y;
        }
    }

    void SwMain::scroll(lib::Direction scrollDirection)
    {
        const int noOfLinesToScroll = 1;

        bool didScrolled = box.scroll(scrollDirection, noOfLinesToScroll);

        if (didScrolled)
        {
            if (scrollDirection == lib::Direction::dirUp)
            {
                topLinePos -= noOfLinesToScroll;

                if (topLinePos < 0)
                    topLinePos = 0;
            }

            else if (scrollDirection == lib::Direction::dirDown)
            {
                topLinePos += noOfLinesToScroll;

                if (topLinePos > maxTopLinePos)
                    topLinePos = maxTopLinePos;
            }

            renderContent();
        }
    }

    int SwMain::setActive()
    {
        if (!box.hasFocus())
            box.setFocus(true);

        while (true)
        {
            int pressedKey = winConio::getch();

            // When reading keys with conio and getch, in order to be able to handle special keys (arrow keys, function keys)
            // while still fitting its return value in a char, getch returns special keys as two-char sequences.
            // The first call returns 224, while the second call returns the code of the special key.
            if (pressedKey == 224)
            {
                const char SPECIAL_ARROW_UP = 72, SPECIAL_ARROW_DOWN = 80;
                // second call
                pressedKey = winConio::getch();

                if (pressedKey == SPECIAL_ARROW_UP)
                    scroll(lib::Direction::dirUp);
                else if (pressedKey == SPECIAL_ARROW_DOWN)
                    scroll(lib::Direction::dirDown);
            }
            // return pressed key
            else if (pressedKey == lib::Chars::escape || pressedKey == lib::Chars::horizontalTab)
                return pressedKey;
        }
    }
}