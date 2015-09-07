#include "utils_bytestream.h"
#include "utils_ctools.h"
#include "utils_types.h"

void bytestream_init(bytestream *stream, unsigned int size)
{
	if (size)
	{
		stream->data = (byte*)mem_alloc(sizeof(byte) * size);
		mem_set(stream->data, 0, sizeof(byte) * size);
	}
	else
	{
		stream->data = NULL;
	}
	stream->len = size;
	stream->cursor = 0;
}

int bytestream_write(bytestream *stream, byte *data, unsigned int size)
{
	assert(stream->cursor + size <= stream->len);
	if (size > 0)
	{
		mem_cpy(stream->data+stream->cursor, data, size);
	}
	stream->cursor += size;
	return size;
}

int bytestream_read(bytestream *stream, byte *out, unsigned int size)
{
	assert(stream->cursor + size <= stream->len);
	mem_cpy(out, stream->data+stream->cursor, size);
	stream->cursor += size;
	return size;
}

void bytestream_destroy(bytestream *stream)
{
	stream->len = 0;
	stream->cursor = 0;
	if (stream->data)
	{
		mem_free(stream->data);
	}
}

char *bytestream_toString(bytestream *stream)
{
	char *res;
	uint i;
	uint len = stream->len;

	if (!len) len = 1;

	res = (char*)mem_alloc(sizeof(byte)*(len*9)); // len * (8 digits + one space or \0)

	for (i = 0; i < len; i++)
	{
		uint j;
		for (j = 0; j < 8; j++)
		{
			if (stream->data[i] & (1 << (7-j)))
			{
				res[9*i+j] = '1';
			}
			else
			{
				res[9*i+j] = '0';
			}
		}
		res[9*i+j] = ' ';
	}
	res[9*len-1] = '\0';
	return res;
}

/*byte randomByte()
{
	byte res = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		res = (2*res) + rand() % 2;
	}
	return res;
}

void fillWithRandom(void *dst, size_t size)
{
	uint i;
	for (i = 0; i < size; i++)
	{
		((byte*)dst)[i] = randomByte();
	}
}*/