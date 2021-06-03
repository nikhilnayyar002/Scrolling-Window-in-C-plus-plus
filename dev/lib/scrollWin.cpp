#pragma once

#include <bits/stdc++.h>
#include <stdexcept>
#include "conio.cpp"
#include "box.cpp"
#include "lib.cpp"

namespace scrollWin
{
    const short SWSELEC_DEFAULT_SELECTED_OPTION_COLOR = winConio::RED;

    class SwBase
    {
    protected:
        // box
        box::BoxWithScrollBar box;

        // lines

        std::vector<std::string> lines;

        // the address of pointer that is storing the address of next window that can be made active after making current window inactive
        // why double pointer? because that pointer may have different value and knowing the address of that pointer we can get its current value in real time.
        // For example:
        //
        // Address of Window 1 : 12345
        // Address of Pointer: 00001
        // Address stored by Pointer: 12345
        // Address stored by double Pointer: 00001
        // :. Address of current window (*pointer): 12345
        //
        // Now lets the pointer value is changed.
        //
        // Now Address of Window 2 : 44444
        // Address of Pointer: 00001
        // Address stored by Pointer: 44444
        // Address stored by double Pointer: 00001
        // :. Address of current window (*pointer): 44444
        //
        // So one can see the the value is reflected in double pointer when it gets changed in original pointer.
        //
        SwBase **nextActiveWindow = nullptr;

        // common code that will be called within setActive member function
        // will return 0:
        //  1. if it cannot do anything related to pressed key
        //  2. it did something related to pressed key for internal changes (scroll contents etc)
        //
        // will return non-zero value if key pressed has some special meaning like ESC, TAB and requires external logic (switch windows etc)
        //
        int setActiveCommon(int pressedKey);

        // add the content of string into lines.
        void _end(std::string str);

        virtual void scroll(lib::Direction scrollDirection) = 0;

        // render the contents inside the box
        virtual void renderContent() = 0;

    public:
        SwBase(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        //

        void setNextActiveWindow(SwBase **p) { nextActiveWindow = p; }
        SwBase *getNextActiveWindow() { return nextActiveWindow ? *nextActiveWindow : nullptr; }

        // return box parameters

        int getInnerHorSize() { return box.innerHorSizePadded; }
        int getInnerVerSize() { return box.innerVerSizePadded; }

        // calls _end function. It can do extra logic before calling it.
        virtual void end() = 0;

        // set the window as active
        virtual int setActive() = 0;

        // rest the output of window: reset scroll and content only.
        virtual void clear();
    };

    class SwMain : public SwBase
    {
        // output stream object
        std::ostringstream out;

        // virtual functions

        void scroll(lib::Direction scrollDirection);
        void renderContent();

    public:
        //output stream functions
        SwMain &operator<<(char val) { out << val; return *this; }
        SwMain &operator<<(unsigned char val) { out << val; return *this; }
        SwMain &operator<<(signed char val) { out << val; return *this; }
        SwMain &operator<<(short val) { out << val; return *this; }
        SwMain &operator<<(unsigned short val) { out << val; return *this; }
        SwMain &operator<<(int val) { out << val; return *this; }
        SwMain &operator<<(unsigned int val) { out << val; return *this; }
        SwMain &operator<<(float val) { out << val; return *this; }
        SwMain &operator<<(double val) { out << val; return *this; }
        SwMain &operator<<(long double val) { out << val; return *this; }
        SwMain &operator<<(long val) { out << val; return *this; }
        SwMain &operator<<(unsigned long val) { out << val; return *this; }
        SwMain &operator<<(long long val) { out << val; return *this; }
        SwMain &operator<<(unsigned long long val) { out << val; return *this; }
        SwMain &operator<<(char *val) { out << val; return *this; }
        SwMain &operator<<(const char *val) { out << val; return *this; }
        SwMain &operator<<(std::string val) { out << val; return *this; }

