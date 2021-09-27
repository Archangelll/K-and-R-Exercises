#include <stdio.h>

void  escape(char s[], char t[]);
void rescape(char t[], char s[]);

int main(void)
{
	char t[] = "Hello\tmy name is\nRoger\tD'avilla\n";
	char s[100];
	escape(s, t);
	printf("%s\n", s);
	rescape(t, s);
	printf("%s\n", s);

	return 0;
}

void escape(char s[], char t[])
{
	int  i = 0, j = 0;
	while (t[i] != '\0') {
		switch (t[i]) {
			case '\n':
				s[j++] = '\\';
				s[j] = 'n';
				break;
			case '\t':
				s[j++] = '\\';
				s[j] = 't';
				break;
			default:
				s[j] = t[i];
				break;
		}
		++i; ++j;
	}
	s[j] = '\0';
}


void rescape(char s[], char t[])
{
	int i, j;

	i = j = 0;
	while (s[i] != '\0') {
		switch (s[i]) {
			case '\\':
				switch (s[++i]) {
					case 'n':
						t[j] = '\n';
						++i;
						break;
					case 't':
						t[j] = '\t';
						++i;
						break;
					default:
						t[j] = '\\';
						break;
				}
				break;
			default:
				t[j] = s[i++];
				break;
		}
		++j;
	}
	t[j] = s[i];
}
