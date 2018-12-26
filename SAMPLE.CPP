#include "scroll.h"
swmain *sw1;
swselec *sw;
char str[60]="bro jjjjjjjjjjjjjjj ttttttttttttt ttttttttt tt";
int isitvalid(char*p) {
	switch(*p)
		{ case 'M':
		  case 'F':
		  case 'G': return 1;
		  default : return 0;
		}
}
void showmsg(char*p) {gotoxy(1,24); printf("%s",p);}
int msg(int p) { gotoxy(1,24); printf("you choosed option: %d",p); return 0; }
int output(int t) {
   sw1->o<<"Hi:"<<str<<'\n';
   sw1->o<<"[]This:"<<str<<'\n';
   sw1->o<<"[]this output breaks to next line since it overflowed.";
   sw1->o<<"\n6666666666666666999999999999999";
   sw1->out();
   sw1->tabswitch();
   return 0;
}

int input(int t) {
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
   return 0;
}
void menu() {
   sw->out("1. Do a simple output.",output);
   sw->out("2. This is new line line since. xxx beats the rock you know ");
   sw->out("3. I am good try input here ",input);
   sw->out("4. Hey man see a msg",msg);
   sw->out("5. Big daddy v gONna suck you up bro undertaker is around baby...");
   sw->exitctr('6');
}
int main(void)
{  clrscr();
   sw=new swselec(4,8,3,70);	//y1,y2,x1,x2
   sw1=new swmain(13,16,3,30);  //y1,y2,x1,x2
   sw->setnxtwin(sw1);
   sw1->setnxtwin(sw);
   swbase*b=sw;
   menu();
   while(b) { b=b->tabswitch(); }
   sw->reset();
   sw->out("You just reseted this shit.");
   gotoxy(1,25); printf("Press any key to quit...");
   getch();
   return 0;
}
