#include "engine_physics.h"
#include <utils_vector.h>

float _gravity[3] = {0, 0, -9.81f};

void updateTransform(transform_t *transform, float deltaTime, bool useGravity)
{
	vectorMA(transform->position, transform->position, deltaTime, transform->velocity);
	vectorMA(transform->angles, transform->angles, deltaTime, transform->rotation);
	if (useGravity)
	{
		vectorMA(transform->velocity, transform->velocity, deltaTime, _gravity);
	}
}
