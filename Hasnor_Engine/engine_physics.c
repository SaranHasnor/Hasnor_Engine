#include "engine_physics.h"
#include <utils_vector.h>


void updateTransform(transform_t *transform, float deltaTime)
{
	vectorMA(transform->position, transform->position, deltaTime, transform->velocity);
	vectorMA(transform->angles, transform->angles, deltaTime, transform->rotation);
}