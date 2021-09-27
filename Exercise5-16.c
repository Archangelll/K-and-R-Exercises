#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE   1000
#define MAXLINES  5000
#define ALLOCSIZE 10000

int  getln(char line[], int maxline);
int  readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
char *alloc(int n);
void quickSort(char *lineptr[], int left, int right, int (*comp)(const char *, const char *));
int  numcmp(const char *, const char *);
void swap(char **, int, int);
int csstrcmp(const char *, const char *);

char *lineptr[MAXLINES];
char allocbuf[ALLOCSIZE];
char *allocp = allocbuf;
int  reverse = 0; /* 1 if sorting in reverse */
int  casedis = 0; /* 1 if case distinctions are not made */
int  drorder = 0; /* 1 if directory order */

int main(int argc, char *argv[])
{
	int c, nlines;
	int numeric = 0; /* 1 if numeric sort */

	while (--argc > 0 && (*++argv)[0] == '-')
		while (c = *++argv[0])
			switch (c) {
				case 'n':
					numeric = 1;
					break;
				case 'r':
					reverse = 1;
					break;
				case 'f':
					casedis = 1;
					break;
				case 'd':
					drorder = 1;
					break;
				default:
					printf("sort: illegal option %c\n", c);
					return 2;
			}

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		quickSort(lineptr, 0, nlines-1, numeric ? numcmp : csstrcmp);
		writelines(lineptr, nlines);
		return 0;
	}
	else {
		printf("input too big to sort\n");
		return 1;
	}
}


int getln(char line[], int maxline)
{
	int c, i;

	for (i = 0; i < maxline-1 && (c = getchar()) != '\n' && c != EOF; ++i)
		line[i] = c;
	
	if (c == '\n')
		line[i++] = '\n';
	
	line[i] = '\0';
	return i;
}


int readlines(char *lineptr[], int maxlines)
{
	char line[MAXLINE], *p;
	int  len, nlines;

	nlines = 0;
	while ((len = getln(line, MAXLINE)) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else {
			line[len - 1] = '\0'; /* delete '\n' (POSIX)*/
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}


void writelines(char *lineptr[], int nlines)
{
	int i = 0;

	while (i < nlines)
		printf("%s\n", lineptr[i++]);
}


char *alloc(int n)
{
	if (allocbuf + ALLOCSIZE - allocp >= n) {
		allocp += n;
		return allocp - n;
	}
	else 
		return NULL;
}


void quickSort(char *v[], int left, int right, int (*comp)(const char *, const char *))
{
	if (left >= right)
		return;

	int i, j;

	i = left;
	for (j = i+1; j <= right; ++j)
		if (reverse ? comp(v[j], v[left]) > 0 : comp(v[j], v[left]) < 0) /* must use standard strcmp */
			swap(v, ++i, j);
	
	swap(v, left, i);
	quickSort(v, left, i-1 , comp);
	quickSort(v, i+1, right, comp);
}


int numcmp(const char *s1, const char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}


void swap(char *v[], int i, int j)
{
	char *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}


int csstrcmp(const char *s1, const char *s2)
{
	char tmpline1[MAXLINE], *t1;
	char tmpline2[MAXLINE], *t2;
	t1 = tmpline1;
	t2 = tmpline2;
	if (drorder) {
		for (; *s1; s1++)
			if (isalnum(*s1) || *s1 == ' ')
				*t1++ = *s1;
		*t1 = '\0';
		for (; *s2; s2++)
			if (isalnum(*s2) || *s2 == ' ')
				*t2++ = *s2;
		*t2 = '\0';
		
		if (casedis) {
			t1 = tmpline1;
			t2 = tmpline2;
			while (*t1 = toupper(*t1))
				++t1;
			while (*t2 = toupper(*t2))
				++t2;
		}
		return strcmp(tmpline1, tmpline2);
	}
	if (casedis) {
		while (*t1++ = toupper(*s1++))
			;
		while (*t2++ = toupper(*s2++))
			;
		return strcmp(tmpline1, tmpline2);
	}
	return strcmp(s1, s2);
}
