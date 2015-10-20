#ifndef HASNOR_VEC3D_H_DEFINED
#define HASNOR_VEC3D_H_DEFINED

/*
utils_vector

- Functions and constants to work with vectors
*/

typedef struct {
	// Constants
	float		axis[3][3];
	float		zero[3];

	// Setters
	void (*set)(float v[3], float x, float y, float z);
	void (*copy)(float dest[3], float src[3]);
	void (*clear)(float v[3]);

	// Unary operators
	float (*squareLength)(float v[3]);
	float (*length)(float v[3]);
	float (*normalize)(float v[3]);

	// Binary operators
	void (*add)(float dest[3], float a[3], float b[3]);
	void (*subtract)(float dest[3], float a[3], float b[3]);
	void (*multiply)(float dest[3], float a[3], float b[3]);
	float (*dotProduct)(float a[3], float b[3]);
	void (*crossProduct)(float out[3], float a[3], float b[3]);
	float (*distance)(float a[3], float b[3]);
	float (*squareDistance)(float a[3], float b[3]);
	float (*angleBetween)(float a[3], float b[3]);
	float (*orientedAngleBetween)(float a[3], float b[3], float normal[3]);

	// Rotation
	void (*toAngles)(float v[3], float angles[3]);
	void (*axisFromAngles)(float angles[3], float *forward, float *right, float *up);
	void (*rotate)(float vec[3], float targetAxis[3][3]);

	// Misc
	void (*scale)(float dest[3], float s, float src[3]);
	void (*lerp)(float v[3], float org[3], float dest[3], float percentage);
	void (*multiplyAdd)(float out[3], float org[3], float s, float move[3]);
} _vector_functions;

_vector_functions Vector;

#ifdef HASNOR_INIT
void initVectorFunctions(void);
#endif

#endif
