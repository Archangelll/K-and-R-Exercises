#include <stdio.h>

unsigned rightrot(unsigned x, unsigned n);

int main(void)
{
	unsigned x = 4845631;
	printf("%u\n", rightrot(x, 18));

	return 0;
}


unsigned rightrot(unsigned x, unsigned n)
{
	unsigned tmp = -1;
	unsigned MSB = 0;
	while((tmp >>= 1) != 0u)
		++MSB;

	n %= MSB+1;
	if(n == 0)
		return x;
	return (x >> n) ^ (x << MSB+1-n);
}
