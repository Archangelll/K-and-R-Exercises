_Bool strend(char *s, char *t)
{
	char *cmp = t;
	while (*s)
		if (*s++ == *t)
			++t;
		else
			t = cmp;
	if (*t)
		return 0;
	return 1;
}
