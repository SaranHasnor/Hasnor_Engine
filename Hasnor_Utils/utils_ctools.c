#include "utils_ctools.h"

#include <stdlib.h>
#include <string.h>

// Allocated memory is stored in a FILO stack for an optimal parsing speed

typedef struct mem_s {
	unsigned int id;
	void *address;
	size_t size;
	struct mem_s *next;
} mem_t;

mem_t *alloc = NULL;

unsigned int id = 0;

void *mem_alloc(size_t size)
{
	void *mem = malloc(size);

	if (mem)
	{
		mem_t *newAlloc = (mem_t*)malloc(sizeof(mem_t));

		newAlloc->address = mem;
		newAlloc->size = size;
		newAlloc->next = alloc;
		newAlloc->id = id++;

		alloc = newAlloc;
	}

	return mem;
}

void *mem_realloc(void *mem, size_t size)
{
	if (!mem)
	{
		return mem_alloc(size);
	}
	else
	{
		mem_t *mem2 = alloc;

		while (mem2)
		{
			if (mem2->address == mem)
			{
				break;
			}
			mem2 = mem2->next;
		}

		if (mem2)
		{ // Found it
#if 0
			void *newMem = malloc(size);
			memcpy(newMem, mem2->address, mem2->size);
			free(mem2->address);
			mem2->address = newMem;
			mem2->size = size;
			return newMem;
#else
			mem2->address = realloc(mem2->address, size);
			mem2->size = size;
			return mem2->address;
#endif
		}
		else
		{ // Just allocate it
			return mem_alloc(size);
		}
	}
}

void *mem_dupe(void *mem, size_t size)
{
	void *newAlloc = mem_alloc(size);
	memcpy(newAlloc, mem, size);
	return newAlloc;
}

void *mem_dupe2(void *mem)
{
	mem_t *mem2 = alloc;

	while (mem2)
	{
		if (mem2->address == mem)
		{
			break;
		}
		mem2 = mem2->next;
	}

	if (mem2)
	{
		void *newAlloc = mem_alloc(mem2->size);
		memcpy(newAlloc, mem, mem2->size);
		return newAlloc;
	}
	else
	{
		return NULL;
	}
}

void mem_free(void *mem)
{
	mem_t *mem2 = alloc;
	mem_t *prev = NULL;

	while (mem2)
	{
		if (mem2->address == mem)
		{
			break;
		}
		prev = mem2;
		mem2 = mem2->next;
	}

	if (mem2)
	{ // Found it
		if (prev)
		{ // Not the first one on the list
			prev->next = mem2->next;
		}
		else
		{ // First one on the list
			alloc = mem2->next;
		}
		free(mem2->address);
		free(mem2);
	}
	else
	{
		printf("WARNING: Freeing memory address %p even though it was not allocated properly!\n", mem);
		assert(0);
		free(mem);
	}
}

void mem_free_all()
{
	mem_t *mem = alloc;

	while (mem)
	{
		mem_t *next = mem->next;
		free(mem->address);
		free(mem);
		mem = next;
	}

	alloc = NULL;
}

void mem_set(void *mem, int val, size_t size)
{
	memset(mem, val, size);
}

void mem_cpy(void *mem, void *src, size_t size)
{
	memcpy(mem, src, size);
}

size_t mem_size(void *mem)
{
	mem_t *mem2 = alloc;

	while (mem2)
	{
		if (mem2->address == mem)
		{
			break;
		}
		mem2 = mem2->next;
	}

	if (mem2)
	{ // Found it
		return mem2->size;
	}
	else
	{
		return 0;
	}
}

void mem_print()
{
	mem_t *mem = alloc;
	size_t total = 0;

	if (!alloc)
	{
		printf("Could not find any allocated memory\n");
		return;
	}

	printf("Allocated memory:\n");
	while (mem)
	{
		printf(" %i - Address %p has %i bytes allocated\n", mem->id, mem->address, mem->size);
		total += mem->size;
		mem = mem->next;
	}

	printf("Total: %i bytes\n", total);
}