#ifndef HASNOR_MATRIX_IMPORTED
#define HASNOR_MATRIX_IMPORTED

#include "utils_types.h"

typedef struct {
	void (*perspective)(float mat[16], float fov, float aspect, float near, float far);
	void (*viewModel)(float mat[16], float position[3], float angle[3]);
	void (*orthographic)(float mat[16], float width, float height, float near, float far);
	void (*rotation)(float out[16], float pitch, float yaw, float roll, bool degrees);
	void (*identity)(float mat[16]);
	
	void (*multiply)(float out[16], float a[16], float b[16]);
} _matrix_functions;

_matrix_functions Matrix;

void initMatrixFunctions();

#endif