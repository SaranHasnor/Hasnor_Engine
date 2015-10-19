#include "utils_math.h"

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

void mat_viewModel(float mat[16], float position[3], float angle[3])
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

void mat_rotation(float out[16], float pitch, float yaw, float roll, bool degrees)
{
	float pitchMat[16], yawMat[16], rollMat[16], tempMat[16];
	float cp = cosf(degrees ? pitch * M_PI / 180.0f : pitch), sp = sinf(degrees ? pitch * M_PI / 180.0f : pitch);
	float cy = cosf(degrees ? yaw * M_PI / 180.0 : yaw), sy = sinf(degrees ? yaw * M_PI / 180.0f : yaw);
	float cr = cosf(degrees ? roll * M_PI / 180.0f : roll), sr = sinf(degrees ? roll * M_PI / 180.0f : roll);

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

void mat_fromQuaternion(float out[16], float src[4])
{
	out[0] =  src[0];
	out[1] = -src[1];
	out[2] = -src[2];
	out[3] = -src[3];

	out[4] =  src[1];
	out[5] =  src[0];
	out[6] =  src[3];
	out[7] = -src[2];

	out[8]  =  src[2];
	out[9]  = -src[3];
	out[10] =  src[0];
	out[11] =  src[1];

	out[12] =  src[3];
	out[13] =  src[2];
	out[14] = -src[1];
	out[15] =  src[0];
}

void mat_multiply(float out[16], float a[16], float b[16])
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
			out[i+j] = val;
		}
	}
}

void mat_transpose(float out[16], float src[16])
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

bool mat_inverse(float out[16], float src[16])
{ // Taken from GLU's implementation
	float inv[16], det;
	int i;

	inv[0] = src[5] * src[10] * src[15] -
			 src[5] * src[11] * src[14] -
			 src[9] * src[6] * src[15] +
			 src[9] * src[7] * src[14] +
			 src[13] * src[6] * src[11] -
			 src[13] * src[7] * src[10];

	inv[4] = -src[4] * src[10] * src[15] +
			  src[4] * src[11] * src[14] +
			  src[8] * src[6] * src[15] -
			  src[8] * src[7] * src[14] -
			  src[12] * src[6] * src[11] +
			  src[12] * src[7] * src[10];

	inv[8] = src[4] * src[9] * src[15] -
			 src[4] * src[11] * src[13] -
			 src[8] * src[5] * src[15] +
			 src[8] * src[7] * src[13] +
			 src[12] * src[5] * src[11] -
			 src[12] * src[7] * src[9];

	inv[12] = -src[4] * src[9] * src[14] +
			   src[4] * src[10] * src[13] +
			   src[8] * src[5] * src[14] -
			   src[8] * src[6] * src[13] -
			   src[12] * src[5] * src[10] +
			   src[12] * src[6] * src[9];

	inv[1] = -src[1] * src[10] * src[15] +
			  src[1] * src[11] * src[14] +
			  src[9] * src[2] * src[15] -
			  src[9] * src[3] * src[14] -
			  src[13] * src[2] * src[11] +
			  src[13] * src[3] * src[10];

	inv[5] = src[0] * src[10] * src[15] -
			 src[0] * src[11] * src[14] -
			 src[8] * src[2] * src[15] +
			 src[8] * src[3] * src[14] +
			 src[12] * src[2] * src[11] -
			 src[12] * src[3] * src[10];

	inv[9] = -src[0] * src[9] * src[15] +
			  src[0] * src[11] * src[13] +
			  src[8] * src[1] * src[15] -
			  src[8] * src[3] * src[13] -
			  src[12] * src[1] * src[11] +
			  src[12] * src[3] * src[9];

	inv[13] = src[0] * src[9] * src[14] -
			  src[0] * src[10] * src[13] -
			  src[8] * src[1] * src[14] +
			  src[8] * src[2] * src[13] +
			  src[12] * src[1] * src[10] -
			  src[12] * src[2] * src[9];

	inv[2] = src[1] * src[6] * src[15] -
			 src[1] * src[7] * src[14] -
			 src[5] * src[2] * src[15] +
			 src[5] * src[3] * src[14] +
			 src[13] * src[2] * src[7] -
			 src[13] * src[3] * src[6];

	inv[6] = -src[0] * src[6] * src[15] +
			  src[0] * src[7] * src[14] +
			  src[4] * src[2] * src[15] -
			  src[4] * src[3] * src[14] -
			  src[12] * src[2] * src[7] +
			  src[12] * src[3] * src[6];

	inv[10] = src[0] * src[5] * src[15] -
			  src[0] * src[7] * src[13] -
			  src[4] * src[1] * src[15] +
			  src[4] * src[3] * src[13] +
			  src[12] * src[1] * src[7] -
			  src[12] * src[3] * src[5];

	inv[14] = -src[0] * src[5] * src[14] +
			   src[0] * src[6] * src[13] +
			   src[4] * src[1] * src[14] -
			   src[4] * src[2] * src[13] -
			   src[12] * src[1] * src[6] +
			   src[12] * src[2] * src[5];

	inv[3] = -src[1] * src[6] * src[11] +
			  src[1] * src[7] * src[10] +
			  src[5] * src[2] * src[11] -
			  src[5] * src[3] * src[10] -
			  src[9] * src[2] * src[7] +
			  src[9] * src[3] * src[6];

	inv[7] = src[0] * src[6] * src[11] -
			 src[0] * src[7] * src[10] -
			 src[4] * src[2] * src[11] +
			 src[4] * src[3] * src[10] +
			 src[8] * src[2] * src[7] -
			 src[8] * src[3] * src[6];

	inv[11] = -src[0] * src[5] * src[11] +
			   src[0] * src[7] * src[9] +
			   src[4] * src[1] * src[11] -
			   src[4] * src[3] * src[9] -
			   src[8] * src[1] * src[7] +
			   src[8] * src[3] * src[5];

	inv[15] = src[0] * src[5] * src[10] -
			  src[0] * src[6] * src[9] -
			  src[4] * src[1] * src[10] +
			  src[4] * src[2] * src[9] +
			  src[8] * src[1] * src[6] -
			  src[8] * src[2] * src[5];

	det = src[0] * inv[0] + src[1] * inv[4] + src[2] * inv[8] + src[3] * inv[12];

	if (det == 0)
	{
		return false;
	}

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
	{
		out[i] = inv[i] * det;
	}

	return true;
}

void initMatrixFunctions()
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
