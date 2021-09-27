#include <stdio.h>

#define TABSTOP 8

int main(void)
{
	unsigned char nxtTS;
	unsigned int  countT = 0, countB = 0, i;

	int c;
	while ((c = getchar()) != EOF){
		if (c == ' '){
			++countB;
		}
		else if (countB != 0){
			nxtTS = (TABSTOP - (countT % TABSTOP));
			if (countB < nxtTS){
				for (i = 0; i < countB; ++i)
					putchar('#'); /* blank */
			}
			else{
				putchar('$'); /* tab */
				for (i = 0; i < (countB - nxtTS) / TABSTOP; ++i)
					putchar('$'); /* tab */
				for (i = 0; i < (countB - nxtTS) % TABSTOP; ++i)
					putchar('#'); /* blank */
			}
			if (c == '\t' || c == '\n')
				countT = 0;
			else
				countT = (countT + countB + 1) % TABSTOP;
			countB = 0;
			putchar(c);
		}
		else{
			putchar(c);
			if (c == '\t' || c == '\n')
				countT = 0;
			else
				++countT;
		}
	}
	return 0;
}
