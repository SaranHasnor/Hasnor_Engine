#ifndef HASNOR_QUATERNION_IMPORTED
#define HASNOR_QUATERNION_IMPORTED

typedef struct {
	/* Setters */
	void (*set)(float out[4], float x, float y, float z, float w);
	void (*copy)(float out[4], const float src[4]);
	void (*fromEuler)(float out[4], float pitch, float yaw, float roll);
	void (*aroundAxis)(float out[4], float axis[3], float angle);
	void (*identity)(float out[4]);

	/* Binary operators */
	void (*multiply)(float out[4], const float a[4], const float b[4]);

	/* Misc */
	void (*conjugate)(float out[4], const float src[4]);
	float (*normalize)(float out[4], const float src[4]);
	void (*rotateVector)(float out[3], const float src[3], const float quaternion[4]);
} _quaternion_functions;

_quaternion_functions Quaternion;

#ifdef HASNOR_INIT
void initQuaternionFunctions(void);
#endif

#endif
