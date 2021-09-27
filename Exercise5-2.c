#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

int bufp = 0;
char buf[BUFSIZE];

int getfloat(double *);
int getch(void);
void ungetch(int);

int main(void)
{
	double pn;
	int    rv;
while(1){
	pn = 0.0;
	rv = getfloat(&pn);
	printf("number: %.20g\nreturned value: %d\n", pn, rv);
}
	return 0;
}


/* 0 : not a number // EOF : EOF // positive value : it's a number */
int getfloat(double *pn)
{
	double exp;
	int c;
	char sign = 0, tmp = 0;

	while (isspace(c = getch()))
		;
	if (c == '-' || c == '+') {
		sign = c;
		c = getch();
	}
	if (!isdigit(c) && c != EOF) {
		if (c == '.')
			tmp = getch();
		if (!isdigit(tmp)) {
			if (tmp)
				ungetch(tmp);
			ungetch(c);
			if (sign)
				ungetch(sign);
			return 0;
		}
		else
			ungetch(tmp);
	}
	for (*pn = 0.0; isdigit(c); c = getch())
		*pn = *pn * 10.0 + (c - '0');
	if (c == '.') {
		c = getch();
		if (isdigit(c)) {
			exp = 1.0;
			do {
				*pn = *pn * 10 + (c - '0');
				exp /= 10;
				c = getch();
			} while (isdigit(c));
			if (c != EOF)
				ungetch(c);
			*pn *= exp;
		}
		else {
			if (c != EOF)
				ungetch(c);
			ungetch('.');
		}
	}
	else if (c != EOF){
		ungetch(c);
	}
	if (sign == '-')
		*pn *= -1;
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
