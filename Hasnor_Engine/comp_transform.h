#ifndef COMP_TRANSFORM_IMPORTED
#define COMP_TRANSFORM_IMPORTED

#include "engine_entity.h"
#include "engine_physics.h"

typedef struct {
	INHERIT_TYPE(component_t)

	transform_t		transform;
	bool			useGravity;
} comp_transform_t;

void comp_transform_update(comp_transform_t *self, const timeStruct_t time);

#endif