#include "utils_string.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

#pragma warning (disable:4996)	// Allow use of deprecated/unsafe functions

void string_initStr(string *str, const char *value, uint length)
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

string *string_new(const char *value)
{
	string *newString = newObject(string);
	string_initStr(newString, value, 0);
	return newString;
}

string *string_duplicate(const string *str)
{
	return string_new(str->s);
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

void string_initVector(string *str, const float *value, uint size)
{
	str->s = strFromVec(value);
	str->len = strlen(str->s);
}

void string_strip(string *str, char c)
{ // Will become redundant when replace is implemented
	unsigned int i=0,j=0;
	bool inString = false;
	
	while (j < str->len)
	{
		if ((str->s[j] == '\"' || str->s[j] == '\'') && (j == 0 || str->s[j-1] != '\\') && str->s[j] != c)
			inString = !inString;

		if (!inString)
		{
			if (str->s[j] == c)
			{
				j++;
				continue;
			}
		}
		str->s[i] = str->s[j];
		i++;
		j++;
	}
	str->s[i] = '\0';
	str->len = i;
}

void string_appendStr(string *str, const char *value)
{
	size_t newLen = str->len + strlen(value);
	str->s = (char*)Memory.realloc(str->s, sizeof(char) * newLen);
	strcpy(str->s + str->len, value);
	str->len = newLen;
}

void string_replace(string *str, const char *pattern, const char *value)
{
	// TODO: Open up my old laptop and get that thing I wrote ages ago, put it here, then get angry at it and rewrite it
}

uint string_length(const char *str)
{
	uint i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return i;
}

string *string_substring(const string *str, uint start, uint end)
{
	string *newString = newObject(string);
	string_initStr(newString, &str->s[start], end-start);
	return newString;
}

bool string_find(const string *str, char c, uint *index)
{
	uint i = 0;
	while (i < str->len)
	{
		if (str->s[i] == c)
		{
			if (index) *index = i;
			return true;
		}
		i++;
	}
	return false;
}

bool string_findString(const string *str, const char *pattern, uint *index)
{
	uint i = 0;
	while (i < str->len)
	{
		int j = 0;
		bool success = true;
		while (i + j < str->len)
		{
			if (str->s[i+j] != pattern[j])
			{
				success = false;
				break;
			}
			j++;
		}

		if (success)
		{
			if (index) *index = i;
			return true;
		}

		i++;
	}
	return false;
}

bool string_compare(const string *str, const char *pattern, bool caseSensitive)
{
	uint i = 0;
	while (i < str->len)
	{
		if (str->s[i] != pattern[i])
		{
			return true;
		}
		i++;
	}
	return false;
}

int string_parseInt(const string *str)
{
	return atoi(str->s);
}

float string_parseFloat(const string *str)
{
	return (float)atof(str->s);
}

void initStringFunctions(void)
{
	String.create = string_new;
	String.init = string_initStr;
	String.fromInt = string_initInt;
	String.fromFloat = string_initFloat;
	String.fromVector = string_initVector;
	String.duplicate = string_duplicate;

	String.strip = string_strip;
	String.append = string_appendStr;
	String.replace = string_replace;

	String.length = string_length;
	String.substring = string_substring;
	String.findChar = string_find;
	String.findString = string_findString;
	String.compare = string_compare;
	String.parseInt = string_parseInt;
	String.parseFloat = string_parseFloat;
}