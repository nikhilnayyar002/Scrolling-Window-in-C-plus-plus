#include <bits/stdc++.h>
#include <windows.h>
#include "./std_scroll_win.hpp"

using namespace std;

namespace win_box
{
    void paint();
}

void printRowsCols()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    printf("columns: %d\n", columns);
    printf("rows: %d\n", rows);
}

int main()
{
    clearScreen();
    setFullScreen();
    printRowsCols();
    paintBackground(0,0,10,10,RED);
    system("PAUSE");
    return 0;
}
