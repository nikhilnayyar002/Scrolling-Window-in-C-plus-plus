<h1>Scrolling-Window-in-C++</h1>

[![Generic badge](https://img.shields.io/badge/OS-Windows-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/Mingw-gcc-green.svg)](https://shields.io/)
\
\
Must watch video first for good insight:
[Video Preview](https://youtu.be/AKy5GKpfbKc)

## Introduction

While creating console programs sometimes one may want to create visuals (colorful, interactive UI) similar to UI programs we are daily used to. These UI programs are generally window like (border, scrollbar, title), some allow users to select from given options (select menu), some allow to scroll through contents (notepad) etc. This project serves that purpose. It provides simple api to create window inside console that can output, scroll through contents, accept input, switch between multiple windows, allow selection of options using selection window. Besides this the project contains api similar to what conio.h provided in the past.

> You can create as many windows inside console that can fill up the console nicely!

## Features

- [x] Output support for Windows.
- [x] Scrolling support for Windows.
- [x] Support for Switching between multiple Windows.
- [ ] Console input support for Main Window.
- [ ] Console input validation support for Main Window.

## Working demo
Read the [sample](./sample.cpp) file for details.

## Prerequisites

* Must know about Pointers concept.
* Must be aware of windows working architecture (for this project) described in file [PROJECT_CONCEPT.md](./PROJECT_CONCEPT.md)
* Setup compiler environment https://code.visualstudio.com/docs/cpp/config-mingw

## Scrolling Windows

* Common
  * saves information that render in this window so that you can scroll and look back. (Similar to notepad in windows)
  * if overflow occurs wrap words.
  * Scrollbars will pop up when necessary.
  * Active window has higlightered borders.
  * Switching between windows using Tab Key is supported.

* SwMain
  * do output similar using `<< operator` similar to `std::cout`.
  * do input similar using `>> operator` similar to `std::cin`. Also provided is `getLine` function for strings.
  * Input field like support is there for input. :. Long text bigger than window horizontal size can be input.

* SwSelec
  * Presents menu items for selection using enter key and trigger callbacks.

## Support
As of know the the classic console apis are supported by microsoft.

> ### Note from Microsoft regarding current console apis support
> This document describes console platform functionality that is no longer a part of our [ecosystem roadmap](https://docs.microsoft.com/en-us/windows/console/ecosystem-roadmap). We do not recommend using this content in new products, but we will continue to support existing usages for the indefinite future. Our preferred modern solution focuses on [virtual terminal sequences](https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences) for maximum compatibility in cross-platform scenarios. You can find more information about this design decision in our [classic console vs. virtual terminal](https://docs.microsoft.com/en-us/windows/console/classic-vs-vt) document.
