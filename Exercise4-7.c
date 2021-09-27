#include <stdio.h>

#define BUFSIZE 100

int bufp = 0;
char buf[BUFSIZE];

void ungetch(int);
void ungets(char []);

int main(void)
{
	char s[] = "hello";
	ungets(s);
	for (int i = 0; buf[i] != 0; ++i)
		putchar(buf[i]);
	putchar('\n');

	return 0;
}


void ungets(char s[])
{
	int i = 0;
	while (s[i] != '\0')
		++i;
	for (--i; i >= 0; --i)
		ungetch(s[i]);
}


void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
