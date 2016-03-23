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

bool _randomBool(void)
{
	return rand() > RAND_MAX / 2;
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

float _sqrt(float a)
{
	return sqrtf(a);
}

float _sqrtI(int a)
{
	return (float)sqrtl(a);
}

float _pow(float value, int power)
{
	float res = 1.0f;
	int i;
	for (i = 0; i < power; i++)
	{
		res *= value;
	}
	return res;
}

int _powI(int value, int power)
{
	int res = 1;
	int i;
	for (i = 0; i < power; i++)
	{
		res *= value;
	}
	return res;
}

float _abs(float a)
{
	return a < 0 ? -a : a;
}

int _absI(int a)
{
	return a < 0 ? -a : a;
}

void initMathFunctions(void)
{
	Math.pi = (float)M_PI;

	Math.sqrt = _sqrt;
	Math.sqrtI = _sqrtI;
	Math.pow = _pow;
	Math.powI = _powI;
	Math.abs = _abs;
	Math.absI = _absI;

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
	Math.randomBool = _randomBool;

	Math.ceil = _ceil;
	Math.floor = _floor;
	Math.round = _round;

	Math.clamp = clamp;
	Math.sign = _sign;
}