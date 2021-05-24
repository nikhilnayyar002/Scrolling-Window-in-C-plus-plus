#include <bits/stdc++.h>
#include "./lib/conio.cpp"
#include "./lib/scrollWin.cpp"

using namespace std;

HANDLE hStdOut;
COORD consoleDimens;
scrollWin::SwMain *console = nullptr;
scrollWin::SwSelec *currentMenu = nullptr;

void onOptionSelected(int optionNo);
void subMenu();

int main()
{
    hStdOut = winConio::getStdHandle();
    winConio::setFullScreen(hStdOut);
    winConio::displayCursor(false, hStdOut);
    consoleDimens = winConio::getConsoleWindowSize(hStdOut);

    scrollWin::SwSelec menu(1, 0, consoleDimens.X / 4 - 1, consoleDimens.Y / 4, "Menu", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);
    scrollWin::SwMain console(consoleDimens.X / 4 + 1, 0, consoleDimens.X - consoleDimens.X / 2, consoleDimens.Y / 4, "Console", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);

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
    currentMenu = &menu;

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

        //if optionNo 2 selected
        if (optionNo == 2)
            subMenu();
    }
}

void subMenu()
{
    scrollWin::SwSelec *previousActiveMenu = currentMenu;

    scrollWin::SwSelec menu(1, 0, consoleDimens.X / 4 - 1, consoleDimens.Y / 4, "Menu 2", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut);

    menu.addOptions({
        {"Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
        {"Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
        {"Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
        {"Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
        {"Menu 2", nullptr},
        {"Menu 2", nullptr},
        {"Menu 2", nullptr},
        {"Menu 2", nullptr},
        {"Menu 2", nullptr},
        {"Menu 2", nullptr},
        {"Menu 2", nullptr},
        {"Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
        {"Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
        {"Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
        {"Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2 Menu 2", nullptr},
    });

    currentMenu = &menu;
    menu.setNextActiveWindow(*console);
    console->setNextActiveWindow(menu);

    scrollWin::windowsRecipe1(menu);

    currentMenu = previousActiveMenu;
    console->setNextActiveWindow(*currentMenu);
}