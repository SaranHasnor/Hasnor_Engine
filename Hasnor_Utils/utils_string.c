#include "utils_string.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

#pragma warning (disable:4996)	// Allow use of deprecated/unsafe functions (until I decide to recode them)

uint string_length(const char *str)
{
	uint i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return i;
}

char *string_new(const char *value)
{
	uint length = string_length(value);
	char *r = newArray(char, length+1);
	strncpy(r, value, length);
	r[length] = '\0';
	return r;
}

char *string_newCapped(const char *value, uint maxLength)
{
	uint sLen = string_length(value);
	uint length = (sLen<maxLength)?sLen:maxLength;
	char *r = newArray(char, maxLength+1);
	strncpy(r, value, length);
	r[length] = '\0';
	return r;
}

char *string_fromInt(int value)
{
	uint len = nbDigits(value);
	char *res = newArray(char, len+1);
	sprintf(res, "%d", value);
	return res;
}

char *string_fromFloat(float value)
{
	uint len = nbDigits((int)value) + 10;	// Something like that...
	char *res = newArray(char, len+1);
	sprintf(res, "%f", value);
	return res;
}

char *string_fromVector(const float *value)
{
	char *res = newArray(char, 48); // Sounds about right
	sprintf(res, "%f %f %f", value[0], value[1], value[2]);
	return res;
}

void string_copy(char *dest, const char *src)
{
	uint i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

void string_copySafe(char *dest, const char *src)
{ // Copies string src to dest while respecting dst's memory size
	size_t maxSize = Memory.size(dest);
	uint i = 0;

	if (!maxSize) return;

	while (src[i] != '\0' && i < maxSize)
	{
		dest[i] = src[i];
		i++;
	}

	if (i >= maxSize)
	{
		dest[maxSize-1] = '\0';
	}
	else
	{
		dest[i] = '\0';
	}
}

void string_strip(char *str, char c)
{ // Will become redundant when replace is implemented
	uint i=0,j=0;
	bool inString = false;
	uint length = string_length(str);
	
	while (j < length)
	{
		if ((str[j] == '\"' || str[j] == '\'') && (j == 0 || str[j-1] != '\\') && str[j] != c)
			inString = !inString;

		if (!inString)
		{
			if (str[j] == c)
			{
				j++;
				continue;
			}
		}
		str[i] = str[j];
		i++;
		j++;
	}
	str[i] = '\0';
}

void string_appendStr(char **str, const char *value)
{
	uint curLen = string_length(*str);
	uint newLen = curLen + string_length(value);
	*str = (char*)Memory.realloc(*str, sizeof(char) * newLen);
	strcpy(*str + curLen, value);
}

void string_replace(char **str, const char *pattern, const char *value)
{
	// TODO: Open up my old laptop and get that thing I wrote ages ago, put it here, then get angry at it and rewrite it
}

void string_insert(char **str, const char *value, uint pos)
{
	// TODO: Open up my old laptop and get that thing I wrote ages ago, put it here, then get angry at it and rewrite it
}

char *string_substring(const char *str, uint start, uint end)
{
	return string_newCapped(str+start, end-start);
}

bool string_find(const char *str, char c, uint *index)
{
	uint length = string_length(str);
	uint i = 0;
	while (i < length)
	{
		if (str[i] == c)
		{
			if (index) *index = i;
			return true;
		}
		i++;
	}
	return false;
}

bool string_findString(const char *str, const char *pattern, uint *index)
{
	uint length = string_length(str);
	uint i = 0;
	while (i < length)
	{
		int j = 0;
		bool success = true;
		while (i + j < length)
		{
			if (str[i+j] != pattern[j])
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

bool string_compare(const char *str, const char *pattern, bool caseSensitive)
{
	uint length = string_length(str);
	uint i = 0;
	while (i < length)
	{
		if (str[i] != pattern[i] && (caseSensitive || !charsEqualCaseInsensitive(str[i], pattern[i])))
		{
			return true;
		}
		i++;
	}
	return false;
}

int string_parseInt(const char *str)
{
	return atoi(str);
}

float string_parseFloat(const char *str)
{
	return (float)atof(str);
}

void string_parseVector(const char *str, float *vec)
{
	char *pos;
	char *pos2;
	char *temp;
	
	temp = string_new(str);
	pos = strchr(temp, ' ');
	*pos = '\0';
	pos2 = strchr(pos+1, ' ');
	*pos2 = '\0';
	vec[0] = (float)atof(temp);
	vec[1] = (float)atof(pos+1);
	vec[2] = (float)atof(pos2+1);
	destroy(temp);
}

void initStringFunctions(void)
{
	String.create = string_new;
	String.createCapped = string_newCapped;
	String.fromInt = string_fromInt;
	String.fromFloat = string_fromFloat;
	String.fromVector = string_fromVector;
	//String.precompute = NULL;

	String.copy = string_copy;
	String.copySafe = string_copySafe;
	String.strip = string_strip;
	String.append = string_appendStr;
	String.replace = string_replace;
	String.insert = string_insert;

	String.length = string_length;
	String.substring = string_substring;
	String.findChar = string_find;
	String.findString = string_findString;
	String.compare = string_compare;
	String.parseInt = string_parseInt;
	String.parseFloat = string_parseFloat;
	String.parseVector = string_parseVector;
}