#include "utils_list.h"

#include "utils.h"

list_t *list_new(void *object)
{
	list_t *newList = (list_t*)mem_alloc(sizeof(list_t));
	newList->content = object;
	newList->next = NULL;
	return newList;
}

void list_add(list_t *list, void *object)
{
	while (list->next)
	{
		list = list->next;
	}
	list->next = list_new(object);
}

void list_insert(list_t *list, void *object, uint pos)
{
	list_t **curList = &list;
	uint i = 0;
	while (*curList && i < pos)
	{
		curList = &(*curList)->next;
		i++;
	}

	if (i == pos)
	{
		list_t *next = *curList;
		*curList = list_new(object);
		(*curList)->next = *curList;
	}
}

void list_remove(list_t *list, void *object)
{
	list_t **curList = &list;
	while (*curList)
	{
		if ((*curList)->content == object)
		{
			list_t *current = *curList;
			*curList = (*curList)->next;
			mem_free(current);
			return;
		}
		curList = &(*curList)->next;
	}
}

void list_removeAt(list_t *list, uint pos)
{
	list_t **curList = &list;
	uint i = 0;
	while (*curList && i < pos)
	{
		curList = &(*curList)->next;
		i++;
	}

	if (i == pos)
	{
		list_t *current = *curList;
		*curList = (*curList)->next;
		mem_free(current);
	}
}
