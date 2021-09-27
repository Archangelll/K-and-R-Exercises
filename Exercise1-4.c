#include <stdio.h>

int main(void)
{
	short int Celsius = 0;
	float Fahr = 0;

	printf("Celsius     Fahrenheit\n");

	for (; Celsius <= 300; Celsius += 20)
	{
		Fahr = Celsius * 1.8 + 32;
		printf("%3hu %13.1f\n", Celsius, Fahr);
	}

	return 0;
}
