#ifndef HASNOR_QUATERNION_IMPORTED
#define HASNOR_QUATERNION_IMPORTED

typedef struct {
	void (*fromEuler)(float out[4], float pitch, float yaw, float roll);
	void (*identity)(float out[4]);
} _quaternion_functions;

_quaternion_functions Quaternion;

void initQuaternionFunctions();

#endif