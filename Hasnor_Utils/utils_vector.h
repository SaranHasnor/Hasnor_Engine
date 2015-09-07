#ifndef HASNOR_VEC3D_H_DEFINED
#define HASNOR_VEC3D_H_DEFINED

/*
utils_vector

- Functions and constants to work with vectors
*/

extern float axis[3][3];
extern float nullVec[3];

void vectorSet(float v[3], float x, float y, float z);
void vectorCopy(float dest[3], float src[3]);
void vectorClear(float v[3]);
void vectorTransition(float v[3], float org[3], float dest[3], float percentage);
void vectoangles( float *vec, float *angles );

void vectorAdd(float dest[3], float a[3], float b[3]);
void vectorSubtract(float dest[3], float a[3], float b[3]);
float vectorLength(float v[3]);
float vectorNormalize(float v[3]);
void vectorMult(float dest[3], float a[3], float b[3]);
void vectorScale(float dest[3], float s, float src[3]);
void vectorMA(float out[3], float org[3], float s, float move[3]);
void AngleVectors(float *angles, float *forward, float *right, float *up);
float vectorDot(float a[3], float b[3]);
void vectorRotate(float vec[3], float targetAxis[3][3]);
float vectorDistance(float a[3], float b[3]);

#endif