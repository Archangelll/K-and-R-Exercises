#include <stdio.h>

unsigned invert(unsigned x, unsigned p, unsigned n);

int main(void)
{
	printf("%u\n", invert(54548, 10, 11));

	return 0;
}


unsigned invert(unsigned x, unsigned p, unsigned n)
{
	unsigned tmp = ~0u;
	unsigned MSB = 0;
	while((tmp >>= 1) != 0u)
		++MSB;

	if (p == MSB && n > p)
		return ~x;
	if (p == MSB)
		return ((~x >> p+1-n) << MSB+1-n) + ((x << n) >> n);
	if (n > p)
		return ((~x << MSB-p) >> MSB-p) + ((x >> n) << n);
	return (((~x >> p+1-n) << MSB+1-n) >> MSB-p) + ((x << MSB+1 - (p+1-n)) >> MSB+1 - (p+1-n)) + ((x >> p+1) << p+1);
}
