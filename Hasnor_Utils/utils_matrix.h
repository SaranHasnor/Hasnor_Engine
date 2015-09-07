#ifndef HASNOR_MATRIX_IMPORTED
#define HASNOR_MATRIX_IMPORTED

void mat_perspective(float mat[16], float fov, float aspect, float near, float far);
void mat_viewModel(float mat[16], float position[3], float angle[3]);
void mat_orthographic(float mat[16], float width, float height, float near, float far);
void mat_rotation(float out[16], float pitch, float yaw, float roll);

void mat_identity(float mat[16]);
void mat_multiply(float out[16], float a[16], float b[16]);

#endif