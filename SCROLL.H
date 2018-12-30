#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <strstream.h>
#define  UP 72
#define  DOWN  80

void color(int x1,int y1,int x2,int y2, int cb=BLACK,int ct=WHITE)
 { textcolor(ct);
   textbackground(cb);
   for(int i=y1;i<=y2;++i)
     for(int j=x1;j<=x2;++j) {gotoxy(j,i);cprintf(" ");}
   textbackground(BLACK);
 }

struct line {  char*buf; char n;  line*next; line*prev; };
class box{
	void dimen(int, int, int, int);
	int y1, y2, x1, x2, active;
	public:
	int hsz, vsz, hst, vst;
	char *title;
	box() { active=0;}
	~box() {delete[] title;}
	box(int y11,int y22,int x11, int x22,char *t=0) {
	dimen(y11,y22,x11,x22);    active=0;  settitle(t);
	}
	void drawver(int y1,int y2,int x1);
	void drawhor(int y1,int x1,int x2, char p);
	void draw() {
		drawver(y1,y2,x1);
		drawver(y1,y2,x2);
		drawhor(y1,x1,x2,'U');
		drawhor(y2,x1,x2,'D');
		color(x1+1,y1+1,x2-1,y2-1,BLUE);
	}
	void setactive(int);
	int isactive() { if(active) return 1; else return 0; }
	void settitle(char*);
 };
void box::setactive(int t) {
	active=0;
	if(t) {textcolor(6); active=1;}
	drawver(y1,y2,x1);
	drawhor(y1,x1,x2,'U');
	drawhor(y2,x1,x2,'D');
	textcolor(WHITE);
 }

void box::dimen(int y11, int y22, int x11, int x22) {
		hsz=x22-x11+1;
		vsz=y22-y11+1;
		vst=y11; hst=x11;
		y1=y11-2; x1=x11-2; x2=x22+2; y2=y22+2;
 }
void box::settitle(char *t) {

	if(t) {
	 int g=strlen(t); int sz=(g>hsz?hsz:g);
	 title=new char[sz+1];
	 strncpy(title, t, sz);
	 *(title+sz) = '\0';
	 if(g>hsz) *(title+sz-2)=*(title+sz-1)='.';
	}
	else { delete[] title; title=0; }
}
void box::drawver(int y1,int y2,int x1) {
	char t=219;
	while(++y1<y2) {gotoxy(x1,y1); cprintf("%c",t);}
 }
void box::drawhor(int y1,int x1,int x2,char p) {
	char t; gotoxy(x1,y1);
	t=(p=='D')?223:220;

	while(x1<=x2) {
	 if(x1==hst)
	  if(title && p!='D')
	   {cprintf("%s",title);x1+=strlen(title);}
	 cprintf("%c",t);
	 ++x1;
	}
 }
class swbase {
	public:
	virtual swbase *tabswitch()=0;
};
struct svpt { int x; line*loc; svpt*next; svpt*prev;};
class swmain: public swbase {
	static char buffer[502];
	int count, x, y, yt;
	line *start,*end,*curr,*pos;
	svpt *spstart,*spend;
	swbase *nxtwin;
	int he,cv;
	void refresh();
	void refresh2(int,int);
	int newnode(char*);
	box b;
	public:
	ostrstream o;
	swmain(int y1, int y2, int x1, int x2, char *t=0)
	:b(y1,y2,x1,x2,t), o(buffer,502,ios::out) {start=0; reset(1); nxtwin=0;}
	~swmain() {reset(0);}
	int in(int,char*,int (*)(char*),void (*)(char*),char*);
	int in(char*,int (*)(char*),void (*)(char*),char*);
	void out(int);
	void reset(int);
	swbase *tabswitch();
	void setnxtwin(swbase *ptr) { nxtwin=ptr; }
	char *getbuf() { return buffer; }
       };
