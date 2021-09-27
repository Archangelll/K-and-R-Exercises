#include <stdio.h>

unsigned bitcount(unsigned x);

int main(void)
{
	unsigned x = -1;
	printf("%u\n", bitcount(x));

	return 0;
}

unsigned bitcount(unsigned x)
{
	unsigned count = 0;
	while(x > 0u){
		x &= x-1;
		++count;
	}	
	return count;
}
