#pragma once

#include <bits/stdc++.h>
#include <stdexcept>
#include "conio.cpp"
#include "box.cpp"
#include "lib.cpp"

namespace scrollWin
{
    class SwBase
    {
    protected:
        // box
        box::BoxWithScrollBar box;

        // lines

        std::vector<std::string> lines;

        // the next window that can be made active after making current window inactive
        SwBase *nextActiveWindow;

        void scroll(lib::Direction scrollDirection);

    public:
        // output stream object
        std::ostringstream out;

        SwBase(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        void setNextActiveWindow(SwBase &ref) { nextActiveWindow = &ref; }
        SwBase *getNextActiveWindow() { return nextActiveWindow; }

        // return box parameters

        int getInnerHorSize() { return box.innerHorSizePadded; }
        int getInnerVerSize() { return box.innerVerSizePadded; }

        // render the contents inside the box
        virtual void renderContent() = 0;

        // add the content from output stream object into lines. After that rerender the lines possibly
        virtual void end() = 0;

        // set the window as active
        virtual int setActive() = 0;
    };

    class SwMain : public SwBase
    {

    public:
        SwMain(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        // render the contents inside the box
        void renderContent();

        // add the content from output stream object into lines. Rerender the content inside the window possibly
        void end();

        int setActive();
    };

    // this recipe when called allows switching between windows using tab key. Also if escape is pressed in a window it will break
    // out of while loop and give back control to the caller function (the one who called it)
    // pass the a window to set it as active initially.
    void windowsRecipe1(SwBase &initialActiveWindow);

    // Example: you are getting output as:
    //
    //    0 Name: Nikhil Nayyar          0
    //    0 Address: D/90/B, Janak Puri, 0
    //    0 New Delhi, Delhi, 110059     0
    //    0 DOB:2nd September 1999       0
    //
    // And you want to display it as (see the address part):
    //
    //    0 Name: Nikhil Nayyar          0
    //    0 Address: D/90/B, Janak Puri, 0
    //    0          New Delhi, Delhi,   0
    //    0          110059              0
    //    0 DOB:2nd September 1999       0
    //
    // To do that call the function with boxWidth, key, value
    //
    //  boxWidth = width of box
    //  key = Address
    //  value =  D/90/B, Janak Puri, New Delhi, Delhi, 110059
    //
    std::string filterTextOutput1(int width, std::string key, std::string value);
}

namespace scrollWin
{
    SwBase::SwBase(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : box(x1, y1, x2, y2, title, backgroundColor, textColor, hOut), out(std::ostringstream::ate)
    {
        nextActiveWindow = nullptr;
    }

    void SwBase::scroll(lib::Direction scrollDirection)
    {
        const int noOfLinesToScroll = 1;

        bool didScrolled = box.scroll(scrollDirection, noOfLinesToScroll);

        if (didScrolled)
            renderContent();
    }

    SwMain::SwMain(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : SwBase(x1, y1, x2, y2, title, backgroundColor, textColor, hOut)
    {
    }

    void SwMain::end()
    {
        const std::string str = out.str();
        // reset the buffer with set overload of ostringstream::str
        out.str("");

        // #07111120052021
        // Concept: lines and parts
        //
        // let \ = newline character (\n) , "" = emty string
        //
        // string = nikhil\here it is bro\
        //
        //      newLines:
        //           nikhil
        //           here it is bro
        //           ""
        //
        // string = \
        //
        //      newLines
        //           ""
        //           ""
        //

        // split the strings based on newLines ('\n') in it
        std::vector<std::string> newLines = lib::strSpit(str, "\n");

        // The first line should be merged with last line (in lines) if possible
        if (lines.size())
        {
            std::string &lastLine = lines.back();
            std::string &newFirstLine = newLines[0];

            //  let 0 = border
            //
            //  0nikhi0  // line1 = nikhi
            //  0l    0  // lastLine = l
            //
            // As one can see the last line can except 4 more charaters to complete the row.
            int emptySpaceLength = box.innerHorSizePadded - lastLine.length();
            if (emptySpaceLength)
            {
                if (newFirstLine.length() > emptySpaceLength)
                {
                    lastLine += newFirstLine.substr(0, emptySpaceLength);
                    // erase the part that is merged with last line
                    newFirstLine.erase(0, emptySpaceLength);
                }
                else
                {
                    lastLine += newFirstLine;
                    // remove the first element from new lines as it has merged fully inside the last line
                    newLines.erase(newLines.begin());
                }
            }
        }

        // concept of parts is described in above comment #07111120052021
        for (auto &line : newLines)
        {
            // if line is less or same sized as inner padded width of box then push it as line.
            if (line.length() <= box.innerHorSizePadded)
                lines.push_back(line);
            // divide the line into parts and push the parts as lines
            else
            {
                for (auto &part : lib::strToEqualSizeParts(line, box.innerHorSizePadded))
                    lines.push_back(part);
            }
        }

        box.setNoOfLines(lines.size());
        renderContent();
    }

    void SwMain::renderContent()
    {
        const short x = box.innerTopLeftCoordPadded.X;
        short y = box.innerTopLeftCoordPadded.Y;
        const short yBottom = box.innerBottomRightCoordPadded.Y;

        int pos = box.topLineIndex, linesSize = lines.size();

        winConio::setTextAndBackgroundColor(box.textColor, box.backgroundColor, box.hOut);

        while (y <= yBottom)
        {
            winConio::gotoxy(x, y, box.hOut);

            // lines are available to render
            if (pos != linesSize)
            {
                // print the line. If it does not fill the row then print spaces to fill the row
                // printing spaces ensures any old content previously rendered in a row is cleared
                std::cout << lines[pos] << std::string(box.innerHorSizePadded - lines[pos].length(), ' ');
                ++pos;
            }

            // print empty line
            else
                std::cout << std::string(box.innerHorSizePadded, ' ');

            ++y;
        }
    }

    int SwMain::setActive()
    {
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
            {
                box.setFocus(false);
                return pressedKey;
            }
        }
    }

    void windowsRecipe1(SwBase &initialActiveWindow)
    {
        SwBase *activeWindow = &initialActiveWindow;

        while (true)
        {
            int p = activeWindow->setActive();

            // if horizontal tab is pressed switch to next active window
            if (p == lib::Chars::horizontalTab)
            {
                scrollWin::SwBase *nextActiveWindow = activeWindow->getNextActiveWindow();
                if (nextActiveWindow)
                    activeWindow = nextActiveWindow;
            }
            // if escape is pressed then break out
            else if (p == lib::Chars::escape)
                break;
        }
    }

    std::string filterTextOutput1(int width, std::string key, std::string value)
    {
        key += ": ";

        int offsetWidth, widthAfterOffset;

        if (key.length() > width)
        {
            std::string lastPart = lib::strToEqualSizeParts(key, width).back();

            offsetWidth = lastPart.length();
            widthAfterOffset = width - offsetWidth;
        }
        else
        {
            offsetWidth = key.length();
            widthAfterOffset = width - offsetWidth;
        }

        if (widthAfterOffset == 0)
        {
            offsetWidth = 0;
            widthAfterOffset = width - offsetWidth;
        }

        if (value.length() > widthAfterOffset)
        {
            key += value.substr(0, widthAfterOffset);
            value.erase(0, widthAfterOffset);

            while (value.length())
            {
                key += std::string(offsetWidth, ' ');

                if (value.length() > widthAfterOffset)
                {
                    key += value.substr(0, widthAfterOffset);
                    value.erase(0, widthAfterOffset);
                }
                else
                {
                    key += value;
                    break;
                }
            }
        }

        return key;
    }
}