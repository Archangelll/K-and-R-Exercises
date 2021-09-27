#include <stdio.h>

int strrindex(char s[], char t[]);

int main(void)
{
	char s[] = "a b c abc";
	char t[] = "a";
	printf("%d\n", strrindex(s, t));

	return 0;
}

int strrindex(char s[], char t[])
{
	int i, j, k;

	i = j = 0;
	while (s[i] != '\0')
		++i;
	while (t[j] != '\0')
		++j;
	if (i == 0 || j == 0)
		return -1;
			
	for (--i, k = --j; i >=0 && k >= 0 && i >= k; --i) {
		if (s[i] == t[k])
			--k;
		else
			k = j;
	}
	if (k == -1)
		return i+1;
	return -1;
}
