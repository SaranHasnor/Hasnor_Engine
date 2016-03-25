#ifndef HASNOR_MEMORY_DEFINED
#define HASNOR_MEMORY_DEFINED

#include <stdio.h>
#include "utils_types.h"

// TODO: leak detection

typedef struct {
	bool (*createStaticCache)(size_t blockSize, size_t blockCount);
	bool (*createDynamicCache)(size_t size);

	void* (*allocate)(size_t size);
	void* (*reallocate)(void *mem, size_t size);

	void* (*duplicate)(const void *mem, size_t size);
	void* (*duplicateSafe)(const void *mem);

	void (*free)(void *mem);
	void (*freeSafe)(void *mem);
	void (*freeAll)(void);

	void (*set)(void *mem, int val, size_t size);
	void (*copy)(void *mem, const void *src, size_t size);
	void (*move)(void *mem, const void *src, size_t size);

	size_t (*size)(void *mem);
	void (*print)(void);
} _memory_functions;

_memory_functions Memory;


#define alloc(type) (type*)Memory.allocate(sizeof(type))
//#define allocZero(type) (type*)Memory.allocate(sizeof(type)) // TODO
#define allocArray(type, count) (type*)Memory.allocate(sizeof(type) * (count))
#define dealloc(object) Memory.free(object)

#ifdef HASNOR_INIT
void initMemoryFunctions(void);
#endif

#endif
