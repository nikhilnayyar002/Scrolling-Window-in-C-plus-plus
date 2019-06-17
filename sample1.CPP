#include "scroll.h"
swselec *sw;
swmain *sw1;
swmain *sw2;
char str[60]="bro jjjjjjjjjjjjjjj ttttttttttttt ttttttttt tt";
int isitvalid(char*p) {
	switch(*p)
		{ case 'M':
		  case 'F':
		  case 'G': return 1;
		  default : return 0;
		}
}
void showmsg(char*p) {sw2->reset(); sw2->o<<p; sw2->out(0);}
void msg(int p) {
 sw2->reset();
 sw2->o<<"you choosed option: "<<p;
 sw2->out(0);
}
void output(int t) {
   sw1->o<<"Hi:"<<str<<'\n';
   sw1->o<<"[]This:"<<str<<'\n';
   sw1->o<<"[]this output breaks to next line since it overflowed.";
   sw1->o<<"\n6666666666666666999999999999999";
   sw1->out(1);
   sw1->tabswitch();
}

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
void menu() {
   sw->out("1. Do a simple output.",output);
   sw->out("2. This is new line line since. xxx beats the rock you know");
   sw->out("3. I am good try input here",input);
   sw->out("4. Hey man see a msg",msg);
   sw->out("5. Big daddy v gONna suck you up bro undertaker is around baby...");
   sw->out("6. Go Back");
}
int main(void)
{  clrscr();
   sw=new swselec(6,17,3,29,"Menu");	//y1,y2,x1,x2
   sw1=new swmain(6,17,35,77,"Console");  //y1,y2,x1,x2
   sw2=new swmain(22,23,3,77,"Message");
   sw->setnxtwin(to(sw1));
   sw1->setnxtwin(to(sw2));
   sw2->setnxtwin(to(sw));
   swbase*b=sw;
   menu();
   while(b) { b=b->tabswitch(); }
   sw->reset();
   sw->out("You just reseted this shit.");
   getch();
   delete sw; delete sw1; delete sw2;
   return 0;
}
