#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "./conio.hpp"
#include "./box.hpp"

using namespace std;

int main()
{
    winConio::clearScreen();
    winConio::setFullScreen();
    winConio::ConsoleDimentions t= winConio::getStdConsoleDimentions();

    box::Box b(0,0,20,20,"Window");
    b.setActiveState(true);

    int a = getch();
    return 0;
}
