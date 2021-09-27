#include <stdio.h>

void reverse(char s[]);

int main(void)
{
	char s[] = "hello";
	reverse(s);
	printf("%s\n", s);
	reverse(s);
	printf("%s\n", s);

	return 0;
}


void reverse(char s[])
{
	static int i, j;
	char tmp;
	if (i == 0) {
		while (s[i] != '\0')
			++i;
		--i;
	}

	if (i > j) {
		tmp = s[j];
		s[j++] = s[i];
		s[i--] = tmp;
		reverse(s);
	}
	else {
		i = j = 0;
	}
}
