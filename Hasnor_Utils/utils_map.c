#include "utils_map.h"

void map_init(map_t *map)
{
	map->list = NULL;
}

ulong _hashForKey(const char *key)
{ // Temporary
	ulong value = 0;
	uint i = 0;
	while (key[i] != '\0')
	{
		value += key[i] << (8 * (i%8));
		i++;
	}
	return value;
}

void *map_getValueForKey(map_t *map, const char *key)
{
	list_t **list = &map->list;
	ulong hash = _hashForKey(key);

	while (*list)
	{
		mapEntry_t *entry = (mapEntry_t*)(*list)->content;
		if (entry->hash == hash)
		{
			return entry->value;
		}
		list = &(*list)->next;
	}
	return NULL;
}

void map_setValueForKey(map_t *map, const char *key, void *value, bool strong)
{
	list_t **list = &map->list;
	ulong hash = _hashForKey(key);
	mapEntry_t *newEntry;

	while (*list)
	{
		mapEntry_t *entry = (mapEntry_t*)(*list)->content;
		if (entry->hash == hash)
		{ // Already exists, replace the old value with the new value
			if (entry->strong)
			{
				destroy(entry->value);
			}
			entry->value = value;
			entry->strong = strong;
			return;
		}
		list = &(*list)->next;
	}

	// Doesn't exist yet
	newEntry = newObject(mapEntry_t);
	newEntry->hash = hash;
	newEntry->key = quickString(key);
	newEntry->value = value;
	newEntry->strong = strong;
	List.add(list, newEntry);
}

void map_removeValueForKey(map_t *map, const char *key)
{
	list_t **list = &map->list;
	ulong hash = _hashForKey(key);

	while (*list)
	{
		mapEntry_t *entry = (mapEntry_t*)(*list)->content;
		if (entry->hash == hash)
		{
			list_t *curObject = *list;
			if (entry->strong)
			{
				destroy(entry->value);
			}
			destroy(entry->key);
			*list = (*list)->next;
			destroy(curObject);
			return;
		}
		list = &(*list)->next;
	}
}

void initMapFunctions()
{
	Map.init = map_init;
	Map.getValueForKey = map_getValueForKey;
	Map.setValueForKey = map_setValueForKey;
	Map.removeValueForKey = map_removeValueForKey;
}