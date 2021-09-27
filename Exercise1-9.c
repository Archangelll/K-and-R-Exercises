#include <stdio.h>

int main(void)
{
	int c, pc = 0;
	while ((c = getchar()) != EOF){
		if (c == ' ' && pc == ' ')
			continue;
		pc = c;
		putchar(c);
	}

	return 0;
}
