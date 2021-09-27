#include <stdio.h>

unsigned setbits(unsigned x, unsigned p, unsigned n, unsigned y);

int main(void)
{
	unsigned a = 3578;
	unsigned b = 1518;
	printf("%u\n", setbits(a, 6, 2, b));

	return 0;
}


unsigned setbits(unsigned x, unsigned p, unsigned n, unsigned y)
{
	unsigned tmp = ~0u;
	unsigned MSB = 0;
	while((tmp = tmp >> 1) != 0u)
		++MSB;

	if(p == MSB && n > p)
		return x;
	return ((x << MSB - p) >> MSB+1-n) ^ ((y >> n) << n);
}
