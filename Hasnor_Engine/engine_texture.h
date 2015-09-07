#include <utils.h>

typedef struct {
	char		*filePath;

	byte		*data;
	int			width, height;

	uint		textureID;
} texture_t;

typedef enum {
	SHADER_VERTEX,
	SHADER_FRAGMENT
} shaderType_t;

typedef struct {
	char			*content;
	char			*filePath;

	shaderType_t	type;

	uint			shaderID;
} shader_t;

typedef struct {
	shader_t	*vertexShader;
	shader_t	*fragmentShader;

	uint		programID;

	// Locations
	uint		originLocation;
	uint		rotationLocation;

	uint		coordsLocation;
	uint		texCoordsLocation;

	uint		viewMatLocation;

	uint		textureLocation;
	uint		colorLocation;
} program_t;

void updateTextureContents(texture_t *texture);
texture_t *textureFromPath(const char *filePath);

shader_t *shaderFromContent(shaderType_t type, const char *contents);
shader_t *shaderFromPath(shaderType_t type, const char *filePath);
program_t *programWithShaders(shader_t *vertexShader, shader_t *fragmentShader);

program_t *defaultProgram(bool forTexture);