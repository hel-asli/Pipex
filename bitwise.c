#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	/** because c is a low level language we use bitwise operators to manipluate individual bits in a variable  --- the operators are used in other heigh level language **/
	int a = 1;
	printf("%d\n", a);
	a <<= 7;
	printf("%d\n", a);
	return (0);
}

