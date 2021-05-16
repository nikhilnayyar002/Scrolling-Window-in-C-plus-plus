#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <strstream>
#include "./dev/lib/conio.cpp"

#define UP 72
#define DOWN 80
#define TO(x) (Swbase **)(&x)
#define MAX_BUFFER_SIZE 1000

HANDLE hStdOut;
winConio::ConsoleDimentions maxConsoleDimens;

void setGlobals(HANDLE hStdOut, winConio::ConsoleDimentions maxConsoleDimens)
{
	::hStdOut = hStdOut;
	::maxConsoleDimens = maxConsoleDimens;
}

struct line
{
	char *buf;
	char n;
	line *next;
	line *prev;
};
class Box
{
	void dimen(int, int, int, int);
	int y1, y2, x1, x2, active;
	char *title;

	winConio::ConsoleTextCapture conTxtCapObj;
	bool bufferExist;

public:
	int hsz, vsz, hst, vst;
	~Box()
	{
		if (title)
			delete[] title;
		if (bufferExist)
			conTxtCapObj.putText(x1, y1, x2, y2, hStdOut);
	}
	Box(int y11, int y22, int x11, int x22, const char *t)
		: conTxtCapObj(maxConsoleDimens)
	{
		dimen(y11, y22, x11, x22);
		active = 0;
		title = 0;
		settitle(t);
	}
	void drawver(int y1, int y2, int x1);
	void drawhor(int y1, int x1, int x2, char p);
	void draw(int t = 1)
	{
		if (t)
		{
			bufferExist = true;
			conTxtCapObj.getText(x1, y1, x2, y2, hStdOut);
		}
		drawver(y1, y2, x1);
		drawver(y1, y2, x2);
		drawhor(y1, x1, x2, 'U');
		drawhor(y2, x1, x2, 'D');
		winConio::paintBackground(x1 + 1, y1 + 1, x2 - 1, y2 - 1, winConio::BLUE, hStdOut);
	}
	void setactive(int);
	int isactive()
	{
		if (active)
			return 1;
		else
			return 0;
	}
	void settitle(const char *);
	char *gettitle() { return title; }
};
void Box::setactive(int t)
{
	active = 0;
	if (t)
	{
		winConio::setTextColor(winConio::YELLOW, hStdOut);
		active = 1;
	}
	drawver(y1, y2, x1);
	drawhor(y1, x1, x2, 'U');
	drawhor(y2, x1, x2, 'D');
	winConio::setTextColor(winConio::WHITE, hStdOut);
}

void Box::dimen(int y11, int y22, int x11, int x22)
{
	hsz = x22 - x11 + 1;
	vsz = y22 - y11 + 1;
	vst = y11;
	hst = x11;
	y1 = y11 - 2;
	x1 = x11 - 2;
	x2 = x22 + 2;
	y2 = y22 + 2;
}
void Box::settitle(const char *t)
{

	if (t)
	{
		int g = strlen(t);
		int sz = (g > hsz ? hsz : g);
		title = new char[sz + 1];
		strncpy(title, t, sz);
		*(title + sz) = '\0';
		if (g > hsz)
			*(title + sz - 2) = *(title + sz - 1) = '.';
	}
	else
	{
		delete[] title;
		title = 0;
	}
}
void Box::drawver(int y1, int y2, int x1)
{
	char t = 219;
	while (++y1 < y2)
	{
		winConio::gotoxy(x1, y1, hStdOut);
		std::cout << t;
	}
}
void Box::drawhor(int y1, int x1, int x2, char p)
{
	char t;
	winConio::gotoxy(x1, y1, hStdOut);
	t = (p == 'D') ? 223 : 220;

	while (x1 <= x2)
	{
		if (x1 == hst)
			if (title && p != 'D')
			{
				std::cout << title;
				x1 += strlen(title);
			}
		std::cout << t;
		++x1;
	}
}
class Swbase
{
protected:
	Swbase **nxtwin;
	int count;
	Box box;
	line *start, *end, *curr, *pos;
	virtual int newnode(const char *) = 0;

public:
	Swbase(int y1, int y2, int x1, int x2, const char *t)
		: box(y1, y2, x1, x2, t)
	{
		start = 0;
		nxtwin = 0;
	}
	virtual Swbase *tabswitch() = 0;
	void setnxtwin(Swbase **ptr) { nxtwin = ptr; }
};
class Swmain : public Swbase
{
	static char buffer[MAX_BUFFER_SIZE];
	int x, y, yt;
	int he, cv;
	void refresh();
	void refresh2(int, int);
	int newnode(const char *);

public:
	std::ostrstream o;
	Swmain(int y1, int y2, int x1, int x2, const char *t)
		: Swbase(y1, y2, x1, x2, t), o(buffer, MAX_BUFFER_SIZE, std::ios::out)
	{
		reset(1);
	}

