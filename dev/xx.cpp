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

    box::Box box(0, 0, 15, 15, "Window", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);
    box.setFocus(true);

    box.out << "Hi:" << '\n';
    box.out << "This:" << '\n';
    box.out << "this output breaks to next line since it overflowed.";
    box.out << "\n6666666666666666999999999999999";
    box.endLine();
    box.out << "Hi:" << '\n';
    box.out << "This:" << '\n';
    box.out << "this output breaks to next line since it overflowed.";
    box.out << "\n6666666666666666999999999999999";
    box.endLine();

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
