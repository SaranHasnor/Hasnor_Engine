#define HASNOR_ENGINE_INTERNAL

#include "engine_physics.h"
#include <utils_vector.h>

float _gravity[3] = {0, 0, -9.81f};

void updateTransform(transform_t *transform, float deltaTime, bool useGravity)
{
	Vector.multiplyAdd(transform->position, transform->position, deltaTime, transform->velocity);
	Vector.multiplyAdd(transform->angles, transform->angles, deltaTime, transform->rotation);
	if (useGravity)
	{
		Vector.multiplyAdd(transform->velocity, transform->velocity, deltaTime, _gravity);
	}
}

#undef HASNOR_ENGINE_INTERNAL
