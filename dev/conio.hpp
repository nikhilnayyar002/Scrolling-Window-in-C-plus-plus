#pragma once
/*******************************************************************************************************************************************
 * headers
 * *****************************************************************************************************************************************/
#include <bits/stdc++.h>
#include <windows.h>
#include "./utility.hpp"

namespace winConio
{
    const short TOTAL_COLORS = 16;
    const short BLACK = 0;
    const short BLUE = 1;
    const short GREEN = 2;
    const short AQUA = 3;
    const short RED = 4;
    const short PURPLE = 5;
    const short YELLOW = 6;
    const short WHITE = 7;
    const short GREY = 8;
    const short LIGHT_BLUE = 9;
    const short LIGHT_GREEN = 10;
    const short LIGHT_AQUA = 11;
    const short LIGHT_RED = 12;
    const short LIGHT_PURPLE = 13;
    const short LIGHT_YELLOW = 14;
    const short BRIGHT_WHITE = 15;

    struct ConsoleDimentions
    {
        int rows;
        int cols;
    };

    // sets the console window to fullscreen windowed mode
    // alternative functionality :  https://docs.microsoft.com/en-us/windows/console/setconsoledisplaymode
    void setFullScreen();

    // clear the screen
    // https://www.cplusplus.com/articles/4z18T05o/
    void clearScreen();

    // go to position x,y in console
    void gotoxy(short int x, short int y);

    // paint rectangular background from x1,y1 -> x2,y2
    void paintBackground(int x1, int y1, int x2, int y2, int color);

    // explicitly set the text color of console
    void setTextColor(int color = WHITE);

    // explicitly set the text color of console
    void setTextAndBackgroundColor(int txtColor = WHITE, int bgColor = BLACK);

    // Create a new console screen buffer
    // https://docs.microsoft.com/en-us/windows/console/reading-and-writing-blocks-of-characters-and-attributes
    HANDLE createConsoleScreenBuffer();

    // set a console screen buffer as active screen buffer
    // https://docs.microsoft.com/en-us/windows/console/reading-and-writing-blocks-of-characters-and-attributes
    int setConsoleActiveScreenBuffer(HANDLE &hOut);

    // get the dimensions of console window given a ConsoleScreenBuffer
    ConsoleDimentions getConsoleDimentions(HANDLE hout);

    // get the dimensions of standard console window
    ConsoleDimentions getStdConsoleDimentions();

    void displayCursor(bool state = true);

    // Class that provides getText and putText functions with shared variables/objects.
    // https://docs.microsoft.com/en-us/windows/console/reading-and-writing-blocks-of-characters-and-attributes
    class ConsoleTextCapture
    {
    private:
        CHAR_INFO *chiBuffer;

        SMALL_RECT srcRect;
        COORD coordBufSize;
        COORD coordBufCoord;
        BOOL fSuccess;

        void setSrcRecAndBufSize(SHORT x1, SHORT y1, SHORT x2, SHORT y2);

    public:
        ConsoleTextCapture(int cols, int rows);
        ~ConsoleTextCapture();

        bool getText(SHORT x1, SHORT y1, SHORT x2, SHORT y2, HANDLE hout);
        bool putText(SHORT x1, SHORT y1, SHORT x2, SHORT y2, HANDLE hout);
    };
}

//definations
namespace winConio
{
    void setFullScreen()
    {
        ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    }

    void clearScreen()
    {
        HANDLE hStdOut;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = {0, 0};

        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdOut == INVALID_HANDLE_VALUE)
            return;

        /* Get the number of cells in the current buffer */
        if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
            return;
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;

        /* Fill the entire buffer with spaces */
        if (!FillConsoleOutputCharacter(
                hStdOut,
                (TCHAR)' ',
                cellCount,
                homeCoords,
                &count))
            return;

        /* Fill the entire buffer with the current colors and attributes */
        if (!FillConsoleOutputAttribute(
                hStdOut,
                csbi.wAttributes,
                cellCount,
                homeCoords,
                &count))
            return;

