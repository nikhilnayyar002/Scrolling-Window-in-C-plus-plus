/*

   Hi,

   Please study the funcions in this order for better insight:
   
   int main(void)
   void menu()
   void input(int t)
   void msg(int p), int isitvalid(char*p) , void showmsg(char*p) 
   void output(int t) 
   
   study sample 1 in the video at: 
      https://www.youtube.com/watch?v=AKy5GKpfbKc&feature=youtu.be
   
   Study sample 2 as you understood sample one. Also watch video above
   it includes sample 2 also.

*/

#include "scroll2.cpp"

/*

   Globally define the pointers for use in any part of program.

*/
swselec *sw;
swmain *sw1;
swmain *sw2;

/*

   We will use at as a buffer or memory for storing user input values.

*/
char str[60]="bro jjjjjjjjjjjjjjj ttttttttttttt ttttttttt tt";

/*

   Custom validator function

*/

int isitvalid(char*p) {
	switch(*p)
		{ case 'M':
		  case 'F':
		  case 'G': return 1;
		  default : return 0;
		}
}

/*

   Custom msg function. Here it outputs the message string to sw2 (Message window).


*/

void showmsg(char*p) {sw2->reset(); sw2->o<<p; sw2->out(0);}


/*

   Custom msg function. Here it outputs the message string to sw2 (Message window).

*/

void msg(int p) {
 sw2->reset();
 sw2->o<<"you choosed option: "<<p;
 sw2->out(0);
}

/*

   You can use special ":" and "[]" in strings to make a special output.

   For example if you are getting output as:

   Name: Nikhil Nayyar
   Address: D/90/B, Janak Puri,
   New Delhi, 110059
   DOB:2nd September 1999

   And you want to display it as (see the address part):

   Name: Nikhil Nayyar
   Address: D/90/B, Janak Puri,
            New Delhi, 110059  
   DOB:2nd September 1999

   To do that:

      sw1->o<<"Address:"<<str<<'[]';

   Here the ":" after "Address" actually starts the magic.
   Then after this is output:

      Address: D/90/B, Janak Puri,
               New Delhi, 110059 

   I used "[]" to stop the magic.

   So the standard way to output Name,Address and DOB will be:

        sw1->o<<"Name:"<<str<<'[]\n';
        sw1->o<<"Address:"<<str<<'[]\n';
        sw1->o<<"DOB:"<<str<<'[]\n';

*/



void output(int t) {
   sw1->o<<"Hi:"<<str<<'\n';
   sw1->o<<"[]This:"<<str<<'\n';
   sw1->o<<"[]this output breaks to next line since it overflowed.";
   sw1->o<<"\n6666666666666666999999999999999";
   sw1->out(1);
   sw1->tabswitch();
}

/*

   Here the callback is making output content on "console window" using:

      its "o" variable similar to cout.

   "in" funtion of scrolling I/O window is the input function. It can be used to input
   values for example here in console window. It has two overloads:  

   @int swmain::in(int v,char*buffer,int (*isvalid)(char*)=0,void (*sendmsg)(char*)=0,char*msg=0)
   @int swmain::in(char*buffer,int (*isvalid)(char*)=0,void (*sendmsg)(char*)=0,char*msg=0)

   
   you can call in function as follow:
      in(v,buffer,isvalid,sendmsg,msg)
      in(v,buffer,isvalid,sendmsg,msg)
      in(v,buffer,isvalid)
      in(v,buffer)
      in(buffer)

      in(buffer,isvalid,sendmsg,msg)
      in(buffer,isvalid,sendmsg,msg)
      in(buffer,isvalid)

   Here 
      v -> max no. of characters. If v not supplied max no. of characters is right side boundary 
           of window.
      buffer -> is used to store the inputted value after successfull input.
      isvalid-> of the form:
         int isvalid(char*)
         is user defined function to validate the input after user presses enter. If it returns 1 
         the input will be stored in buffer otherwise the user has to input again correctly. 
         If you want to break out press escape.
      sendmsg-> of the form:
         void sendmsg(char*)
         is user defined function designed to show message. Therefore it returns void and
         have message string as argument.
      msg-> the char* argument to pass to the sendmsg function.     

   Dont worry about the size of characters "v" you can set.

   Next,
   sw1->tabswitch();
      causes console window to become active. The main purpose is that user stays on this
      window even after the input. If he wants to scroll he can scroll using up and down
      arrow key.
   sw1->reset();
      It causes the output of window to reset. Means window is cleaned up.

   @void swmain::out(int ln=0)
      Please call this function if you want the output recorded in "o" to actually output
      on window.  
         with argument value 1 if you want to output "o" and start succesive output to start
         from new line in scrolling window.
         with argument value 0 if you want to output "o" and do not want to start succesive output
         from new line in scrolling window.

      You might be thinking why:

            sw1->o<<"Enter any thing:";
            sw1->in(str);

      here "Enter any thing:" got printed without calling "out". Here the "in" did the magic.
      The "in" function saw that "o" was unempty. It therefore first did output its content
      by calling "sw1->out(0)" inherently and then set for user to input. Otherwise always call
      "out" function if you want recorded output or content of "o" to really output at that
      instant of time.

*/

