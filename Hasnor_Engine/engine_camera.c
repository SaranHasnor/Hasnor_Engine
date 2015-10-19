#define HASNOR_ENGINE_INTERNAL

#include <GL/glut.h>
#include "engine_camera.h"
#include "engine_physics.h"
#include <utils.h>
#include <utils_vector.h>
#include <utils_matrix.h>

typedef struct {
	transform_t		transform;

	float			viewportSize[2];
	float			fov;
	bool			isOrthographic;
} camera_t;

camera_t _mainCamera;

float _renderProjectionMatrix[16];
float _interfaceProjectionMatrix[16];
float _renderModelViewMatrix[16];

void _updateProjectionMatrix()
{
	// Render
	if (_mainCamera.isOrthographic)
	{
		Matrix.orthographic(_renderProjectionMatrix, 0.0f, _mainCamera.viewportSize[0], _mainCamera.viewportSize[1], 0.0f, 0.1f, 50.0f);
	}
	else
	{
		Matrix.perspective(_renderProjectionMatrix, _mainCamera.fov, _mainCamera.viewportSize[0]/_mainCamera.viewportSize[1], 0.1f, 200.0f);
	}

	// Interface
	Matrix.orthographic(_interfaceProjectionMatrix, 0.0f, _mainCamera.viewportSize[0], _mainCamera.viewportSize[1], 0.0f, -1.0f, 1.0f);
}

void initCamera()
{
	Vector.copy(_mainCamera.transform.velocity, Vector.zero);
	Vector.copy(_mainCamera.transform.rotation, Vector.zero);
	Vector.set(_mainCamera.transform.position, 0.0f, -10.0f, 0.0f);
	Vector.set(_mainCamera.transform.angles, 0.0f, 90.0f, 0.0f);

	_mainCamera.viewportSize[0] = 1.0f;
	_mainCamera.viewportSize[1] = 1.0f;
	_mainCamera.fov = 60.0f;
	_mainCamera.isOrthographic = false;

	_updateProjectionMatrix();
}

void setCameraSize(float width, float height)
{
	_mainCamera.viewportSize[0] = width;
	_mainCamera.viewportSize[1] = height;

	_updateProjectionMatrix();
}

void setCameraFOV(float fov)
{
	_mainCamera.fov = fov;

	_updateProjectionMatrix();
}

void makeCameraPerspective()
{
	if (_mainCamera.isOrthographic)
	{
		_mainCamera.isOrthographic = false;
		
		_updateProjectionMatrix();
	}
}

void makeCameraOrthographic()
{
	if (!_mainCamera.isOrthographic)
	{
		_mainCamera.isOrthographic = true;
		
		_updateProjectionMatrix();
	}
}

void engine_setCameraPosition(float position[3])
{
	Vector.copy(_mainCamera.transform.position, position);
}

void engine_getCameraPosition(float out[3])
{
    Vector.copy(out, _mainCamera.transform.position);
}

void engine_moveCamera(float move[3])
{
	Vector.add(_mainCamera.transform.position, _mainCamera.transform.position, move);
}

void engine_setCameraVelocity(float velocity[3])
{ // NOTE: The velocity is calculated relatively to the camera's local axis
	Vector.copy(_mainCamera.transform.velocity, velocity);
}

void engine_setCameraAngles(float angles[3])
{
	Vector.copy(_mainCamera.transform.angles, angles);
}

void engine_getCameraAngles(float out[3])
{
    Vector.copy(out, _mainCamera.transform.angles);
}

void engine_rotateCamera(float rotation[3])
{
	Vector.add(_mainCamera.transform.angles, _mainCamera.transform.angles, rotation);
}

void engine_setCameraRotation(float rotation[3])
{
	Vector.copy(_mainCamera.transform.rotation, rotation);
}

void engine_getViewMatrix(float out[16])
{
	Matrix.multiply(out, _renderProjectionMatrix, _renderModelViewMatrix);
}

void _clampCameraAngles()
{
	if (_mainCamera.transform.angles[0] < -85)
		_mainCamera.transform.angles[0] = -85;
	if (_mainCamera.transform.angles[0] > 85)
		_mainCamera.transform.angles[0] = 85;

	if (_mainCamera.transform.angles[1] < -180)
		_mainCamera.transform.angles[1] += 360;
	if (_mainCamera.transform.angles[1] > 180)
		_mainCamera.transform.angles[1] -= 360;
}

void updateGLCamera(timeStruct_t time, inputStruct_t input)
{
	float forward[3], right[3];
	float movement[3];

	// Apply rotation
	Vector.multiplyAdd(_mainCamera.transform.angles, _mainCamera.transform.angles, time.deltaTimeSeconds, _mainCamera.transform.rotation);

	_clampCameraAngles();

	// Get the camera's local axis
	Vector.axisFromAngles(_mainCamera.transform.angles, forward, right, NULL);

	// Update the camera position (cheap method because math is hard)
	Vector.scale(movement, _mainCamera.transform.velocity[2], Vector.axis[2]);
	Vector.multiplyAdd(movement, movement, _mainCamera.transform.velocity[1], forward);
	Vector.multiplyAdd(movement, movement, _mainCamera.transform.velocity[0], right);
	
	Vector.multiplyAdd(_mainCamera.transform.position, _mainCamera.transform.position, time.deltaTimeSeconds, movement);

	Matrix.viewModel(_renderModelViewMatrix, _mainCamera.transform.position, _mainCamera.transform.angles);
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
	glLoadMatrixf(_interfaceProjectionMatrix);

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
	Camera->setFOV = setCameraFOV;
	Camera->makeOrtho = makeCameraOrthographic;
	Camera->makePerspective = makeCameraPerspective;
}

#undef HASNOR_ENGINE_INTERNAL
