#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	int (*swapcase)(int c) = (strcmp("./lower", argv[0]) == 0) ? tolower : toupper;
	int c;
	while ((c = getchar()) != EOF)
		putchar(swapcase(c));

	return 0;
}
