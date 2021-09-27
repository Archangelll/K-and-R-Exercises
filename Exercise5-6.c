int getline(char *s, size_t max)
{
	int c;
	size_t i;
	for (i = 0; i < max && (c = getchar()) != '\n' && c != EOF; ++i, *s++ = c)
		;
	if (c == '\n') {
		*s++ = '\n';
		++i;
	}
	*s = '\0';
	return i;
}


int atoi(char *s)
{
	int sign, val = 0;
	
	sign = (*s == '-') ? (++s, -1) : 1;
	if (*s == '+')
		++s;
	while (*s)
		val = val * 10 + (*s++ - '0');
	
	return val * sign;
}


void itoa(int n, char *s)
{
	char tmp, *cp;
	if (n < 0) {
		*s++ = '-';
		cp = s;
		while (n) {
			*s++ = -(n % 10) + '0';
			n /= 10;
		}
	}
	else {
		cp = s;
		while (n) {
			*s++ = n % 10 + '0';
			n /= 10;
		}
	}
	*s-- = '\0';
	while (s > cp) {
		tmp   = *cp;
		*cp++ = *s;
		*s--  = tmp;
	}
}


void reverse(char *s)
{
	char tmp, *t = s;

	while (*t)
		t++;
	if (t > s)
		--t;
	while (t > s) {
		tmp  = *s;
		*s++ = *t;
		*t-- = tmp;
	}
}


int strindex(char *s, char *t)
{
	int i, j;

	i = j = 0;
	while (*s) {
		if (!*t)
			break;
		else if (*s == *t)
			++t;
		else {
			t -= i - j;
			j = i+1;
		}
		++i;
		++s;
	}
	if (!*t)
		return j;
	return -1;
}


int getop(char *s)
{
	int c;

	while ((*s = c = getch()) == ' ' || c == '\t')
		;
	*(s+1) = '\0';
	if (!isdigit(c) && c != '.')
		return c;
	if (isdigit(c))
		while (isdigit(*++s = c = getch()))
			;
	if (c == '.')
		while (isdigit(*++s = c = getch()))
			;
	*s = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}
