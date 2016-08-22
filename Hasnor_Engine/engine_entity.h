#ifndef HASNOR_ENGINE_ENTITY_IMPORTED
#define HASNOR_ENGINE_ENTITY_IMPORTED

#include <utils.h>
#include <utils_array.h>
#include "engine_callbacks.h"

typedef struct {
	INHERIT_HASNOR

	uint		entityTypeID;
} entity_t;



typedef struct {
	entity_t*	(*create)(void);

//#ifdef HASNOR_ENGINE_INTERNAL
	void		(*init)(entity_t *entity);
	void		(*update)(entity_t *entity, const timeStruct_t time);
	void		(*render)(entity_t *entity, const float viewMatrix[16]);
	void		(*destroy)(entity_t *entity);
//#endif
} _scene_entity_functions;

#ifdef HASNOR_ENGINE_INTERNAL
_scene_entity_functions *EntityInternal;
#endif

#define compNameForType(type) #type

#ifdef HASNOR_INIT
void initEntityFunctions(_scene_entity_functions *Entity);
#endif

#endif