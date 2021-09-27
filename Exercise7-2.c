#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define NONGRAPHWIDTH 2 // must be >= printf("%o", character)
#define MAXLINE 30 // must be >= NONGRAPHWIDTH

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: prog -ho\n");
		return 1;
	}

	int c, col = 0;
	void printh(int), printo(int);
	void (*printn)(int c) = (strcmp(argv[1], "-h") == 0) ? printh : printo;

	while ((c = getchar()) != EOF)
		if (isgraph(c)) {
			if (col >= MAXLINE) {
				putchar('\n');
				col = 0;
			}
			putchar(c);
			col += 1;
		}
		else {
			if (col + NONGRAPHWIDTH > MAXLINE) {
				putchar('\n');
				col = 0;
			}
			printn(c);
			col += NONGRAPHWIDTH + 1; // +1 for '\\'
		}
	putchar('\n');

	return 0;
}


void printh(int c)
{
	printf("\\%-*x", NONGRAPHWIDTH, c);
}


void printo(int c)
{
	printf("\\%-*o", NONGRAPHWIDTH, c);
}
