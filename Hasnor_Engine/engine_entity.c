#define HASNOR_ENGINE_INTERNAL

#include "engine_entity.h"
#include "all_components.h"

#include <utils_map.h>
#include <utils_string.h>
#include <utils_matrix.h>

map_t _prefabMap;

entity_t *newEntity(void)
{
	entity_t *entity = alloc(entity_t);
	Array.init(&entity->components, 5);
	return entity;
}

void addComponentToEntity(entity_t *entity, const char *componentName)
{
	component_t *component = make_component(componentName);
	if (component)
	{
		Array.add(&entity->components, component);
	}
}

prefab_t *newPrefab(const char *name, array_t components)
{
	prefab_t *prefab = alloc(prefab_t);
	prefab->name = name ? String.create(name) : NULL;
	prefab->super.components = components;
	if (name)
	{
		Map.setValueForKey(&_prefabMap, name, prefab, true);
	}
	return prefab;
}

component_t *_cloneComponent(component_t *self)
{
	return clone_component(self);
}

entity_t *instantiatePrefab(const prefab_t *prefab)
{
	entity_t *newEntity = alloc(entity_t);
	uint i;

	Array.init(&newEntity->components, prefab->super.components.size);
	for (i = 0; i < prefab->super.components.size; i++)
	{
		Array.add(&newEntity->components, _cloneComponent((component_t*)prefab->super.components.content[i]));
	}

	return newEntity;
}

void init_entity(entity_t *self)
{
	uint i;
	for (i = 0; i < self->components.size; i++)
	{
		init_component((component_t*)self->components.content[i]);
	}
}

void update_entity(entity_t *self, const timeStruct_t time)
{
	uint i;
	for (i = 0; i < self->components.size; i++)
	{
		update_component((component_t*)self->components.content[i], time);
	}
}

component_t *getEntityComponent(entity_t *ent, const char *compName)
{
	uint i;
	for (i = 0; i < ent->components.size; i++)
	{
		component_t *comp = (component_t*)ent->components.content[i];
		if (objIsType(&comp->super, compName))
		{
			return comp;
		}
	}
	return NULL;
}

void render_entity(entity_t *self, const float viewMatrix[16])
{
	comp_renderer_t *rendererComp = (comp_renderer_t*)getEntityComponent(self, compNameForType(comp_renderer_t));
	comp_transform_t *transformComp = (comp_transform_t*)getEntityComponent(self, compNameForType(comp_transform_t));

	if (rendererComp && transformComp)
	{
		Vector.copy(rendererComp->mesh->origin, transformComp->transform.position);
		Matrix.rotation(rendererComp->mesh->rotation,
			transformComp->transform.rotation[0],
			transformComp->transform.rotation[1],
			transformComp->transform.rotation[2]);
	}
}

void destroy_entity(entity_t *self)
{
	uint i;
	for (i = 0; i < self->components.size; i++)
	{
		destroy_component((component_t*)self->components.content[i]);
	}
}

void initEntityFunctions(_scene_entity_functions *Entity)
{
	Map.init(&_prefabMap);

	EntityInternal = Entity;

	Entity->create = newEntity;
	Entity->addComponent = addComponentToEntity;
	Entity->getComponent = getEntityComponent;

	Entity->makePrefab = newPrefab;
	Entity->instantiatePrefab = instantiatePrefab;

	Entity->init = init_entity;
	Entity->update = update_entity;
	Entity->render = render_entity;
	Entity->destroy = destroy_entity;
}

#undef HASNOR_ENGINE_INTERNAL
