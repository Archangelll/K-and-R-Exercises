#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#define PERCLEN 10 // maximum length of a % in scanf (E.g., "%*20Lf" == 6)

int main(void)
{
	void minscanf(char *fmt, ...);
	
	int a = 0;
	short b = 0;
	long c = 0;
	float d = 0;
	double e = 0;
	long double f = 0;
	char g = '\0';
	char h[50] = "";

	//minscanf("%d %hd %ld %f %lf %Lf %*c %c %s", &a, &b, &c, &d, &e, &f, &g, h);
	minscanf("%%\n%*5s %10s", h); // isspace('\n') == true
	
	//minscanf("%d", &a);
	//printf("%d\n", a);
	
	//minscanf("%hd", &b);
	//printf("%hd\n", b);
	
	//minscanf("%ld", &c);
	//printf("%ld\n", c);
	
	//minscanf("%f", &d);
	//printf("%f\n", d);
	
	//minscanf("%lf", &e);
	//printf("%lf\n", e);
	
	//minscanf("%Lf", &f);
	//printf("%Lf\n", f);
	
	//minscanf("%*c %c", &g);
	//printf("%c\n", g);
	
	//minscanf("%s", h);
	printf("%s\n", h);

	return 0;
}


void minscanf(char *fmt, ...)
{
	va_list ap;
	char perc[PERCLEN] = "%";
	int cperc = 1;

	va_start(ap, fmt);
	for ( ; *fmt; fmt++) {
		if (isspace(*fmt))
			continue;
		else if (*fmt != '%' || ((fmt[1] == '%') ? ++fmt, 1 : 0)) {
			if (getchar() != *fmt)
				return;
			continue;
		}
		if (*++fmt == '*')
			perc[cperc++] = *fmt++;
		while (isdigit(*fmt))
			perc[cperc++] = *fmt++;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
			perc[cperc++] = *fmt++;

		perc[cperc++] = *fmt;
		perc[cperc] = '\0';
		if (perc[1] == '*')
			scanf(perc);
		else
			switch (*fmt) {
				case 'd':
					if (perc[cperc - 2] == 'h')
						scanf(perc, va_arg(ap, short int *));
					else if (perc[cperc - 2] == 'l')
						scanf(perc, va_arg(ap, long int *));
					else
						scanf(perc, va_arg(ap, int *));
					break;
				case 'f':
					if (perc[cperc - 2] == 'L')
						scanf(perc, va_arg(ap, long double *));
					else if (perc[cperc - 2] == 'l')
						scanf(perc, va_arg(ap, double *));
					else
						scanf(perc, va_arg(ap, float *));
					break;
				case 'c':
				case 's':
					scanf(perc, va_arg(ap, char *));
					break;
				default:
					putchar(*fmt);
					break;
			}
		cperc = 1;
	}
	va_end(ap);
}
