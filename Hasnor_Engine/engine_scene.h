#ifndef HASNOR_ENGINE_SCENE_IMPORTED
#define HASNOR_ENGINE_SCENE_IMPORTED

#include <utils.h>
#include "engine_callbacks.h"
#include "engine_entity.h"


typedef struct {
	_scene_entity_functions		Entity;

	void			(*addEntity)(entity_t* entity);
	void			(*removeEntity)(entity_t *entity);
	void			(*clearEntities)(void);

	void			(*update)(const timeStruct_t time);
	void			(*render)(const float viewMatrix[16]);
} _scene_functions;

_scene_functions Scene;

#ifdef HASNOR_INIT
void initSceneFunctions(void);
#endif

#endif
