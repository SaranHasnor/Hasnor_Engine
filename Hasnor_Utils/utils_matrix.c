#include "utils_matrix.h"
#include "utils_vector.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "utils_ctools.h"

static float _workMat[16];

void mat_perspective(float mat[16], float fov, float aspect, float near, float far)
{
	float depth = far - near;
	float right = near * tanf(fov * (3.141592f / 360.0f));
	float top = right/aspect;

	Memory.set(mat, 0, sizeof(float) * 16);
	mat[0] = near / right;
	mat[5] = near / top;
	mat[10] = -(far + near) / depth;
	mat[11] = -1.0f;
	mat[14] = -2.0f * far * near / depth;
}

void mat_viewModel(float mat[16], const float position[3], const float angle[3])
{
	int i;
	float fwd[3], right[3], up[3];

	Vector.axisFromAngles(angle, fwd, right, up);

	for (i = 0; i < 3; i++)
	{
		mat[4*i+0] = right[i];
		mat[4*i+1] = up[i];
		mat[4*i+2] = -fwd[i];
		mat[4*i+3] = 0.0f;
	}

	mat[4*i+0] = -Vector.dotProduct(right, position);
	mat[4*i+1] = -Vector.dotProduct(up, position);
	mat[4*i+2] = Vector.dotProduct(fwd, position);
	mat[4*i+3] = 1.0f;
}

void mat_orthographic(float mat[16], float left, float right, float bottom, float top, float near, float far)
{
	float width = right - left;
	float height = top - bottom;
	float depth = far - near;

	Memory.set(mat, 0, sizeof(float) * 16);

	mat[0] = 2.0f / width;

	mat[5] = 2.0f / height;

	mat[10] = -2.0f / depth;

	mat[12] = -(right + left) / width;
	mat[13] = -(top + bottom) / height;
	mat[14] = -(far + near) / depth;

	mat[15] = 1.0f;
}

void mat_rotation(float out[16], float pitch, float yaw, float roll)
{
	float pitchMat[16], yawMat[16], rollMat[16], tempMat[16];
	float cp = cosf(pitch), sp = sinf(pitch);
	float cy = cosf(yaw), sy = sinf(yaw);
	float cr = cosf(roll), sr = sinf(roll);

	Memory.set(pitchMat, 0, sizeof(float) * 16);
	Memory.set(yawMat, 0, sizeof(float) * 16);
	Memory.set(rollMat, 0, sizeof(float) * 16);

	pitchMat[0] = 1.0f;
	pitchMat[5] = cp;
	pitchMat[6] = sp;
	pitchMat[9] = -sp;
	pitchMat[10] = cp;
	pitchMat[15] = 1.0f;

	yawMat[0] = cy;
	yawMat[1] = sy;
	yawMat[4] = -sy;
	yawMat[5] = cy;
	yawMat[10] = 1.0f;
	yawMat[15] = 1.0f;

	rollMat[0] = cr;
	rollMat[2] = -sr;
	rollMat[5] = 1.0f;
	rollMat[8] = sr;
	rollMat[10] = cr;
	rollMat[15] = 1.0f;

	Matrix.multiply(tempMat, yawMat, pitchMat);
	Matrix.multiply(out, tempMat, rollMat);
}

void mat_identity(float mat[16])
{
	int i;
	for (i = 0; i < 16; i++)
	{
		if ((i % 5) == 0)
		{
			mat[i] = 1.0f;
		}
		else
		{
			mat[i] = 0.0f;
		}
	}
}

