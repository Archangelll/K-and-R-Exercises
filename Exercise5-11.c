#include <stdio.h>
#include <stdlib.h>

#define DEFAULT 8

int main(int argc, char *argv[])
{

	int c, ccount, cond;
	char **arg;

	arg = ++argv;
	cond = -1;
	ccount = 0;
	while ((c = getchar()) != EOF) {
		if (c != '\t')
			if (putchar(c) != '\n')
				++ccount;
			else {
				ccount = 0;
				arg = argv;
				cond = -1;
			}
		else {
			while (*arg && cond <= ccount)
				cond = atoi(*arg++);
			if (cond <= ccount)
				cond = ccount + (DEFAULT - (ccount % DEFAULT));
			while (ccount < cond) {
				putchar(' ');
				++ccount;
			}
		}
	}
	return 0;

/* entab
	int c, ccount, bcount, lts;
	char **arg;

	arg = ++argv;
	lts = -1;
	bcount = ccount = 0;
	while ((c = getchar()) != EOF) {
		if (c == ' ')
			++bcount;
		else if (bcount == 0)
			if (putchar(c) != '\n')
				++ccount;
			else {
				ccount = 0;
				arg = argv;
				lts = -1;
			}
		else {
			bcount += ccount;
			while (ccount < bcount) {
				while (*arg && lts <= ccount)
					lts = atoi(*arg++);
				if (lts <= ccount) {
					lts = ccount + (DEFAULT - (ccount % DEFAULT));
				}
				if (lts <= bcount) {
					putchar('$'); // tab
					ccount = lts;
				}
				else {
					while (ccount < bcount) {
						putchar('#'); // space
						++ccount;
					}
					break;
				}
			}
			bcount = 0;
			if (putchar(c) != '\n')
				++ccount;
			else {
				ccount = 0;
				arg = argv;
				lts = -1;
			}
		}
	}
	return 0;
*/
}
