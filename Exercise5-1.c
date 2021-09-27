#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

int bufp = 0;
char buf[BUFSIZE];

int getint(int *);
int getch(void);
void ungetch(int);

int main(void)
{
	int pn, rv;

	pn = 0;
	rv = getint(&pn);
	printf("number: %d\nreturned value: %d\n", pn, rv);

	return 0;
}


int getint(int *pn)
{
	char sign = 0;
	int c;

	while (isspace(c = getch())) /* skip white spaces */
		;
	if (c == '-' || c == '+') {
		sign = c;
		c = getch();
	}
	if (!isdigit(c) && c != EOF) {
		if (sign)
			ungetch(sign);
		ungetch(c); /* it's not a number */
		return 0;
	}
	for (*pn = 0; isdigit(c); c = getch())
		*pn = 10 * *pn + (c - '0');
	if (sign == '-')
		*pn *= -1;
	if (c != EOF)
		ungetch(c);
	return c;
}


int getch(void)
{
	return bufp > 0 ? buf[--bufp] : getchar();
}


void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