char swmain::buffer[502];
void swmain::reset(int t=1) {
	while(start!=0) {
	 line *temp=start; start=start->next;
	 delete[] temp->buf;
	 delete temp;
	}
	while (spstart!=0) {
	 svpt *temp=spstart; spstart=spstart->next;
	 delete temp;
	}
	if(t) {
	 b.draw();
	 pos=0; count=0; x=b.hst; y=yt=b.vst;
	 he=0; cv=b.vsz; buffer[0]=0; o.seekp(0);
	 spstart=spend=0;
	 start=new line;
	 start->buf=new char[b.hsz+1];
	 start->buf[0]=0;
	 start->next=start->prev=0;
	 pos=end=curr=start;
	 count=1;
	 gotoxy(x,y);
	}
 }

int swmain::newnode(char *data)
 {
  line *temp=new line;
  temp->buf=new char[b.hsz+1];
  strcpy(temp->buf,data);
  end->next=temp;
  temp->prev=end;
  end=temp;
  temp->next=0;
  curr=curr->next;
  x=b.hst; yt=++y;
  ++count;
  return 0;
 }

void swmain::out(int ln=0) {
   if(!start || !*buffer) return;
   o<<ends;
   if(y!=yt) refresh();
   int k,flag,flag2,c,setpos=0,t=count>b.vsz?1:0;
   char *p=0,*str=buffer;
   while(*str) {
    if(setpos) {
	int i=setpos-b.hst+1;   k=0;
	while(--i) curr->buf[k++]=' ';
	x=setpos;
    }
    p=0;
    gotoxy(x,y);
    k=x-b.hst;
    flag=0;
    for(; x<b.hst+b.hsz; ++x) {
     if(*str) {
      if(*str==' ') p=str;
      if(*str=='\n') {++str; break; }
      if(*str==':') setpos=x+1;
      curr->buf[k++]=*str;
      if(!t) printf("%c",*(str));
      ++str;
     }
     else { flag=1; break; }
    }
    curr->buf[k]=0;
    if(!flag){
      if(*str && *str!=' '&& p && (*(str-1)!=' '&& *(str-1)!='\n')) {
       int i=str-p;
       curr->buf[k-(str-p)]=0; str=p+1;
       while(--i) {gotoxy(--x,y); printf(" ");}
      }
      newnode("");
      if(*str=='[' && *(str+1)==']') {setpos=0; str+=2; }
      if(count>b.vsz) {t=1; pos=pos->next;y=b.vst;}
    }
   }
   o.seekp(0); buffer[0]=0;
   if(ln) {
    newnode("");
    if(count>b.vsz) {pos=pos->next;}
   }
   if(count>b.vsz) refresh();
}

void swmain::refresh() {    //refresh the output from start
    if(y!=yt) {pos=end; int k=b.vsz; while(--k) pos=pos->prev;}
    line*temp=pos; y=b.vst;
    int i=y,x=b.hst; he=count-b.vsz; cv=b.vsz-he;
    gotoxy(b.hst+b.hsz+1,b.vst-1);
    if(pos!=start) printf("%c",char(30)); else printf("%c",char(219));
    gotoxy(b.hst+b.hsz+1,b.vst+b.vsz); printf("%c",char(219));
    while(i<y+b.vsz) {gotoxy(x+b.hsz+1,i); printf("%c",char(177)); ++i;}
    i=0;
    if(cv>1)
     while(i<cv) { gotoxy(x+b.hsz+1,y+i+he); printf("%c",char(178)); ++i;}
    else
     {gotoxy(x+b.hsz+1,b.vst+b.vsz-1); printf("%c",char(178)); }
    while(temp) {
	  gotoxy(x,y++);
	  printf("%s",temp->buf);
	  for(int l=strlen(temp->buf);l<b.hsz; ++l)
	  printf(" ");
	  temp=temp->next;
    }
    yt=--y;
}
void swmain::refresh2(int p,int yt) {
	line *temp=pos;
	int pr,ne,k=count-b.vsz+1;
	gotoxy(b.hst+b.hsz+1,b.vst-1);
	if(pos!=start) printf("%c",char(30)); else printf("%c",char(219));
	gotoxy(b.hst+b.hsz+1,b.vst+b.vsz);
	if(y!=yt) printf("%c",char(31)); else printf("%c",char(219));
	if(p==UP) {
	 --he;
	 if(cv>0) {
	  gotoxy(b.hst+b.hsz+1,b.vst+he); printf("%c",char(178));
	  gotoxy(b.hst+b.hsz+1,b.vst+he+cv); printf("%c",char(177));
	 }
	 else {pr=(b.vsz*(he+1)/k); ne=(b.vsz*he/k);}
	}
	else {
	 ++he;
	 if(cv>0) {
	  gotoxy(b.hst+b.hsz+1,b.vst+he-1+cv); printf("%c",char(178));
	  gotoxy(b.hst+b.hsz+1,b.vst+he-1); printf("%c",char(177));
	 }
	 else {pr=(b.vsz*(he-1)/k), ne=(b.vsz*he/k);}
	}
	if(cv<=0) {
	 if(pr!=ne) {
	  gotoxy(b.hst+b.hsz+1,b.vst+ne); printf("%c",char(178));
	  gotoxy(b.hst+b.hsz+1,b.vst+pr); printf("%c",char(177));
	 }
	}
	for(int j=0; j<b.vsz; ++j) {
	       gotoxy(b.hst,j+b.vst);
	       printf("%s",temp->buf);
	       for(int l=strlen(temp->buf);l<b.hsz; ++l)
	       printf(" ");
	       temp=temp->next;
	}
}
int swmain::in(char*buffer,int (*isvalid)(char*)=0,void (*sendmsg)(char*)=0,char*msg=0)
 {
  return in(0,buffer,isvalid,sendmsg,msg);
 }
