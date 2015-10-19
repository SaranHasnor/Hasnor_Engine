#ifndef HASNOR_ENGINE_CAMERA_IMPORTED
#define HASNOR_ENGINE_CAMERA_IMPORTED

#include "engine_callbacks.h"
#include <utils_types.h>

/*
engine_camera

- Handles the camera's position
*/

typedef struct {
	void (*setPosition)(float position[3]);
	void (*getPosition)(float out[3]);
	void (*move)(float move[3]);
	void (*setVelocity)(float velocity[3]);
	void (*setAngles)(float angles[3]);
	void (*getAngles)(float out[3]);
	void (*rotate)(float rotation[3]);
	void (*setRotation)(float rotation[3]);
	void (*getViewMatrix)(float out[16]);
	void (*setFOV)(float fov);
	void (*makeOrtho)();
	void (*makePerspective)();
} _camera_functions;

#ifdef HASNOR_ENGINE_INTERNAL
_camera_functions *CameraInternal;

void initCamera();
void setCameraSize(float width, float height);
void updateGLCamera(timeStruct_t time, inputStruct_t input);
void positionGLCameraForRender();
void positionGLCameraForInterface();
#endif

#ifdef HASNOR_INIT
void initCameraFunctions(_camera_functions *Camera);
#endif

#endif