#include <stdio.h>

#define MAXWORDLEN 10

int main(void)
{
	unsigned short spaces, count = 0;
	int c, pc = ' ';
	unsigned long i, currL, cpcurrL, len[MAXWORDLEN];

	for (i = 0; i < MAXWORDLEN; ++i)
		len[i] = 0;

	while ((c = getchar()) != EOF){
		if (c == ' ' || c == '\n' || c == '\t'){
			if (pc == ' ' || pc == '\n' || pc == '\t')
				continue;
			
			++len[count - 1];
			count = 0;
			pc = c;
			continue;
		}

		count += 1;
		pc = c;
	}
/*
	for (i = 0; i < MAXWORDLEN; ++i){
		printf("%2d ", i + 1);
		for (int j = 0; j < len[i]; ++j)
			putchar('#');
		putchar('\n');
	}
*/
	currL = 0;
	for (i = 0; i < MAXWORDLEN; ++i)
		if (len[i] > currL)
			currL = len[i];
	cpcurrL = currL;
	spaces = 1;
	while ((cpcurrL = cpcurrL / 10) != 0)
		++spaces;

	for (i = currL; i > 0; --i){
		printf("%*lu", spaces, i);
		for (int j = 0; j < MAXWORDLEN; ++j){
			if (len[j] == i){
				printf("%5c", '#');
				len[j] -= 1;
			} else {
				printf("%5c", ' ');
			}
		}
		putchar('\n');
	}
	printf("%*c", spaces, ' ');
	for (i = 0; i < MAXWORDLEN; ++i)
		printf("%5lu", i + 1);
	putchar('\n');

	return 0;
}
