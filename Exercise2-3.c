#include <stdio.h>

int htoi(char s[]);

int main(void)
{
	char hexstr[] = "0xFF";
	printf("%d\n", htoi(hexstr));

	return 0;
}


int htoi(char s[])
{
	int exp, n, i;

	i = 0;
	if(s[0] == '-'){
		++i;
		exp = -1;
	}
	else{
		exp = 1;
	}
	if(s[i+1] == 'x' || s[i+1] == 'X')
		i += 2;
	n = i;
	for(; s[n] != '\0'; ++n)
		exp *= 16;
	n = 0;

	for(; s[i] != '\0'; ++i){
		exp /= 16;
		if(s[i] >= '0' && s[i] <= '9'){
			n += (s[i] - '0') * exp;
		}
		else{
			if(s[i] >= 'A' && s[i] <= 'F')
				n += (s[i] - 'A' + 10) * exp;
			else
				n += (s[i] - 'a' + 10) * exp;
		}
	}
	return n;
}
