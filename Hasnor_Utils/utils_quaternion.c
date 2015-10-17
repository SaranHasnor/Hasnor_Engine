#include "utils_math.h"

static float _workQuat[4];

void quaternion_fromEuler(float out[4], float pitch, float yaw, float roll)
{
	out[0] = cosf(pitch/2.0f) * cosf(yaw/2.0f) * cosf(roll/2.0f) + sinf(pitch/2.0f) * sinf(yaw/2.0f) * sinf(roll/2.0f);
	out[1] = sinf(pitch/2.0f) * cosf(yaw/2.0f) * cosf(roll/2.0f) - cosf(pitch/2.0f) * sinf(yaw/2.0f) * sinf(roll/2.0f);
	out[2] = cosf(pitch/2.0f) * sinf(yaw/2.0f) * cosf(roll/2.0f) + sinf(pitch/2.0f) * cosf(yaw/2.0f) * sinf(roll/2.0f);
	out[3] = cosf(pitch/2.0f) * cosf(yaw/2.0f) * sinf(roll/2.0f) - sinf(pitch/2.0f) * sinf(yaw/2.0f) * cosf(roll/2.0f);
}

void quaternion_identity(float out[4])
{
	quaternion_fromEuler(out, 0.0f, 0.0f, 0.0f);
}

void quaternion_multiply(float out[4], float a[4], float b[4])
{
	_workQuat[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
	_workQuat[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
	_workQuat[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
	_workQuat[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];

	out[0] = _workQuat[0];
	out[1] = _workQuat[1];
	out[2] = _workQuat[2];
	out[3] = _workQuat[3];
}

void quaternion_inverse(float out[4], float src[4])
{
	// TODO
}

void initQuaternionFunctions()
{
	Quaternion.fromEuler = quaternion_fromEuler;
	Quaternion.identity = quaternion_identity;
	Quaternion.multiply = quaternion_multiply;
	Quaternion.inverse = quaternion_inverse;
}
