#include <bits/stdc++.h>
#include "./lib/conio.cpp"
#include "./lib/scrollWin.cpp"

using namespace std;

int main()
{
    HANDLE hStdOut = winConio::getStdHandle();

    winConio::clearScreen(hStdOut);
    winConio::setFullScreen();
    winConio::displayCursor(false, hStdOut);
    // winConio::ConsoleDimentions consoleDimens= winConio::getConsoleDimentions(hStdOut);

    scrollWin::SwMain sWMain(0, 0, 15, 15, "Window", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);

    sWMain.out << "Hi:" << '\n';
    sWMain.out << "This:" << '\n';
    sWMain.out << "this output breaks to next line since it overflowed.";
    sWMain.out << "\n6666666666666666999999999999999";
    sWMain.endLine();
    sWMain.out << "Hi:" << '\n';
    sWMain.out << "This:" << '\n';
    sWMain.out << "this output breaks to next line since it overflowed.";
    sWMain.out << "\n6666666666666666999999999999999";
    sWMain.endLine();

    Sleep(1000);
    sWMain.scroll(lib::Direction::dirDown);
    Sleep(1000);
    sWMain.scroll(lib::Direction::dirDown);
    Sleep(1000);
    sWMain.scroll(lib::Direction::dirUp);
    Sleep(1000);
    sWMain.scroll(lib::Direction::dirUp);
    Sleep(1000);

    int a = winConio::getch();

    return 0;
}
