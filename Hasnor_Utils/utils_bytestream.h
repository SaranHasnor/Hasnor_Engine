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
} bytestream;

void bytestream_init(bytestream *stream, uint size);
int bytestream_write(bytestream *stream, byte *data, uint size);
int bytestream_read(bytestream *stream, byte *out, uint size);
void bytestream_destroy(bytestream *stream);
char *bytestream_toString(bytestream *stream);

//byte randomByte();

#endif