void mat_fromQuaternion(float out[16], const float src[4])
{
	const float w = src[3];
	const float x = src[0];
	const float y = src[1];
	const float z = src[2];

	out[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
	out[1] = 2.0f * x * y + 2.0f * z * w;
	out[2] = 2.0f * x * z - 2.0f * y * w;
	out[3] = 0.0f;

	out[4] = 2.0f * x * y - 2.0f * z * w;
	out[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
	out[6] = 2.0f * y * z + 2.0f * x * w;
	out[7] = 0.0f;

	out[8]  = 2.0f * x * z + 2.0f * y * w;
	out[9]  = 2.0f * y * z - 2.0f * x * w;
	out[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
	out[11] = 0.0f;

	out[12] =  0.0f;
	out[13] =  0.0f;
	out[14] =  0.0f;
	out[15] =  1.0f;
}

void mat_multiply(float out[16], const float a[16], const float b[16])
{
	int i,j;
	for (j=0; j<16; j+=4)
	{
		for (i=0; i<4; i++)
		{
			float val = 0.0f;
			int k;
			for (k=0; k<4;k++)
			{
				val += a[i+(4*k)] * b[k+j];
			}
			_workMat[i+j] = val;
		}
	}

	for (i=0; i<16; i++)
	{
		out[i] = _workMat[i];
	}
}

void mat_transpose(float out[16], const float src[16])
{
	float temp;

#define _swapIndexes(x, y) temp = src[x]; out[x] = src[y]; out[y] = temp;

	_swapIndexes(1, 4);
	_swapIndexes(2, 8);
	_swapIndexes(3, 12);

	_swapIndexes(6, 9);
	_swapIndexes(7, 13);

	_swapIndexes(11, 14);

#undef _swapIndexes
}

bool mat_inverse(float out[16], const float src[16])
{ // Taken from GLU's implementation
	float det;
	int i;

	_workMat[0] = src[5] * src[10] * src[15] -
			 src[5] * src[11] * src[14] -
			 src[9] * src[6] * src[15] +
			 src[9] * src[7] * src[14] +
			 src[13] * src[6] * src[11] -
			 src[13] * src[7] * src[10];

	_workMat[4] = -src[4] * src[10] * src[15] +
			  src[4] * src[11] * src[14] +
			  src[8] * src[6] * src[15] -
			  src[8] * src[7] * src[14] -
			  src[12] * src[6] * src[11] +
			  src[12] * src[7] * src[10];

	_workMat[8] = src[4] * src[9] * src[15] -
			 src[4] * src[11] * src[13] -
			 src[8] * src[5] * src[15] +
			 src[8] * src[7] * src[13] +
			 src[12] * src[5] * src[11] -
			 src[12] * src[7] * src[9];

	_workMat[12] = -src[4] * src[9] * src[14] +
			   src[4] * src[10] * src[13] +
			   src[8] * src[5] * src[14] -
			   src[8] * src[6] * src[13] -
			   src[12] * src[5] * src[10] +
			   src[12] * src[6] * src[9];

	_workMat[1] = -src[1] * src[10] * src[15] +
			  src[1] * src[11] * src[14] +
			  src[9] * src[2] * src[15] -
			  src[9] * src[3] * src[14] -
			  src[13] * src[2] * src[11] +
			  src[13] * src[3] * src[10];

	_workMat[5] = src[0] * src[10] * src[15] -
			 src[0] * src[11] * src[14] -
			 src[8] * src[2] * src[15] +
			 src[8] * src[3] * src[14] +
			 src[12] * src[2] * src[11] -
			 src[12] * src[3] * src[10];

	_workMat[9] = -src[0] * src[9] * src[15] +
			  src[0] * src[11] * src[13] +
			  src[8] * src[1] * src[15] -
			  src[8] * src[3] * src[13] -
			  src[12] * src[1] * src[11] +
			  src[12] * src[3] * src[9];

	_workMat[13] = src[0] * src[9] * src[14] -
			  src[0] * src[10] * src[13] -
			  src[8] * src[1] * src[14] +
			  src[8] * src[2] * src[13] +
			  src[12] * src[1] * src[10] -
			  src[12] * src[2] * src[9];

	_workMat[2] = src[1] * src[6] * src[15] -
			 src[1] * src[7] * src[14] -
			 src[5] * src[2] * src[15] +
			 src[5] * src[3] * src[14] +
			 src[13] * src[2] * src[7] -
			 src[13] * src[3] * src[6];

	_workMat[6] = -src[0] * src[6] * src[15] +
			  src[0] * src[7] * src[14] +
			  src[4] * src[2] * src[15] -
			  src[4] * src[3] * src[14] -
			  src[12] * src[2] * src[7] +
			  src[12] * src[3] * src[6];

	_workMat[10] = src[0] * src[5] * src[15] -
			  src[0] * src[7] * src[13] -
			  src[4] * src[1] * src[15] +
			  src[4] * src[3] * src[13] +
			  src[12] * src[1] * src[7] -
			  src[12] * src[3] * src[5];

	_workMat[14] = -src[0] * src[5] * src[14] +
			   src[0] * src[6] * src[13] +
			   src[4] * src[1] * src[14] -
			   src[4] * src[2] * src[13] -
			   src[12] * src[1] * src[6] +
			   src[12] * src[2] * src[5];

	_workMat[3] = -src[1] * src[6] * src[11] +
			  src[1] * src[7] * src[10] +
			  src[5] * src[2] * src[11] -
			  src[5] * src[3] * src[10] -
			  src[9] * src[2] * src[7] +
			  src[9] * src[3] * src[6];

	_workMat[7] = src[0] * src[6] * src[11] -
			 src[0] * src[7] * src[10] -
			 src[4] * src[2] * src[11] +
			 src[4] * src[3] * src[10] +
			 src[8] * src[2] * src[7] -
			 src[8] * src[3] * src[6];

	_workMat[11] = -src[0] * src[5] * src[11] +
			   src[0] * src[7] * src[9] +
			   src[4] * src[1] * src[11] -
			   src[4] * src[3] * src[9] -
			   src[8] * src[1] * src[7] +
			   src[8] * src[3] * src[5];

	_workMat[15] = src[0] * src[5] * src[10] -
			  src[0] * src[6] * src[9] -
			  src[4] * src[1] * src[10] +
			  src[4] * src[2] * src[9] +
			  src[8] * src[1] * src[6] -
			  src[8] * src[2] * src[5];

	det = src[0] * _workMat[0] + src[1] * _workMat[4] + src[2] * _workMat[8] + src[3] * _workMat[12];

	if (det == 0)
	{
		return false;
	}

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
	{
		out[i] = _workMat[i] * det;
	}

	return true;
}

void initMatrixFunctions(void)
{
	Matrix.identity = mat_identity;
	Matrix.multiply = mat_multiply;
	Matrix.orthographic = mat_orthographic;
	Matrix.perspective = mat_perspective;
	Matrix.rotation = mat_rotation;
	Matrix.viewModel = mat_viewModel;
	Matrix.transpose = mat_transpose;
	Matrix.inverse = mat_inverse;
	Matrix.fromQuaternion = mat_fromQuaternion;
}
