#define HASNOR_ENGINE_INTERNAL

#include "engine_scene.h"

#include <utils_list.h>

list_t *_entities = NULL;

void scene_addEntity(entity_t *entity)
{
	List.add(&_entities, entity);
	
	EntityInternal->init(entity);
}

void scene_removeEntity(entity_t *entity)
{
	List.remove(&_entities, entity);

	EntityInternal->destroy(entity);
}

void scene_clearEntities(void)
{
	list_t **iterator = &_entities;

	while (*iterator)
	{
		entity_t *entity = (entity_t*)(*iterator)->content;

		EntityInternal->destroy(entity);

		iterator = &(*iterator)->next;
	}

	List.clear(&_entities);
}

void scene_update(const timeStruct_t time)
{
	list_t **iterator = &_entities;

	while (*iterator)
	{
		entity_t *entity = (entity_t*)(*iterator)->content;

		EntityInternal->update(entity, time);

		iterator = &(*iterator)->next;
	}
}

void scene_render(const float viewMatrix[16])
{
	list_t **iterator = &_entities;

	while (*iterator)
	{
		entity_t *entity = (entity_t*)(*iterator)->content;

		EntityInternal->render(entity, viewMatrix);

		iterator = &(*iterator)->next;
	}
}

void initSceneFunctions(void)
{
	Scene.addEntity = scene_addEntity;
	Scene.removeEntity = scene_removeEntity;
	Scene.clearEntities = scene_clearEntities;
	Scene.update = scene_update;
	Scene.render = scene_render;
}

#undef HASNOR_ENGINE_INTERNAL
