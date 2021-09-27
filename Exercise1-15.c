#include <stdio.h>

int main(void)
{
	float FtoC(int fahr);

	printf("Fahrenheit     Celsius\n");
	for (int i = 0; i <= 300; i += 20)
		printf("%10d%12.1f\n", i, FtoC(i));

	return 0;
}

float FtoC(int fahr)
{
	return (5.0 / 9.0) * (fahr - 32);
}
