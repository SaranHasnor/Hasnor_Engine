#ifndef HASNOR_ENGINE_IMPORTED
#define HASNOR_ENGINE_IMPORTED

#include "engine_callbacks.h"
#include "engine_camera.h"
#include "engine_geometry.h"
#include "engine_particles.h"

typedef struct {
	_texture_functions	Texture;
	_shader_functions	Shader;
	_program_functions	Program;
	_geometry_functions	Geometry;
	_particle_functions	Particles;
} _render_functions;

typedef struct {
	void (*run)(int argc, char **argv, int windowWidth, int windowHeight, char *windowName, engineListener_t listener);
	void (*shutdown)();

	_camera_functions Camera;
	_render_functions Render;
} _engine_functions;

_engine_functions Engine;

#ifdef HASNOR_INIT
void initEngineFunctions();
#endif

#endif