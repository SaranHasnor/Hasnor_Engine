#define HASNOR_ENGINE_INTERNAL

#include "engine_shader.h"
#include "engine_texture.h"

#include <utils_file.h>
#include <utils_string.h>

#include <GL/glew.h>

const char *defaultVertexShader = 
	"in vec3 pos;															\
	in vec2 texPos;															\
	in vec3 normal;															\
	varying vec2 v_texCoord;												\
	varying vec3 v_normal;													\
	uniform mat4 viewMatrix;												\
	uniform vec3 origin;													\
	uniform mat4 rotation;													\
	void main(void)															\
	{																		\
		v_texCoord = texPos;												\
		v_normal = normal;													\
		vec4 actualPos = vec4(origin, 0.0) + rotation * vec4(pos, 1.0);		\
		gl_Position = actualPos * viewMatrix;								\
	}";

const char *defaultFragmentShader =
	"uniform vec4 customColor;					\
	void main(void)								\
	{											\
		gl_FragColor = customColor;				\
	}";

const char *defaultFragmentShader_texture =
	"uniform sampler2D tex;										\
	uniform vec4 customColor;									\
	varying vec2 v_texCoord;									\
	void main(void)												\
	{															\
		gl_FragColor = texture(tex, v_texCoord) * customColor;	\
	}";

void _printShaderLog(GLuint obj)
{
	int infoLogLength = 0;
	int maxLength;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &maxLength);

	infoLog = allocArray(char, maxLength);

	glGetShaderInfoLog(obj, maxLength, &infoLogLength, infoLog);

	if (infoLogLength > 0)
	{
		Console.print("printShaderLog: %s\n", infoLog);
	}

	dealloc(infoLog);
}

GLenum _translateShaderType(shaderType_t type)
{
	switch (type)
	{
	case SHADER_FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case SHADER_VERTEX:
		return GL_VERTEX_SHADER;
	case SHADER_GEOMETRY:
		return GL_GEOMETRY_SHADER;
	default:
		return 0;
	}
}

shader_t *shaderFromContent(shaderType_t type, const char *contents)
{
	shader_t *newShader = alloc(shader_t);
	newShader->filePath = NULL;
	newShader->content = String.create(contents);
	newShader->type = type;

	newShader->shaderID = glCreateShader(_translateShaderType(type));
	glShaderSource(newShader->shaderID, 1, &newShader->content, NULL);
	glCompileShader(newShader->shaderID);

#ifdef _DEBUG
	_printShaderLog(newShader->shaderID);
#endif

	return newShader;
}

shader_t *shaderFromPath(shaderType_t type, const char *filePath)
{
	char *contents = NULL;
	if (File.read(filePath, &contents))
	{
		shader_t *newShader = shaderFromContent(type, contents);
		newShader->filePath = String.create(filePath);
		return newShader;
	}
	return NULL;
}

void _printProgramLog(GLuint obj)
{
	int infoLogLength = 0;
	int maxLength;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &maxLength);

	infoLog = allocArray(char, maxLength);

	glGetProgramInfoLog(obj, maxLength, &infoLogLength, infoLog);

	if (infoLogLength > 0)
	{
		Console.print("printProgramLog: %s\n", infoLog);
	}

	dealloc(infoLog);
}

