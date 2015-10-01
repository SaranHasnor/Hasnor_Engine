#include "utils_list.h"

#include "utils.h"

void list_add(list_t **list, void *object)
{
	while (*list)
	{
		list = &(*list)->next;
	}
	*list = newObject(list_t);
	(*list)->content = object;
	(*list)->next = NULL;
}

void list_insert(list_t **list, void *object, uint pos)
{
	uint i = 0;
	while (*list && i < pos)
	{
		list = &(*list)->next;
		i++;
	}

	if (i == pos)
	{
		list_t *next = *list;
		*list = newObject(list_t);
		(*list)->content = object;
		(*list)->next = next;
	}
}

void list_remove(list_t **list, void *object)
{
	while (*list)
	{
		if ((*list)->content == object)
		{
			list_t *current = *list;
			*list = (*list)->next;
			destroy(current);
			return;
		}
		list = &(*list)->next;
	}
}

void list_removeAt(list_t **list, uint pos)
{
	uint i = 0;
	while (*list && i < pos)
	{
		list = &(*list)->next;
		i++;
	}

	if (i == pos)
	{
		list_t *current = *list;
		*list = (*list)->next;
		destroy(current);
	}
}

void initListFunctions()
{
	List.add = list_add;
	List.insert = list_insert;
	List.remove = list_remove;
	List.removeAt = list_removeAt;
}