int swmain::in(int v,char*buffer,int (*isvalid)(char*)=0,void (*sendmsg)(char*)=0,char*msg=0)
 {
    if(swmain::buffer[0]) out();
    if(y!=yt) refresh();
    char p,*str=0; int i=x,k=0,w=b.hst+b.hsz-x+1,o=0,t,err=0;
    if(!b.isactive()) b.setactive(1);
    if(v==-1) w=0; else w=v?(v+1):w;
    if(w) {str=new char[w];}
    str[0]=0;
    while(1) {
	gotoxy(i,yt);  p=getch();
	if(p==0) {
	   p=getch();
	   if(p==UP && pos!=start) {
	     pos=pos->prev;  ++yt;
	     refresh2(UP,yt);
	     if(yt>y)_setcursortype(_NOCURSOR);
	     else _setcursortype(_NORMALCURSOR);
	   }
	   else if(p==DOWN && yt>y) {
	     pos=pos->next;  --yt; refresh2(DOWN,yt);
	     if(yt>y)_setcursortype(_NOCURSOR);
	     else _setcursortype(_NORMALCURSOR);
	     if(yt==y && str) {gotoxy(x,y);printf("%s",str+o);}
	   }
	}
	else if(yt==y && w) {
	   if(p==13 && str[0]) {
	     if(isvalid && !isvalid(str)) {
	      if(sendmsg) sendmsg(msg?msg:"");
	      if(!err) {
	       textbackground(RED);
	       gotoxy(x,y); for(t=x;t<b.hsz+b.hst-1;++t) cprintf(" ");
	       gotoxy(x,y); printf("%s",str+o);
	       textbackground(BLACK);
	       err=1;
	      }
	      continue;
	     }
	     if(err) {
	      textbackground(BLUE);
	      gotoxy(x,y); for(t=x;t<b.hsz+b.hst-1;++t) cprintf(" ");
	      gotoxy(x,y); printf("%s",str+o);
	      textbackground(BLACK);
	     }
	     strcat(curr->buf,str+o); break;
	   }
	   else if(p==13) continue;
	   else if(p!='\b'&&k<w-1 && p!='\t') {
	     str[k++]=p;  str[k]='\0';
	     if(i==b.hst+b.hsz) {
	      ++o; t=o;
	      while(t<k-1) {
	      gotoxy(x+t-o,y); printf("%c",*(str+t++));
	      }
	     }
	     else gotoxy(i++,yt);
	     printf("%c",p);
	   }
	   else if(p=='\b'&&i>x) {
	     if(o) {
	      --o; t=o;
	      while(t<k-1) {
	      gotoxy(x+t-o,y); printf("%c",*(str+t++));
	      }
	     }
	     else
	     {gotoxy(--i,yt);printf(" ");}
	     str[--k]='\0';
	   }
	}
	else if(p==9 && !w) {
		if(str) delete[] str;
		b.setactive(0);
		return 0;
	}
    }
    newnode("");
    if(count>b.vsz) {pos=pos->next;refresh();}
    strcpy(buffer,str);
    delete[] str;
    b.setactive(0);
    return 0;
 }
