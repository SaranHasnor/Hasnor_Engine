#ifndef HASNOR_MAP_IMPORTED
#define HASNOR_MAP_IMPORTED

#include "utils.h"
#include "utils_list.h"

typedef struct {
	ulong		hash;
	char		*key;
	void		*value;
	bool		strong;
} mapEntry_t;

typedef struct {
	list_t		*list;
} map_t;

typedef struct {
	void (*init)(map_t *map);
	void* (*getValueForKey)(map_t *map, const char *key);
	void (*setValueForKey)(map_t *map, const char *key, void *value, bool strong);
	void (*removeValueForKey)(map_t *map, const char *key);
} _map_functions;

_map_functions Map;

#ifdef HASNOR_INIT
void initMapFunctions();
#endif

#endif