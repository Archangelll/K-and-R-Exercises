#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXOP   100  /* max size of operand or operator */
#define MAXVAL  100  /* maximum depth of val stack      */
#define BUFSIZE 100  /* buffer for ungetch              */
#define NUMBER  '0'  /* signal that a number was found  */

int sp = 0;          /* next free stack position        */
int bufp = 0;        /* next free position in  buf      */
char buf[BUFSIZE];   /* buffer for ungetch              */
double val[MAXVAL];  /* value stack                     */

int getop(char []);
int getch(void);
void ungetch(int);
void push(double);
double pop(void);

int main(void)
{
	int type;
	double op2, tmp;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
			case NUMBER:
				push(atof(s));
				break;
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '/':
				op2 = pop();
				if (op2 != 0.0)
					push(pop() / op2);
				else
					printf("error: zero divisor\n");
				break;
			case '%':
				op2 = pop();
				push(fmod(pop(), op2));
				break;
			case 'p':
				if (sp > 0)
					printf("\t%.8g\n", val[sp-1]);
				else
					printf("error: stack empty\n");
				break;
			case 'd':
				if (sp > 0)
					push(val[sp - 1]);
				else
					printf("error: stack empty\n");
				break;
			case 's':
				if (sp < 1)
					printf("error: stack empty\n");
				else if (sp < 2)
					printf("error: stack has only one element\n");
				else {
					tmp = val[sp - 2];
					val[sp - 2] = val[sp - 1];
					val[sp - 1] = tmp;
				}
				break;
			case 'c':
				sp = 0;
				break;
			case '\n':
				printf("\t%.8g\n", pop());
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}
	return 0;
}


/* push : push f onto value stack */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}


/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}


/* getop: get next operator or numeric operand */
int getop(char s[])
{
	int i, c;

	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';

	i = 0;
	switch (s[0]) {
		case '-':
			if (isdigit(s[1] = c = getch()) || c == '.')
				++i;
			else {
				ungetch(c);
				c = '-';
			}
			break;
		case 'p': /* just print the top element of the stack     */
		case 'd': /* just duplicate the top element of the stack */
		case 's': /* just swap the top two elements of the stack */
		case 'c': /* just clear the stack                        */
			return s[0];
	}

	if (!isdigit(c) && c != '.')
		return c;     /* not a number */
	if (isdigit(c))   /* collect integer part */
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')     /* collect fraction part */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}


/* get a (possibly pushed back) character */
int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}


/* push character back on input */
void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
