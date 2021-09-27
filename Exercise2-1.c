#include <stdio.h>
#include <limits.h>
#include <float.h>

float       getMaximumFloat(void);
double      getMaximumDouble(void);
long double getMaximumLongDouble(void);

int main(void)
{
	printf("Integer Types:\n");
		printf("\tSTANDARD HEADER:\n");
			printf("\t\tunsigned char\t\t[0, %u]\n"   , UCHAR_MAX);
			printf("\t\tsigned char\t\t[%d, %d]\n"    , SCHAR_MIN, SCHAR_MAX);
			printf("\t\tunsigned short\t\t[0, %u]\n"  , USHRT_MAX);
			printf("\t\tsigned short\t\t[%d, %d]\n"   , SHRT_MIN, SHRT_MAX);
			printf("\t\tunsigned int\t\t[0, %u]\n"    , UINT_MAX);
			printf("\t\tsigned int\t\t[%d, %d]\n"     , INT_MIN, INT_MAX);
			printf("\t\tunsigned long\t\t[0, %lu]\n"  , ULONG_MAX);
			printf("\t\tsigned long\t\t[%ld, %ld]\n"  , LONG_MIN, LONG_MAX);
		printf("\tDIRECT COMPUTATION: (Two's complement)\n"); 
			printf("\t\tunsigned char\t\t[0, %d]\n"   ,   (unsigned char) -1);
			printf("\t\tsigned char\t\t[%d, %d]\n"    , ~((unsigned char) -1 >> 1), (unsigned char) -1 >> 1);
			printf("\t\tunsigned short\t\t[0, %d]\n"  ,   (unsigned short) -1);
			printf("\t\tsigned short\t\t[%d, %d]\n"   , ~((unsigned short) -1 >> 1), (unsigned short) -1 >> 1);
			printf("\t\tunsigned int\t\t[0, %u]\n"    ,   -1);
			printf("\t\tsigned int\t\t[%d, %d]\n"     , ~((unsigned) -1 >> 1), (unsigned) -1 >> 1);
			printf("\t\tunsigned long\t\t[0, %lu]\n"  ,   (unsigned long) -1);
			printf("\t\tsigned long\t\t[%ld, %ld]\n"  , ~((unsigned long) -1 >> 1), (unsigned long) -1 >> 1);
	printf("Floating-Point Types\n");
		printf("\tSTANDARD HEADER:\n");
			printf("\t\tfloat\t\t\t[%E, %E]\n"        , -FLT_MAX, FLT_MAX);
			printf("\t\tdouble\t\t\t[%lE, %lE]\n"     , -DBL_MAX, DBL_MAX);
			printf("\t\tlong double\t\t[%LE, %LE]\n"  , -LDBL_MAX, LDBL_MAX);
		printf("\tDIRECT COMPUTATION: (IEEE 754)\n");
			printf("\t\tfloat\t\t\t[%E, %E]\n"        , -getMaximumFloat(), getMaximumFloat());
			printf("\t\tdouble\t\t\t[%lE, %lE]\n"     , -getMaximumDouble(), getMaximumDouble());
			printf("\t\tlong double\t\t[%LE, %LE]\n"  , -getMaximumLongDouble(), getMaximumLongDouble());

	return 0;
}


float getMaximumFloat(void)
{
	unsigned int exp = 1, m_dig = 0;
	float m = 1, tmp = 1, ptmp;
/*	
	while((m *= 2) + 1 != m)
		++m_dig;               max mantissa    undefined 
	while(tmp < (tmp *= 2))    max exponent    behavior
		++exp;
*/
	while(1){
		m *= 2;
		if(m + 1 == m)
			 break;
		++m_dig;
	}
	--m;

	while(1){
		ptmp = tmp;
		tmp *= 2;
		if(ptmp >= tmp)
			break;
		++exp;
	}
	exp -= 2;

	for(; exp > m_dig; --exp)
		m *= 2;
	return m;
}


double getMaximumDouble(void)
{
	unsigned int exp = 1, m_dig = 0;
	double m = 1, tmp = 1, ptmp;
/*	
	while((m *= 2) + 1 != m)
		++m_dig;               max mantissa    undefined 
	while(tmp < (tmp *= 2))    max exponent    behavior
		++exp;
*/
	while(1){
		m *= 2;
		if(m + 1 == m)
			 break;
		++m_dig;
	}
	--m;

	while(1){
		ptmp = tmp;
		tmp *= 2;
		if(ptmp >= tmp)
			break;
		++exp;
	}
	exp -= 2;

	for(; exp > m_dig; --exp)
		m *= 2;
	return m;
}


long double getMaximumLongDouble(void)
{
	unsigned int exp = 1, m_dig = 0;
	long double m = 1, tmp = 1, ptmp;
/*	
	while((m *= 2) + 1 != m)
		++m_dig;               max mantissa    undefined 
	while(tmp < (tmp *= 2))    max exponent    behavior
		++exp;
*/
	while(1){
		m *= 2;
		if(m + 1 == m)
			 break;
		++m_dig;
	}
	--m;

	while(1){
		ptmp = tmp;
		tmp *= 2;
		if(ptmp >= tmp)
			break;
		++exp;
	}
	exp -= 2;

	for(; exp > m_dig; --exp)
		m *= 2;	
	return m;
}
