#ifndef HASNOR_ENGINE_IMPORTED
#define HASNOR_ENGINE_IMPORTED

#include "engine_callbacks.h"
#include "engine_camera.h"
#include "engine_interface.h"
#include "engine_window.h"

#include "engine_geometry.h"
#include "engine_particles.h"

typedef struct {
	void (*run)(int argc, char **argv, int windowWidth, int windowHeight, const char *windowName, engineListener_t listener);
	void (*shutdown)(void);

	_camera_functions Camera;
	_interface_functions UI;
	_window_functions Window;
} _engine_functions;

_engine_functions Engine;

#ifdef HASNOR_INIT
void initEngineFunctions(void);
#endif

#endif