        SwMain(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        // virtual functions

        void end();
        int setActive();
        void clear();
    };

    struct SwSelecOption
    {
        std::string optionName;        // name of option
        void (*onOptionSelected)(int); // callback to call when option is selected by enter key
    };

    class SwSelec : public SwBase
    {
        struct Option
        {
            int index;                     // index of first line in option
            int linesSpanned;              //  no of lines an option span within box
            void (*onOptionSelected)(int); // callback to call when option is selected by enter key
        };

        Option selectedOption;
        std::vector<Option> options;

        short selectedOptionIndex = -1;
        short selectedOptionColor = SWSELEC_DEFAULT_SELECTED_OPTION_COLOR;

        // virtual functions

        void scroll(lib::Direction scrollDirection);
        void renderContent();

    public:
        SwSelec(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut);

        void addOptions(std::vector<SwSelecOption> swSelecOptions);

        // virtual functions

        void end();
        int setActive();
        void clear();
    };

    // this recipe when called allows switching between windows using tab key. Also if escape is pressed in a window it will break
    // out of while loop and give back control to the caller function (the one who called it)
    // pass the a window to set it as active initially.
    void windowsRecipe1(SwBase &initialActiveWindow);

    // return pair of string and the no of lines it will span in box.
    //
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
    std::pair<std::string, int> filterTextOutput1(int width, std::string key, std::string value);
}

namespace scrollWin
{
    SwBase::SwBase(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : box(x1, y1, x2, y2, title, backgroundColor, textColor, hOut)
    {
    }

    void SwBase::clear()
    {
        box.resetContent();
        box.resetLines(0);
        lines.clear();
    }

    void SwBase::_end(std::string str)
    {
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
    }

    int SwBase::setActiveCommon(int pressedKey)
    {
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

            return 0;
        }
        // return pressed key
        else if (pressedKey == lib::Chars::escape || pressedKey == lib::Chars::horizontalTab)
        {
            box.setFocus(false);
            return pressedKey;
        }
        else
            return 0;
    }

    SwMain::SwMain(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : SwBase(x1, y1, x2, y2, title, backgroundColor, textColor, hOut), out(std::ostringstream::ate)
    {
    }

    void SwMain::clear()
    {
        out.str("");
        SwBase::clear();
    }

    void SwMain::end()
    {
        const std::string str = out.str();
        out.str("");

        _end(str);

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

            int returnedValue = setActiveCommon(pressedKey);

            if (returnedValue)
                return returnedValue;
        }
    }

    void SwMain::scroll(lib::Direction scrollDirection)
    {
        bool didScrolled = box.scroll(scrollDirection, 1);

        if (didScrolled)
            renderContent();
    };

    SwSelec::SwSelec(short x1, short y1, short x2, short y2, std::string title, short backgroundColor, short textColor, HANDLE hOut)
        : SwBase(x1, y1, x2, y2, title, backgroundColor, textColor, hOut)
    {
    }

    void SwSelec::clear()
    {
        selectedOptionIndex = -1;
        options.clear();

        SwBase::clear();
    }

    void SwSelec::addOptions(std::vector<SwSelecOption> swSelecOptions)
    {
        if (swSelecOptions.size())
        {
            int newOptionNo = options.size() ? (options.size() + 1) : 1;
            int indexOfFirstLineInNewOption = lines.size() ? (lines.size() - 1) : 0;

            for (auto &swOption : swSelecOptions)
            {
                std::pair<std::string, int> filteredStr = filterTextOutput1(box.innerHorSizePadded, "" + std::to_string(newOptionNo), swOption.optionName);
                _end(filteredStr.first + "\n");

                Option newOption = {indexOfFirstLineInNewOption, filteredStr.second, swOption.onOptionSelected};
                options.push_back(newOption);

                ++newOptionNo;
                indexOfFirstLineInNewOption += filteredStr.second;
            }

            // remove the last empty string created because of splitting newline in last option
            lines.pop_back();

            if (selectedOptionIndex == -1)
            {
                selectedOption = options[0];
                selectedOptionIndex = 0;
            }

            box.setNoOfLines(lines.size());
            renderContent();
        }
    }

