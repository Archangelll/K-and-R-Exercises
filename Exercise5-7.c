int readlines(char stor[], int storsiz, char *lineprt[], int maxlines)
{
	int len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	p = stor;
	while ((len = getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || stor + storsiz - p < len)
			return -1;
		else {
			line[len-1] = '\0';
			strcpy(p, line);
			lineprt[nlines++] = p;
			p += len;
		}
		return nlines;
}
