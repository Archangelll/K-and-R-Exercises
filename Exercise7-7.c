#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

int getln(char *line, int max);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [-xn] PATTERN [FILE]...\n", argv[0]);
		exit(1);
	}

	char *progName = argv[0];
	int c, except = 0, number = 0;

	while (--argc > 0 && **++argv == '-')
		while (c = *++*argv)
			switch (c) {
				case 'x':
					except = 1;
					break;
				case 'n':
					number = 1;
					break;
				default:
					fprintf(stderr, "%s: illegal option %c\n", progName, c);
					argc = 0;
					break;
			}
	if (argc < 1) {
		fprintf(stderr, "Usage: %s [-xn] PATTERNS [FILE]...\n", progName);
		exit(2);
	}
	else {
		char line[MAXLINE];
		int found = 0;
		long lineno = 0;

		char *pattern = *argv++;
		if (*argv) {
			FILE *fp = fopen(*argv, "r");

			do {
				if (!fp) {
					fprintf(stderr, "%s: can't open %s\n", progName, *argv);
					exit(3);
				}
				while (fgets(line, MAXLINE, fp)) {
					lineno++;
					if ((strstr(line, pattern) != NULL) != except) {
						printf("%s:", *argv);
						if (number)
							printf("%ld:", lineno);
						printf("%s", line);
						found++;
					}
				}
				printf("%d lines found\n", found);
				lineno = 0;
				found = 0;

				fclose(fp);	
				if (*++argv)
					fp = fopen(*argv, "r");
				else
					break;
			} while (1);
		}
		else {
			while (fgets(line, MAXLINE, stdin)) {
				lineno++;
				if ((strstr(line, pattern) != NULL) != except) {
					if (number)
						printf("%ld:", lineno);
					printf("%s", line);
					found++;
				}
			}
			printf("%d lines found\n", found);
		}
	}

	return 0;
}
