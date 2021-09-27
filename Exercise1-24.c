#include <stdio.h>

#define MAXLEN 10000

int main(void)
{
	void inpProg(char []);
	void synerrc(char []);

	char prog[MAXLEN];

	inpProg(prog);
	synerrc(prog);

	return 0;
}


void inpProg(char prog[])
{
	int i, c;

	for (i = 0; (c = getchar()) != EOF; ++i)
		prog[i] = c;
	prog[i] = '\0';
}


#define MAXPAOPEN 100
#define MAXBKOPEN 100
#define MAXBCOPEN 100

_Bool ckesc(char prog[], int i, int escos[], int row[]);


void synerrc(char prog[])
{
	char tmp, pc;
	int i, escos[1];
	int paopenln[MAXPAOPEN], bkopenln[MAXBKOPEN], bcopenln[MAXBCOPEN], pap, bkp, bcp;
	int row[1] = {1};

	pap = bkp = bcp = i = 0;
	while (prog[i]) {
		if (prog[i] == '\n') {
			++row[0];
		}
		else if  (prog[i] == '(') {
			if (pap < MAXPAOPEN)
				paopenln[pap++] = row[0];
			else {
				printf("line %d: maximum number of unclosed '(' was exceeded\n", row[0]);
				return;
			}
		}
		else if  (prog[i] == ')') {
			if (pap > 0)
				--pap;
			else {
				printf("line %d: expected '(' before ')'\n", row[0]);
				return;
			}
		}
		else if  (prog[i] == '[') {
			if (bkp < MAXBKOPEN)
				bkopenln[bkp++] = row[0];
			else {
				printf("line %d: maximum number of unclosed '[' was exceeded\n", row[0]);
				return;
			}
		}
		else if  (prog[i] == ']') {
			if (bkp > 0)
				--bkp;
			else {
				printf("line %d: expected '[' before ']'\n", row[0]);
				return;
			}
		}
		else if  (prog[i] == '{') {
			if (bcp < MAXBCOPEN)
				bcopenln[bcp++] = row[0];
			else {
				printf("line %d: maximum number of unclosed '{' was exceeded\n", row[0]);
				return;
			}
		}
		else if  (prog[i] == '}') {
			if (bcp > 0)
				--bcp;
			else {
				printf("line %d: expected '{' before '}'\n", row[0]);
				return;
			}
		}
		else if (prog[i] == '\'' || prog[i] == '"') {
			tmp = prog[i];
			while (prog[++i] != tmp && prog[i] != '\n' && prog[i]) {
				if (prog[i] == '\\')
					if (ckesc(prog, i, escos, row)) {
						printf("line %d: invalid escape sequence\n", row[0]);
						return;
					}
					else
						i = escos[0];
			}
			if (prog[i] == '\n') {
				printf("line %d: missing \\ or %c\n", row[0], tmp);
				return;
			}
			if (prog[i] == '\0') {
				printf("line %d: missing %c\n", row[0], tmp);
				return;
			}
		}
		else if (prog[i] == '\\') {
			while (prog[++i] == ' ')
				;
			if (prog[i++] != '\n') {
				printf("line %d: invalid use of the '\\' character\n", row[0]);
				return;
			}
			else
				++row[0];
		}
		else if (prog[i] == '/' && pc == '/') {
			do {
				while (prog[++i] != '\n' && prog[i]) {
					if (prog[i] != ' ')
						pc = prog[i];
				}
				if (prog[i])
					++row[0];
			} while (pc == '\\' && prog[i]);
		}
		else if (prog[i] == '*' && pc == '/') {
			escos[0] = 0; /* escos will be used to count '\n' characters, thus if the comment never ends row will keep its value */
			while ((prog[i] != '/' || pc != '*') && prog[i]) {
				if (prog[i] == '\n')
					++escos[0];
				pc = prog[i++];
			}
			if (!prog[i]) {
				printf("line %d: comment never ends\n", row[0]);
				return;
			}
			row[0] += escos[0];
		}
		pc = prog[i++];
	}
	if (pap)
		printf("line %d: unclosed '('\n", paopenln[pap-1]);
	if (bkp)
		printf("line %d: unclosed '['\n", bkopenln[bkp-1]);
	if (bcp)
		printf("line %d: unclosed '{'\n", bcopenln[bcp-1]);
}


_Bool inhex(char c);


_Bool ckesc(char prog[], int i, int escos[], int row[])
{
	/* escape sequences = {a, b, f, n, r, t, v, \, ?, ", ', 0, ooo, xhh} */
	int j = 0;

	if (!(prog[++i] == 'a' || prog[i] == 'b' || prog[i] == 'f' || prog[i] == 'n' || prog[i] == 'r' || prog[i] == 't' || prog[i] == 'v' || prog[i] == '\\' || prog[i] == '?' || prog[i] == '"' || prog[i] == '\'' || prog[i] == '\n')) {
		if (prog[i] == 'x') {
			while (j < 2 && inhex(prog[i+j+1]))
				++j;
			i += j;
		}
		else if (prog[i] == ' ') {
			while (prog[++i] == ' ')
				;
			if (prog[i] != '\n')
				return 1;
			else
				++row[0];
		}
		else {
			while (j < 3 && (prog[i+j] >= '0' && prog[i+j] <= '7'))
				++j;
			i += j-1;
		}
		if (j == 0)
			return 1;
	}
	escos[0] = i; /* last escape sequence character */
	return 0;
}


_Bool inhex(char c)
{
	if (c >= '0' && c <= '9' ||
	    c == 'A' || c == 'a' || c == 'B' || c == 'b' || c == 'C' || c == 'c' ||
		c == 'D' || c == 'd' || c == 'E' || c == 'e' || c == 'F' || c == 'f')
		return 1;
	return 0;
}
