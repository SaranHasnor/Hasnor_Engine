#ifndef HASNOR_TEXTURE_IMPORTED
#define HASNOR_TEXTURE_IMPORTED

#include <utils.h>
#include <utils_map.h>

typedef struct {
	char		*filePath;

	byte		*data;
	int			width, height;

	uint		textureID;
} texture_t;

typedef enum {
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY
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
	shader_t	*geometryShader;

	uint		programID;

	// Locations
	uint		originLocation;
	uint		rotationLocation;

	uint		coordsLocation;
	uint		texCoordsLocation;

	uint		viewMatLocation;

	uint		textureLocation;
	uint		colorLocation;

	map_t		customLocations;
} program_t;

typedef struct {
	texture_t*	(*fromPath)(const char *filePath);
	void		(*update)(texture_t *texture);
} _texture_functions;

typedef struct {
	shader_t*	(*fromContent)(shaderType_t type, const char *contents);
	shader_t*	(*fromPath)(shaderType_t type, const char *filePath);
} _shader_functions;

typedef struct {
	program_t*	(*withShaders)(shader_t *vertexShader, shader_t *fragmentShader, shader_t *geometryShader);

	void		(*registerCustomUniform)(program_t *program, const char *name);
	int			(*getCustomUniformLocation)(program_t *program, const char *name);

	program_t*	(*getDefault)(bool forTexture);
} _program_functions;

#ifdef HASNOR_ENGINE_INTERNAL
_texture_functions *TextureInternal;
_shader_functions *ShaderInternal;
_program_functions *ProgramInternal;
#endif

#ifdef HASNOR_INIT
void initTextureFunctions(_texture_functions *Texture);
void initShaderFunctions(_shader_functions *Shader);
void initProgramFunctions(_program_functions *Program);
#endif

#endif
