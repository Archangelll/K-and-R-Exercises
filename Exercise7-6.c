#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 100

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3) {
		printf("Usage: %s file1 [file2]=stdin\n", argv[0]);
		exit(1);
	}
	FILE *file1, *file2;
	char line1[MAXLINE], line2[MAXLINE], *pl1, *pl2;
	int lno;
	file1 = fopen(argv[1], "r");
	file2 = (argv[2]) ? fopen(argv[2], "r") : stdin;
	
	if (!file1 || !file2) {
		fprintf(stderr, "%s: can't open %s\n", argv[0],
		        file1 ? (argv[2] ? argv[2] : "stdin") : argv[1]);	
		exit(2);
	}

	lno = 0;
	do {
		pl1 = fgets(line1, MAXLINE, file1);
		pl2 = fgets(line2, MAXLINE, file2);
		lno += 1;
	} while (pl1 && pl2 && strcmp(line1, line2) == 0);
	
	printf("line %d\n", lno);
	if (pl1)
		printf("file1: %s", line1);
	if (pl2)
		printf("file2: %s", line2);

	return 0;
}
