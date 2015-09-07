#ifndef HASNOR_ENGINE_IMPORTED
#define HASNOR_ENGINE_IMPORTED

#include "engine_callbacks.h"

void engine_run(int argc, char **argv, int windowWidth, int windowHeight, char *windowName, engineListener_t listener);
void engine_shutdown();

void engine_setCameraPosition(float position[3]);
void engine_moveCamera(float move[3]);
void engine_setCameraVelocity(float velocity[3]);
void engine_setCameraAngles(float angles[3]);
void engine_rotateCamera(float rotation[3]);
void engine_setCameraRotation(float rotation[3]);
void engine_getViewMatrix(float out[16]);

#endif