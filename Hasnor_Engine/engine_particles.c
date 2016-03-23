#define HASNOR_ENGINE_INTERNAL

#include "engine_particles.h"
#include "engine_camera.h"

#include "utils_vector.h"

#include <GL/glew.h>
#include <GL/glut.h>

program_t *_defaultParticleProgram;
program_t *_defaultFeedbackProgram;

const char *_particleVertexShader =  
	"in vec3 pos;																		\
	in vec2 texPos;																		\
	varying vec2 v_texCoord;															\
	uniform vec3 origin;																\
	uniform float particleAngle;														\
	uniform float scale;																\
	uniform mat4 viewMatrix;															\
	void main(void)																		\
	{																					\
		v_texCoord = texPos;																\
		gl_Position = vec4(origin, 1.0) * viewMatrix + vec4(pos, 0.0) * scale;			\
	}";

const char *_particleFragmentShader =  
	"uniform sampler2D tex;																\
	uniform vec4 customColor;															\
	varying vec2 v_texCoord;																\
	void main(void)																		\
	{																					\
		vec3 inColor = texture2D(tex, v_texCoord).rgb * customColor.rgb * customColor.a;	\
		float intensity = (inColor.r + inColor.g + inColor.b) / 3.0;					\
		gl_FragColor = vec4(inColor, intensity);										\
	}";

const char *_particleTransformShader =
	"layout(location = 0) in vec3 a_position;											\
	layout(location = 1) in vec3 a_velocity;											\
	layout(location = 2) in vec3 a_acceleration;										\
	uniform float u_deltaTime;															\
	out vec3 o_position;																\
	out vec3 o_velocity;																\
	void main(void)																		\
	{																					\
		o_position = a_position + a_velocity * u_deltaTime + a_acceleration * u_deltaTime * u_deltaTime * 0.5;\
		o_velocity = a_velocity + a_acceleration * u_deltaTime;							\
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
uint _particleCount;
list_t *_emitterList;

bool _pause;

void particles_InitRenderer(void)
{
	_defaultParticleProgram = GLProgram.withShaders(GLShader.fromContent(SHADER_VERTEX, _particleVertexShader),
													GLShader.fromContent(SHADER_FRAGMENT, _particleFragmentShader),
													NULL);
	
	GLProgram.registerCustomUniform(_defaultParticleProgram, "scale");

	glGenBuffers(1, &_defaultParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _defaultParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(float), _defaultParticleCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &_defaultParticleEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _defaultParticleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(byte), _defaultParticleElements, GL_STATIC_DRAW);

	_particleList = NULL;
	_particleCount = 0;
	_emitterList = NULL;
	_pause = false;
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

void particles_Render(const float viewMatrix[16])
{
	list_t **particleListIterator = &_particleList;

	// Disable writing to the depth buffer to avoid transparency issues
	glDepthMask(GL_FALSE);

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
		glUniform1f(GLProgram.getCustomUniformLocation(particle->model->program, "scale"), particle->currentData.scale);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

		particleListIterator = &(*particleListIterator)->next;
	}

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthMask(GL_TRUE);
}

particle_t *_newParticleFromEmitter(emitter_t *emitter, particleModel_t *model)
{
	particle_t *newParticle = newObject(particle_t);
	float axis[3][3];

	newParticle->model = model;
	newParticle->currentData = *model->startData;

	newParticle->transform = emitter->model->particleTransform;
	Vector.add(newParticle->transform.position, emitter->model->particleTransform.position, emitter->transform.position);

	Vector.axisFromAngles(emitter->transform.angles, axis[0], axis[1], axis[2]);
	Vector.copy(newParticle->transform.velocity, emitter->model->particleTransform.velocity);
	Vector.rotate(newParticle->transform.velocity, axis);
	Vector.add(newParticle->transform.velocity, newParticle->transform.velocity, emitter->transform.velocity);
	
	return newParticle;
}

void _sortMerge(list_t **list, uint count, uint middle)
{
	uint i0 = 0, i1 = middle;
	uint i;
	list_t **list0 = list;
	list_t **list1 = list;
	particle_t *particle0;
	particle_t *particle1;

	for (i = 0; i < middle; i++)
	{
		list1 = &(*list1)->next;
	}

	particle0 = (particle_t*)(*list0)->content;
	particle1 = (particle_t*)(*list1)->content;

	for (i = 0; i < count; i++)
	{
		if (i0 < middle &&
			(i1 >= count || particle0->squaredDistanceToCamera >= particle1->squaredDistanceToCamera))
		{
			if (list == list0)
			{
				list0 = &(*list0)->next;
			}
			else
			{
				list_t *curPos = *list;
				*list = *list0;
				*list0 = (*list0)->next;
				(*list)->next = curPos;
			}

			particle0 = *list0 ? (particle_t*)(*list0)->content : NULL;
			i0++;
		}
		else
		{
			if (list == list1)
			{
				list1 = &(*list1)->next;
			}
			else
			{
				list_t *curPos = *list;
				*list = *list1;
				*list1 = (*list1)->next;
				(*list)->next = curPos;
			}

			if (list0 == list)
			{ // We just offset the pointer he was targeting so move him
				list0 = &(*list0)->next;
				particle0 = *list0 ? (particle_t*)(*list0)->content : NULL;
			}
			particle1 = *list1 ? (particle_t*)(*list1)->content : NULL;
			i1++;
		}
		list = &(*list)->next;
	}
}

