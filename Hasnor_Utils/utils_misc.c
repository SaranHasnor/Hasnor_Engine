#include "utils_types.h"
#include "utils_ctools.h"

#include <stdlib.h>

uint nbDigits(int n)
{ // Returns the amount of digits in the integer (also counts the minus sign if required)
	uint i=0;

	if (n == 0)
		return 1;

	while (abs(n) > 0)
	{
		i++;
		n /= 10;
	}

	if (n<0)
		i++;

	return i;
}

bool charsEqualCaseInsensitive(char a, char b)
{
	return a == b
		|| (a >= 'a' && a <= 'z' && b == (a + 'A' - 'a'))
		|| (a >= 'A' && a <= 'Z' && b == (a + 'a' - 'A'));
}

void quit(void)
{ // Exit the program
	Memory.freeAll();
	exit(0);
}