program_t *programWithShaders(shader_t *vertexShader, shader_t *fragmentShader, shader_t *geometryShader)
{
	program_t *newProgram = alloc(program_t);
	newProgram->vertexShader = vertexShader;
	newProgram->fragmentShader = fragmentShader;
	newProgram->geometryShader = geometryShader;

	newProgram->programID = glCreateProgram();
	glAttachShader(newProgram->programID, vertexShader->shaderID);
	glAttachShader(newProgram->programID, fragmentShader->shaderID);
	if (geometryShader)
	{
		glAttachShader(newProgram->programID, geometryShader->shaderID);
	}
	glLinkProgram(newProgram->programID);

#ifdef _DEBUG
	_printProgramLog(newProgram->programID);
#endif

	newProgram->coordsLocation = glGetAttribLocation(newProgram->programID, "pos");
	newProgram->texCoordsLocation = glGetAttribLocation(newProgram->programID, "texPos");
	newProgram->normalsLocation = glGetAttribLocation(newProgram->programID, "normal");
	newProgram->originLocation = glGetUniformLocation(newProgram->programID, "origin");
	newProgram->rotationLocation = glGetUniformLocation(newProgram->programID, "rotation");
	newProgram->viewMatLocation = glGetUniformLocation(newProgram->programID, "viewMatrix");
	newProgram->textureLocation = glGetUniformLocation(newProgram->programID, "tex");
	newProgram->colorLocation = glGetUniformLocation(newProgram->programID, "customColor");

	Map.init(&newProgram->customLocations);

	return newProgram;
}

program_t *transformFeedbackProgram(shader_t *transformShader, const char *varyings[], uint varyingsCount)
{
	program_t *newProgram = alloc(program_t);
	newProgram->vertexShader = transformShader;
	newProgram->fragmentShader = NULL;
	newProgram->geometryShader = NULL;

	newProgram->programID = glCreateProgram();
	glAttachShader(newProgram->programID, transformShader->shaderID);

	glTransformFeedbackVaryings(newProgram->programID, varyingsCount, varyings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(newProgram->programID);

#ifdef _DEBUG
	_printProgramLog(newProgram->programID);
#endif

	newProgram->coordsLocation = glGetAttribLocation(newProgram->programID, "pos");
	newProgram->texCoordsLocation = glGetAttribLocation(newProgram->programID, "texPos");
	newProgram->normalsLocation = glGetAttribLocation(newProgram->programID, "normal");
	newProgram->originLocation = glGetUniformLocation(newProgram->programID, "origin");
	newProgram->rotationLocation = glGetUniformLocation(newProgram->programID, "rotation");
	newProgram->viewMatLocation = glGetUniformLocation(newProgram->programID, "viewMatrix");

	Map.init(&newProgram->customLocations);

	return newProgram;
}

void registerCustomUniformForProgram(program_t *program, const char *name)
{
	int *location = alloc(int);
	*location = glGetUniformLocation(program->programID, name);
	Map.setValueForKey(&program->customLocations, name, location, true);
}

int getCustomUniformLocationForProgram(program_t *program, const char *name)
{
	int *location = (int*)Map.getValueForKey(&program->customLocations, name);
	if (location)
	{
		return *location;
	}
	return -1;
}

program_t *defaultProgram(bool forTexture)
{
	static program_t *_defaultProgram = NULL;
	static program_t *_defaultTextureProgram = NULL;

	if (forTexture)
	{
		if (!_defaultTextureProgram)
		{
			shader_t *vShader = shaderFromContent(SHADER_VERTEX, defaultVertexShader);
			shader_t *fShader = shaderFromContent(SHADER_FRAGMENT, defaultFragmentShader_texture);
			_defaultTextureProgram = programWithShaders(vShader, fShader, NULL);
		}

		return _defaultTextureProgram;
	}
	else
	{
		if (!_defaultProgram)
		{
			shader_t *vShader = shaderFromContent(SHADER_VERTEX, defaultVertexShader);
			shader_t *fShader = shaderFromContent(SHADER_FRAGMENT, defaultFragmentShader);
			_defaultProgram = programWithShaders(vShader, fShader, NULL);
		}

		return _defaultProgram;
	}
}

void initShaderFunctions(void)
{
	GLShader.fromContent = shaderFromContent;
	GLShader.fromPath = shaderFromPath;

	GLProgram.getDefault = defaultProgram;
	GLProgram.getCustomUniformLocation = getCustomUniformLocationForProgram;
	GLProgram.withShaders = programWithShaders;
	//GLProgram.transformFeedback = transformFeedbackProgram;
	GLProgram.registerCustomUniform = registerCustomUniformForProgram;
}

#undef HASNOR_ENGINE_INTERNAL
