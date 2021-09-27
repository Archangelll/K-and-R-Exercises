#include <stdio.h>

#define isupper(c) ((int)(c) - 'A' < 26) // 'Z' - 'A' == 25 (ASCII)

/* isupper() is implementation-defined, i will consider an implementation using ASCII */
int main(void)
{
	int Fisupper(int c);
	printf("%d\n", isupper('A')); // saves time (macro)
	printf("%d\n", isupper('a'));
	printf("%d\n", isupper('Z'));
	printf("%d\n", isupper('z'));
	printf("%d\n", Fisupper('A')); // saves space (function)
	printf("%d\n", Fisupper('a'));
	printf("%d\n", Fisupper('Z'));
	printf("%d\n", Fisupper('z'));
}


int Fisupper(int c)
{
	return (c - 'A' < 26) ? 1 : 0;
}
