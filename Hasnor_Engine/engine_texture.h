#ifndef HASNOR_TEXTURE_IMPORTED
#define HASNOR_TEXTURE_IMPORTED

#include <utils.h>

typedef struct {
	char		*filePath;

	byte		*data;
	int			width, height;

	uint		textureID;
} texture_t;

typedef struct {
	texture_t*	(*fromPath)(const char *filePath);
	void		(*update)(texture_t *texture);
} _texture_functions;

_texture_functions GLTexture;

#ifdef HASNOR_INIT
void initTextureFunctions(void);
#endif

#endif
