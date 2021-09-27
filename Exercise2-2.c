#include <stdio.h>

int main(void)
{
	int i, c, lim = 100;
	char s[lim];

	for (i = 0; i < lim-1 ? ((c = getchar()) != '\n' ? (c != EOF) : 0) : 0; ++i)
		s[i] = c;
/*
	if (c == '\n'){
		s[i] = '\n';
		++i;
	}
	s[i] = '\0';

	for (i = 0; s[i] != '\0'; ++i)
		putchar(s[i]);
*/
	return 0;
}