void _sortSplitMerge(list_t **list, uint count, uint startIndex) // startIndex is only used for debugging
{
	if (count > 1)
	{
		uint i;
		uint middle = count / 2;
		list_t **middleList;

		_sortSplitMerge(list, middle, startIndex);

		middleList = list;
		for (i = 0; i < middle; i++)
		{
			middleList = &(*middleList)->next;
		}
		_sortSplitMerge(middleList, count - middle, startIndex + middle);

		_sortMerge(list, count, middle);
	}
}

void _particlesMergeSort(list_t **list, uint count)
{
	_sortSplitMerge(list, count, 0);
}

void particles_Update(const timeStruct_t time)
{
	list_t **particleListIterator = &_particleList;
	list_t **emitterListIterator = &_emitterList;
	float camPos[3];

	CameraInternal->getPosition(camPos);

	if (_pause)
	{
		while (*particleListIterator)
		{
			particle_t *particle = (particle_t*)(*particleListIterator)->content;
			particle->squaredDistanceToCamera = Vector.squareDistance(particle->transform.position, camPos);
			particleListIterator = &(*particleListIterator)->next;
		}
	}
	else
	{
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

				Vector.multiplyAdd(particle->transform.position, particle->transform.position, time.deltaTimeSeconds, particle->transform.velocity);
				updateTransform(&particle->transform, time.deltaTimeSeconds, particle->model->useGravity);
				particle->squaredDistanceToCamera = Vector.squareDistance(particle->transform.position, camPos);
				particleListIterator = &(*particleListIterator)->next;
			}
			else
			{
				list_t *curItem = *particleListIterator;
				*particleListIterator = (*particleListIterator)->next;
				destroy(particle);
				destroy(curItem);
				_particleCount--;
			}
		}

		// Update emitters
		while (*emitterListIterator)
		{
			emitter_t *emitter = (emitter_t*)(*emitterListIterator)->content;

			updateTransform(&emitter->transform, time.deltaTimeSeconds, false);

			while (time.currentTime - emitter->lastSpawn >= emitter->spawnData.nextWave->delay)
			{ // Time to create a new particle
				array_t *particlesToSpawn = &emitter->spawnData.nextWave->models;
				uint i;

				for (i = 0; i < particlesToSpawn->size; i++)
				{
					particleModel_t *model = (particleModel_t*)particlesToSpawn->content[i];
					particle_t *newParticle = _newParticleFromEmitter(emitter, model);
					newParticle->spawnTime = time.currentTime;
					newParticle->squaredDistanceToCamera = Vector.squareDistance(emitter->transform.position, camPos);

					List.add(particleListIterator, newParticle); // Add it right there since this is the pointer to the last node
					particleListIterator = &(*particleListIterator)->next;
					_particleCount++;
				}

				emitter->lastSpawn = time.currentTime; // Consider using += emitter->spawnData.nextWave->delay instead to avoid delaying further waves
				emitter->spawnData.nextWave = emitter->spawnData.nextWave->next;
			}

			emitterListIterator = &(*emitterListIterator)->next;
		}
	}

	// Sort particles
	_particlesMergeSort(&_particleList, _particleCount);
}

particleData_t *_newParticleState()
{
	particleData_t *state = newObject(particleData_t);
	state->red = 1.0f;
	state->green = 1.0f;
	state->blue = 1.0f;
	state->alpha = 1.0f;
	state->scale = 1.0f;
	return state;
}

particleModel_t *particles_newParticleModel(texture_t *texture, long life, bool transition)
{
	particleModel_t *newModel = newObject(particleModel_t);
	newModel->texture = texture;
	newModel->program = _defaultParticleProgram;
	newModel->life = life;
	newModel->startData = _newParticleState();
	newModel->endData = transition ? _newParticleState() : NULL;
	newModel->useGravity = false;
	return newModel;
}

emitterModel_t *particles_newEmitterModel(void)
{
	emitterModel_t *newModel = newObject(emitterModel_t);
	Memory.set(&newModel->particleTransform, 0, sizeof(transform_t));
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
	*curWave = newObject(spawnWave_t);
	(*curWave)->delay = delay;
	(*curWave)->next = emitter->spawnDataModel.nextWave;
	Array.init(&(*curWave)->models);
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
	Array.add(&(*curWave)->models, model);
}

emitter_t *particles_CreateEmitterFromModel(emitterModel_t *model)
{
	emitter_t *newEmitter = newObject(emitter_t);
	newEmitter->model = model;
	newEmitter->spawnData = model->spawnDataModel;
	newEmitter->lastSpawn = 0;
	Memory.set(&newEmitter->transform, 0, sizeof(transform_t));
	
	List.add(&_emitterList, newEmitter);

	return newEmitter;
}

void particles_DeleteEmitter(emitter_t *emitter)
{
	List.remove(&_emitterList, emitter);
	destroy(emitter);
}

void particles_setPause(bool pause)
{
	_pause = pause;
}

void initParticleFunctions(void)
{
	GLParticles.newParticle = particles_newParticleModel;
	GLParticles.setPause = particles_setPause;

	GLParticleEmitter.addNewParticleToLastWave = particles_AddNewParticleToEmitter;
	GLParticleEmitter.createNewWave = particles_CreateNewWaveForEmitter;
	GLParticleEmitter.instantiate = particles_CreateEmitterFromModel;
	GLParticleEmitter.newEmitter = particles_newEmitterModel;
	GLParticleEmitter.remove = particles_DeleteEmitter;
}

#undef HASNOR_ENGINE_INTERNAL
