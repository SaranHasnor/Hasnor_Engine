#include "utils_matrix.h"
#include "utils_ctools.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "utils_vector.h"

void mat_perspective(float mat[16], float fov, float aspect, float near, float far)
{
	float depth = far - near;
	float right = near * tanf(fov * (3.141592f / 360.0f));
	float top = right/aspect;

	mem_set(mat, 0, sizeof(float) * 16);
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

	AngleVectors(angle, fwd, right, up);

	for (i = 0; i < 3; i++)
	{
		mat[4*i+0] = right[i];
		mat[4*i+1] = up[i];
		mat[4*i+2] = -fwd[i];
		mat[4*i+3] = 0.0f;
	}

	mat[4*i+0] = -vectorDot(right, position);
	mat[4*i+1] = -vectorDot(up, position);
	mat[4*i+2] = vectorDot(fwd, position);
	mat[4*i+3] = 1.0f;
}

void mat_orthographic(float mat[16], float width, float height, float near, float far)
{
	float depth = far - near;

	mem_set(mat, 0, sizeof(float) * 16);
	mat[0] = 2.0f / width;
	mat[5] = 2.0f / height;
	mat[10] = -2.0f / depth;
	mat[14] = -(far + near) / depth;
	mat[15] = 1.0f;
}

void mat_rotation(float out[16], float pitch, float yaw, float roll)
{
	float pitchMat[16], yawMat[16], rollMat[16], tempMat[16];
	float cp = cosf(pitch), sp = sinf(pitch);
	float cy = cosf(yaw), sy = sinf(yaw);
	float cr = cosf(roll), sr = sinf(roll);

	mem_set(pitchMat, 0, sizeof(float) * 16);
	mem_set(yawMat, 0, sizeof(float) * 16);
	mem_set(rollMat, 0, sizeof(float) * 16);

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

	mat_multiply(tempMat, pitchMat, yawMat);
	mat_multiply(out, tempMat, rollMat);
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