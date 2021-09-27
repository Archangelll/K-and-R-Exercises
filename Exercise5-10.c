#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define MAXARG 100
#define NUMBER 0
#define INVOP  1
#define OPRT   2

int main(int argc, char *argv[])
{
	/* return 0 ---> OK
	 * return 1 ---> stack full
	 * return 2 ---> invalid argument
	 * return 3 ---> zero division
	 * return 4 ---> stack empty
	 * return 5 ---> no arguments
	 */
	int getop(char *s);

	if (argc == 1){
		printf("error: too few arguments.\n");
		return 5;
	}
	else {
		double val[MAXARG];
		int valp = 0;

		while (--argc > 0) {
			switch(getop(*++argv)) {
				case NUMBER:
					if (valp < MAXARG)
						val[valp++] = atof(*argv);
					else {
						printf("error: stack full\n");
						return 1;
					}
					break;
				case INVOP:
					printf("error: invalid argument\n");
					return 2;
				default:
					if (valp >= 2) {
						switch (**argv) {
							case '+':
								val[valp-2] += val[valp-1];
								break;
							case '-':
								val[valp-2] -= val[valp-1];
								break;
							case '*':
								val[valp-2] *= val[valp-1];
								break;
							case '/':
								if (val[valp-1] != 0)
									val[valp-2] /= val[valp-1];
								else {
									printf("error: division by zero\n");
									return 3;
								}
								break;
							case '%':
								val[valp-2] = fmod(val[valp-2], val[valp-1]);
								break;
						}
						--valp;
					}
					else {
						printf("error: stack empty\n");
						return 4;
					}
					break;
			}
		}
		printf("%lf\n", val[valp-1]);
	}
	return 0;
}


int getop(char *s) 
{
	/* .2 == 0.2
	 * 2. == 2.0
	 * .  == 0.0
	 */
    switch(s[0]) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			if (!s[1])
				return OPRT;
	}
	while (isdigit(*s))
		++s;
	if (*s == '.')
		while (isdigit(*++s))
			;
	if (*s)
		return INVOP;
	return NUMBER;
}
