#include "utils_list.h"

#include "utils.h"

void list_add(list_t **list, void *object)
{
	while (*list)
	{
		list = &(*list)->next;
	}
	*list = alloc(list_t);
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
		*list = alloc(list_t);
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
			dealloc(current);
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
		dealloc(current);
	}
}

void list_clear(list_t **list)
{
	if (*list)
	{
		list_clear(&(*list)->next);
		dealloc(*list);
		*list = NULL;
	}
}

bool list_contains(list_t **list, void *object)
{
	while (*list)
	{
		if ((*list)->content == object)
		{
			return true;
		}
		list = &(*list)->next;
	}
	return false;
}

list_t *list_copy(list_t **list)
{
	list_t *res = NULL;
	list_t **cursor = &res;
	while (*list)
	{
		*cursor = alloc(list_t);
		(*cursor)->content = (*list)->content;
		(*cursor)->next = NULL;

		cursor = &(*cursor)->next;
		list = &(*list)->next;
	}
	return res;
}

int list_count(list_t **list)
{
	int count = 0;
	while (*list)
	{
		count++;
		list = &(*list)->next;
	}
	return count;
}

void initListFunctions(void)
{
	List.add = list_add;
	List.insert = list_insert;
	List.remove = list_remove;
	List.removeAt = list_removeAt;
	List.clear = list_clear;
	List.contains = list_contains;
	List.copy = list_copy;
	List.count = list_count;
}