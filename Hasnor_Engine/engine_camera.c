#define HASNOR_ENGINE_INTERNAL

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
	Vector.copy(_camVelocity, Vector.zero);
	Vector.copy(_camRotation, Vector.zero);
	Vector.set(_camPosition, 0.0f, -10.0f, 0.0f);
	Vector.set(_camAngles, 0.0f, 90.0f, 0.0f);
}

void setCameraSize(int width, int height)
{
	Matrix.perspective(_renderProjectionMatrix, 60.0f, (float)width/(float)height, 0.1f, 200.0f);
	Matrix.orthographic(_interfaceProjectionMatrix, width, height, -200.0f, 200.0f); // FIXME
	_screenWidth = width;
	_screenHeight = height;
}

void engine_setCameraPosition(float position[3])
{
	Vector.copy(_camPosition, position);
}

void engine_getCameraPosition(float out[3])
{
    Vector.copy(out, _camPosition);
}

void engine_moveCamera(float move[3])
{
	Vector.add(_camPosition, _camPosition, move);
}

void engine_setCameraVelocity(float velocity[3])
{ // NOTE: The velocity is calculated relatively to the camera's local axis
	Vector.copy(_camVelocity, velocity);
}

void engine_setCameraAngles(float angles[3])
{
	Vector.copy(_camAngles, angles);
}

void engine_getCameraAngles(float out[3])
{
    Vector.copy(out, _camAngles);
}

void engine_rotateCamera(float rotation[3])
{
	Vector.add(_camAngles, _camAngles, rotation);
}

void engine_setCameraRotation(float rotation[3])
{
	Vector.copy(_camRotation, rotation);
}

void engine_getViewMatrix(float out[16])
{
	Matrix.multiply(out, _renderProjectionMatrix, _renderModelViewMatrix);
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
	Vector.multiplyAdd(_camAngles, _camAngles, time.deltaTimeSeconds, _camRotation);

	_clampCameraAngles();

	// Get the camera's local axis
	Vector.axisFromAngles(_camAngles, forward, right, NULL);

	// Update the camera position (cheap method because math is hard)
	Vector.scale(movement, _camVelocity[2], Vector.axis[2]);
	Vector.multiplyAdd(movement, movement, _camVelocity[1], forward);
	Vector.multiplyAdd(movement, movement, _camVelocity[0], right);
	
	Vector.multiplyAdd(_camPosition, _camPosition, time.deltaTimeSeconds, movement);

	Matrix.viewModel(_renderModelViewMatrix, _camPosition, _camAngles);
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

void initCameraFunctions(_camera_functions *Camera)
{
	CameraInternal = Camera;

	Camera->getAngles = engine_getCameraAngles;
	Camera->getPosition = engine_getCameraPosition;
	Camera->getViewMatrix = engine_getViewMatrix;
	Camera->move = engine_moveCamera;
	Camera->rotate = engine_rotateCamera;
	Camera->setAngles = engine_setCameraAngles;
	Camera->setPosition = engine_setCameraPosition;
	Camera->setRotation = engine_setCameraRotation;
	Camera->setVelocity = engine_setCameraVelocity;
}

#undef HASNOR_ENGINE_INTERNAL
