#include <stdio.h>

int lower(int c);

int main(void)
{
	printf("%c\n", lower('A'));
	return 0;
}

int lower(int c)
{
	return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}
