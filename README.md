<h1>Scrolling-Window-in-C++ ( C++ v 3.0 supported)</h1>

Must watch video first for good insight:
<a href="https://youtu.be/AKy5GKpfbKc">Video Preview</a>

In short there is a header file you can include in your program such as Database Management Program to have clear look and clean code. It follows C++ v 3.0 standard. You just have to include SCROLL.H in your program as:

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

I will give examples to understand how these two classes are useful. Use these examples as templates.

<h2>Prerequisites</h2>

Must have good knowledge of following c++ features:
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



<h2>Support</h2>

You can study the Sample programs. For support contact nikhilnayyar002@gmail.com by giving me a msg with subject 'ScrollingWindow'. I will help you out.
