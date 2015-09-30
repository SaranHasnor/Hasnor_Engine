
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

void map_init(map_t *map);
void *map_getValueForKey(map_t *map, const char *key);
void map_setValueForKey(map_t *map, const char *key, void *value, bool strong);
void map_removeValueForKey(map_t *map, const char *key);