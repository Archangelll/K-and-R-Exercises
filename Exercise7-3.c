#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define PERCLEN 10 // maximum length of a % in printf minus the last character (E.g., "%-20ld" == 6 - 1('d') == 5)
#define MFWLEN  5  // maximum length of a %'s minimum field width (E.g., "%-20d" == strlen("20") == 2)
#define PRCLEN  5  // maximum lenght of a %'s precision (E.g., "%.123s" == strlen("123") == 3)

int main(void)
{
	void minprintf(char *fmt, ...);
	char *s = "hello, world";
	minprintf(":%s:\n", s);
	minprintf(":%10s:\n", s);
	minprintf(":%.10s:\n", s);
	minprintf(":%-10s:\n", s);
	minprintf(":%.15s:\n", s);
	minprintf(":%-15s:\n", s);
	minprintf(":%15.10s:\n", s);
	minprintf(":%-15.10s:\n", s);
	minprintf("===============================================\n");
	minprintf(":%hd:\n", 12345);
	minprintf(":%10hd:\n", 12345);
	minprintf(":%.10hd:\n", 12345);
	minprintf(":%-10hd:\n", 12345);
	minprintf(":%.15hd:\n", 12345);
	minprintf(":%-15hd:\n", 12345);
	minprintf(":%15.10hd:\n", 12345);
	minprintf(":%-15.10hd:\n", 12345);
	minprintf("===============================================\n");
	minprintf(":%f:\n", 123.123);
	minprintf(":%15f:\n", 123.123);
	minprintf(":%.2f:\n", 123.123);
	minprintf(":%-15f:\n", 123.123);
	minprintf(":%.5f:\n", 123.123);
	minprintf(":%15.2f:\n", 123.123);
	minprintf(":%-15.2f:\n", 123.123);

	return 0;
}


void minprintf(char *fmt, ...)
{
	va_list ap; /* points to each unnamed arg in turn */
	char *p, perc[PERCLEN] = "%";

	_Bool ms; // minus sign
	char horl[2]; // short or long or neither + '\0'
	int mfw, prc; // minimum field width, precision
	char mfws[MFWLEN], prcs[PRCLEN];
	horl[1] = '\0';

	va_start(ap, fmt); /* make ap point to 1st unnamed arg */
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		// minus sign
		if (*++p == '-') {
			ms = 1;
			p++;
		}
		else
			ms = 0;
		// minus sign
		// minimum field width
		for (mfw = 0; isdigit(*p); p++)
			mfws[mfw++] = *p;
		mfws[mfw] = '\0';
		if (*p == '*') {
			mfw = va_arg(ap, int);
			sprintf(mfws, "%d", mfw);
			p++;
		}
		// minimum field width
		// precision
		if (*p == '.') {
			prcs[0] = '.';
			for (prc = 1; isdigit(*++p); )
				prcs[prc++] = *p;
			prcs[prc] = '\0';
			if (*p == '*') {
				prc = va_arg(ap, int);
				sprintf(prcs, "%d", prc);
				p++;
			}
		}
		else
			prcs[0] = '\0';
		// precision
		// h or l
		if (*p == 'h' || *p == 'l')
			horl[0] = *p++;
		else
			horl[0] = '\0';
		// h or l
		if (ms)
			strcat(perc, "-");
		if (mfws[0])
			strcat(perc, mfws);
		if (prcs[0])
			strcat(perc, prcs);
		if (horl[0])
			strcat(perc, horl);
		switch (*p) {
			case 'd':
				//printf("%sd\n", perc);
				printf(strcat(perc, "d"), va_arg(ap, int));
				break;
			case 'f':
				//printf("%sf\n", perc);
				printf(strcat(perc, "f"), va_arg(ap, double));
				break;
			case 's':
				//printf("%ss\n", perc);
				printf(strcat(perc, "s"), va_arg(ap, char *));
				break;
			default:
				putchar(*p);
				break;
		}
		strcpy(perc, "%");
	}
	va_end(ap); /* clean up when done */
}
