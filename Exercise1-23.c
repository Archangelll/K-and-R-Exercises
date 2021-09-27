#include <stdio.h>

#define MAXLEN 10000

int main(void)
{
	void inpProg(char prog[]);
	void rmcom(char prog[]);
	void outProg(char prog[]);

	char prog[MAXLEN];

	inpProg(prog);
	rmcom(prog);
	outProg(prog);

	return 0;
}


void inpProg(char prog[])
{
	int i, c;

	for (i = 0; (c = getchar()) != EOF; ++i)
		prog[i] = c;
	prog[i] = '\0';
}


void outProg(char prog[])
{
	for (int i = 0; putchar(prog[i]); ++i)
		;
}


void rmcom(char prog[])
{
	int i, j, pc;
	_Bool insq, indq;
	
	pc = -1;
	i = j = insq = indq = 0;
	while (pc) {
		if (prog[i] == '\'' && pc != '\\' && !indq) {
			if (insq)
				insq = 0;
			else
				insq = 1;
		}
		else if (prog[i] == '"' && pc != '\\' && !insq) {
			if (indq)
				indq = 0;
			else
				indq = 1;
		}
		else if (prog[i] == '/' && pc == '/' && !insq && !indq) {
			do {
				while (prog[++i] != '\n' && prog[i])
					if (prog[i] != ' ')
						pc = prog[i];
			} while (pc == '\\' && prog[i]);
			--j;
		}
		else if (prog[i] == '*' && pc == '/' && !insq && !indq) {
			while ((prog[i] != '/' || pc != '*') && prog[i])
				pc = prog[i++];
			if (prog[i])
				++i;
			--j;
		}
		pc = prog[j++] = prog[i++];
	}
}
