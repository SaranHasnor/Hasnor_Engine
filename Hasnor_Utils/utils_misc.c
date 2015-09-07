#include "utils_types.h"
#include "utils_ctools.h"

#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma warning (disable:4996)	// Allow use of deprecated/unsafe functions

int randomIntBetween(int min, int max)
{
	if (max < min)
	{
		int temp = min;
		min = max;
		max = temp;
	}

	return min+rand()%(max-min+1);
}

float randomValueBetween(float min, float max)
{
	if (max < min)
	{
		float temp = min;
		min = max;
		max = temp;
	}

	return min+((float)rand() / (float)RAND_MAX) * (max-min);
}

int isInRange(float min, float val, float max)
{
	return ((min <= val) && (val <= max));
}

unsigned int nbDigits(int n)
{ // Returns the amount of digits in the integer (also counts the minus sign if required)
	unsigned int i=0;

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

char *quickString(const char *s)
{
	int length = strlen(s);
	char *r = (char*)mem_alloc(sizeof(char)*(length+1));
	strncpy(r, s, length);
	r[length] = '\0';
	return r;
}

char *quickString2(const char *s, unsigned int len)
{
	unsigned int sLen = strlen(s);
	int length = (sLen<len)?sLen:len;
	char *r = (char*)mem_alloc(sizeof(char)*(len+1));
	strncpy(r, s, length);
	r[length] = '\0';
	return r;
}

char *strFromInt(int n)
{
	uint len = nbDigits(n);
	char *res = (char*)mem_alloc(sizeof(char)*len+1);
	sprintf(res, "%d", n);
	return res;
}

int parseInt(char *s)
{
	return atoi(s);
}

char *strFromFloat(float n)
{
	uint len = nbDigits((int)n) + 10;	// Something like that...
	char *res = (char*)mem_alloc(sizeof(char)*len+1);
	sprintf(res, "%f", n);
	return res;
}

float parseFloat(char *s)
{
	return (float)atof(s);
}

char *strFromVec(float vec[3])
{
	char *res = (char*)mem_alloc(sizeof(char)*48); // Sounds about right
	sprintf(res, "%f %f %f", vec[0], vec[1], vec[2]);
	return res;
}

void strip(char *s, char c)
{
	unsigned int i=0,j=0;
	unsigned int len = strlen(s);
	bool inString = false;
	
	while (j < len)
	{
		if ((s[j] == '\"' || s[j] == '\'') && (j == 0 || s[j-1] != '\\') && s[j] != c)
			inString = !inString;

		if (!inString)
		{
			if (s[j] == c)
			{
				j++;
				continue;
			}
		}
		s[i] = s[j];
		i++;
		j++;
	}
	s[i] = '\0';
}

void vecFromStr(float vec[3], char *str)
{
	char *pos;
	char *pos2;
	char *temp;
	
	temp = quickString(str);
	pos = strchr(temp, ' ');
	*pos = '\0';
	pos2 = strchr(pos+1, ' ');
	*pos2 = '\0';
	vec[0] = (float)atof(temp);
	vec[1] = (float)atof(pos+1);
	vec[2] = (float)atof(pos2+1);
	mem_free(temp);
}

float atLeastOne(float x)
{ // Utility function that prevents x's rounded value to be equal to 0 unless it actually is 0 (yes, seriously)
	if (x > -1 && x < 0)
		return -1;
	if (x > 0 && x < 1)
		return 1;

	return x;
}

void strcpy_safe(char *dst, char *src)
{ // Copies string src to dst while respecting dst's memory size
	size_t maxSize = mem_size(dst);
	unsigned int i=0;

	if (!maxSize) return;

	while (src[i] != '\0' && i < maxSize)
	{
		dst[i] = src[i];
		i++;
	}

	if (i >= maxSize)
	{
		dst[maxSize-1] = '\0';
	}
	else
	{
		dst[i] = '\0';
	}
}

bool charsEqualCaseInsensitive(char a, char b)
{
	return a == b
		|| (a >= 'a' && a <= 'z' && b == (a + 'A' - 'a'))
		|| (a >= 'A' && a <= 'Z' && b == (a + 'a' - 'A'));
}

void quit(void)
{ // Exit the program
	mem_free_all();
	exit(0);
}