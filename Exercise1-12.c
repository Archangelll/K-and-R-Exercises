#include <stdio.h>

int main(void)
{
	int c, pc = 0;
	while ((c = getchar()) != EOF){
		if (c != ' ' && c != '\n' && c != '\t')
			putchar(c);
		else if (pc == ' ' || pc == '\n' || pc == '\t')
			continue;
		else
			putchar('\n');

		pc = c;
	}

	return 0;
}
