# Support for C++ Current Standard 

[![Generic badge](https://img.shields.io/badge/C++11-GREEN.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/C++14-GREEN.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/C++17-GREEN.svg)](https://shields.io/)
\
\
\
\
The master branch currently support C++ standard v3. The issues with current branch is outdated/depreciated codes and libraries. These are:

* <strstream.h>
* <conio.h>

These can be resolved by using new standard implementations. The easiest possibilty is this way:

Painting the characters, lines using ``` <windows.h> ```.
 Example code:

```
/**
 * https://www.youtube.com/watch?v=v30nUWg6pks
 * https://docs.microsoft.com/en-us/windows/console/console-screen-buffers#span-idwin32characterattributesspanspan-idwin32characterattributesspancharacter-attributes
 * 
 */


#include <windows.h>
#include <iostream>
#include <strstream>
using namespace std;

int main() {
	for(int i=0;i<256;++i){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),i);
		cout<<"HelloWorld "<<i<<" \n";
		Sleep( 500 );
	}

	return 0;
}
```

Also we need to replace ``` <strstream.h> ``` by ``` <sstream> ``` i.e and use ``` istringstream, ostringstream, stringstream``` as neccessary.

Also some of the ``` <conio.h> ``` functions like ``` getch() ``` can be replaced.