#include <stdio.h>

int main(void)
{
	int bl = 0, tb = 0, nl = 0, c = 0;

	while ((c = getchar()) != EOF){
		if (c == ' ')
			++bl;
		else if (c == '\t')
			++tb;
		else if (c == '\n')
			++nl;
	}
	printf("\rBlanks: %d\nTabs: %d\nNewLines: %d\n", bl, tb, nl);

	return 0;
}
