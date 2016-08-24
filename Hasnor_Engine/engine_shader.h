#ifndef HASNOR_SHADER_IMPORTED
#define HASNOR_SHADER_IMPORTED

#include <utils.h>
#include <utils_map.h>

typedef enum {
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY
	//SHADER_TRANSFORM,
	//SHADER_COMPUTE
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
	uint		normalsLocation;

	uint		viewMatLocation;

	uint		textureLocation;
	uint		colorLocation;

	map_t		customLocations;
} program_t;

typedef struct {
	shader_t*	(*fromContent)(shaderType_t type, const char *contents);
	shader_t*	(*fromPath)(shaderType_t type, const char *filePath);
} _glshader_functions;

typedef struct {
	program_t*	(*withShaders)(shader_t *vertexShader, shader_t *fragmentShader, shader_t *geometryShader);
	//program_t*	(*transformFeedback)(shader_t *transformShader, const char *varyings[], uint varyingsCount);

	void		(*registerCustomUniform)(program_t *program, const char *name);
	int			(*getCustomUniformLocation)(program_t *program, const char *name);

	program_t*	(*getDefault)(bool forTexture);
} _glprogram_functions;

_glshader_functions GLShader;
_glprogram_functions GLProgram;

#ifdef HASNOR_INIT
void initShaderFunctions(void);
#endif

#endif
