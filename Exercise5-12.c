#include <stdio.h>
#include <stdlib.h>

#define DEFAULT 8

int main(int argc, char *argv[])
{
/* detab
	int m, n;
	int c, ccount, cond;
	char **arg;

	n = -1;
	arg = ++argv;
	if (*argv && **argv == '-') {
		m = atoi((*argv)+1);
		n = atoi(*++argv);
		arg = ++argv;
	}

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
				if (n >= 0 && ccount >= m-1)
					cond = ccount + (n - (ccount % n));
				else
					cond = ccount + (DEFAULT - (ccount % DEFAULT));
			while (ccount < cond) {
				putchar(' ');
				++ccount;
			}
		}
	}
	return 0;
*/

	int m, n;
	int c, ccount, bcount, lts;
	char **arg;

	n = -1;
	arg = ++argv;
	if (*argv && **argv == '-') {
		m = atoi((*argv)+1);
		n = atoi(*++argv);
		arg = ++argv;
	}

	lts = -1;
	bcount = ccount = 0;
	while ((c = getchar()) != EOF || bcount) {
		if (c == ' ') {
			++bcount;
			continue;
		}
		else if (bcount){
			bcount += ccount;
			while (ccount < bcount) {
				while (*arg && lts <= ccount)
					lts = atoi(*arg++);
				if (lts <= ccount) {
					if (n >= 0 && ccount >= m-1)
						lts = ccount + (n - (ccount % n));
					else
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
		}
		if (putchar(c) != '\n')
			++ccount;
		else {
			ccount = 0;
			arg = argv;
			lts = -1;
		}
	}
	return 0;

}