	~Swmain() { reset(0); }
	int in(int, char *, int (*)(char *), void (*)(const char *), const char *);
	int in(char *, int (*)(char *), void (*)(const char *), const char *);
	void out(int);
	void reset(int);
	Swbase *tabswitch();
	char *getbuf() { return buffer; }
};
char Swmain::buffer[MAX_BUFFER_SIZE];
void Swmain::reset(int t = 1)
{
	int g = start ? 1 : 0;
	while (start != 0)
	{
		line *temp = start;
		start = start->next;
		delete[] temp->buf;
		delete temp;
	}
	if (t)
	{
		if (g)
			box.draw(0);
		else
			box.draw();
		pos = 0;
		count = 0;
		x = box.hst;
		y = yt = box.vst;
		he = 0;
		cv = box.vsz;
		buffer[0] = 0;
		o.seekp(0);
		start = new line;
		start->buf = new char[box.hsz + 1];
		start->buf[0] = 0;
		start->next = start->prev = 0;
		pos = end = curr = start;
		count = 1;
		winConio::gotoxy(x, y, hStdOut);
	}
}

int Swmain::newnode(const char *data)
{
	line *temp = new line;
	temp->buf = new char[box.hsz + 1];
	strcpy(temp->buf, data);
	end->next = temp;
	temp->prev = end;
	end = temp;
	temp->next = 0;
	curr = curr->next;
	x = box.hst;
	yt = ++y;
	++count;
	return 0;
}

void Swmain::out(int ln = 0)
{
	if (!start || !*buffer)
		return;
	o << std::ends;
	if (y != yt)
		refresh();
	int k, flag, flag2, c, setpos = 0, t = count > box.vsz ? 1 : 0;
	char *p = 0, *str = buffer;
	while (*str)
	{
		if (setpos)
		{
			int i = setpos - box.hst + 1;
			k = 0;
			while (--i)
				curr->buf[k++] = ' ';
			x = setpos;
		}
		p = 0;
		winConio::gotoxy(x, y, hStdOut);
		k = x - box.hst;
		flag = 0;
		for (; x < box.hst + box.hsz; ++x)
		{
			if (*str)
			{
				if (*str == ' ')
					p = str;
				if (*str == '\n')
				{
					++str;
					break;
				}
				if (*str == ':')
					setpos = x + 1;
				curr->buf[k++] = *str;
				if (!t)
					printf("%c", *(str));
				++str;
			}
			else
			{
				flag = 1;
				break;
			}
		}
		curr->buf[k] = 0;
		if (!flag)
		{
			if (*str && *str != ' ' && p && (*(str - 1) != ' ' && *(str - 1) != '\n'))
			{
				int i = str - p;
				curr->buf[k - (str - p)] = 0;
				str = p + 1;
				while (--i)
				{
					winConio::gotoxy(--x, y, hStdOut);
					printf(" ");
				}
			}
			newnode("");
			if (*str == '[' && *(str + 1) == ']')
			{
				setpos = 0;
				str += 2;
			}
			if (count > box.vsz)
			{
				t = 1;
				pos = pos->next;
				y = box.vst;
			}
		}
	}
	o.seekp(0);
	buffer[0] = 0;
	if (ln)
	{
		newnode("");
		if (count > box.vsz)
		{
			pos = pos->next;
		}
	}
	if (count > box.vsz)
		refresh();
}

