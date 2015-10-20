#ifndef UTILS_LIST_IMPORTED
#define UTILS_LIST_IMPORTED

#include "utils_types.h"

typedef struct list_s {
	void			*content;

	struct list_s	*next;
} list_t;

typedef struct {
	void (*add)(list_t **list, void *object);
	void (*insert)(list_t **list, void *object, uint pos);
	void (*remove)(list_t **list, void *object);
	void (*removeAt)(list_t **list, uint pos);
	void (*clear)(list_t **list);
	bool (*contains)(list_t **list, void *object);
	list_t* (*copy)(list_t **list);
	int (*count)(list_t **list);
} _list_functions;

_list_functions List;

#ifdef HASNOR_INIT
void initListFunctions(void);
#endif

#endif
