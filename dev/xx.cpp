#include <bits/stdc++.h>
#include "./lib/conio.cpp"
#include "./lib/box.cpp"

using namespace std;

int main()
{
    HANDLE hStdOut = winConio::getStdHandle();

    winConio::clearScreen(hStdOut);
    winConio::setFullScreen();
    winConio::displayCursor(false, hStdOut);
    // winConio::ConsoleDimentions consoleDimens= winConio::getConsoleDimentions(hStdOut);

    box::Box box(0, 0, 15, 15, "Window", winConio::BLUE, winConio::WHITE, hStdOut);
    box.setFocus(true);
    box.setNoOfLines(14);
    Sleep(1000);
    box.scroll(box::SCROLL_DOWN);
    Sleep(1000);
    box.scroll(box::SCROLL_DOWN);
    Sleep(1000);
    box.scroll(box::SCROLL_UP);
    Sleep(1000);
    box.scroll(box::SCROLL_UP);
    Sleep(1000);

    int a = winConio::getch();

    return 0;
}