swbase *swmain::tabswitch() {
		in(-1,0);
		if(nxtwin) return nxtwin;
		else return 0;
}
//######################################

struct item {
	line*pos; line*end; item*next; item*prev;
	int (*cb)(int);  char p;
	};

class swselec: public swbase {
	int count;
	line *start,*pos,*curr,*end;
	item *istart,*ipos,*iend,*icurr;
	char exitch;
	swbase *nxtwin;
	void refresh(int);
	void refresh2(int);
	int newnode(char*);
	box b;
	public:
	swselec(int y1, int y2, int x1, int x2, char *t=0)
	:b(y1,y2,x1,x2,t) {
	 istart=0; start=0;   nxtwin=0; reset(1);
	}
	~swselec() {reset(0);}
	void exitctr(char c,char*str=0) {
		exitch=c;
		out(str?str:":Go Back:",0);
	}
	void out(char*,int (*)(int));
	void reset(int);
	virtual swbase *tabswitch();
	void setnxtwin(swbase *ptr) { nxtwin=ptr; }
       };
void swselec::reset(int t=1) {
	while(start!=0) {
	 line *temp=start; start=start->next;
	 delete[] temp->buf;
	 delete temp;
	}
	item *temp2;
	while(istart!=0) {
	 temp2=istart; istart=istart->next;
	 delete temp2;
	}
	if(t) {
	 b.draw();
	 pos=0; count=0; icurr=iend=0;
	 start=new line;
	 start->n=1;
	 start->buf=new char[b.hsz+1];
	 start->buf[0]=0;
	 start->next=start->prev=0;
	 pos=end=curr=start;
	 count=1;
	}
 }
int swselec::newnode(char *data)
 {
  line *temp=new line;
  temp->buf=new char[b.hsz+1];
  strcpy(temp->buf,data);
  temp->n=(end->n)+1;
  end->next=temp;
  temp->prev=end;
  end=temp;
  temp->next=0;
  curr=curr->next;
  ++count;
  return 0;
 }

void swselec::out(char*buffer,int (*callback)(int)=0) {
   if(!start || !*buffer) return;
   int k,flag,c,x=b.hst,y=end->n+b.vst-1;
   char *p=0,*str=buffer;
   item *t= new item;
   t->pos=end;
   t->prev=t->next=0;
   t->cb=callback?callback:0;
   if(!istart) {istart=iend=icurr=ipos=t;  t->p=1; }
   else { t->p=iend->p+1; iend->next=t; t->prev=iend; iend=t;}
   if(count<=b.vsz) {gotoxy(x-1,y); printf("%c",'>');}
   while(*str) {
    p=0;
    gotoxy(x,y);
    k=x-b.hst;
    flag=0;
    for(; x<b.hst+b.hsz; ++x) {
     if(*str) {
      if(*str==' ') p=str;
      curr->buf[k++]=*str;
      if(count<=b.vsz) printf("%c",*str);
      ++str;
     }
     else { flag=1; break; }
    }
    end->buf[k]=0;
    if(!flag){
      if(count<=b.vsz && *str && *str!=' '&& p && *(str-1)!=' ') {
       int i=str-p;
       curr->buf[k-(str-p)]=0; str=p+1;
       while(--i) {gotoxy(--x,y); printf(" ");}
      }
      newnode("");
      x=b.hst;  ++y;
    }
   }
   t->end=end;
   if(!istart->next) refresh(0);
   if(count>b.vsz)   refresh(1);
   newnode("");
}
void swselec::refresh(int g=0) {
	line *temp=icurr->pos,*temp2=icurr->next->end; int i=0;
	if(g) {gotoxy(b.hst+b.hsz+1,b.vst+b.vsz); printf("%c",char(31));}
	int x=b.hst,y=b.vst;
	textbackground(RED);
	while(temp!=temp2 && i<b.vsz &&!g) {
	       gotoxy(x,i+y);
	       cprintf("%s",temp->buf);
	       for(int l=strlen(temp->buf);l<b.hsz; ++l)
	       cprintf(" ");
	       temp=temp->next;
	       ++i;
	}
	textbackground(BLACK);
 }

