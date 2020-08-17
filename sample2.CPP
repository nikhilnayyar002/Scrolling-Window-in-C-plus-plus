

//#include<fstream.h>
//#include<stdlib.h>
//#include<iomanip.h>
//#include<dos.h>
#include "SCROLL.H"

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ global
swselec *sw; swmain *mw, *msg;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ menu

void myfavmsg(int t) {
	msg->reset();
	msg->o<<"Either press keys (1-"<<t<<") or roll UP/DOWN & ENTER.";
	msg->o<<" [ ESC-> exit/back ]\n[ TAB-> switch b/w windows.";
	msg->o<<" ]"<<"[ UP/DOWN -> inside win. to scroll ]";
	msg->out();
}
void menu1(int t) {
	swselec swt(6,17,3,29,"Students Menu");
	swselec *swo=sw; sw=&swt; swt.setnxtwin(to(mw));
	swbase*b=sw;
	sw->out("1. Create a new student ID");
	sw->out("2. Delete a student ID");
	sw->out("3. Delete all student ID's");
	sw->out("4. View a student ID");
	sw->out("5. Modify a student ID");
	sw->out("6. View all student ID's (sorted by admission no.)");
	sw->out("7. View all student ID's (sorted by class)");
	sw->out("8. Back");
	while(b) {
		if(b==sw) myfavmsg(8);
		b=b->tabswitch();
	}
	sw=swo;
}
void menu2(int t) {
	swselec swt(6,17,3,29,"Issue Menu");
	swselec *swo=sw; sw=&swt; swt.setnxtwin(to(mw));
	swbase*b=sw;
	sw->out("1. Change the fine charge");
	sw->out("2. Issue/return book - guest/teacher");
	sw->out("3. Issue a book");
	sw->out("4. Return back the book");
	sw->out("5. Clear out all the records (currently registered one)");
	sw->out("6. Delete the data file");
	sw->out("7. View the details of a specific book owner");
	sw->out("8. View the details of all the book owners");
	sw->out("9. View history");
	sw->out("10 View both history and details of all the book owners");
	sw->out("11 Back");
	while(b) {
		myfavmsg(11);
		b=b->tabswitch();
	}
	sw=swo;
}
void menu3(int t) {
	swselec swt(6,17,3,29,"Books Menu");
	swselec *swo=sw; sw=&swt; swt.setnxtwin(to(mw));
	swbase*b=sw;
	sw->out("1. Add new book");
	sw->out("2. Edit no. of book copies of a book");
	sw->out("3. Delete a book ( and its all copies)");
	sw->out("4. Delete all the books from a  book type");
	sw->out("5. Delete all the books");
	sw->out("6. Search for a book");
	sw->out("7. Display the book list");
	sw->out("8. Back");
	while(b) {
		myfavmsg(8);
		b=b->tabswitch();
	}
	sw=swo;
}
void menu() {
	sw->out("1. Student",menu1);
	sw->out("2. Issue",menu2);
	sw->out("3. Book",menu3);
	sw->out("4. Exit");
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ main
void main() {
	clrscr();
	sw=new swselec(6,17,3,29,"Main Menu");	//y1,y2,x1,x2,title
	mw=new swmain(6,17,35,77,"Console");
	msg=new swmain(22,23,3,77,"Message");
	sw->setnxtwin(to(mw)); mw->setnxtwin(to(msg));
	msg->setnxtwin(to(sw));
	swbase*b=sw;
	menu();
	while(b) {
		myfavmsg(4);
		b=b->tabswitch();
	}
	delete sw; delete msg; delete mw;
}
