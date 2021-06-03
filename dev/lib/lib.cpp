#pragma once

#include <bits/stdc++.h>
#include <stdexcept>

namespace lib
{
    enum Chars
    {
        borderHorizontalBottom = 223, // ▀
        borderHorizontalTop = 220,    // ▄
        borderVertical = 219,         // █
        scrollBar = 177,              // ▒
        scrollBarThumb = 178,         // ▓
        scrollButtonTop = 30,         // ▲
        scrollButtonDown = 31,        // ▼
        escape = 27,
        carriageReturn = 13,
        lineFeed = 10,
        horizontalTab = 9,
    };

    enum Position
    {
        posTop = 1,
        posBottom = 2,
        posLeft = 3,
        posRight = 4
    };

    enum Direction
    {
        dirUp = 1,
        dirDown = 2,
        dirLeft = 3,
        dirRight = 4
    };

    inline void nameFailedWithCode(const std::string name, long code)
    {
        std::cout << name << " failed - (" << code << ")\n";
    }

    // example:
    //
    // strSpit("", "\n") -> [""]
    // strSpit("\n", "\n") -> ["",""]
    // strSpit("65\n", "\n") -> ["65",""]
    // strSpit("\n65", "\n") -> ["","65"]
    // strSpit("65\n65", "\n") -> ["65","65"]
    std::vector<std::string> strSpit(std::string str, std::string delimiter)
    {
        size_t pos = 0;
        std::vector<std::string> tokens;

        while ((pos = str.find(delimiter)) != std::string::npos)
        {
            // push the string part before delimiter
            tokens.push_back(str.substr(0, pos));
            // erase the string part before delimiter + delimiter
            str.erase(0, pos + delimiter.length());
        }

        // after there is no delimiter present in string then the erased string is either "" or "%PART_OF_STRING%"
        // push that string
        tokens.push_back(str);

        return tokens;
    }

    // Divides a line into smaller lines of fixed width
    // Word Break Rule: break-all (https://developer.mozilla.org/en-US/docs/Web/CSS/word-break)
    //
    // example:
    //
    //  string = 0123456789
    //  part length = 4
    //  no of parts =  10/4 = 2.5 = 3
    //  parts:
    //       0123 pos=0
    //       4567 pos=4
    //       89   pos=8 (last part)
    //
    std::vector<std::string> strToEqualSizeParts(std::string str, unsigned int partLength)
    {
        std::vector<std::string> parts;

        unsigned int strLength = str.length();
        unsigned short noOfParts = ceil(float(strLength) / partLength);
        unsigned short pos = 0;

        // parts before 'last part'
        for (short i = 1; i <= (noOfParts - 1); ++i)
        {
            parts.push_back(str.substr(pos, partLength));
            pos += partLength;
        }

        // last part
        parts.push_back(str.substr(pos, strLength - pos));

        return parts;
    }
}
