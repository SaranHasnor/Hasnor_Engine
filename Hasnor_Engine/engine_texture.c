#define HASNOR_ENGINE_INTERNAL

#include "engine_texture.h"

#include <utils_file.h>
#include <utils_string.h>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const char *defaultVertexShader = 
	"in vec3 pos;															\
	in vec2 texPos;															\
	varying vec2 texCoord;													\
	uniform mat4 viewMatrix;												\
	uniform vec3 origin;													\
	uniform mat4 rotation;													\
	void main(void)															\
	{																		\
		texCoord = texPos;													\
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
	varying vec2 texCoord;										\
	void main(void)												\
	{															\
		gl_FragColor = texture(tex, texCoord) * customColor;	\
	}";


void updateTextureContents(texture_t *texture)
{
	texture->data = stbi_load(texture->filePath, &texture->width, &texture->height, NULL, STBI_rgb_alpha);

	glEnable(GL_TEXTURE_2D);

	if (!texture->textureID)
	{
		glGenTextures(1, &texture->textureID);
	}

	glActiveTexture(GL_TEXTURE0 + texture->textureID);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			texture->width,
			texture->height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			texture->data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE0);
}

texture_t *textureFromPath(const char *filePath)
{
	texture_t *newTexture = newObject(texture_t);

	newTexture->filePath = String.create(filePath);
	newTexture->textureID = 0;

	updateTextureContents(newTexture);

	return newTexture;
}

void _printShaderLog(GLuint obj)
{
	int infoLogLength = 0;
	int maxLength;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &maxLength);

	infoLog = newArray(char, maxLength);

	glGetShaderInfoLog(obj, maxLength, &infoLogLength, infoLog);

	if (infoLogLength > 0)
	{
		printf("printShaderLog: %s\n", infoLog);
	}

	destroy(infoLog);
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
	shader_t *newShader = newObject(shader_t);
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

	infoLog = newArray(char, maxLength);

	glGetProgramInfoLog(obj, maxLength, &infoLogLength, infoLog);

	if (infoLogLength > 0)
	{
		printf("printProgramLog: %s\n", infoLog);
	}

	destroy(infoLog);
}

program_t *programWithShaders(shader_t *vertexShader, shader_t *fragmentShader, shader_t *geometryShader)
{
	program_t *newProgram = newObject(program_t);
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
	newProgram->originLocation = glGetUniformLocation(newProgram->programID, "origin");
	newProgram->rotationLocation = glGetUniformLocation(newProgram->programID, "rotation");
	newProgram->viewMatLocation = glGetUniformLocation(newProgram->programID, "viewMatrix");
	newProgram->textureLocation = glGetUniformLocation(newProgram->programID, "tex");
	newProgram->colorLocation = glGetUniformLocation(newProgram->programID, "customColor");

	Map.init(&newProgram->customLocations);

	return newProgram;
}

void registerCustomUniformForProgram(program_t *program, const char *name)
{
	int *location = newObject(int);
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

void initTextureFunctions(_texture_functions *Texture)
{
	TextureInternal = Texture;

	Texture->fromPath = textureFromPath;
	Texture->update = updateTextureContents;
}

void initShaderFunctions(_shader_functions *Shader)
{
	ShaderInternal = Shader;

	Shader->fromContent = shaderFromContent;
	Shader->fromPath = shaderFromPath;
}

void initProgramFunctions(_program_functions *Program)
{
	ProgramInternal = Program;

	Program->getDefault = defaultProgram;
	Program->getCustomUniformLocation = getCustomUniformLocationForProgram;
	Program->withShaders = programWithShaders;
	Program->registerCustomUniform = registerCustomUniformForProgram;
}

#undef HASNOR_ENGINE_INTERNAL
