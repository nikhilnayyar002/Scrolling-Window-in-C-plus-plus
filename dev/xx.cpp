#include <bits/stdc++.h>
#include "./lib/conio.cpp"
#include "./lib/scrollWin.cpp"

using namespace std;

void callMe(int optionNo)
{
    winConio::gotoxy(30, 36, winConio::getStdHandle());
    cout << optionNo << " Selected !!!!!!!!!!!!!!!";
}

int main()
{
    HANDLE hStdOut = winConio::getStdHandle();

    winConio::clearScreen(hStdOut);
    winConio::setFullScreen();
    winConio::displayCursor(false, hStdOut);
    // winConio::ConsoleDimentions consoleDimens= winConio::getConsoleDimentions(hStdOut);

    scrollWin::SwMain sWMain1(0, 0, 15, 15, "Console", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);

    sWMain1.out << "Hi:" << '\n';
    sWMain1.out << "This:" << '\n';
    sWMain1.out << "\n66666";
    sWMain1.end();
    sWMain1.out << "Hi:" << '\n';
    sWMain1.out << "This:" << '\n';
    sWMain1.out << "this output breaks to next line since it overflowed.";
    sWMain1.out << "\n6666666666666666999999999999999";
    sWMain1.out << scrollWin::filterTextOutput1(sWMain1.getInnerHorSize(), "Address", "D/90/B, Janak Puri, New Delhi, Delhi, 110059").first;
    sWMain1.end();

    scrollWin::SwSelec swSelec(0 + 25, 20, 15 + 25, 20 + 15, "Window2", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);
    vector<scrollWin::SwSelecOption> options = {
        {"Option Option Option Option", nullptr},
        {"Option Option Option Option Option Option Option Option Option Option Option Option", callMe},
        {"Option Option Option Option", nullptr},
        {"Option Option Option Option Option Option Option Option", nullptr},
        {"Option", nullptr},
        {"Option", nullptr},
        {"Option", nullptr},
        {"Option", nullptr},
        {"Option", nullptr},
        {"Option", nullptr},
        {"Option", nullptr},
        {"Option Option Option Option", nullptr},
        {"Option Option Option Option Option Option Option Option Option Option Option Option", nullptr},
        {"Option Option Option Option", nullptr},
        {"Option Option Option Option Option Option Option Option", nullptr},
    };
    swSelec.addOptions(options);

    sWMain1.setNextActiveWindow(swSelec);
    swSelec.setNextActiveWindow(sWMain1);
    scrollWin::windowsRecipe1(swSelec);

    return 0;
}
