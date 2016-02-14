#include "utils_quaternion.h"

#define _USE_MATH_DEFINES
#include <math.h>

static float _workQuat[4];

void quaternion_set(float out[4], float x, float y, float z, float w)
{
	out[0] = x;
	out[1] = y;
	out[2] = z;
	out[3] = w;
}

void quaternion_copy(float out[4], const float src[4])
{
	out[0] = src[0];
	out[1] = src[1];
	out[2] = src[2];
	out[3] = src[3];
}

void quaternion_fromEuler(float out[4], float pitch, float yaw, float roll)
{ // We use a left-hand axis, so yaw and roll are swapped
	float cp, cy, cr;
	float sp, sy, sr;

	cp = cosf(pitch/2.0f);
	cy = cosf(roll/2.0f);
	cr = cosf(yaw/2.0f);

	sp = sinf(pitch/2.0f);
	sy = sinf(-roll/2.0f);
	sr = sinf(yaw/2.0f);

	out[0] = sp * cy * cr - cp * sy * sr;
	out[1] =-cp * sy * cr - sp * cy * sr;
	out[2] = cp * cy * sr + sp * sy * cr;
	out[3] = cp * cy * cr - sp * sy * sr;
}

void quaternion_aroundAxis(float out[4], const float axis[3], float angle)
{
	const float s = sinf(angle / 2.0f);

	out[0] = s * axis[0];
	out[1] = s * axis[1];
	out[2] = s * axis[2];
	out[3] = cosf(angle / 2.0f);
}

void quaternion_fromMatrix(float out[4], const float matrix[16])
{
	float trace = matrix[0] + matrix[5] + matrix[10];
	if (trace > -1.0f)
	{
		float w4;
		out[3] = sqrtf(1.0f + trace) / 2.0f;
		w4 = 4.0f * out[3];
		out[0] = (matrix[6] - matrix[9]) / w4;
		out[1] = (matrix[8] - matrix[2]) / w4;
		out[2] = (matrix[1] - matrix[4]) / w4;
	}
	else
	{ // These have not been tested yet...
		if (trace > 0.0f)
		{
			float s = 2.0f * sqrtf(1.0f + trace);
			out[3] = 0.25f * s;
			out[0] = (matrix[9] - matrix[6]) / s;
			out[1] = (matrix[2] - matrix[8]) / s;
			out[2] = (matrix[4] - matrix[1]) / s;
		}
		else
		{
			if (matrix[0] > matrix[5] && matrix[0] > matrix[10])
			{
				float s = 2.0f * sqrtf(1.0f + matrix[0] - matrix[5] - matrix[10]);
				out[3] = (matrix[9] - matrix[6]) / s;
				out[0] = 0.25f * s;
				out[1] = (matrix[1] + matrix[4]) / s;
				out[2] = (matrix[2] + matrix[8]) / s;
			}
			else if (matrix[5] > matrix[10])
			{
				float s = 2.0f * sqrtf(1.0f + matrix[5] - matrix[0] - matrix[10]);
				out[3] = (matrix[2] - matrix[8]) / s;
				out[0] = (matrix[1] + matrix[4]) / s;
				out[1] = 0.25f * s;
				out[2] = (matrix[6] + matrix[9]) / s;
			}
			else
			{
				float s = 2.0f * sqrtf(1.0f + matrix[10] - matrix[0] - matrix[5]);
				out[3] = (matrix[4] - matrix[1]) / s;
				out[0] = (matrix[2] + matrix[8]) / s;
				out[1] = (matrix[6] + matrix[9]) / s;
				out[2] = 0.25f * s;
			}
		}
	}
}

void quaternion_identity(float out[4])
{
	out[0] = 0.0f;
	out[1] = 0.0f;
	out[2] = 0.0f;
	out[3] = 1.0f;
}

void quaternion_multiply(float out[4], const float a[4], const float b[4])
{
	const float w1 = a[3];
	const float x1 = a[0];
	const float y1 = a[1];
	const float z1 = a[2];
	const float w2 = b[3];
	const float x2 = b[0];
	const float y2 = b[1];
	const float z2 = b[2];

	out[0] = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
	out[1] = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
	out[2] = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;
	out[3] = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
}

void quaternion_conjugate(float out[4], const float src[4])
{
	out[0] = -src[0];
	out[1] = -src[1];
	out[2] = -src[2];
	out[3] =  src[3];
}

float quaternion_normalize(float out[4], const float src[4])
{
	const float norm = sqrtf(src[0] * src[0] + src[1] * src[1] + src[2] * src[2] + src[3] * src[3]);
	out[0] = src[0] / norm;
	out[1] = src[1] / norm;
	out[2] = src[2] / norm;
	out[3] = src[3] / norm;
	return norm;
}

void rotateVector(float out[3], const float src[3], const float quat[4])
{
	float posQuat[4];
	float temp[4], conj[4];

	posQuat[0] = src[0];
	posQuat[1] = src[1];
	posQuat[2] = src[2];
	posQuat[3] = 0.0f;

	quaternion_multiply(temp, quat, posQuat);
	quaternion_conjugate(conj, quat);
	quaternion_multiply(temp, temp, conj);

	out[0] = temp[0];
	out[1] = temp[1];
	out[2] = temp[2];
}

void initQuaternionFunctions(void)
{
	Quaternion.set = quaternion_set;
	Quaternion.copy = quaternion_copy;
	Quaternion.fromEuler = quaternion_fromEuler;
	Quaternion.aroundAxis = quaternion_aroundAxis;
	Quaternion.fromMatrix = quaternion_fromMatrix;
	Quaternion.identity = quaternion_identity;
	Quaternion.multiply = quaternion_multiply;
	Quaternion.conjugate = quaternion_conjugate;
	Quaternion.normalize = quaternion_normalize;
	Quaternion.rotateVector = rotateVector;
}
