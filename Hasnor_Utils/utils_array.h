#ifndef UTILS_ARRAY_IMPORTED
#define UTILS_ARRAY_IMPORTED

#include "utils_types.h"

typedef struct {
	void		**content;
	uint		size;
} array_t;

typedef struct {
	void (*init)(array_t *a);
	void (*add)(array_t *a, void *object);
	void (*insert)(array_t *a, void *object, uint pos);
	void (*remove)(array_t *a, void *object);
	void (*removeAt)(array_t *a, uint pos);
	int (*indexOf)(array_t *a, void *object);
	int (*indexOfFirstMatch)(array_t *a, bool (*predicate)(void *object));
} _array_functions;

_array_functions Array;

#ifdef HASNOR_INIT
void initArrayFunctions();
#endif

#endif
