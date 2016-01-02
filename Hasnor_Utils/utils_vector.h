#ifndef HASNOR_VEC3D_H_DEFINED
#define HASNOR_VEC3D_H_DEFINED

typedef struct {
	/* Constants */
	float		axis[3][3];		// World axis
	float		zero[3];		// (0,0,0)

	/* Setters */
	void (*set)(float v[3], float x, float y, float z);						// v = (x,y,z)
	void (*copy)(float dest[3], const float src[3]);						// dest = src
	void (*clear)(float v[3]);												// v = (0,0,0)

	/* Unary operators */
	float (*squareLength)(const float v[3]);								// sqrLen(v)
	float (*length)(const float v[3]);										// len(v)
	float (*normalize)(float v[3]);											// v = v / len(v), return len(v)

	/* Binary operators */
	void (*add)(float dest[3], const float a[3], const float b[3]);			// dest = a + b
	void (*subtract)(float dest[3], const float a[3], const float b[3]);	// dest = a - b
	void (*multiply)(float dest[3], const float a[3], const float b[3]);	// dest = a * b
	float (*dotProduct)(const float a[3], const float b[3]);				// a . b
	void (*crossProduct)(float out[3], const float a[3], const float b[3]);	// a x b
	float (*distance)(const float a[3], const float b[3]);					// len(a - b)
	float (*squareDistance)(const float a[3], const float b[3]);			// sqrLen(a - b)
	float (*angleBetween)(const float a[3], const float b[3]);
	float (*orientedAngleBetween)(const float a[3], const float b[3], const float normal[3]);

	/* Rotation */
	void (*toAngles)(const float v[3], float angles[3]);
	void (*axisFromAngles)(const float angles[3], float *forward, float *right, float *up);
	void (*rotate)(float vec[3], const float targetAxis[3][3]);

	/* Misc */
	void (*scale)(float dest[3], float s, const float src[3]);								// dest = s * src
	void (*lerp)(float v[3], const float org[3], const float dest[3], float percentage);	// v = percentage * org + (1 - percentage) * dest
	void (*multiplyAdd)(float out[3], const float org[3], float s, const float move[3]);	// out = org + s * move
} _vector_functions;

_vector_functions Vector;

#ifdef HASNOR_INIT
void initVectorFunctions(void);
#endif

#endif
