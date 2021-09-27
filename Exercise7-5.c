#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXOP   100  /* max size of operand or operator */
#define MAXVAL  100  /* maximum depth of val stack      */

int sp = 0;          /* next free stack position        */
double val[MAXVAL];  /* value stack                     */

void push(double);
double pop(void);

int main(void)
{
	/* all operands and operators must be separated by blanks */
	/* print result when 'r' is read not '\n', since scanf skips blanks */

	double op2; // used to push too
	char s[MAXOP];
	while (scanf("%s", s) != EOF) {
		if (isdigit(*s) || *s == '.') {
			sscanf(s, "%lf", &op2);
			push(op2);
		}
		else
			switch (*s) {
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
				case 'r':
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
