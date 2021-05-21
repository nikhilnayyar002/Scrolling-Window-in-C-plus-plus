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

    scrollWin::SwMain sWMain1(0, 0, 15, 15, "Window", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);

    sWMain1.out << "Hi:" << '\n';
    sWMain1.out << "This:" << '\n';
    sWMain1.out << "\n66666";
    sWMain1.end();
    sWMain1.out << "Hi:" << '\n';
    sWMain1.out << "This:" << '\n';
    sWMain1.out << "this output breaks to next line since it overflowed.";
    sWMain1.out << "\n6666666666666666999999999999999";
    sWMain1.end();

    scrollWin::SwMain sWMain2(0 + 25, 0, 15 + 35, 15, "Window2", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);

    sWMain2.out << "Hi:" << '\n';
    sWMain2.out << "This:" << '\n';
    sWMain2.out << "\n66666";
    sWMain2.end();
    sWMain2.out << "Hi:" << '\n';
    sWMain2.out << "This:" << '\n';
    sWMain2.out << scrollWin::filterTextOutput1(sWMain2.getInnerHorSize(), "Address", "D/90/B, Janak Puri, New Delhi, Delhi, 110059");
    sWMain2.end();

    sWMain1.setNextActiveWindow(sWMain2);
    sWMain2.setNextActiveWindow(sWMain1);
    scrollWin::windowsRecipe1(sWMain1);

    return 0;
}
