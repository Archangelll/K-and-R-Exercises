#include <stdio.h>

#define MAXLINE 10

int getln(char line[], int maxline);
void copy(char to[], char from[]);

int main(void)
{
	int len, max, ccount;
	char line[MAXLINE], longest[MAXLINE];

	max = ccount = 0;
	while ((len = getln(line, MAXLINE)) > 0) {
		ccount += len;
		if (line[len - 1] == '\n') {
			if (ccount > max) {
				max = ccount;
				copy(longest, line);
			}
			ccount = 0;
		}
	}
	if (max > 0)
		printf("%d: %s", max, longest);
	return 0;
}


int getln(char s[], int lim)
{
	int c, i;

	for (i = 0; i < lim-1 && (c = getchar()) != '\n' && c != EOF; ++i)
		s[i] = c;
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}


void copy(char to[], char from[])
{
	int i;

	i = 0;
	while ((to[i] = from[i]) != '\0')
		++i;
}
