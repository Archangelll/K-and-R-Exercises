#include <stdio.h>

#define MIN 81

int main(void)
{
	char i, line[MIN];
	int c;

	while (1){
		for (i = 0; i < MIN && (c = getchar()) != EOF && c != '\n'; ++i)
			line[i] = c;

		putchar('\n');
		if (i == MIN){
			for (i = 0; i < MIN; ++i)
				putchar(line[i]);

			while ((c = getchar()) != EOF && c != '\n')
				putchar(c);
		}
		putchar('\n');

		if (c == EOF)
			break;
	}

	return 0;
}
