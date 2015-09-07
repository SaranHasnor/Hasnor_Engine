#include <GL/glut.h>
#include <utils.h>
#include <utils_vector.h>

/*
render_utils.c

- Utility rendering functions
*/

void move2(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void move(float vec[3])
{
	move2(vec[0], vec[1], vec[2]);
}

void rotate2(float pitch, float yaw, float roll)
{
	glRotatef(roll, 1.0f, 0.0f, 0.0f);
	glRotatef(yaw, 0.0f, 0.0f, 1.0f);
	glRotatef(pitch, 0.0f, 1.0f, 0.0f);
}

void rotate(float ang[3])
{
	rotate2(ang[0], ang[1], ang[2]);
}

void scale2(float x, float y, float z)
{
	glScalef(x, y, z);
}

void scale(float vec[3])
{
	scale2(vec[0], vec[1], vec[2]);
}

void scaleUniform(float scale)
{
	glScalef(scale, scale, scale);
}

void makeLine(float pos1[3], float pos2[3])
{
	glBegin(GL_LINES);
	glVertex3f(pos1[0], pos1[1], pos1[2]);
	glVertex3f(pos2[0], pos2[1], pos2[2]);
	glEnd();
}

void makeLineFromVec2(float x, float y, float z, float length)
{
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(x*length, y*length, z*length);
	glEnd();
}

void makeLineFromVec(float vec[3], float length)
{
	makeLineFromVec2(vec[0], vec[1], vec[2], length);
}

void drawAxis()
{ // Draws the local axis, X being drawn red, Y green and Z blue
	glColor3fv(axis[0]);
	makeLine(nullVec, axis[0]);
	glColor3fv(axis[1]);
	makeLine(nullVec, axis[1]);
	glColor3fv(axis[2]);
	makeLine(nullVec, axis[2]);
}