void Swmain::refresh()
{ //refresh the output from start
	if (y != yt)
	{
		pos = end;
		int k = box.vsz;
		while (--k)
			pos = pos->prev;
	}
	line *temp = pos;
	y = box.vst;
	int i = y, x = box.hst;
	he = count - box.vsz;
	cv = box.vsz - he;
	winConio::gotoxy(box.hst + box.hsz + 1, box.vst - 1, hStdOut);
	if (pos != start)
		printf("%c", char(30));
	else
		printf("%c", char(219));
	winConio::gotoxy(box.hst + box.hsz + 1, box.vst + box.vsz, hStdOut);
	printf("%c", char(219));
	while (i < y + box.vsz)
	{
		winConio::gotoxy(x + box.hsz + 1, i, hStdOut);
		printf("%c", char(177));
		++i;
	}
	i = 0;
	if (cv > 1)
		while (i < cv)
		{
			winConio::gotoxy(x + box.hsz + 1, y + i + he, hStdOut);
			printf("%c", char(178));
			++i;
		}
	else
	{
		winConio::gotoxy(x + box.hsz + 1, box.vst + box.vsz - 1, hStdOut);
		printf("%c", char(178));
	}
	while (temp)
	{
		winConio::gotoxy(x, y++, hStdOut);
		printf("%s", temp->buf);
		for (int l = strlen(temp->buf); l < box.hsz; ++l)
			printf(" ");
		temp = temp->next;
	}
	yt = --y;
}
void Swmain::refresh2(int p, int yt)
{
	line *temp = pos;
	int pr, ne, k = count - box.vsz + 1;
	winConio::gotoxy(box.hst + box.hsz + 1, box.vst - 1, hStdOut);
	if (pos != start)
		printf("%c", char(30));
	else
		printf("%c", char(219));
	winConio::gotoxy(box.hst + box.hsz + 1, box.vst + box.vsz, hStdOut);
	if (y != yt)
		printf("%c", char(31));
	else
		printf("%c", char(219));
	if (p == UP)
	{
		--he;
		if (cv > 0)
		{
			winConio::gotoxy(box.hst + box.hsz + 1, box.vst + he, hStdOut);
			printf("%c", char(178));
			winConio::gotoxy(box.hst + box.hsz + 1, box.vst + he + cv, hStdOut);
			printf("%c", char(177));
		}
		else
		{
			pr = (box.vsz * (he + 1) / k);
			ne = (box.vsz * he / k);
		}
	}
	else
	{
		++he;
		if (cv > 0)
		{
			winConio::gotoxy(box.hst + box.hsz + 1, box.vst + he - 1 + cv, hStdOut);
			printf("%c", char(178));
			winConio::gotoxy(box.hst + box.hsz + 1, box.vst + he - 1, hStdOut);
			printf("%c", char(177));
		}
		else
		{
			pr = (box.vsz * (he - 1) / k), ne = (box.vsz * he / k);
		}
	}
	if (cv <= 0)
	{
		if (pr != ne)
		{
			winConio::gotoxy(box.hst + box.hsz + 1, box.vst + ne, hStdOut);
			printf("%c", char(178));
			winConio::gotoxy(box.hst + box.hsz + 1, box.vst + pr, hStdOut);
			printf("%c", char(177));
		}
	}
	for (int j = 0; j < box.vsz; ++j)
	{
		winConio::gotoxy(box.hst, j + box.vst, hStdOut);
		printf("%s", temp->buf);
		for (int l = strlen(temp->buf); l < box.hsz; ++l)
			printf(" ");
		temp = temp->next;
	}
}
int Swmain::in(char *buffer, int (*isvalid)(char *) = 0, void (*sendmsg)(const char *) = 0, const char *msg = 0)
{
	return in(0, buffer, isvalid, sendmsg, msg);
}
int Swmain::in(int v, char *buffer, int (*isvalid)(char *) = 0, void (*sendmsg)(const char *) = 0, const char *msg = 0)
{
	if (Swmain::buffer[0])
		out();
	if (y != yt)
		refresh();
	char p, *str = 0;
	int i = x, k = 0, w = box.hst + box.hsz - x + 1, o = 0, t, err = 0;
	if (!box.isactive())
		box.setactive(1);
	if (v == -1)
		w = 0;
	else
		w = v ? (v + 1) : w;
	if (w)
		str = new char[w];
	str[0] = 0;
	while (1)
	{
		winConio::gotoxy(i, yt, hStdOut);
		p = getch();
		if (p == 0)
		{
			p = getch();
			if (p == UP && pos != start)
			{
				pos = pos->prev;
				++yt;
				refresh2(UP, yt);
				if (yt > y)
					winConio::displayCursor(false, hStdOut);
				else
					winConio::displayCursor(true, hStdOut);
			}
			else if (p == DOWN && yt > y)
			{
				pos = pos->next;
				--yt;
				refresh2(DOWN, yt);
				if (yt > y)
					winConio::displayCursor(false, hStdOut);
				else
					winConio::displayCursor(true, hStdOut);
				if (yt == y && str)
				{
					winConio::gotoxy(x, y, hStdOut);
					printf("%s", str + o);
				}
			}
		}
		else if (yt == y && w)
		{
			if (p == 13 && str[0])
			{
				if (isvalid && !isvalid(str))
				{
					if (sendmsg)
						sendmsg(msg ? msg : "");
					if (!err)
					{
						winConio::setBackgroundColor(winConio::RED, hStdOut);
						winConio::gotoxy(x, y, hStdOut);
						for (t = x; t < box.hsz + box.hst - 1; ++t)
							std::cout << ' ';
						winConio::gotoxy(x, y, hStdOut);
						printf("%s", str + o);
						winConio::setBackgroundColor(winConio::BLACK, hStdOut);
						err = 1;
					}
					continue;
				}
				if (err)
				{
					winConio::setBackgroundColor(winConio::BLUE, hStdOut);
					winConio::gotoxy(x, y, hStdOut);
					for (t = x; t < box.hsz + box.hst - 1; ++t)
						std::cout << ' ';
					winConio::gotoxy(x, y, hStdOut);
					printf("%s", str + o);
					winConio::setBackgroundColor(winConio::BLACK, hStdOut);
				}
				strcat(curr->buf, str + o);
				break;
			}
			else if (p == 13)
				continue;
			else if (p == 27)
			{
				*str = 0;
				break;
			}
			else if (p != '\b' && k < w - 1 && p != '\t')
			{
				str[k++] = p;
				str[k] = '\0';
				if (i == box.hst + box.hsz)
				{
					++o;
					t = o;
					while (t < k - 1)
					{
						winConio::gotoxy(x + t - o, y, hStdOut);
						printf("%c", *(str + t++));
					}
				}
				else
					winConio::gotoxy(i++, yt, hStdOut);
				printf("%c", p);
			}
			else if (p == '\b' && i > x)
			{
				if (o)
				{
					--o;
					t = o;
					while (t < k - 1)
					{
						winConio::gotoxy(x + t - o, y, hStdOut);
						printf("%c", *(str + t++));
					}
				}
				else
				{
					winConio::gotoxy(--i, yt, hStdOut);
					printf(" ");
				}
				str[--k] = '\0';
			}
		}
		else if (p == 9 && !w)
		{
			if (str)
				delete[] str;
			box.setactive(0);
			return 0;
		}
	}
	newnode("");
	if (count > box.vsz)
	{
		pos = pos->next;
		refresh();
	}
	strcpy(buffer, str);
	delete[] str;
	box.setactive(0);
	return 0;
}
Swbase *Swmain::tabswitch()
{
	in(-1, 0);
	if (nxtwin)
		return *nxtwin;
	else
		return 0;
}
//######################################

