#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

int main(void)
{
	int c;
	int getword(char *word, int lim);
	char word[100];
	do {
		c = getword(word, 99);
		printf("%s\n", word);
	} while (c != EOF);

	return 0;
}


char buf[BUFSIZE];
int bufp = 0;

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


int getword(char *word, int lim)
{
	int dsoc(int), dprp(void), dcom(void);

	int c;
	char *w = word;

	while (isspace(c = getch()))
		;
	/* it's faster to remove all line continuation characters before calling getword */
	while (1) {
		if (c == '\"' || c == '\'')
			c = dsoc(c);
		else if (c == '#')
			c = dprp();
		else if (c == '/') {
			if ((c = dcom()) == 1) {
				c = '/';
				break;
			}
		}
		else
			break;
	}
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c) && c != '_') {
		*w = '\0';
		return c;
	}

	for ( ; --lim > 0; w++)
		if (!isalnum(*w = getch()) && *w != '_') {
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}


int dsoc(int q)
{
	int c, pc = 0;
	for (c = getch(); (c != q || pc == '\\') && c != EOF; c = getch())  
		if (pc == '\\' && c == '\\')
 			pc = 0; /* '\\' */      
        else
        	pc = c;
	if (c == q)
		c = getch();
	return c;
}


int dcom(void)
{
	int c, pc = 0;

	if ((c = getch()) == '/')
		do {
			while ((c = getch()) != '\n' && c != EOF) {
				if (c != ' ' && c != '\t')
					pc = c;
			}
		} while (pc == '\\' && c != EOF);
	else if (c == '*')
		for (c = getch(); (c != '/' || pc != '*') && c != EOF; c = getch())
			pc = c;
	else {
		ungetch(c);
		return 1; /* it's not a comment */
	}
	if (c == '/') { /* skips '\n' on multi line comments */
		if ((c = getch()) == '\n')
			c = getch();
	}
	else if (c == '\n') /* skips '\n' on single line comments */
		c = getch();
	return c;
}


int dprp(void)
{
	int c, pc = 0;
	while ((c = getch()) == ' ' || c == '\t')
		;
	
	if (c == 'i' && (c = getch()) == 'f') { /* assumes that every preprocessor directive followed by "if" is conditional */
		int i;
		char sendif[] = "endif";
		
		while ((c = getch()) != '\n' && c != EOF) /* endif must be at a different line */
			;
		while (pc != '#' && c != EOF)
			for (i = 0; sendif[i] != '\0' && c != EOF; c = getch()) {
				if (c == sendif[i]) {
					++i;
					continue;
				}
				else if (!isspace(c))
					pc = c;
				i = 0;
			}
		if (sendif[i] == '\0' && pc == '#' && c == '\n') /* skips first '\n' */
			c = getch();
	}
	else {
		while (c != '\n' && c != EOF)
			c = getch();
		if (c == '\n') /* skips first '\n' */
			c = getch();
	}
	return c;
}
