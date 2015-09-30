#include "engine_particles.h"
#include "engine_render.h"

#include <GL/glew.h>
#include <GL/glut.h>

program_t *_defaultParticleProgram;

const char *_particleVertexShader =  
	"in vec3 pos;																		\
	in vec2 texPos;																		\
	varying vec2 texCoord;																\
	uniform vec3 origin;																\
	uniform float particleAngle;														\
	uniform float scale;																\
	uniform mat4 viewMatrix;															\
	void main(void)																		\
	{																					\
		texCoord = texPos;																\
		gl_Position = vec4(origin, 1.0) * viewMatrix + vec4(pos, 1.0) * scale;			\
	}";

const char *_particleFragmentShader =  
	"uniform sampler2D tex;																\
	uniform vec4 customColor;															\
	varying vec2 texCoord;																\
	void main(void)																		\
	{																					\
		vec3 inColor = texture2D(tex, texCoord).rgb * customColor.rgb * customColor.a;	\
		float intensity = (inColor.r + inColor.g + inColor.b) / 3.0;					\
		gl_FragColor = vec4(inColor, intensity);										\
	}";

static const float _defaultParticleCoords[] = {
	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f
};

static const byte _defaultParticleElements[] = {
	0, 1, 2,
	0, 2, 3
};

static GLuint _defaultParticleVBO, _defaultParticleEBO;

list_t *_particleList;
list_t *_emitterList;

void particles_InitRenderer()
{
	_defaultParticleProgram = programWithShaders(shaderFromContent(SHADER_VERTEX, _particleVertexShader),
												 shaderFromContent(SHADER_FRAGMENT, _particleFragmentShader));
	
	registerCustomUniformForProgram(_defaultParticleProgram, "scale");

	glGenBuffers(1, &_defaultParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _defaultParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(float), _defaultParticleCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &_defaultParticleEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _defaultParticleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(byte), _defaultParticleElements, GL_STATIC_DRAW);

	_particleList = NULL;
	_emitterList = NULL;
}

void _lerpParticleData(particleData_t *from, particleData_t *to, float percentage, particleData_t *out)
{
	float fromPercentage = 1.0f - percentage;

	out->red = from->red * fromPercentage + to->red * percentage;
	out->green = from->green * fromPercentage + to->green * percentage;
	out->blue = from->blue * fromPercentage + to->blue * percentage;
	out->alpha = from->alpha * fromPercentage + to->alpha * percentage;

	out->scale = from->scale * fromPercentage + to->scale * percentage;
}