struct item
{
	line *pos;
	line *end;
	item *next;
	item *prev;
	void (*cb)(int);
	char p;
};

class Swselec : public Swbase
{
	item *istart, *ipos, *iend, *icurr;
	void refresh(int);
	void refresh2(int);
	int newnode(const char *);

public:
	Swselec(int y1, int y2, int x1, int x2, const char *t)
		: Swbase(y1, y2, x1, x2, t)
	{
		istart = 0;
		reset(1);
	}
	~Swselec() { reset(0); }
	void out(const char *, void (*)(int));
	void reset(int);
	virtual Swbase *tabswitch();
};
void Swselec::reset(int t = 1)
{
	int g = start ? 1 : 0;
	while (start != 0)
	{
		line *temp = start;
		start = start->next;
		delete[] temp->buf;
		delete temp;
	}
	item *temp2;
	while (istart != 0)
	{
		temp2 = istart;
		istart = istart->next;
		delete temp2;
	}
	if (t)
	{
		if (g)
			box.draw(0);
		else
			box.draw();
		pos = 0;
		count = 0;
		icurr = iend = 0;
		start = new line;
		start->n = 1;
		start->buf = new char[box.hsz + 1];
		start->buf[0] = 0;
		start->next = start->prev = 0;
		pos = end = curr = start;
		count = 1;
	}
}
int Swselec::newnode(const char *data)
{
	line *temp = new line;
	temp->buf = new char[box.hsz + 1];
	strcpy(temp->buf, data);
	temp->n = (end->n) + 1;
	end->next = temp;
	temp->prev = end;
	end = temp;
	temp->next = 0;
	curr = curr->next;
	++count;
	return 0;
}

