#ifndef MEM_H_DEFINED
#define MEM_H_DEFINED

/*
utils_ctools

- Memory allocation assistant
- Debugging tools
*/

#include <stdio.h>
#include <assert.h>

// TODO: leak detection

typedef struct {
	void* (*alloc)(size_t size);
	void* (*realloc)(void *mem, size_t size);
	void* (*duplicate)(void *mem, size_t size);
	void* (*duplicateSafe)(void *mem);
	void (*free)(void *mem);
	void (*freeSafe)(void *mem);
	void (*freeAll)(void);
	void (*set)(void *mem, int val, size_t size);
	void (*copy)(void *mem, void *src, size_t size);
	size_t (*size)(void *mem);
	void (*print)(void);
} _memory_functions;

_memory_functions Memory;

#define newObject(x) (x*)Memory.alloc(sizeof(x))
#define newArray(x, n) (x*)Memory.alloc(sizeof(x) * n)
#define destroy(x) Memory.free(x)

#ifdef HASNOR_INIT
void initMemoryFunctions(void);
#endif

#endif