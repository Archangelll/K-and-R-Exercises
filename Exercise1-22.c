#include <stdio.h>

#define MAXLINE 100
#define TABSTOP 8

int getfline(char s[]);

int main(void)
{
	char s[MAXLINE];
	getfline(s);
	printf("%s", s);

	return 0;
}


int getfline(char s[])
{
	static int t = 1;
	int i, j, c;

	i = j = 0;
	while(i < MAXLINE - 2 && (c = getchar()) != '\n' && c != EOF){
		if(c == '\t'){
			i += TABSTOP - (t % TABSTOP);
			if(i >= MAXLINE){
				i -= TABSTOP - (t % TABSTOP);
				t = 0;
				break;
			}
			t = 0;
		}
		s[j] = c;
		++i; ++j; ++t;
	}
	if(i == 0 && c == EOF)
		return 0;
	if(c == '\n')
		t = 1;
	--j;
	for(; j >= 0 && (s[j] == ' ' || s[j] == '\t'); --j);
	++j;

	s[j] = '\n';
	++j;
	s[j] = '\0';

	return j;
}
