#include <stdio.h>
#include <limits.h>

int  itoa(int n, char s[]);
void reverse(char s[], int i);

int main(void)
{
	char s[100];
	int i = INT_MIN;

	printf("%d\n", itoa(i, s));
	printf("%d\n", i);
	printf("%s\n", s);

	return 0;
}

int itoa(int n, char s[])
{
	int i = 0;
	if (n >= 0) {
		do {
			s[i++] = '0' + n % 10;
		} while (n /= 10);
	}
	else {
		do {
			s[i++] = '0' - n % 10;
		} while (n /= 10);
		s[i++] = '-';
	}
	s[i] = '\0';
	reverse(s, i-1);

	return i;
}


void reverse(char s[], int i)
{
	char tmp;

	for (int j = 0; i > j; ++j, --i) {
		tmp = s[j];
		s[j] = s[i];
		s[i] = tmp;
	}
}
