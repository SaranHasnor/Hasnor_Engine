#ifndef HASNOR_QUATERNION_IMPORTED
#define HASNOR_QUATERNION_IMPORTED

typedef struct {
	void (*fromEuler)(float out[4], float pitch, float yaw, float roll);
	void (*identity)(float out[4]);

	void (*multiply)(float out[4], const float a[4], const float b[4]);
	void (*inverse)(float out[4], const float src[4]);
} _quaternion_functions;

_quaternion_functions Quaternion;

#ifdef HASNOR_INIT
void initQuaternionFunctions(void);
#endif

#endif
