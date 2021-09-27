#include <stdio.h>

#define MAX_CHAR 256

int main(void)
{
	int c;
	unsigned long i, frequencies[MAX_CHAR], greater;

	for (i = 0; i < MAX_CHAR; ++i)
		frequencies[i] = 0;

	while ((c = getchar()) != EOF)
		++frequencies[c];

	for (i = 0; i < MAX_CHAR; ++i){
		printf("%3lu ", i);

		for (unsigned long j = 0; j < frequencies[i]; ++j)
			printf("%c", '#');
		putchar('\n');
	}
/*
	greater = 0;
	for (i = 0; i < MAX_CHAR; ++i)
		if (frequencies[i] > greater)
			greater = frequencies[i];
	
	for (i = greater; i > 0; --i){
		printf("%3lu", i);
		for (unsigned short j = 0; j < MAX_CHAR; ++j){
			if (frequencies[j] == i){
				printf("%3c", '#');
				--frequencies[j];
			} else {
				printf("%3c", ' ');
			}
		}
		putchar('\n');
	}
	printf("%3c", ' ');
	for (i = 0; i < MAX_CHAR; ++i)
		printf("%3lu", i);

	putchar('\n');
*/
	return 0;
}
