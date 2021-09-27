#include <stdio.h>
#include <ctype.h>

double atof(char s[]);

int main(void)
{
	char s[] = "123.45e-6";
	printf("%g\n", atof(s));

	return 0;
}


double atof(char s[])
{
	double val, power;
	int i, sign;
	int Epower, Esign;

	for (i = 0; isspace(s[i]); i++)
		;
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');
	if (s[i] == '.')
		++i;
	for (power = 1.0; isdigit(s[i]); ++i) {
		val = 10.0 * val + (s[i] - '0');
		power *= 10.0;
	}
	if (s[i] == 'e' || s[i] == 'E') {
		Esign = (s[++i] == '-') ? -1 : 1;
		if (s[i] == '-' || s[i] == '+')
			++i;
		Epower = 0;
		while (s[i] != '\0'){
			Epower = 10 * Epower + (s[i] - '0');
			++i;
		}
		if (Esign == -1)
			for (i = 0; i < Epower; ++i)
				power *= 10.0;
		else 
			for (i = 0; i < Epower; ++i)
				power /= 10.0;
	}
		
	return sign * val / power;
}
