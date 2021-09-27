#include <stdio.h>
#include <limits.h>

void itoa(int n, char s[]);

int main(void)
{
	int n = INT_MIN;
	char s[100];
	itoa(n, s);

	printf("%s\n", s);
	printf("%d\n", n);

	return 0;
}


void itoa(int n, char s[])
{
	static int i, j;

	if (n / 10) {
		++j;
		itoa(n / 10, s);
	}
	if (n < 0) {
		if (n >= -9)
			s[i++] = '-';
		s[i++] = (-(n % 10) + '0');
		s[i]  = '\0';
		if (i == j+2)
			i = j = 0;
	}
	else {
		s[i++] = (n % 10 + '0');
		s[i]  = '\0';
		if (i == j+1)
			i = j = 0;
	}
}
