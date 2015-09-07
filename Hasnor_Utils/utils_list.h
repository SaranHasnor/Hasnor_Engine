#ifndef UTILS_LIST_IMPORTED
#define UTILS_LIST_IMPORTED

#include "utils_types.h"

typedef struct {
	void		**content;
	uint		size;
} list_t;

void list_init(list_t *list);
void list_add(list_t *list, void *object);
void list_insert(list_t *list, void *object, uint pos);
void list_remove(list_t *list, void *object);
void list_removeAt(list_t *list, uint pos);

#endif