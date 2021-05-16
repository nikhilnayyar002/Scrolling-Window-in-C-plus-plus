#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "./conio.hpp"
#include "./box.hpp"

using namespace std;

int main()
{
    HANDLE hStdOut = winConio::getStdHandle();

    winConio::clearScreen(hStdOut);
    winConio::setFullScreen();
    // winConio::ConsoleDimentions consoleDimens= winConio::getConsoleDimentions(hStdOut);

    box::Box box(0, 0, 9, 9, "Window", winConio::BLUE, winConio::WHITE, hStdOut);
    box.setFocus(true);
    winConio::displayCursor(false, hStdOut);

    int a = getch();

    return 0;
}