        /* Move the cursor home */
        SetConsoleCursorPosition(hStdOut, homeCoords);
    }

    void gotoxy(short int x, short int y)
    {
        COORD c = {x, y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    void paintBackground(int x1, int y1, int x2, int y2, int color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TOTAL_COLORS * color + WHITE);

        int width = x2 - x1 + 1;
        for (int i = y1; i <= y2; ++i)
        {
            gotoxy(x1, i);
            std::cout << std::string(width, ' ');
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    }

    void setTextColor(int color = WHITE)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void setTextAndBackgroundColor(int txtColor = WHITE, int bgColor = BLACK)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TOTAL_COLORS * bgColor + txtColor);
    }

    HANDLE createConsoleScreenBuffer()
    {
        HANDLE hNewScreenBuffer;

        hNewScreenBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | // read/write access
                GENERIC_WRITE,
            FILE_SHARE_READ |
                FILE_SHARE_WRITE,    // shared
            NULL,                    // default security attributes
            CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
            NULL);                   // reserved; must be NULL
        if (hNewScreenBuffer == INVALID_HANDLE_VALUE)
        {
            WINCONIO_FAILURE_MESSAGE("CreateConsoleScreenBuffer", GetLastError());
            return nullptr;
        }

        return hNewScreenBuffer;
    }

    int setConsoleActiveScreenBuffer(HANDLE &hOut)
    {
        if (!SetConsoleActiveScreenBuffer(hOut))
        {
            WINCONIO_FAILURE_MESSAGE("SetConsoleActiveScreenBuffer", GetLastError());
            return 1;
        }
        return 0;
    }

    ConsoleTextCapture::ConsoleTextCapture(int cols, int rows)
    {
        chiBuffer = new CHAR_INFO[rows * cols];

        // The top left destination cell of the temporary buffer is
        // row 0, col 0.

        coordBufCoord.X = 0;
        coordBufCoord.Y = 0;
    }

    ConsoleTextCapture::~ConsoleTextCapture()
    {
        delete chiBuffer;
    }

    void ConsoleTextCapture::setSrcRecAndBufSize(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
    {
        // Set the source rectangle.

        srcRect.Top = y1; // top left: row, col
        srcRect.Left = x1;
        srcRect.Bottom = y2; // bot. right: row, col
        srcRect.Right = x2;

        // The temporary buffer size is (y2 - y1 + 1) rows x (x2 - x1 + 1) columns.

        coordBufSize.Y = y2 - y1 + 1;
        coordBufSize.X = x2 - x1 + 1;
    }

    bool ConsoleTextCapture::getText(SHORT x1, SHORT y1, SHORT x2, SHORT y2, HANDLE hout)
    {
        setSrcRecAndBufSize(x1, y1, x2, y2);

        fSuccess = ReadConsoleOutput(
            hout,          // screen buffer to read from
            chiBuffer,     // buffer to copy into
            coordBufSize,  // col-row size of chiBuffer
            coordBufCoord, // top left dest. cell in chiBuffer
            &srcRect);     // screen buffer source rectangle

        if (!fSuccess)
        {
            WINCONIO_FAILURE_MESSAGE("ReadConsoleOutput", GetLastError());
            return false;
        }
        return true;
    }

    bool ConsoleTextCapture::putText(SHORT x1, SHORT y1, SHORT x2, SHORT y2, HANDLE hout)
    {
        setSrcRecAndBufSize(x1, y1, x2, y2);

        fSuccess = WriteConsoleOutput(
            hout,          // screen buffer to write to
            chiBuffer,     // buffer to copy from
            coordBufSize,  // col-row size of chiBuffer
            coordBufCoord, // top left src cell in chiBuffer
            &srcRect);     // dest. screen buffer rectangle

        if (!fSuccess)
        {
            WINCONIO_FAILURE_MESSAGE("WriteConsoleOutput", GetLastError());
            return false;
        }
        return true;
    }

    ConsoleDimentions getConsoleDimentions(HANDLE hout)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        ConsoleDimentions temp;

        GetConsoleScreenBufferInfo(hout, &csbi);
        temp.cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        temp.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        return temp;
    }

    ConsoleDimentions getStdConsoleDimentions()
    {
        return getConsoleDimentions(GetStdHandle(STD_OUTPUT_HANDLE));
    }

    void displayCursor(bool state = true)
    {
        CONSOLE_CURSOR_INFO prev;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

        GetConsoleCursorInfo(hStdOut, &prev);
        prev.bVisible = state;
        SetConsoleCursorInfo(hStdOut, &prev);
    }
}