    void SwSelec::end()
    {
    }

    void SwSelec::renderContent()
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
                if (pos == selectedOption.index)
                {
                    int i = 0;
                    winConio::setTextAndBackgroundColor(box.textColor, selectedOptionColor, box.hOut);

                    // print highlighted option
                    while (i < selectedOption.linesSpanned && y <= yBottom)
                    {
                        winConio::gotoxy(x, y, box.hOut);
                        std::cout << lines[pos + i] << std::string(box.innerHorSizePadded - lines[pos + i].length(), ' ');
                        ++i;
                        ++y;
                    }

                    pos += i;                                                                          // skip the painted lines
                    winConio::setTextAndBackgroundColor(box.textColor, box.backgroundColor, box.hOut); // reset background color
                }
                else
                {
                    std::cout << lines[pos] << std::string(box.innerHorSizePadded - lines[pos].length(), ' ');
                    ++pos;
                    ++y;
                }
            }
            // print empty line
            else
            {
                std::cout << std::string(box.innerHorSizePadded, ' ');
                ++y;
            }
        }
    }

    void SwSelec::scroll(lib::Direction scrollDirection)
    {
        int noOfLinesToScroll = 0;

        if (scrollDirection == lib::Direction::dirUp && selectedOptionIndex > 0)
        {
            --selectedOptionIndex;
            selectedOption = options[selectedOptionIndex];
            noOfLinesToScroll = selectedOption.linesSpanned;
        }
        else if (scrollDirection == lib::Direction::dirDown && selectedOptionIndex != (options.size() - 1))
        {
            ++selectedOptionIndex;
            noOfLinesToScroll = selectedOption.linesSpanned;
            selectedOption = options[selectedOptionIndex];
        }

        if (noOfLinesToScroll)
        {
            box.scroll(scrollDirection, noOfLinesToScroll);
            renderContent();
        }
    };

    int SwSelec::setActive()
    {
        box.setFocus(true);

        while (true)
        {
            int pressedKey = winConio::getch();

            // enter key is pressed
            if (pressedKey == lib::Chars::carriageReturn && selectedOption.index > -1 && selectedOption.onOptionSelected)
                selectedOption.onOptionSelected(selectedOptionIndex + 1);

            int returnedValue = setActiveCommon(pressedKey);

            if (returnedValue)
                return returnedValue;
        }
    }

    std::pair<std::string, int> filterTextOutput1(int width, std::string key, std::string value)
    {
        key += ": ";

        int offsetWidth, widthAfterOffset;

        int noOfLines = 0;

        if (key.length() > width)
        {
            const std::vector<std::string> parts = lib::strToEqualSizeParts(key, width);
            std::string lastPart = parts.back();

            noOfLines = parts.size();

            offsetWidth = lastPart.length();
            widthAfterOffset = width - offsetWidth;
        }
        else
        {
            noOfLines = 1;

            offsetWidth = key.length();
            widthAfterOffset = width - offsetWidth;
        }

        if (widthAfterOffset == 0)
        {
            offsetWidth = 0;
            widthAfterOffset = width - offsetWidth;
        }

        if (value.length() <= widthAfterOffset)
            key += value;
        else
        {
            key += value.substr(0, widthAfterOffset);
            value.erase(0, widthAfterOffset);
            noOfLines += 1;

            while (value.length())
            {
                key += std::string(offsetWidth, ' ');

                if (value.length() > widthAfterOffset)
                {
                    key += value.substr(0, widthAfterOffset);
                    value.erase(0, widthAfterOffset);
                    ++noOfLines;
                }
                else
                {
                    key += value;
                    break;
                }
            }
        }

        return {key, noOfLines};
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
}