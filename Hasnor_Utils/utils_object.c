#include "utils_object.h"
#include "utils_memory.h"
#include "utils_string.h"

hasnor_object_t *object_new(size_t size, const char *type)
{
	hasnor_object_t *newObject = (hasnor_object_t*)Memory.allocate(size);
	newObject->size = size;
	newObject->type = type;
	return newObject;
}

bool object_isType(const hasnor_object_t *obj, const char *type)
{
	return !String.compare(obj->type, type, true);
}

void object_destroy(hasnor_object_t *object)
{
	if (object->onFree)
	{
		object->onFree(object);
	}

	Memory.free(object);
}

void initObjectFunctions(void)
{
	Object.create = object_new;
	Object.isType = object_isType;
	Object.destroy = object_destroy;
}