void particles_Render(float viewMatrix[16])
{
	list_t **particleListIterator = &_particleList;

	while (*particleListIterator)
	{
		particle_t *particle = (particle_t*)(*particleListIterator)->content;

		glUseProgram(particle->model->program->programID);

		glActiveTexture(GL_TEXTURE0 + particle->model->texture->textureID);
		glBindTexture(GL_TEXTURE_2D, particle->model->texture->textureID);
		glUniform1i(particle->model->program->textureLocation, particle->model->texture->textureID);

		glBindBuffer(GL_ARRAY_BUFFER, _defaultParticleVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _defaultParticleEBO);

		glVertexAttribPointer(particle->model->program->coordsLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
		glEnableVertexAttribArray(particle->model->program->coordsLocation);
		glVertexAttribPointer(particle->model->program->texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(particle->model->program->texCoordsLocation);

		glUniform3fv(particle->model->program->originLocation, 1, particle->transform.position);
		//glUniform1f(particle->model->program->angleLocation, 0.0f); // TODO

		glUniformMatrix4fv(particle->model->program->viewMatLocation, 1, GL_TRUE, viewMatrix);

		glUniform4f(particle->model->program->colorLocation,
			particle->currentData.red, particle->currentData.green, particle->currentData.blue, particle->currentData.alpha);
		glUniform1f(getCustomUniformLocationForProgram(particle->model->program, "scale"), particle->currentData.scale);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

		particleListIterator = &(*particleListIterator)->next;
	}

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

particle_t *_newParticleFromEmitter(emitter_t *emitter, particleModel_t *model)
{
	particle_t *newParticle = (particle_t*)mem_alloc(sizeof(particle_t));
	float axis[3][3];

	newParticle->model = model;
	newParticle->currentData = *model->startData;

	newParticle->transform = emitter->model->particleTransform;
	vectorAdd(newParticle->transform.position, emitter->model->particleTransform.position, emitter->transform.position);

	AngleVectors(emitter->transform.angles, axis[0], axis[1], axis[2]);
	vectorCopy(newParticle->transform.velocity, emitter->model->particleTransform.velocity);
	vectorRotate(newParticle->transform.velocity, axis);
	vectorAdd(newParticle->transform.velocity, newParticle->transform.velocity, emitter->transform.velocity);
	
	return newParticle;
}

void particles_Update(timeStruct_t time)
{
	list_t **particleListIterator = &_particleList;
	list_t **emitterListIterator = &_emitterList;

	// Update particles
	while (*particleListIterator)
	{
		particle_t *particle = (particle_t*)(*particleListIterator)->content;

		if (time.currentTime < particle->spawnTime + particle->model->life)
		{
			if (particle->model->startData && particle->model->endData)
			{
				_lerpParticleData(particle->model->startData, particle->model->endData, (time.currentTime - particle->spawnTime) / (float)particle->model->life, &particle->currentData);
			}

			vectorMA(particle->transform.position, particle->transform.position, time.deltaTimeSeconds, particle->transform.velocity);
			updateTransform(&particle->transform, time.deltaTimeSeconds, particle->model->useGravity);
			particleListIterator = &(*particleListIterator)->next;
		}
		else
		{
			list_t *curItem = *particleListIterator;
			*particleListIterator = (*particleListIterator)->next;
			mem_free(particle);
			mem_free(curItem);
		}
	}

	// Update emitters
	while (*emitterListIterator)
	{
		emitter_t *emitter = (emitter_t*)(*emitterListIterator)->content;

		updateTransform(&emitter->transform, time.deltaTimeSeconds, false);

		if (time.currentTime - emitter->lastSpawn >= emitter->spawnData.nextWave->delay)
		{ // Time to create a new particle
			array_t *particlesToSpawn = &emitter->spawnData.nextWave->models;
			uint i;

			for (i = 0; i < particlesToSpawn->size; i++)
			{
				particleModel_t *model = (particleModel_t*)particlesToSpawn->content[i];
				particle_t *newParticle = _newParticleFromEmitter(emitter, model);
				newParticle->spawnTime = time.currentTime;

				*particleListIterator = list_new(newParticle); // Add it right there since we have the pointer to the last node
				particleListIterator = &(*particleListIterator)->next;
			}

			emitter->lastSpawn = time.currentTime; // Consider using += emitter->spawnData.nextWave->delay instead to avoid delaying further waves
			emitter->spawnData.nextWave = emitter->spawnData.nextWave->next;
		}

		emitterListIterator = &(*emitterListIterator)->next;
	}
}

particleModel_t *particles_newParticleModel(texture_t *texture, float r, float g, float b, float a, float scale, long life, bool useGravity)
{
	particleModel_t *newModel = (particleModel_t*)mem_alloc(sizeof(particleModel_t));
	newModel->texture = texture;
	newModel->program = _defaultParticleProgram;
	newModel->startData = (particleData_t*)mem_alloc(sizeof(particleData_t));
	newModel->startData->red = r;
	newModel->startData->green = g;
	newModel->startData->blue = b;
	newModel->startData->alpha = a;
	newModel->startData->scale = scale;
	newModel->endData = NULL;
	newModel->life = life;
	newModel->useGravity = useGravity;
	return newModel;
}

void particles_addFinalStateToParticleModel(particleModel_t *model, float r, float g, float b, float a, float scale)
{
	model->endData = (particleData_t*)mem_alloc(sizeof(particleData_t));
	model->endData->red = r;
	model->endData->green = g;
	model->endData->blue = b;
	model->endData->alpha = a;
	model->endData->scale = scale;
}

emitterModel_t *particles_newEmitterModel()
{
	emitterModel_t *newModel = (emitterModel_t*)mem_alloc(sizeof(emitterModel_t));
	mem_set(&newModel->particleTransform, 0, sizeof(transform_t));
	newModel->spawnDataModel.nextWave = NULL;
	return newModel;
}

void particles_CreateNewWaveForEmitter(emitterModel_t *emitter, long delay)
{
	spawnWave_t **curWave = &emitter->spawnDataModel.nextWave;
	while (*curWave)
	{
		curWave = &(*curWave)->next;
		if (*curWave == emitter->spawnDataModel.nextWave)
		{
			break;
		}
	}
	*curWave = (spawnWave_t*)mem_alloc(sizeof(spawnWave_t));
	(*curWave)->delay = delay;
	(*curWave)->next = emitter->spawnDataModel.nextWave;
	array_init(&(*curWave)->models);
}

void particles_AddNewParticleToEmitter(emitterModel_t *emitter, particleModel_t *model)
{
	spawnWave_t **curWave = &emitter->spawnDataModel.nextWave;
	while (*curWave)
	{
		curWave = &(*curWave)->next;
		if (*curWave == emitter->spawnDataModel.nextWave)
		{
			break;
		}
	}
	array_add(&(*curWave)->models, model);
}

emitter_t *particles_CreateEmitterFromModel(emitterModel_t *model)
{
	emitter_t *newEmitter = (emitter_t*)mem_alloc(sizeof(emitter_t));
	newEmitter->model = model;
	newEmitter->spawnData = model->spawnDataModel;
	newEmitter->lastSpawn = 0;
	mem_set(&newEmitter->transform, 0, sizeof(transform_t));
	
	if (_emitterList)
	{
		list_add(_emitterList, newEmitter);
	}
	else
	{
		_emitterList = list_new(newEmitter);
	}

	return newEmitter;
}