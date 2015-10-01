#ifndef HASNOR_BYTESTREAM_DEFINED
#define HASNOR_BYTESTREAM_DEFINED

/*
utils_bytestream

- Functions and structures for writing and reading streams of bytes
*/

#include "utils_types.h"

typedef struct {
	byte	*data;
	uint	len;
	uint	cursor;
} bytestream_t;

typedef struct {
	void (*init)(bytestream_t *stream, uint size);
	int (*write)(bytestream_t *stream, byte *data, uint size);
	int (*read)(bytestream_t *stream, byte *out, uint size);
	void (*free)(bytestream_t *stream);
	char* (*toString)(bytestream_t *stream);
} _bytestream_functions;

_bytestream_functions ByteStream;

void initByteStreamFunctions();

#endif