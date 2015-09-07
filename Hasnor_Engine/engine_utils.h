#ifndef RENDER_UTILS_H_DEFINED
#define RENDER_UTILS_H_DEFINED

void move2(float x, float y, float z);
void move(float vec[3]);
void rotate2(float pitch, float yaw, float roll);
void rotate(float ang[3]);
void scale2(float x, float y, float z);
void scaleUniform(float scale);
void scale(float vec[3]);
void makeLine(float pos1[3], float pos2[3]);
void makeLineFromVec2(float x, float y, float z, float length);
void makeLineFromVec(float vec[3], float length);
void drawAxis();

#endif