#ifdef HASNOR_ENGINE_INTERNAL

#ifndef ALL_COMPONENTS_IMPORTED
#define ALL_COMPONENTS_IMPORTED

#include "engine_entity.h"
#include "comp_renderer.h"
#include "comp_transform.h"

component_t *make_component(const char *name);
component_t *clone_component(const component_t *self);

void init_component(component_t *self);
void update_component(component_t *self, const timeStruct_t time);
void destroy_component(component_t *self);

#endif

#endif
