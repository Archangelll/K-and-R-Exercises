#include <stdio.h>
#include <limits.h>

int  itob(int n, char s[], int b);
void reverse(char s[], int i);

int main(void)
{
	char s[100];
	int i = INT_MIN;

	printf("%d\n", itob(i, s, 16));
	printf("%s\n", s);

	return 0;
}


int itob(int n, char s[], int b)
{
	char d[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; /* MAX_BASE == 36*/
	int i = 0;

	if (n >= 0) {
		do {
			s[i++] = d[n % b];
		} while (n /= b);
	}
	else {
		do {
			s[i++] = d[-(n % b)];
		} while (n /= b);
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
