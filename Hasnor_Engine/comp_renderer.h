#ifndef COMP_RENDERER_IMPORTED
#define COMP_RENDERER_IMPORTED

#include "engine_entity.h"
#include "engine_mesh.h"

typedef struct {
	INHERIT_TYPE(component_t)

	mesh_t		*mesh;
} comp_renderer_t;

#endif