void Swselec::out(const char *buffer, void (*callback)(int) = 0)
{
	if (!start || !*buffer)
		return;
	int k, flag, c, x = box.hst, y = end->n + box.vst - 1;
	char *p = 0, *str = const_cast<char *>(buffer);
	item *t = new item;
	t->pos = end;
	t->prev = t->next = 0;
	t->cb = callback ? callback : 0;
	if (!istart)
	{
		istart = iend = icurr = ipos = t;
		t->p = 1;
	}
	else
	{
		t->p = iend->p + 1;
		iend->next = t;
		t->prev = iend;
		iend = t;
	}
	if (count <= box.vsz)
	{
		winConio::gotoxy(x - 1, y, hStdOut);
		printf("%c", '>');
	}
	while (*str)
	{
		p = 0;
		winConio::gotoxy(x, y, hStdOut);
		k = x - box.hst;
		flag = 0;
		for (; x < box.hst + box.hsz; ++x)
		{
			if (*str)
			{
				if (*str == ' ')
					p = str;
				curr->buf[k++] = *str;
				if (count <= box.vsz)
					printf("%c", *str);
				++str;
			}
			else
			{
				flag = 1;
				break;
			}
		}
		end->buf[k] = 0;
		if (!flag)
		{
			if (count <= box.vsz && *str && *str != ' ' && p && *(str - 1) != ' ')
			{
				int i = str - p;
				curr->buf[k - (str - p)] = 0;
				str = p + 1;
				while (--i)
				{
					winConio::gotoxy(--x, y, hStdOut);
					printf(" ");
				}
			}
			newnode("");
			x = box.hst;
			++y;
		}
	}
	t->end = end;
	if (!istart->next)
		refresh(0);
	if (count > box.vsz)
		refresh(1);
	newnode("");
}
void Swselec::refresh(int g = 0)
{
	line *temp = icurr->pos, *temp2 = icurr->next->end;
	int i = 0;
	if (g)
	{
		winConio::gotoxy(box.hst + box.hsz + 1, box.vst + box.vsz, hStdOut);
		printf("%c", char(31));
	}
	int x = box.hst, y = box.vst;
	winConio::setBackgroundColor(winConio::RED, hStdOut);
	while (temp != temp2 && i < box.vsz && !g)
	{
		winConio::gotoxy(x, i + y, hStdOut);
		std::cout << temp->buf;
		for (int l = strlen(temp->buf); l < box.hsz; ++l)
			std::cout << ' ';
		temp = temp->next;
		++i;
	}
	winConio::setBackgroundColor(winConio::BLACK, hStdOut);
}

