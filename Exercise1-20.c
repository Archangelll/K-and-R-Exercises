#include <stdio.h>

#define TABSTOP 8

int main(void)
{
	int c;
	unsigned int i = 0, j;

	while ((c = getchar()) != EOF){
		if (c == '\n'){
			i = 0;
			putchar('\n');
		}
		else if (c != '\t'){
			putchar(c);
			++i;
		}
		else{
			for (j = 0; j < (TABSTOP - (i % TABSTOP)); ++j)
				putchar(' ');
			i = 0;
		}
	}

	return 0;
}
