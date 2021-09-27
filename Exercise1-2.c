#include <stdio.h>

int main(void)
{	
	printf("========================================================================================================================\012");
	printf("The same printf() can have different outputs, because it sometimes depends on the size of the output window\n");
	printf("(\\a) --> play the sys alarm.\a\n");
	printf("(\\b) --> moves the current cursor position to the previous position on the current line.\b.\n");
	printf("(\\f) --> can be used to skip to the next page of something.\n");
	printf("(\\r) --> moves the position of the cursor to the first position on the same line.\r(\\r)\n");
	printf("(\\?) --> prints a question mark, it tells the compiler that '?' is not part of a trigraph.\n");
	printf("(\\') --> prints a single quote.\n");
	printf("(\\\") --> prints a double quote.\n");
	printf("(\\\\) --> prints a backslash.\n");
	printf("(\\ooo) --> ooo is an octal number that represents the ascii value for some escape sequence, for example \\012 == \\n\n");
	printf("(\\xhh) --> xhh is an hexadecimal number that represents the ascii value for some escape sequence, for example, \\x0A == \\n\n");
	printf("(\\t) -->\tprints a horizontal tab.\n");
	printf("(\\v) --> prints\va\vvertical\vtab\n");
	printf("========================================================================================================================\x0A");

	return 0;
}

