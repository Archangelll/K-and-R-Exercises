#include <stdio.h>
#define MAXLINE 10

unsigned int getln(char s[]);

int main(void)
{
	char line[MAXLINE + 1], fmtl[MAXLINE + 1];
	int i, j;

	while ((i = getln(line)) != 0){
		for (j = i - 1; j > -1 && (line[j] == ' ' || line[j] == '\t' || line[j] == '\n'); --j)
		;
		if  (j == -1){
			fmtl[0] = '\0';
			continue;
		}
		 
		for (i = 0; i < j + 1; ++i)
			fmtl[i] = line[i];
		fmtl[i] = '\0';

		printf("%s\n", fmtl);
	}
	return 0;
}


unsigned int getln(char s[])
{
	unsigned int i;
	int c;

	for (i = 0; i < MAXLINE && (c = getchar()) != EOF && c != '\n'; ++i)
		s[i] = c;

	if (c == '\n'){
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}