void Swselec::refresh2(int g = 0)
{
	line *temp = pos, *temp2 = icurr->pos, *temp3 = icurr->end->next;
	item *temp4 = ipos;
	int j = box.vst, k = box.vst + box.vsz - 1;
	winConio::gotoxy(box.hst + box.hsz + 1, box.vst - 1, hStdOut);
	if (icurr != istart)
		printf("%c", char(30));
	else
		printf("%c", char(219));
	winConio::gotoxy(box.hst + box.hsz + 1, box.vst + box.vsz, hStdOut);
	if (icurr != iend)
		printf("%c", char(31));
	else
		printf("%c", char(219));
	if (g == UP)
	{
		temp = icurr->pos;
		j = temp->n - pos->n + box.vst;
		k = icurr->next->end->n - pos->n + box.vst;
		temp4 = icurr;
	}
	else if (g == DOWN)
	{
		temp = icurr->prev->pos;
		j = temp->n - pos->n + box.vst;
		k = icurr->end->n - pos->n + box.vst;
		temp4 = icurr->prev;
	}
	for (; j <= k && (temp || g == 1); ++j)
	{
		winConio::gotoxy(box.hst, j, hStdOut);
		if (temp && temp == temp4->pos)
		{
			printf("\b%c", '>');
			temp4 = temp4->next;
		}
		else
			printf("\b ");
		if (temp && temp == temp2 && temp2 != temp3)
		{
			winConio::setBackgroundColor(winConio::RED, hStdOut);
			temp2 = temp2->next;
		}
		else
			winConio::setBackgroundColor(winConio::BLUE, hStdOut);
		if (temp)
			std::cout << temp->buf;
		for (int l = strlen(temp->buf); l < box.hsz; ++l)
			std::cout << ' ';
		if (temp)
			temp = temp->next;
	}
	winConio::setBackgroundColor(winConio::BLACK, hStdOut);
}

Swbase *Swselec::tabswitch()
{
	char p = 0;
	int i, t;
	item *itemp;
	winConio::displayCursor(false, hStdOut);
	while (1)
	{
		if (!box.isactive())
		{
			winConio::displayCursor(false, hStdOut);
			box.setactive(1);
		}
		p = getch();
		winConio::gotoxy(1, 21, hStdOut);
		if (p == 0)
		{
			p = getch();
			if (p == UP && icurr != istart)
			{
				icurr = icurr->prev;
				t = UP;
				if (icurr->pos->n < pos->n)
				{
					pos = icurr->pos;
					ipos = icurr;
					t = 0;
				}
				refresh2(t);
			}
			else if (p == DOWN && icurr->next)
			{
				icurr = icurr->next;
				i = pos->n + box.vsz - 1;
				t = DOWN;
				if (icurr->end->n > i)
				{
					t = 0;
					if (icurr->end->n - icurr->pos->n + 1 <= box.vsz)
					{
						i = icurr->end->n - i;
						while (i)
						{
							pos = pos->next;
							--i;
							if (pos->prev == ipos->pos)
								ipos = ipos->next;
						}
					}
					else
					{
						pos = icurr->pos;
						ipos = icurr;
					}
				}
				refresh2(t);
			}
		}
		else if (p == 13 || (p > '0' && p <= iend->p + '0') || p == 27)
		{
			box.setactive(0);
			winConio::displayCursor(true, hStdOut);
			if (p == 27)
				return 0;
			itemp = istart;
			if (p == 13)
				itemp = icurr;
			else
			{
				t = '1';
				while (t != p)
				{
					itemp = itemp->next;
					++t;
				}
			}
			if (itemp->cb)
				itemp->cb(itemp->p);
			else
				return 0;
		}
		else if (p == 9)
		{
			box.setactive(0);
			winConio::displayCursor(true, hStdOut);
			if (nxtwin)
				return *nxtwin;
			else
				return 0;
		}
	}
}