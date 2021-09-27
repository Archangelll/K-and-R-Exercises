#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h> // syscalls.h

#define PERMS 0666

int main(int argc, char *argv[])
{
	char *prog = argv[0];
	char buf[BUFSIZ];
	int n, ifd;
	clock_t tic = clock();

	ifd = (--argc < 1) ? 0 : open(*++argv, O_RDONLY, 0);
	do {
		if (ifd < 0) {
			fprintf(stderr, "%s: can't open %s\n", prog, *argv);
			exit(1);
		}
		while ((n = read(ifd, buf, BUFSIZ)) > 0)
			if (write(1, buf, n) != n) {
				fprintf(stderr, "%s: error writing to stdout\n", prog);
				exit(2);
			}
		if (n < 0) {
			fprintf(stderr, "%s: error reading %s\n", prog, (*argv) ? (*argv) : "stdin");
			exit(3);
		}
		if (--argc > 0) {
			close(ifd); // leaves the last file opened, that way stdin will not be closed if argc < 2
			ifd = open(*++argv, O_RDONLY, 0);
		}
		else
			break;
	} while (1);
	
	clock_t toc = clock();
	printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	exit(0);
}
/*
int main(int argc, char *argv[])
{
	FILE *fp;
	void filecopy(FILE *, FILE *);
	char *prog = argv[0];
	clock_t tic = clock();

	if (argc == 1)
		filecopy(stdin, stdout);
	else
		while (--argc > 0)
			if ((fp = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "%s: can't open %s\n", prog, *argv);
				exit(1);
			}
			else {
				filecopy(fp, stdout);
				fclose(fp);
			}
	if (ferror(stdout)) {
		fprintf(stderr, "%s: error writing stdout\n", prog);
		exit(2);
	}

	clock_t toc = clock();
	printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	exit(0);
}


void filecopy(FILE *ifp, FILE *ofp)
{
	int c;

	while ((c = getc(ifp)) != EOF)
		putc(c, ofp);
}
*/
