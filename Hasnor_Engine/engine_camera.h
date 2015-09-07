#ifndef HASNOR_ENGINE_CAMERA_IMPORTED
#define HASNOR_ENGINE_CAMERA_IMPORTED

#include "engine_callbacks.h"
#include <utils_types.h>

/*
engine_camera

- Handles the camera's position
*/

void initCamera();
void setCameraSize(int width, int height);
void updateGLCamera(timeStruct_t time, inputStruct_t input);
void positionGLCameraForRender();
void positionGLCameraForInterface();

void engine_setCameraPosition(float position[3]);
void engine_moveCamera(float move[3]);
void engine_setCameraVelocity(float velocity[3]);
void engine_setCameraAngles(float angles[3]);
void engine_rotateCamera(float rotation[3]);
void engine_setCameraRotation(float rotation[3]);
void engine_getViewMatrix(float out[16]);

#endif