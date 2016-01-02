#ifndef HASNOR_MATRIX_IMPORTED
#define HASNOR_MATRIX_IMPORTED

#include "utils_types.h"

typedef struct {
	void (*perspective)(float mat[16], float fov, float aspect, float near, float far);
	void (*viewModel)(float mat[16], const float position[3], const float angle[3]);
	void (*orthographic)(float mat[16], float left, float right, float bottom, float top, float near, float far);
	void (*rotation)(float out[16], float pitch, float yaw, float roll, bool degrees);
	void (*identity)(float mat[16]);
	void (*fromQuaternion)(float out[16], const float src[4]);
	
	void (*multiply)(float out[16], const float a[16], const float b[16]);
	void (*transpose)(float out[16], const float src[16]);
	bool (*inverse)(float out[16], const float src[16]);
} _matrix_functions;

_matrix_functions Matrix;

#ifdef HASNOR_INIT
void initMatrixFunctions(void);
#endif

#endif
