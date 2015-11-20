#include "utils_math.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdlib.h>

float _cos(float n)
{
	return cosf(n);
}

float _sin(float n)
{
	return sinf(n);
}

float _tan(float n)
{
	return tanf(n);
}

float _acos(float n)
{
	return acosf(n);
}

float _asin(float n)
{
	return asinf(n);
}

float _atan(float n)
{
	return atanf(n);
}

float deg2rad(float deg)
{
	return deg * (float)M_PI / 180.0f;
}

float rad2deg(float rad)
{
	return rad * 180.0f / (float)M_PI;
}

float clamp(float min, float value, float max)
{
	if (value <= min)
	{
		return min;
	}
	if (value >= max)
	{
		return max;
	}
	return value;
}

int _ceil(float n)
{
	return (int)ceilf(n);
}

int _floor(float n)
{
	return (int)floorf(n);
}

int _round(float n)
{
	return _floor(n + 0.5f);
}

float _random(void)
{
	return (float)rand() / (float)RAND_MAX;
}

float _randomInterval(float min, float max)
{
	return min+((float)rand() / (float)RAND_MAX) * (max-min);
}

int	_randomInteger(int min, int max)
{
	return min+rand()%(max-min+1);
}

int _sign(float value)
{
	if (value < 0.0f)
	{
		return -1;
	}
	if (value > 0.0f)
	{
		return 1;
	}
	return 0;
}

void initMathFunctions(void)
{
	Math.pi = (float)M_PI;

	Math.cos = _cos;
	Math.sin = _sin;
	Math.tan = _tan;
	
	Math.acos = _acos;
	Math.asin = _asin;
	Math.atan = _atan;

	Math.deg2rad = deg2rad;
	Math.rad2deg = rad2deg;

	Math.random = _random;
	Math.randomInterval = _randomInterval;
	Math.randomInteger = _randomInteger;

	Math.ceil = _ceil;
	Math.floor = _floor;
	Math.round = _round;

	Math.clamp = clamp;
	Math.sign = _sign;
}