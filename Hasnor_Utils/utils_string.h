#ifndef HASNOR_STRING_DEFINED
#define HASNOR_STRING_DEFINED

#include "utils_types.h"

/*
utils_string

- Functions and structures for creating and parsing arrays of characters
*/

typedef struct {
	char			*s;
	unsigned int	len;
} string;

typedef struct {
	void (*init)(string *str, char *value, uint optionalSize);
	void (*fromInt)(string *str, int value);
	void (*fromFloat)(string *str, float value);
	void (*fromVector)(string *str, float *vec, uint size);

	void (*strip)(string *str, char c);
	void (*append)(string *str, char *value);
} _string_functions;

_string_functions String;

void initStringFunctions();

#endif