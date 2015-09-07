#ifndef MEM_H_DEFINED
#define MEM_H_DEFINED

/*
utils_ctools

- Memory allocation assistant
- Debugging tools
*/

#include <stdio.h>
#include <assert.h>

// TODO: Macros, memcpy/memset, leak detection

void *mem_alloc(size_t size);
void *mem_realloc(void *mem, size_t size);
void *mem_dupe(void *mem, size_t size);
void *mem_dupe2(void *mem);
void mem_free(void *mem);
void mem_free_safe(void *mem);
void mem_free_all();
void mem_set(void *mem, int val, size_t size);
void mem_cpy(void *mem, void *src, size_t size);
size_t mem_size(void *mem);
void mem_print();

#endif