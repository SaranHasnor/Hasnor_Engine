#define HASNOR_ENGINE_INTERNAL

#include "engine_texture.h"

#include <utils_string.h>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

void initTextureFunctions(void)
{
	GLTexture.fromPath = textureFromPath;
	GLTexture.update = updateTextureContents;
}

#undef HASNOR_ENGINE_INTERNAL
