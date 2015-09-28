#ifndef UTILS_ARRAY_IMPORTED
#define UTILS_ARRAY_IMPORTED

#include "utils_types.h"

typedef struct {
	void		**content;
	uint		size;
} array_t;

void array_init(array_t *ar);
void array_add(array_t *ar, void *object);
void array_insert(array_t *ar, void *object, uint pos);
void array_remove(array_t *ar, void *object);
void array_removeAt(array_t *ar, uint pos);
int array_indexOf(array_t *ar, void *object);
int array_indexOfFirstMatch(array_t *ar, bool (*predicate)(void *object));

#endif
