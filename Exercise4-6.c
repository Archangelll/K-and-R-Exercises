#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP    100  /* max size of operand or operator       */
#define MAXAR    100  /* max size of argument (math.h)         */
#define MAXVAL   100  /* maximum depth of val stack            */
#define BUFSIZE  100  /* buffer for ungetch                    */
#define NUMBER   '0'  /* signal that a number was found        */
#define MAXMNL   5    /* max identifier length (math.h)        */
#define FCE      0    /* error on functon call (getop)         */
#define AV       1    /* assignment // does nothing            */
#define VARCOUNT 27   /* number of variables                   */

int sp = 0;           /* next free stack position              */
int bufp = 0;         /* next free position in  buf            */
char buf[BUFSIZE];    /* buffer for ungetch                    */
double val[MAXVAL];   /* value stack                           */
char varI;            /* variable identifier                   */
char varA;            /* we will assign a value to varA        */
char varO;            /* operator before '='                   */
double var[VARCOUNT]; /* 26 uppercase letters and '_'          */
int varidtP;          /* varidt index                          */ 
char varidt[VARCOUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                         'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_'};

int getop(char []);
int getch(void);
void ungetch(int);
void push(double);
double pop(void);
double stomfunc(char s[]);

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
				if (sp > 0) {
					printf("\t%.8g\n", val[sp-1]);
					var[VARCOUNT - 1] = val[sp-1];
				}
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
			case 'm':
				push(stomfunc(s));
				break;
			case 'v':
				for (varidtP = 0; varidtP < VARCOUNT-1 && varidt[varidtP] != varA; ++varidtP) /* '_' is not allowed */
					;
				if (varidtP == VARCOUNT-1)
					printf("\tAssignment error: ';' out of context\n");
				else {
					switch(varO) {
						case 0:
							var[varidtP]  = val[sp-1];
							break;
						case '+':
							var[varidtP] += val[sp-1];
							val[sp-1]  = var[varidtP];
							break;
						case '-':
							var[varidtP] -= val[sp-1];
							val[sp-1]  = var[varidtP];
							break;
						case '*':
							var[varidtP] *= val[sp-1];
							val[sp-1]  = var[varidtP];
							break;
						case '/':
							var[varidtP] /= val[sp-1];
							val[sp-1]  = var[varidtP];
							break;
						case '%':
							var[varidtP] = fmod(var[varidtP], val[sp-1]);
							val[sp-1]  = var[varidtP];
							break;
						default:
							printf("Assignment error: %c is not a valid operator\n", varO);
							break;
					}
				}
				varA = 0;
				break;
			case 'V':
				for (varidtP = 0; varidtP < VARCOUNT && varidt[varidtP] != varI; ++varidtP)
					;
				push(var[varidtP]);
				break;
			case AV:
				break;
			case '\n':
				var[VARCOUNT - 1] = pop();
				printf("\t%.8g\n", var[VARCOUNT - 1]);
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
	else {
		printf("error: stack full, can't push %g\n", f);
		var[VARCOUNT - 1] = f;
	}
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
	int i, c, idfl;
	char fipb[MAXMNL];

	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	i = 0;
	/* we will consider a function to be any sequence of characters that:
	 * contains the following characters: '(' and ')'
	 * have a identifier that starts with one or more lowercase letters and optionally ends with 1 or 2 digits
	 * have a length <= MAXMNL
	 */
	if (islower(c)) {
		idfl = 1;
		while ((islower(fipb[i] = getch()) || (isdigit(fipb[i]) && idfl > 2)) && ++idfl <= MAXMNL) /* isdigit --> log10 atan2 */
			++i;

		if (fipb[i] == '(') {
			for (int j = 0; j <= i; ++j)
				s[j+1] = fipb[j];
			++i;
			while ((s[++i] = c = getch()) != ')' && c != '\n' && c != EOF)
				;
			if (s[i] == ')') {
				s[++i] = '\0';
				return 'm';
			}
			else {
				return FCE;
			}
		}
		else {
			for (int j = i; j >= 0; --j)
				ungetch(fipb[j]);
		}
	}
	else if (isupper(c) || c == '_') {
		while ((s[1] = getch()) == ' ' || c == '\t')
			;
		if (s[1] != '\n')
			s[2] = getch();
		else
			s[2] = 0;

		if ((s[1] == '=' || s[2] == '=') && c != '_') {
			if (s[2] == '=')
				varO = s[1];
			else {
				ungetch(s[2]);
				varO = 0;
			}
			varA = c;
			return AV;
		}
		else {
			varI = c;
			if (s[1] != '\n')
				ungetch(s[2]);
			ungetch(s[1]);
			return 'V'; /* just push varI onto stack  */
		}
	}
	s[1] = '\0';

	switch (s[0]) {
		case '-':
			if (isdigit(s[1] = c = getch()) || c == '.')
				++i;
			else {
				ungetch(c);
				c = '-';
			}
			break;
		case 'p': /* just print the top element of the stack          */
		case 'd': /* just duplicate the top element of the stack      */
		case 's': /* just swap the top two elements of the stack      */
		case 'c': /* just clear the stack                             */
			return s[0];
		case ';': /* just assign the top element of the stack to varA */
			return 'v';
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


/* calls the math.h function stored in s */
double stomfunc(char s[])
{
	char fn[MAXMNL+1];
	int  i, j;
	char p1[MAXAR];

	i = 0;
	while (s[i] != '(') {
		fn[i] = s[i];
		++i;
	}
	fn[i] = '\0';
	j = 0;
	while (s[++i] != ',' && s[i] != ')')
		p1[j++] = s[i];
	p1[j] = '\0';
	if (s[i] == ')') { /* the math.h function takes only one argument */
		if (strcmp(fn, "sin") == 0)
			return sin(atof(p1));
		else if (strcmp(fn, "cos") == 0)
			return cos(atof(p1));
		else if (strcmp(fn, "tan") == 0)
			return tan(atof(p1));
		else if (strcmp(fn, "asin") == 0)
			return asin(atof(p1));
		else if (strcmp(fn, "acos") == 0)
			return acos(atof(p1));
		else if (strcmp(fn, "atan") == 0)
			return atan(atof(p1));
		else if (strcmp(fn, "sinh") == 0)
			return sinh(atof(p1));
		else if (strcmp(fn, "cosh") == 0)
			return cosh(atof(p1));
		else if (strcmp(fn, "tanh") == 0)
			return tanh(atof(p1));
		else if (strcmp(fn, "exp") == 0)
			return exp(atof(p1));
		else if (strcmp(fn, "log") == 0)
			return log(atof(p1));
		else if (strcmp(fn, "log10") == 0)
			return log10(atof(p1));
		else if (strcmp(fn, "sqrt") == 0)
			return sqrt(atof(p1));
		else if (strcmp(fn, "ceil") == 0)
			return ceil(atof(p1));
		else if (strcmp(fn, "floor") == 0)
			return floor(atof(p1));
		else if (strcmp(fn, "fabs") == 0)
			return fabs(atof(p1));
	}
	else if (s[i] == ',') { /* the math.h function takes two argument */
		char p2[MAXAR];
		j = 0;
		while (s[++i] != ')')
			p2[j++] = s[i];
		p2[j] = '\0';
		
		if (strcmp(fn, "atan2") == 0)
			return atan2(atof(p1), atof(p2));
		else if (strcmp(fn, "pow") == 0)
			return pow(atof(p1), atof(p2));
		else if (strcmp(fn, "ldexp") == 0)
			return ldexp(atof(p1), atoi(p2));
		else if (strcmp(fn, "fmod") == 0)
			return fmod(atof(p1), atof(p2));
	}
	printf("error on function call, push 0\n");
	var[VARCOUNT - 1] = 0.0;
	return 0.0;
}
