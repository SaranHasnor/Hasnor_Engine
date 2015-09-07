#include <GL/glut.h>
#include "engine_camera.h"
#include <utils.h>
#include <utils_vector.h>
#include <utils_matrix.h>

float _camPosition[3];
float _camAngles[3];

float _camVelocity[3];
float _camRotation[3];

float _renderProjectionMatrix[16];
float _interfaceProjectionMatrix[16];
float _renderModelViewMatrix[16];

// TEMP
int _screenWidth, _screenHeight;

void initCamera()
{
	vectorCopy(_camVelocity, nullVec);
	vectorCopy(_camRotation, nullVec);
	vectorSet(_camPosition, 0.0f, -10.0f, 0.0f);
	vectorSet(_camAngles, 0.0f, 90.0f, 0.0f);
}

void setCameraSize(int width, int height)
{
	mat_perspective(_renderProjectionMatrix, 60.0f, (float)width/(float)height, 1.0f, 200.0f);
	mat_orthographic(_interfaceProjectionMatrix, width, height, -200.0f, 200.0f); // FIXME
	_screenWidth = width;
	_screenHeight = height;
}

void engine_setCameraPosition(float position[3])
{
	vectorCopy(_camPosition, position);
}

void engine_moveCamera(float move[3])
{
	vectorAdd(_camPosition, _camPosition, move);
}

void engine_setCameraVelocity(float velocity[3])
{ // NOTE: The velocity is calculated relatively to the camera's local axis
	vectorCopy(_camVelocity, velocity);
}

void engine_setCameraAngles(float angles[3])
{
	vectorCopy(_camAngles, angles);
}

void engine_rotateCamera(float rotation[3])
{
	vectorAdd(_camAngles, _camAngles, rotation);
}

void engine_setCameraRotation(float rotation[3])
{
	vectorCopy(_camRotation, rotation);
}

void engine_getViewMatrix(float out[16])
{
	mat_multiply(out, _renderProjectionMatrix, _renderModelViewMatrix);
}

void _clampCameraAngles()
{
	if (_camAngles[0] < -85)
		_camAngles[0] = -85;
	if (_camAngles[0] > 85)
		_camAngles[0] = 85;

	if (_camAngles[1] < -180)
		_camAngles[1] += 360;
	if (_camAngles[1] > 180)
		_camAngles[1] -= 360;
}

void updateGLCamera(timeStruct_t time, inputStruct_t input)
{
	float forward[3], right[3];
	float movement[3];

	// Apply rotation
	vectorMA(_camAngles, _camAngles, time.deltaTimeSeconds, _camRotation);

	_clampCameraAngles();

	// Get the camera's local axis
	AngleVectors(_camAngles, forward, right, NULL);

	// Update the camera position (cheap method because math is hard)
	vectorScale(movement, _camVelocity[2], axis[2]);
	vectorMA(movement, movement, _camVelocity[1], forward);
	vectorMA(movement, movement, _camVelocity[0], right);
	
	vectorMA(_camPosition, _camPosition, time.deltaTimeSeconds, movement);

	mat_viewModel(_renderModelViewMatrix, _camPosition, _camAngles);
}

void positionGLCameraForRender()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_renderProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(_renderModelViewMatrix);
}

void positionGLCameraForInterface()
{
	glMatrixMode(GL_PROJECTION);
	//glLoadMatrixf(_interfaceProjectionMatrix);
	glLoadIdentity();
	gluOrtho2D(0.0, _screenWidth, _screenHeight, 0.0); // TEMP

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}