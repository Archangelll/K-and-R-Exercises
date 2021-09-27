char *strncpy(char *s, const char *t, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		if (*s++ = *t)
			++t;
	return (s -= n);
}


char *strncat(char *s, const char *t, size_t n)
{
	char *scp = s;
	while (*s)
		s++;
	for (size_t i = 0; i < n; ++i)
		if (*s++ = *t)
			++t;
		else
			break;
	*s = '\0';
	return scp;
}


int strncmp(char *s, const char *t, size_t n)
{
	size_t i;

	for (i = 0; (i < n) && (*s == *t); ++i, ++s, ++t)
		;
	if (i == n)
		return 0;
	return *s - *t;
}
