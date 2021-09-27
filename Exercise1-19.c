#include <stdio.h>
#define MAXLINE 10

unsigned int getln(char s[]);
void reverse(char s[]);

int main(void)
{
	char str[MAXLINE + 1];

	while (getln(str) != 0){
		reverse(str);
		printf("%s\n", str);
	}

	return 0;
}


void reverse(char s[])
{
	unsigned int i = 0, j;

	while (s[i] != '\0')
		++i;
	char cp[i + 1];

	--i;
	for (j = 0; j <= i; ++j)
		cp[j] = s[i - j];
	cp[j] = '\0';
	
	for (i = 0; i < j + 1; ++i)
		s[i] = cp[i];

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
