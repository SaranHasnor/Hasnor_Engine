#define HASNOR_ENGINE_INTERNAL

#include "all_components.h"

#include <utils_string.h>

component_t *make_component(const char *name)
{
	if (!String.compare(name, compNameForType(comp_transform_t), true))
	{
		return (component_t*)newObj(comp_transform_t);
	}
	else if (!String.compare(name, compNameForType(comp_renderer_t), true))
	{
		return (component_t*)newObj(comp_renderer_t);
	}
	else
	{
		return NULL;
	}
}

component_t *clone_component(const component_t *self)
{
	if (objIsType(&self->super, comp_renderer_t))
	{
		comp_renderer_t *copy = (comp_renderer_t*)Memory.duplicate(self, self->super.size);
		copy->mesh = Mesh.duplicateMesh(copy->mesh);
		return &copy->super;
	}
	else
	{
		return (component_t*)Memory.duplicate(self, self->super.size);
	}
}

void init_component(component_t *self)
{
	
}

void update_component(component_t *self, const timeStruct_t time)
{
	if (objIsType(&self->super, comp_transform_t))
	{
		comp_transform_update((comp_transform_t*)self, time);
	}
}

void destroy_component(component_t *self)
{
	if (objIsType(&self->super, comp_renderer_t))
	{
		Mesh.destroyMesh(((comp_renderer_t*)self)->mesh);
	}
}

#undef HASNOR_ENGINE_INTERNAL
