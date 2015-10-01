#include "utils_bytestream.h"
#include "utils_ctools.h"

void bytestream_init(bytestream_t *stream, uint size)
{
	if (size)
	{
		stream->data = newArray(byte, size);
		Memory.set(stream->data, 0, sizeof(byte) * size);
	}
	else
	{
		stream->data = NULL;
	}
	stream->len = size;
	stream->cursor = 0;
}

int bytestream_write(bytestream_t *stream, byte *data, uint size)
{
	assert(stream->cursor + size <= stream->len);
	if (size > 0)
	{
		Memory.copy(stream->data+stream->cursor, data, size);
	}
	stream->cursor += size;
	return size;
}

int bytestream_read(bytestream_t *stream, byte *out, uint size)
{
	assert(stream->cursor + size <= stream->len);
	Memory.copy(out, stream->data+stream->cursor, size);
	stream->cursor += size;
	return size;
}

void bytestream_destroy(bytestream_t *stream)
{
	stream->len = 0;
	stream->cursor = 0;
	if (stream->data)
	{
		destroy(stream->data);
	}
}

char *bytestream_toString(bytestream_t *stream)
{
	char *res;
	uint i;
	uint len = stream->len;

	if (!len) len = 1;

	res = newArray(char, len*9); // len * (8 digits + one space or \0)

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

void initByteStreamFunctions()
{
	ByteStream.init = bytestream_init;
	ByteStream.write = bytestream_write;
	ByteStream.read = bytestream_read;
	ByteStream.free = bytestream_destroy;
	ByteStream.toString = bytestream_toString;
}