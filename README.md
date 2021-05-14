<h1>Scrolling-Window-in-C++</h1>

[![Generic badge](https://img.shields.io/badge/OS-Windows-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/C++-Stable-green.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/Mingw-gcc-green.svg)](https://shields.io/)
\
\
Must watch video first for good insight:
[Video Preview](https://youtu.be/AKy5GKpfbKc)
\
For future support of C++ Current Standard read markdown file 
[C++_CUR_STD_SUPPORT.md](./C++_CUR_STD_SUPPORT.md)

[Summary](https://nikhilnayyar.web.app/projects#scrolling-window)

<h2>Introduction</h2>

In short there is a header file you can include in your program such as Database Management Program to have clear look and clean code. You just have to include SCROLL.H in your program as:

<pre>
#include "scroll.h"
</pre>

It includes a two classes : 
<ul>
<li>scrolling input/output Window</li>
<li>scrolling selection Menu Window</li>
</ul>

<h2>Why i developed this file?</h2>

This is a tweak something i wanted when i was in 12th standard when i was creating a Library Database Project in C++ using <b>Turbo C++ v 3.0</b>. I wanted to present the database program in good manner.

<h2>How easily i could use it?</h2>

Just go through sample1.cpp follow the instructions.

<h2>Prerequisites</h2>

Must have good knowledge of following c++ features along with oops:
<ul>
<li>Pointers</li>
</ul>
<h2>Scrolling input/output Window</h2>

<ol type="1">
<li>saves information that render in this window so that you can scroll and look back. (Similar to notepad in windows)</li>
<li><b>do output</b> using object similar to iostream <b>cout</b> object</li>
<li><b>does not support</b> input similar to iostream <b>cin</b> object</li>
<li>if overflow occurs words <b>wrap up at the end of line </b></li>
<li> a special filter is included: <b>':'</b> and <b> '[]'</b>. Will explain in example.</li>

<li>Input data similar to GUI textbox you see. You can  supply <b>fixed width input, automatic or an input with     validity check</b> along with msgs to pop up if incorrect input (then reinput correctly).</li>
<li>Scrollbars will pop up when necessary</li>
<li>Tab Switching support so that you can switch between multiple Scrolling input/output Windows or scrolling selection Menu Windows. You get an orange colored bordered layout whenever one window is active.</li>
</ol>

<h3>What you can customize?</h3>
Generally you don't need to customize anything. The visual effects are all fine. If you really need to change background colors or text color you can customize the scroll.h file. I have actually hard coded values.

<h2>Scrolling selection Menu Windows</h2>

Offers item selection control where each item is bounded to a function which it triggers on selection through enter key or keycode (I will explain you in example). It shares some features of Scrolling input/output Window.

<h2></h2>

<h2>Support</h2>

You can study the Sample programs. For support contact nikhilnayyar002@gmail.com by giving me a msg with subject 'ScrollingWindow'. I will help you out.
