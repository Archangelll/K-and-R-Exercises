#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 1000
#define MAXLINE  1000
#define DEFAULTN 10

int main(int argc, char *argv[])
{
	char *alloc(int);
	int getln(char *, int);

	int n, len, linesp;
	char *p, *lines[MAXLINES], line[MAXLINE];

	n = DEFAULTN;
	if (argv[0] && argv[1] && argv[1][0] == '-')
		n = atoi(argv[1]+1);
	if (n <= 0) {
		printf("Usage: tail -n\nn = %d\n", n);
		return 1;
	}
	
	linesp = 0;
	while ((len = getln(line, MAXLINE)) > 0) {
		if ((p = alloc(len)) == NULL) {
			printf("error: buf: no more space avaliable\n");
			return 2;
		}
		if (linesp >= MAXLINES) {
			printf("error: lines: no more space avaliable\n");
			return 3;
		}
		line[len-1] = '\0'; /* deletes '\n'(POSIX) to save space */
		strcpy(p, line);
		lines[linesp++] = p;
	}
	for(len = (linesp - n > 0) ? linesp - n : 0; len < linesp; ++len)
		printf("%s\n", lines[len]);

	return 0;
}


int getln(char s[], int len)
{
	int i, c;
	for (i = 0; i < len-1 && (c = getchar()) != '\n' && c != EOF; ++i)
		s[i] = c;
	if (c == '\n')
		s[i++] = '\n';

	s[i] = '\0'; 
	return i;
}


#define BUFSIZE  10000

char buf[BUFSIZE];
char *bufp = buf;

char *alloc(int n)
{
	if (buf + BUFSIZE - bufp >= n) {
		bufp += n;
		return bufp - n;
	}
	else
		return NULL;
}
