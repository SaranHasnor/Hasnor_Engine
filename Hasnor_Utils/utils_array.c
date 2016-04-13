#include "utils_array.h"

#include "utils.h"

void array_init(array_t *ar, uint capacity)
{
	ar->size = 0;
	ar->capacity = capacity;
	if (capacity > 0)
	{
		ar->content = newArray(void*, capacity);
	}
	else
	{
		ar->content = NULL;
	}
}

void _resizeArrayIfNeeded(array_t *ar)
{ // TODO: Resize larger every time this step is required
	if (ar->size >= ar->capacity)
	{
		ar->content = (void**)Memory.realloc(ar->content, sizeof(void*) * (ar->size+1));
	}
}

void array_add(array_t *ar, void *object)
{
	_resizeArrayIfNeeded(ar);
	ar->content[ar->size++] = object;
}

void array_insert(array_t *ar, void *object, uint pos)
{
	if (pos <= ar->size)
	{
		uint i;
		_resizeArrayIfNeeded(ar);
		for (i = ar->size; i > pos; i--)
		{
			ar->content[i] = ar->content[i-1];
		}
		ar->content[pos] = object;
		ar->size++;
	}
}

void array_remove(array_t *ar, void *object)
{
	uint i;
	bool found = false;
	for (i = 0; i < ar->size; i++)
	{
		if (ar->content[i] == object)
		{
			found = true;
		}

		if (found && i < ar->size - 1)
		{
			ar->content[i] = ar->content[i+1];
		}
	}
	if (found)
	{
		ar->size--;
	}
}

void array_removeAt(array_t *ar, uint pos)
{
	if (pos < ar->size)
	{
		uint i;
		for (i = pos; i < ar->size - 1; i++)
		{
			ar->content[i] = ar->content[i+1];
		}
		ar->size--;
	}
}

int array_indexOf(array_t *ar, void *object)
{
	uint i = 0;
	while (i < ar->size)
	{
		if (ar->content[i] == object)
		{
			return (int)i;
		}
		i++;
	}
	return -1;
}

int array_indexOfFirstMatch(array_t *ar, bool (*predicate)(void *object))
{
	uint i = 0;
	while (i < ar->size)
	{
		if (predicate(ar->content[i]))
		{
			return (int)i;
		}
		i++;
	}
	return -1;
}

void initArrayFunctions(void)
{
	Array.init = array_init;
	Array.add = array_add;
	Array.insert = array_insert;
	Array.remove = array_remove;
	Array.removeAt = array_removeAt;
	Array.indexOf = array_indexOf;
	Array.indexOfFirstMatch = array_indexOfFirstMatch;
}