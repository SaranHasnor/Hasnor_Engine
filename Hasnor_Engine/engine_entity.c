#define HASNOR_ENGINE_INTERNAL

#include "engine_entity.h"

#include <utils_map.h>
#include <utils_string.h>
#include <utils_matrix.h>

map_t _prefabMap;

entity_t *newEntity(void)
{
	entity_t *entity = newObj(entity_t);
	entity->entityTypeID = 0;
	return entity;
}

void init_entity(entity_t *self)
{
	
}

void update_entity(entity_t *self, const timeStruct_t time)
{
	
}

void render_entity(entity_t *self, const float viewMatrix[16])
{
	
}

void destroy_entity(entity_t *self)
{
	
}

void initEntityFunctions(_scene_entity_functions *Entity)
{
	Map.init(&_prefabMap);

	EntityInternal = Entity;

	Entity->create = newEntity;

	Entity->init = init_entity;
	Entity->update = update_entity;
	Entity->render = render_entity;
	Entity->destroy = destroy_entity;
}

#undef HASNOR_ENGINE_INTERNAL
