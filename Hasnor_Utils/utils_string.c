#include "utils_string.h"
#include "utils.h"

#include <string.h>

#pragma warning (disable:4996)	// Allow use of deprecated/unsafe functions

void string_initStr(string *str, char *value, uint length)
{
	if (length)
	{
		str->s = quickString2(value, length);
	}
	else
	{
		str->s = quickString(value);
	}
	str->len = strlen(str->s);
}

void string_initInt(string *str, int value)
{
	str->s = strFromInt(value);
	str->len = strlen(str->s);
}

void string_initFloat(string *str, float value)
{
	str->s = strFromFloat(value);
	str->len = strlen(str->s);
}

void string_initVector(string *str, float *value, uint size)
{
	str->s = strFromVec(value);
	str->len = strlen(str->s);
}

void string_strip(string *str, char c)
{
	strip(str->s, c);
	str->len = strlen(str->s);
}

void string_appendStr(string *str, char *value)
{
	size_t newLen = str->len + strlen(value);
	str->s = (char*)Memory.realloc(str->s, sizeof(char) * newLen);
	strcpy(str->s + str->len, value);
	str->len = newLen;
}

void initStringFunctions(void)
{
	String.init = string_initStr;
	String.fromInt = string_initInt;
	String.fromFloat = string_initFloat;
	String.fromVector = string_initVector;
	String.strip = string_strip;
	String.append = string_appendStr;
}