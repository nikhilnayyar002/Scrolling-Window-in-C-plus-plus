#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "./lib/conio.cpp"
#include "./lib/box.cpp"

using namespace std;

int main()
{
    HANDLE hStdOut = winConio::getStdHandle();

    winConio::clearScreen(hStdOut);
    winConio::setFullScreen();
    // winConio::ConsoleDimentions consoleDimens= winConio::getConsoleDimentions(hStdOut);

    box::Box box(0, 0, 15, 15, "Window", winConio::BLUE, winConio::WHITE, hStdOut);
    box.setFocus(true);
    winConio::displayCursor(false, hStdOut);

    int a = getch();

    return 0;
}
