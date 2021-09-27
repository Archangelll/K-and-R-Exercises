#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXPTR   10
#define MAXTOKEN 100
#define BUFSIZE  100

enum {NAME, PARENS, BRACKETS};

void dcl(void);
void dirdcl(void);
int  gettoken(void);
int  getch(void);
void ungetch(int);
int getDataType(void);

int  tokentype;            /* type of last token          */
char token[MAXTOKEN];      /* last token string           */
char name[MAXTOKEN];       /* identifier name             */
char datatype[MAXTOKEN];   /* data type = char, int, etc  */
char out[1000];            /* output string               */
char buf[BUFSIZE];         /* buffer for ungetch          */
int  bufp = 0;

int main(void) /* convert declaration to words */
{
	while (getDataType() != EOF) {  /* 1st token on line */
		dcl(); /* parse rest of line */
		if (tokentype != '\n') {
			while ((tokentype = getch()) != '\n' && tokentype != EOF) /* recover from input errors */
				;
			printf("syntax error\n");
		}
		printf("%s: %s %s\n", name, out, datatype);
		name[0] = '\0';
		out[0] = '\0';
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
			strcat(out, " function");
			return tokentype = PARENS;
		}
		else if (name[0] != '\0') {
			ungetch(c);
			strcat(out, " function (");
			do {
				char currname[MAXTOKEN];
				char currdatatype[MAXTOKEN];
				strcpy(currname, name);
				strcpy(currdatatype, datatype);
				name[0] = '\0';

				getDataType(); /* datatype (parameter list) */
				dcl(); /* parse rest of parameter */
				strcat(out, " ");
				strcat(out, datatype);
				strcat(out, " ");
				
				strcpy(name, currname);
				strcpy(datatype, currdatatype);

				if (tokentype == ',') {
					strcat(out, ", ");
					continue;
				}
				break;
			} while(1);
	
			if (tokentype != ')') {
				printf("missing )\n");
				ungetch(tokentype);
			}
			strcat(out, ")");
			return PARENS;
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
	int ns = 0, c;
	char nsw[MAXPTR][MAXTOKEN];
	char tmp[MAXTOKEN]; /* sprintf restrict-quaified parameter */

	while (1) {
		for (; (c = gettoken()) == '*'; ns++) /* count *'s */
			strcpy(nsw[ns], " pointer to");
		if (c == NAME && strcmp(token, "const") == 0) { /* const is the only pointer type qualifier learned so far */
			strcpy(tmp, nsw[ns-1]);
			sprintf(nsw[ns-1], " %s%s", token, tmp);
		}
		else
			break;
	}
	dirdcl();
	while (ns-- > 0)
		strcat(out, nsw[ns]);
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
			strcat(out, " returning");
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


int getDataType(void)
{
	int c;
	char *p = datatype;
	while ((c = getch()) == ' ' || c == '\t')
		;
	if (isalpha(c))
		do {
			for (*p++ = c; isalnum(c = getch()); )
				*p++ = c;
			if (c == ' ' || c == '\t')
				for (*p++ = ' '; (c = getch()) == ' ' || c == '\t'; )
					;
		} while (isalpha(c));
	*p = '\0';
	ungetch(c);
	return c;
}
