#include <stdio.h>

int any(char s1[], char s2[]);

int main(void)
{
	char s1[] = "Write the function any(s1,s2), which...";
	char s2[] = "&";
	printf("%d\n", any(s1, s2));

	return 0;
}


int any(char s1[], char s2[])
{
	int i, j;

	i = j = 0;
	while(s1[i] != '\0'){
		while(s2[j] != '\0')
			if(s1[i] == s2[j++])
				return i;
		j = 0;
		++i;
	}
	return -1;
}
