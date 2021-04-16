# Support for C++ Current Standard 

[![Generic badge](https://img.shields.io/badge/C++11-GREEN.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/C++14-GREEN.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/C++17-GREEN.svg)](https://shields.io/)
\
\
The master branch currently support C++ standard v3. The issues with current branch is outdated/depreciated codes and libraries. These are:

* <strstream.h>
* <conio.h>

* We need to replace ``` <strstream.h> ``` by ``` <sstream> ``` i.e and use ``` istringstream, ostringstream, stringstream``` as neccessary.
	http://www.cplusplus.com/reference/sstream/ostringstream/ostringstream/

## ```dev``` Folder

This contains work for modern c++. Implementations are for windows os only. This has done pretty much replacment for ```conio.h``` .
* get console rows and cols
* set full screen windowed console
* clearscreen
* paint background
* text capturing
* creating box
* hiding cursor


## Extra

* Console events : https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events