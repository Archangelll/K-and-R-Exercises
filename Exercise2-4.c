#include <stdio.h>

void squeeze(char s1[], char s2[]);

int main(void)
{
	char s1[] = "Write an alternative version of squeeze(s1,s2) that deletes...";
	char s2[] = "abcdef. ";
	squeeze(s1, s2);

	printf("%s\n", s1);

	return 0;
}


void squeeze(char s1[], char s2[])
{
	int i, j, k;
	_Bool l;

	i = j = k = 0;
	l = 1;
	while(s1[i] != '\0'){
		while(s2[j] != '\0'){
			if(s1[i] == s2[j++]){
				l = 0;
				break;
			}
		}
		if(l){
			s1[k] = s1[i];
			++k;
		}
		else{
			l = 1;
		}
		j = 0;
		++i;
	}
	s1[k] = '\0';
}
