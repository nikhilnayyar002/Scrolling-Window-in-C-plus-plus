#include <bits/stdc++.h>
#include "./lib/conio.cpp"
#include "./lib/scrollWin.cpp"

using namespace std;

scrollWin::SwMain *console = nullptr;

void onOptionSelected(int optionNo);

int main()
{
    HANDLE hStdOut = winConio::getStdHandle();

    winConio::clearScreen(hStdOut);
    winConio::setFullScreen();
    winConio::displayCursor(false, hStdOut);
    winConio::ConsoleDimentions consoleDimens = winConio::getConsoleDimentions(hStdOut);

    scrollWin::SwSelec menu(1, 0, consoleDimens.cols / 4 - 1, consoleDimens.rows / 4, "Menu", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);
    scrollWin::SwMain console(consoleDimens.cols / 4 + 1, 0, consoleDimens.cols - consoleDimens.cols / 2, consoleDimens.rows / 4, "Console", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);

    menu.addOptions({
        {"Option Option Option Option", onOptionSelected},
        {"Option Option Option Option Option Option Option Option Option Option Option Option", onOptionSelected},
        {"Option Option Option Option", onOptionSelected},
        {"Option Option Option Option Option Option Option Option", onOptionSelected},
        {"Option", onOptionSelected},
        {"Option", onOptionSelected},
        {"Option", onOptionSelected},
        {"Option", onOptionSelected},
        {"Option", onOptionSelected},
        {"Option", onOptionSelected},
        {"Option", onOptionSelected},
        {"Option Option Option Option", onOptionSelected},
        {"Option Option Option Option Option Option Option Option Option Option Option Option", onOptionSelected},
        {"Option Option Option Option", onOptionSelected},
        {"Option Option Option Option Option Option Option Option", onOptionSelected},
    });

    console.out << "Hi:" << '\n';
    console.out << "This:" << '\n';
    console.out << "\n66666";
    console.end();

    console.out << "Hi:" << '\n';
    console.out << "This:" << '\n';
    console.out << "this output breaks to next line since it overflowed.";
    console.out << "\n6666666666666666999999999999999";
    console.out << "\n"
                << scrollWin::filterTextOutput1(console.getInnerHorSize(), "Address", "D/90/B, Janak Puri, AMT Tower, C96 Green Park, New Delhi, Delhi, 110059").first
                << "\n";
    console.out << scrollWin::filterTextOutput1(console.getInnerHorSize(), "Some text", "text text text text text text text text text text text text text text text text text text").first
                << "\n";
    console.end();

    // make console globally acessible
    ::console = &console;

    // connect windows in circular fashion
    console.setNextActiveWindow(menu);
    menu.setNextActiveWindow(console);

    // set a window active and allowing switching b/w them. On special occasions like ESC key pressed, return out of this function.
    scrollWin::windowsRecipe1(menu);

    return 0;
}

void onOptionSelected(int optionNo)
{
    static bool isConsoleResetDone = false;

    if (console)
    {
        // reset console
        if (!isConsoleResetDone)
        {
            console->clear();
            isConsoleResetDone = true;
        }

        // do output
        console->out << optionNo << " Selected !\n";
        console->end();
    }
}