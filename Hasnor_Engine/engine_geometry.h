#ifndef ENGINE_GEOMETRY_IMPORTED
#define ENGINE_GEOMETRY_IMPORTED

#include "engine_mesh.h"

typedef struct {
	/* Primitives */
	mesh_t*		(*sphere)(float radius, int rings, int sectors, texture_t *texture, program_t *program);
	mesh_t*		(*ring)(float innerRadius, float outerRadius, int sectors, texture_t *texture, program_t *program);
} _geometry_functions;

_geometry_functions Primitives;

#ifdef HASNOR_INIT
void initGeometryFunctions(void);
#endif

#endif