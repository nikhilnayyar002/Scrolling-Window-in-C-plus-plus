#include <bits/stdc++.h>
#include <windows.h>
#include "./conio.hpp"
#include <conio.h>

using namespace std;

int main()
{
    winConio::clearScreen();
    winConio::setFullScreen();
    winConio::ConsoleDimentions t= winConio::getConsoleDimentions();
    winConio::paintBackground(0, 0, 10, 10, winConio::RED);
    int a = getch();
    return 0;
}
