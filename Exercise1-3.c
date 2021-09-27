#include <stdio.h>

int main(void)
{
	short int Fahr = 0;
	float Celsius = 0;

	printf("Fahrenheit     Celsius\n");

	for (; Fahr <= 300; Fahr += 20)
	{
		Celsius = (Fahr - 32) / 1.8;
		printf("%3hu %17.1f\n", Fahr, Celsius);
	}

	return 0;
}
