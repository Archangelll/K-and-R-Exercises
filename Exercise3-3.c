#include <stdio.h>

int in(char c, char s[], int n);
void expand(char s1[], char s2[]);

int main(void)
{
	char s1[1000] = "-a-z0-9-3\nA-Z-A\nA-A\nA-Z\nZ-A\na-z\nz-a\n0-9\n9-0-", s2[1000];
	expand(s1, s2);
	printf("%s\n", s2);

	return 0;
}


void expand(char s1[], char s2[])
{
	char la[27] = "abcdefghijklmnopqrstuvwxyz";
	char up[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char dg[11] = "0123456789";

	int i, j;
	int pc, nc;
	int p1, p2;

	i = j = pc = nc = 0;
	while (s1[i] != '\0') {
		if (s1[i] == '-') {
			if      ((p1 = in(pc, la, 27)) != -1 && (p2 = in(nc, la, 27)) != -1) {
				--j;
				if (p1 <= p2)
					for (; p1 <= p2; ++p1)
						s2[j++] = la[p1];
				else
					for (; p1 >= p2; --p1)
						s2[j++] = la[p1];
				pc = s1[++i];
				nc = s1[++i + 1];
				continue;
			}
			else if ((p1 = in(pc, up, 27)) != -1 && (p2 = in(nc, up, 27)) != -1) {
				--j;
				if (p1 <= p2)
					for (; p1 <= p2; ++p1)
						s2[j++] = up[p1];
				else
					for (; p1 >= p2; --p1)
						s2[j++] = up[p1];
				pc = s1[++i];
				nc = s1[++i + 1];
				continue;
			}
			else if ((p1 = in(pc, dg, 11)) != -1 && (p2 = in(nc, dg, 11)) != -1) {
				--j;
				if (p1 <= p2)
					for (; p1 <= p2; ++p1)
						s2[j++] = dg[p1];
				else
					for (; p1 >= p2; --p1)
						s2[j++] = dg[p1];
				pc = s1[++i];
				nc = s1[++i + 1];
				continue;
			}
		}
		pc = s1[i];
		s2[j++] = s1[i++];
		nc = s1[i+1];
	}
	s2[j] = s1[i];
}


int in(char c, char s[], int n)
{
	for (int i = 0; i < n-1; ++i)
		if (s[i] == c)
			return i;
	return -1;
}
