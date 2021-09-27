#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define BUFSIZE  100

enum {NAME, PARENS, BRACKETS};

void dcl(void);
void dirdcl(void);
int  gettoken(void);
int  getch(void);
void ungetch(int);

int  tokentype;            /* type of last token          */
char token[MAXTOKEN];      /* last token string           */
char name[MAXTOKEN];       /* identifier name             */
char datatype[MAXTOKEN];   /* data type = char, int, etc  */
char out[1000];            /* output string               */
char buf[BUFSIZE];         /* buffer for ungetch          */
int  bufp = 0;

int main(void) /* convert declaration to words */
{
	while (gettoken() != EOF) {  /* 1st token on line */
		strcpy(datatype, token); /* is the datatype   */
		out[0] = '\0';

		dcl(); /* parse rest of line */
		if (tokentype != '\n') {
			while ((tokentype = getch()) != '\n' && tokentype != EOF) /* recover from input errors */
				;
			printf("syntax error\n");
		}
		printf("%s: %s %s\n", name, out, datatype);
	}
	return 0;
}

int gettoken(void) /* return next token */
{
	int  c;
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(') {
		/* assume that:
		 * "(\n" == "()\n"
		 */
		while ((c = getch()) == ' ' || c == '\t')
			;
		if (c == ')' || c == '\n') {
			if (c == '\n') {
				printf("missing )\n");
				ungetch('\n');
			}
			strcpy(token, "()");
			return tokentype = PARENS;
		}
		else {
			ungetch(c);
			return tokentype = '(';
		}
	}
	else if (c == '[') {
		/* assume that:
		 * "x[y)" == "x[y])"
		 * "x[y\n" == "x[y]\n"
		 */
		for (*p++ = c; (*p = getch()) != ']' && *p != ')' && *p != '\n'; ++p)
			;
		if (*p != ']') {
			printf("missing ]\n");
			ungetch(*p);
			*p = ']';
		}
		*++p = '\0';
		return tokentype = BRACKETS;
	}
	else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch()); *p++ = c)
			;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	}
	else
		return tokentype = c;
}


/* dcl: parse a declarator */
void dcl(void)
{
	int ns;

	for (ns = 0; gettoken() == '*'; ns++) /* count *'s */
		;
	dirdcl();
	while (ns-- > 0)
		strcat(out, " pointer to");
}


/* dirdcl: parse a direct declarator */
void dirdcl(void)
{
	int type;

	if (tokentype == '(') {     /* ( dcl ) */
		dcl();
		if (tokentype != ')')
			printf("error: missing )\n");
	}
	else if (tokentype == NAME) /* variable name */
		strcpy(name, token);
	else
		printf("error: expected name or (dcl)\n");

	/* assume that:
	 * (((x\n == (((x)))\n
	 */
	while (tokentype != '\n' && (( type = gettoken()) == PARENS || type == BRACKETS))
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
}


int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}


void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
