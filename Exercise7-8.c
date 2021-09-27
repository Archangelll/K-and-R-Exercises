#include <stdio.h>

/* what is a page (in 2020)? i do not know */
int main(int argc, char *argv[])
{
	/* what does "page count for each file" means ? a single file can be printed in 2 different pages (what is a page ?)? maybe in 1988 
	 * i will consider "page count" always as 1, therefore no need to print it
	 */

	FILE *fp;
	char *progName = argv[0];
	int c;

	while (--argc > 0) {
		if ((fp = fopen(*++argv, "r")) == NULL) {
			fprintf(stderr, "%s: can't open %s\n", progName, *argv);
			return 1;
		}

		printf(">>>>new page<<<<\t");
		printf("Title: %s\n", *argv);
		while ((c = getc(fp)) != EOF)
			putchar(c);

		if (!feof(fp)) {
			fprintf(stderr, "%s: error while reading  %s\n", progName, *argv);
			return 2;
		}
	}

	return 0;
}
