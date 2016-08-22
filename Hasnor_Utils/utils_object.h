#ifndef HASNOR_OBJECT_DEFINED
#define HASNOR_OBJECT_DEFINED

#include <stdio.h>
#include "utils_types.h"

typedef struct hasnor_object_s {
	const char	*type;
	size_t		size;

	void		(*onFree)(struct hasnor_object_s *self);
} hasnor_object_t;

typedef struct {
	hasnor_object_t*	(*create)(size_t size, const char *type);
	bool				(*isType)(const hasnor_object_t *obj, const char *type);
	void				(*destroy)(hasnor_object_t *obj);
} _object_functions;

_object_functions Object;

#define INHERIT_TYPE(type) type super;
#define INHERIT_HASNOR INHERIT_TYPE(hasnor_object_t)

#define newObj(type) (type*)Object.create(sizeof(type), #type)
#define objIsType(obj, type) Object.isType(obj, #type)
#define freeObj(obj) Object.destroy((hasnor_object_t*)obj)

#ifdef HASNOR_INIT
void initObjectFunctions(void);
#endif

#endif
