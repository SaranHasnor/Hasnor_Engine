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

void string_initStr(string *str, char *value);
void string_initStrFixedSize(string *str, char *value, unsigned int length);
void string_initInt(string *str, int value);
void string_initFloat(string *str, float value);
void string_initVector(string *str, float *vec, uint size);

void string_strip(string *str, char c);

#endif