void input(int t) {
   sw1->o<<"Enter any thing:";
   sw1->in(str);
   sw1->o<<"Enter any thing(max 30 char):";
   sw1->in(30,str);
   sw1->o<<"Enter gender(wanna try 'H'):";
   sw1->in(1,str,isitvalid,showmsg,"Please enter correct gender (M,F or G)");
   sw1->tabswitch();
   sw1->reset();
   sw1->o<<str;
   sw1->out(1);
}
/*

Here the out function sets the items in menu in order:
   @void swselec::out(char*buffer,void (*callback)(int)=0)

      the first argument describes the item.
      the second argument is optional. It is callback function if passed will
      be called whenever you select that item.

   Item 2,5 and 6 has not set a callback. So that when user hits enter on one of them
   it would break out of window. 

   The callback must have a defination as:
      void callback(int);
   when the callback will be called, the int argument will receive the item no.
   For example if you hit enter on first item then if its registered callback
   will be called with integer argument value 1.


*/
void menu() {
   sw->out("1. Do a simple output.",output);
   sw->out("2. This is new line line since. xxx beats the rock you know");
   sw->out("3. I am good try input here",input);
   sw->out("4. Hey man see a msg",msg);
   sw->out("5. Big daddy v gONna suck you up bro undertaker is around baby...");
   sw->out("6. Go Back");
}
int main(void)
{
   /*

   Definations and prerequisites:

   @Active window: 
      It means the window which is currently active and can do input and output. Only
      onw window is active at a time.
   @window is dead
      It means the program breaked out of that window and control goes back to earlier function
      in which window was called to be active.
   @pointers:
      they are special variable that stores addresses. They are declared with a "*", and can be
      dereferenced with a "*":

      int b=10, *a=&b;
      cout<<" value of b is : "<<*a<<endl;

      there can be also pointer to pointer...
      Eg: pointer to a pointer: **a

      let us have b a int variable, a as int pointer to b and c as int pointer to a. Then:
      
      b=2, &b=0x1244354
      a=0x1244354, *a=2, &a=0x5544532
      b=0x5544532, *b=0x1244354, **b=2

   @typecasting:
      it can be done as:
         (type) (variable)

   @virtual polymorphism
      if 
         Base<-Derived1, Base<-Derived2
      and
         Base has virtual function "func"
      and 
         derived classes has implemented "func" somehow
      then
         we can use base class pointer to point to derived classes object
         and call their virtual function implementation. This results in
         dynamic polymorphism.

   @char*
      It means pointer to a string or character in memory.

   */


   clrscr();
   /*
      Create 3 windows and return pointers to these windows
      using new operator:

      Menu:  6,17->(y1,y2)| 3,29->(x1,x2)
      Console: 6,17->(y1,y2)| 35,77->(x1,x2)
      Message: 6,17->(y1,y2)| 3,29->(x1,x2)

      -------------------------------------------------------

      Constuctor available: 

      @ swselec -> create scrolling selection menu window
      @ swmain -> create scrolling input/output Window

      requires vertical ordinates (y1,y2), horizontal abscissa(x1,x2) and window_title
      and these parameters passed to the constructors as follows:

      swbase(int y1, int y2, int x1, int x2, char *t=0) {
         ..
      }

      swbase is abstract base class of swselec and swmain.
   */

   sw=new swselec(6,17,3,29,"Menu");	//y1,y2,x1,x2,window_title
   sw1=new swmain(6,17,35,77,"Console");  //y1,y2,x1,x2,window_title
   sw2=new swmain(22,23,3,77,"Message"); //y1,y2,x1,x2,window_title

   /*
      Link these windows by calling method: 
   
      @ void setnxtwin(swbase **ptr)
   
      Linkage is as follows:  sw->sw1, sw1->sw2, sw2->sw
      If current window is sw, then:
      on 1st pressing the tab button switches sw1 as active window and sw as inactive,
      on 2nd pressing the tab button switches sw2 as active window and sw1 as inactive,
      on 3rd pressing the tab button switches sw as active window and sw2 as inactive

      -------------------------------------------------------

      "to" is macro and it simplies writing complex expression:

      #define to(x) (swbase**)(&x)

   */

   sw->setnxtwin(to(sw1));
   sw1->setnxtwin(to(sw2));
   sw2->setnxtwin(to(sw));

   /*
   
      let start with sw window as current active window. For that:

      1. We will create a loop such as while.
      2. the tabswitch method (described below) sets a window active and wait
         for user input (key press). Escape, tab and enter has special meaning.
      3. We have set three windows and if we keep on pressing an inactive window
         will become active in order.
         What we have designed in this example is:
         
         1. A "menu" window for selecting items. Each item except last two has callback registered.
            (see menu defination above). Actually you can have all items set to some callback and
            except the last item. So that when we hit enter on that item the while loop breaks and
            we are in main. This is the practice not a hard code instruction.
         2. We have a "console" window where we can do input and output.
         3. We have "message" window to give user feedback like error messages or give control 
            instructions.

      -------------------------------------------------
      @ virtual swbase *tabswitch()=0;

      This function set the current window for user input.

      # Suppose any scrolling I/O window is active. It is active because currently
        the virtual tabswitch function is called on its object using the base class
        pointer:
          b->tabswitch();

        the tabswitch method sets a infinite loop and waits for user input:

        if 
         input is empty (cursor on screen remains at initial position) and "enter" is hit.
         then loop continues
        else if
          input is non-empty and "enter" is hit.
          then if 
                 nextwin is set then its address will be returned.
               else 
                 the infinte loop ends and window is dead.
       else if 
         escape is hit
          then if 
                 nextwin is set its address will be returned.
               else 
                 the infinte loop ends and window is dead.
       else if 
         tab is hit
          then if 
                 nextwin is set its address will be returned.
               else 
                 the infinte loop ends and window is dead.


      # Suppose any scrolling selection window is active. It is active because currently
        the virtual tabswitch function is called on its object using the base class
        pointer:
          b->tabswitch();
      
         the menu has to be created first before calling the above. In this program
         I have called a menu() function in main() before calling tabswitch of selection window.
         The working of menu function is described in its defination above.

        the tabswitch method sets a infinite loop and waits for user input (selection 
        of an item in menu):

        if 
          "enter" is hit.
          if 
           callback is registered then that callback will be called
          else
           the infinte loop ends and window is dead.
        else if 
         escape is hit
          then the infinte loop ends and window is dead.
        else if 
         tab is hit
          then if 
                 nextwin is set its address will be returned.
               else 
                 the infinte loop ends and window is dead.




   */

   swbase*b=sw;
   menu();
   while(b) { b=b->tabswitch(); }
   /*



   */
   sw->reset();
   /*

      descibed in details above.

   */
   sw->out("You just reseted this shit.");


   getch();

   /*

      clean up the memory.
   
   */
   delete sw; delete sw1; delete sw2;
   return 0;
}
