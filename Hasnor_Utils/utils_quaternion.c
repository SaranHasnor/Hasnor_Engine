#include "utils_quaternion.h"

#include <math.h>

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