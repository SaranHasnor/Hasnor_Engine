#define HASNOR_ENGINE_INTERNAL

#include "comp_transform.h"


void comp_transform_update(comp_transform_t *self, const timeStruct_t time)
{
	updateTransform(&self->transform, time.deltaTimeSeconds, self->useGravity);
}

#undef HASNOR_ENGINE_INTERNAL
