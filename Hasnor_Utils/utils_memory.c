#include "utils_memory.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	CACHE_NONE,		// Memory is allocated on the fly
	CACHE_STATIC,	// Memory is pre-allocated and distributed in blocks of uniform size
	CACHE_DYNAMIC	// Memory is pre-allocated and distributed in blocks of varying size (slower, but less wasted space)
} cache_type_t;

typedef struct mem_s {
	uint			id;

	void			*address;
	size_t			size;

	struct mem_s	*next;
} mem_t;

void *_memCache = NULL;						// The allocated cache
cache_type_t _memCacheType = CACHE_NONE;	// The cache type
size_t _memCacheSize = 0;					// The total size of the cache
size_t _memCacheBlockSize = 0;				// If the cache is static, the size of each block

mem_t *_memStack = NULL;					// FILO stack of allocated areas
uint _memID = 0;							// Unique ID of allocated areas

void *_cache_alloc(size_t size)
{
	if (_memCacheType == CACHE_NONE)
	{
		return malloc(size);
	}
	else if (_memCacheType == CACHE_STATIC)
	{ // TODO
		return NULL;
	}
	else if (_memCacheType == CACHE_DYNAMIC)
	{
		mem_t *mem = _memStack;
		if (mem)
		{
			size_t difference = (size_t)mem->address - (size_t)_memCache;
			if (size < difference)
			{ // There's a slot at the beginning of the cache
				return _memCache;
			}
			else
			{
				while (mem->next)
				{ // FIXME: consecutively allocated addresses are not necessarily consecutive in the cache
					difference = (size_t)mem->next->address - ((size_t)mem->address + mem->size);

					if (size < difference)
					{ // There's a slot between two allocated spaces
						return (void*)((size_t)mem->address + mem->size);
					}

					mem = mem->next;
				}

				difference = (size_t)_memCache + _memCacheSize - ((size_t)mem->address + mem->size);

				if (size < difference)
				{ // There's a slot after the last allocated space
					return (void*)((size_t)mem->address + mem->size);
				}

				return NULL;
			}
		}
		else
		{ // Cache is empty
			return size < _memCacheSize ? _memCache : NULL;
		}
	}

	assert(0);
	return NULL;
}

void _cache_free(void *mem)
{
	if (_memCacheType == CACHE_NONE)
	{
		free(mem);
	}
	else if (_memCacheType == CACHE_STATIC)
	{
		// Nothing?
	}
	else if (_memCacheType == CACHE_DYNAMIC)
	{
		// Nothing...
	}
}

bool createStaticCache(size_t blockSize, size_t blockCount)
{
	_memCacheBlockSize = blockSize;
	_memCacheSize = blockSize * blockCount;
	_memCacheType = CACHE_STATIC;
	_memCache = malloc(_memCacheSize);

	return (_memCache != NULL);
}

bool createDynamicCache(size_t size)
{
	_memCacheSize = size;
	_memCacheType = CACHE_DYNAMIC;
	_memCache = malloc(size);

	return (_memCache != NULL);
}

void *mem_alloc(size_t size)
{
	void *mem = _cache_alloc(size);

	if (mem)
	{ // For now this data does not use the cache...
		mem_t *newAlloc = (mem_t*)malloc(sizeof(mem_t));

		newAlloc->address = mem;
		newAlloc->size = size;
		newAlloc->next = _memStack;
		newAlloc->id = _memID++;

		_memStack = newAlloc;
	}

	return mem;
}

void *mem_realloc(void *mem, size_t size)
{
	if (mem)
	{
		mem_t *mem2 = _memStack;

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
			void *newMem = _cache_alloc(size);
			memcpy(newMem, mem2->address, mem2->size);
			_cache_free(mem2->address);
			mem2->address = newMem;
			mem2->size = size;
			return newMem;
		}
		else
		{ // We were asked to reallocate a piece of memory we didn't create, let's just allocate it?
			assert(0);
			return mem_alloc(size);
		}
	}
	else
	{
		return mem_alloc(size);
	}
}

void *mem_dupe(const void *mem, size_t size)
{
	void *newAlloc = mem_alloc(size);
	memcpy(newAlloc, mem, size);
	return newAlloc;
}

void *mem_dupe2(const void *mem)
{
	mem_t *mem2 = _memStack;

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
	mem_t *mem2 = _memStack;
	mem_t *prev = NULL;

	if (!mem)
	{
		return;
	}

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
	{
		if (prev)
		{ // Not the first one on the list
			prev->next = mem2->next;
		}
		else
		{
			_memStack = mem2->next;
		}
		_cache_free(mem2->address);
		free(mem2);
	}
	else
	{
		printf("WARNING: Freeing memory address %p even though it was not allocated properly!\n", mem);
		assert(0);
		free(mem);
	}
}

void mem_free_safe(void *mem)
{
	mem_t *mem2 = _memStack;
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
	{
		if (prev)
		{ // Not the first one on the list
			prev->next = mem2->next;
		}
		else
		{
			_memStack = mem2->next;
		}
		_cache_free(mem2->address);
		free(mem2);
	}
}

void mem_free_all(void)
{
	mem_t *mem = _memStack;

	while (mem)
	{
		mem_t *next = mem->next;
		_cache_free(mem->address);
		free(mem);
		mem = next;
	}

	_memStack = NULL;
}

void mem_set(void *mem, int val, size_t size)
{
	memset(mem, val, size);
}

void mem_cpy(void *mem, const void *src, size_t size)
{
	memcpy(mem, src, size);
}

void mem_move(void *mem, const void *src, size_t size)
{
	memmove(mem, src, size);
}

size_t mem_size(void *mem)
{
	mem_t *mem2 = _memStack;

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
		return mem2->size;
	}
	else
	{
		return 0;
	}
}

void mem_print(void)
{
	mem_t *mem = _memStack;
	size_t total = 0;

	if (!_memStack)
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

void initMemoryFunctions(void)
{
	Memory.createStaticCache = createStaticCache;
	Memory.createDynamicCache = createDynamicCache;
	Memory.allocate = mem_alloc;
	Memory.reallocate = mem_realloc;
	Memory.duplicate = mem_dupe;
	Memory.duplicateSafe = mem_dupe2;
	Memory.free = mem_free;
	Memory.freeSafe = mem_free_safe;
	Memory.freeAll = mem_free_all;
	Memory.set = mem_set;
	Memory.copy = mem_cpy;
	Memory.move = mem_move;
	Memory.size = mem_size;
	Memory.print = mem_print;
}