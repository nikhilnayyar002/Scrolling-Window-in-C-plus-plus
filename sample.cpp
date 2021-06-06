#include <bits/stdc++.h>
#include "./lib/conio.cpp"
#include "./lib/scrollWin.cpp"

using namespace std;

HANDLE hStdOut, hStdIn;
COORD consoleDimens;
scrollWin::SwMain *console = nullptr;
scrollWin::SwSelec *currentMenu = nullptr;

void onOptionSelected(int optionNo);
void subMenu();

int main()
{
    hStdOut = winConio::getStdOutputHandle();
    hStdIn = winConio::getStdInputHandle();
    winConio::setFullScreen(hStdOut);
    winConio::displayCursor(false, hStdOut);
    consoleDimens = winConio::getConsoleWindowSize(hStdOut);

    scrollWin::SwSelec menu(
        1, 0, consoleDimens.X / 4 - 1, consoleDimens.Y / 4, "Menu", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut, hStdIn);
    scrollWin::SwMain console(
        consoleDimens.X / 4 + 1, 0, consoleDimens.X - consoleDimens.X / 2, consoleDimens.Y / 4, "Console", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut, hStdIn);

    menu.addOptions({{"Menu Menu Menu Menu", onOptionSelected},
                     {"Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu", onOptionSelected},
                     {"Menu Menu Menu Menu", onOptionSelected},
                     {"Menu Menu Menu Menu Menu Menu Menu Menu", onOptionSelected},
                     {"Menu", onOptionSelected},
                     {"Menu", onOptionSelected},
                     {"Menu", onOptionSelected},
                     {"Menu", onOptionSelected},
                     {"Menu", onOptionSelected},
                     {"Menu", onOptionSelected},
                     {"Menu", onOptionSelected},
                     {"Menu Menu Menu Menu", onOptionSelected},
                     {"Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu", onOptionSelected},
                     {"Menu Menu Menu Menu", onOptionSelected},
                     {"Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu \
Menu Menu Menu Menu Menu Menu",
                      onOptionSelected},
                     {"Menu Menu Menu Menu", onOptionSelected}});

    console << "Hi:" << '\n';
    console << "This:" << '\n';
    console << "\n66666";
    console.end();

    console << "Hi:" << '\n';
    console << "This:" << '\n';
    console << "this output breaks to next line since it overflowed.";
    console << "\n6666666666666666999999999999999";
    console << "\n"
            << scrollWin::filterTextOutput1(console.getInnerHorSize(), "Address", "D/90/B, Janak Puri, AMT Tower, C96 Green Park, New Delhi, Delhi, 110059").first
            << "\n";
    console << scrollWin::filterTextOutput1(console.getInnerHorSize(), "Some text", "text text text text text text text text text text text text text text text text text text").first
            << "\n";
    console.end();

    // make console globally acessible
    ::console = &console;
    // set current menu
    ::currentMenu = &menu;

    // connect windows in circular fashion
    console.setNextActiveWindow(&::currentMenu);
    menu.setNextActiveWindow(&::console);

    // set a window active and allowing switching b/w them. On special occasions like ESC key pressed, return out of this function.
    scrollWin::windowsRecipe1(menu);

    return 0;
}

void onOptionSelected(int optionNo)
{
    static bool isConsoleResetDone = false;

    if (::console)
    {
        scrollWin::SwMain &console = *::console;

        // reset console
        if (!isConsoleResetDone)
        {
            console.clear();
            isConsoleResetDone = true;
        }

        // do output
        console << optionNo << " Selected !\n";
        console.end();

        //if optionNo 2 selected
        if (optionNo == 2)
            subMenu();
    }
}

void subMenu()
{
    scrollWin::SwSelec subMenu(
        1, 0, consoleDimens.X / 4 - 1, consoleDimens.Y / 4, "Menu >> More Choices", winConio::BLUE, winConio::BRIGHT_WHITE, hStdOut, hStdIn);

    subMenu.addOptions({
        {"SubMenu SubMenu SubMenu SubMenu", nullptr},
        {"SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu", nullptr},
        {"SubMenu SubMenu SubMenu SubMenu", nullptr},
        {"SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu", nullptr},
        {"SubMenu", nullptr},
        {"SubMenu", nullptr},
        {"SubMenu", nullptr},
        {"SubMenu", nullptr},
        {"SubMenu", nullptr},
        {"SubMenu", nullptr},
        {"SubMenu", nullptr},
        {"SubMenu SubMenu SubMenu SubMenu", nullptr},
        {"SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu", nullptr},
        {"SubMenu SubMenu SubMenu SubMenu", nullptr},
        {"SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu SubMenu", nullptr},
    });

    // store address of previously active menu
    scrollWin::SwSelec *previousActiveMenu = ::currentMenu;
    // set new current menue
    ::currentMenu = &subMenu;
    // set subMenu next window
    subMenu.setNextActiveWindow(&::console);
    // set sub menu active and allow switching between windows
    scrollWin::windowsRecipe1(subMenu);
    // when current menu becomes inactive set currentMenu value back to previous menu.
    ::currentMenu = previousActiveMenu;
}