#include <stdio.h>

_Bool bufp = 0;      /* 0 --> buffer is empty           */
char buf;            /* buffer for ungetch              */

int getch(void);
void ungetch(int);

int main(void)
{
	int a;

	putchar(a = getch());
	ungetch(a);
	putchar(a = getch());
	putchar('\n');

	return 0;
}


int getch(void)
{
	return (bufp > 0) ? (bufp = 0, buf) : getchar();
}


void ungetch(int c)
{
	bufp = 1;
	buf = c;
}
