#ifndef HASNOR_BYTESTREAM_DEFINED
#define HASNOR_BYTESTREAM_DEFINED

/*
utils_bytestream

- Functions and structures for writing and reading streams of bytes
*/

typedef unsigned char byte;
typedef char sbyte;

typedef struct {
	byte			*data;
	unsigned int	len;
	unsigned int	cursor;
} bytestream;

void bytestream_init(bytestream *stream, unsigned int size);
int bytestream_write(bytestream *stream, byte *data, unsigned int size);
int bytestream_read(bytestream *stream, byte *out, unsigned int size);
void bytestream_destroy(bytestream *stream);
char *bytestream_toString(bytestream *stream);

//byte randomByte();

#endif