void swselec::refresh2(int g=0) {
	line *temp=pos, *temp2=icurr->pos, *temp3=icurr->end->next;
	item *temp4=ipos;  int j=b.vst,k=b.vst+b.vsz-1;
	gotoxy(b.hst+b.hsz+1,b.vst-1);
	if(icurr!=istart) printf("%c",char(30)); else printf("%c",char(219));
	gotoxy(b.hst+b.hsz+1,b.vst+b.vsz);
	if(icurr!=iend) printf("%c",char(31)); else printf("%c",char(219));
	if(g==UP) {
	 temp=icurr->pos; j=temp->n-pos->n+b.vst;
	 k=icurr->next->end->n-pos->n+b.vst;
	 temp4=icurr;
	}
	else if(g==DOWN) {
	 temp=icurr->prev->pos; j=temp->n-pos->n+b.vst;
	 k=icurr->end->n-pos->n+b.vst;
	 temp4=icurr->prev;
	}
	for(; j<=k && (temp || g==1); ++j) {
	       gotoxy(b.hst,j);
	       if(temp && temp==temp4->pos) {
		printf("\b%c",'>');  temp4=temp4->next;
	       }
	       else printf("\b ");
	       if(temp && temp==temp2 && temp2!=temp3)
		 {textbackground(RED); temp2=temp2->next; }
	       else
		 textbackground(BLUE);
	       if(temp) cprintf("%s",temp->buf);
		for(int l=strlen(temp->buf);l<b.hsz; ++l)
	       cprintf(" ");
	       if(temp) temp=temp->next;
	}
	textbackground(BLACK);
}

swbase *swselec::tabswitch()
 {
    char p=0; int i,t; item *itemp;
    _setcursortype(_NOCURSOR);
    while(1) {
	if(!b.isactive()) {_setcursortype(_NOCURSOR);b.setactive(1);}
	p=getch();
	gotoxy(1,21);
	if(p==0) {
	   p=getch();
	   if(p==UP && icurr!=istart) {
	     icurr=icurr->prev; t=UP;
	     if(icurr->pos->n < pos->n) {pos=icurr->pos; ipos=icurr; t=0;}
	     refresh2(t);
	   }
	   else if(p==DOWN && icurr->next) {
	      icurr=icurr->next; i=pos->n + b.vsz -1;  t=DOWN;
	      if(icurr->end->n > i) {
	       t=0;
	       if(icurr->end->n - icurr->pos->n +1 <=b.vsz) {
		i=icurr->end->n - i;
		while(i) {
		 pos=pos->next; --i;
		 if(pos->prev==ipos->pos) ipos=ipos->next;
		}
	       }
	       else {pos=icurr->pos; ipos=icurr;}
	      }
	      refresh2(t);
	   }
	}
	else if(p==13 || (p>'0'&&p<=iend->p+'0') || p==exitch) {
	 b.setactive(0);
	 _setcursortype(_NORMALCURSOR);
	 if(icurr==iend || p==exitch) return 0;
	 itemp=istart;
	 if(p==13) itemp=icurr;
	 else {  t='1'; while(t!=p) {itemp=itemp->next; ++t; } }
	 if(itemp->cb) if(itemp->cb(itemp->p)) refresh2(1);
	}
	else if(p==9) {
	 b.setactive(0);
	 _setcursortype(_NORMALCURSOR);
	 if(nxtwin) return nxtwin;
	 else return 0;
	}
    }
 }