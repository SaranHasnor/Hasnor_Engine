#include "utils_vector.h"

#define _USE_MATH_DEFINES
#include <math.h>

static float _workVec[3];

void vectorSet(float v[3], float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void vectorCopy(float dest[3], const float src[3])
{
	vectorSet(dest, src[0], src[1], src[2]);
}

void vectorClear(float v[3])
{
	vectorSet(v, 0.0f, 0.0f, 0.0f);
}

void vectorNegate(float v[3])
{
	vectorSet(v, -v[0], -v[1], -v[2]);
}

float vectorLength(const float v[3])
{
	return sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

float vectorSquareLength(const float v[3])
{
	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
}

float vectorNormalize(float out[3], const float src[3])
{
	const float length = vectorLength(src);
	if (length)
	{
		vectorSet(out, src[0]/length, src[1]/length, src[2]/length);
	}
	return length;
}

void vectorAdd(float dest[3], const float a[3], const float b[3])
{ // dest = a + b
	int i;
	for (i=0; i<3; i++)
		dest[i] = a[i] + b[i];
}

void vectorSubtract(float dest[3], const float a[3], const float b[3])
{ // dest = a - b
	int i;
	for (i=0; i<3; i++)
		dest[i] = a[i] - b[i];
}

void vectorMult(float dest[3], const float a[3], const float b[3])
{ // dest = a x b
	vectorSet(dest, a[0]*b[0], a[1]*b[1], a[2]*b[2]);
}

void vectorScale(float dest[3], float s, const float src[3])
{ // dest = s * src
	vectorSet(dest, s*src[0], s*src[1], s*src[2]);
}

void vectorMA(float out[3], const float org[3], float s, const float move[3])
{ // v = org + s * move
	int i;
	for (i=0; i<3; i++)
	{
		out[i] = org[i] + s * move[i];
	}
}

void vectorTransition(float v[3], const float org[3], const float dest[3], float percentage)
{ // v = org + percentage * (dest - org)
	int i;
	for (i=0; i<3; i++)
	{
		v[i] = org[i] + percentage * (dest[i] - org[i]);
	}
}

float vectorDot(const float a[3], const float b[3])
{ // Returns the dot product of both vectors
	float res = 0;
	int i;
	for (i=0; i<3; i++)
	{
		res += a[i] * b[i];
	}
	return res;
}

void vectorCross(float out[3], const float a[3], const float b[3])
{
	_workVec[0] = a[1] * b[2] - a[2] * b[1];
	_workVec[1] = a[2] * b[0] - a[0] * b[2];
	_workVec[2] = a[0] * b[1] - a[1] * b[0];

	out[0] = _workVec[0];
	out[1] = _workVec[1];
	out[2] = _workVec[2];
}

void vectorRotate(float vec[3], const float targetAxis[3][3])
{ // Rotates the given vector to match the target axis
	_workVec[0] = vectorDot(vec, targetAxis[0]);
	_workVec[1] = vectorDot(vec, targetAxis[1]);
	_workVec[2] = vectorDot(vec, targetAxis[2]);
	
	vec[0] = _workVec[0];
	vec[1] = _workVec[1];
	vec[2] = _workVec[2];
}

float vectorDistance(const float a[3], const float b[3])
{
	vectorSubtract(_workVec, a, b);
	return vectorLength(_workVec);
}

float vectorSquareDistance(const float a[3], const float b[3])
{
	return (b[0] - a[0]) * (b[0] - a[0]) +
		(b[1] - a[1]) * (b[1] - a[1]) +
		(b[2] - a[2]) * (b[2] - a[2]);
}

void vectoangles(const float vec[3], float angles[3])
{
	double	forward;
	double	yaw, pitch;
	
	if ( vec[1] == 0 && vec[0] == 0 ) {
		yaw = 0;
		if ( vec[2] > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( vec[0] ) {
			yaw = ( atan2 ( (double)vec[1], (double)vec[0] ) * 180 / M_PI );
		}
		else if ( vec[1] > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = (double)sqrtf( vec[0]*vec[0] + vec[1]*vec[1] );
		pitch = ( atan2((double)vec[2], forward) * 180 / M_PI );
		if ( pitch < 0 ) {
			pitch += 360;
		}
	}

	angles[0] = (float)(-pitch);
	angles[1] = (float)(yaw);
	angles[2] = 0.0f;
}

void AngleVectors(const float angles[3], float *forward, float *right, float *up) {
	double	angle;
	double	sr, sp, sy, cr, cp, cy;
	
	angle = angles[1] * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[0] * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[2] * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);
	
	if (forward)
	{
		forward[0] = (float)(cp*cy);
		forward[1] = (float)(cp*sy);
		forward[2] = (float)(-sp);
	}
	if (right)
	{
		right[0] = (float)(-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (float)(-1*sr*sp*sy+-1*cr*cy);
		right[2] = (float)(-1*sr*cp);
	}
	if (up)
	{
		up[0] = (float)(cr*sp*cy+-sr*-sy);
		up[1] = (float)(cr*sp*sy+-sr*cy);
		up[2] = (float)(cr*cp);
	}
}

float vectorAngleBetween_norm(const float normA[3], const float normB[3])
{
	return 90.0f - 90.0f * vectorDot(normA, normB);
}

float vectorAngleBetween(const float a[3], const float b[3])
{
	float normA[3], normB[3];
	vectorNormalize(normA, a);
	vectorNormalize(normB, b);
	return vectorAngleBetween_norm(normA, normB);
}

float vectorOrientedAngleBetween(const float a[3], const float b[3], const float normal[3])
{
	float normA[3], normB[3];
	float angle;
	float cross[3];

	vectorNormalize(normA, a);
	vectorNormalize(normB, b);

	angle = vectorAngleBetween_norm(normA, normB);

	vectorCross(cross, normA, normB);
	if (vectorDot(normal, cross) < 0.0f)
	{
		angle = -angle;
	}

	return angle;
}

void initVectorFunctions(void)
{
	vectorSet(Vector.axis[0], 1.0f, 0.0f, 0.0f);
	vectorSet(Vector.axis[1], 0.0f, 1.0f, 0.0f);
	vectorSet(Vector.axis[2], 0.0f, 0.0f, 1.0f);

	vectorClear(Vector.zero);

	Vector.add = vectorAdd;
	Vector.axisFromAngles = AngleVectors;
	Vector.clear = vectorClear;
	Vector.copy = vectorCopy;
	Vector.distance = vectorDistance;
	Vector.squareDistance = vectorSquareDistance;
	Vector.dotProduct = vectorDot;
	Vector.crossProduct = vectorCross;
	Vector.squareLength = vectorSquareLength;
	Vector.length = vectorLength;
	Vector.lerp = vectorTransition;
	Vector.multiply = vectorMult;
	Vector.multiplyAdd = vectorMA;
	Vector.normalize = vectorNormalize;
	Vector.rotate = vectorRotate;
	Vector.scale = vectorScale;
	Vector.set = vectorSet;
	Vector.subtract = vectorSubtract;
	Vector.toAngles = vectoangles;
	Vector.angleBetween = vectorAngleBetween;
	Vector.orientedAngleBetween = vectorOrientedAngleBetween;
}
