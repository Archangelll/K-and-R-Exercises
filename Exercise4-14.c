#include <stdio.h>

#define swap(t, x, y)\
{\
	if (#x[0] != 'a' && #y[0] != 'a') {\
		t a = x; x = y; y = a;\
	}\
	else if (#x[0] != 'b' && #y[0] != 'b') {\
		t b = x; x = y; y = b;\
	}\
	else {\
		t c = x; x = y; y = c;\
	}\
}

int main(void)
{
	int x = 516;
	int y = 6564;
	swap(int, x, y);
	printf("x = %d\ny = %d\n", x, y);

	return 0;
}
