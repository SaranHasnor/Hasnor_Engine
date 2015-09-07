#include "utils_list.h"

#include "utils.h"

void list_init(list_t *list)
{
	list->content = NULL;
	list->size = 0;
}

void list_add(list_t *list, void *object)
{
	list->content = (void**)mem_realloc(list->content, sizeof(void*) * (list->size+1));
	list->content[list->size] = object;
	list->size++;
}

void list_insert(list_t *list, void *object, uint pos)
{
	uint i;
	list->content = (void**)mem_realloc(list->content, sizeof(void*) * (list->size+1));
	for (i = list->size; i > pos; i--)
	{
		list->content[i] = list->content[i-1];
	}
	list->content[pos] = object;
	list->size++;
}

void list_remove(list_t *list, void *object)
{
	uint i;
	bool found = false;
	for (i = 0; i < list->size; i++)
	{
		if (list->content[i] == object)
		{
			found = true;
		}

		if (found && i < list->size - 1)
		{
			list->content[i] = list->content[i+1];
		}
	}
	if (found)
	{
		if (list->size == 1)
		{
			list->content = NULL;
		}
		else
		{
			list->content = (void**)mem_realloc(list->content, sizeof(void*) * (list->size-1));
		}
		list->size--;
	}
}

void list_removeAt(list_t *list, uint pos)
{
	if (pos < list->size)
	{
		uint i;
		for (i = pos; i < list->size - 1; i++)
		{
			list->content[i] = list->content[i+1];
		}
		if (list->size == 1)
		{
			list->content = NULL;
		}
		else
		{
			list->content = (void**)mem_realloc(list->content, sizeof(void*) * (list->size-1));
		}
		list->size--;
	}
}