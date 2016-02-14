#ifndef HASNOR_PARTICLES_IMPORTED
#define HASNOR_PARTICLES_IMPORTED

#include "engine_texture.h"
#include "engine_physics.h"
#include "engine_callbacks.h"
#include <utils_array.h>
#include <utils_list.h>

typedef struct {
	float				red, green, blue, alpha;
	float				scale;
} particleData_t;

typedef struct {
	long				life;
	bool				useGravity;

	texture_t			*texture;
	program_t			*program;

	particleData_t		*startData;
	particleData_t		*endData;
} particleModel_t;

typedef struct {
	transform_t			transform;
	float				squaredDistanceToCamera;
	particleData_t		currentData;

	long				spawnTime;
	particleModel_t		*model;
} particle_t;

typedef struct spawnWave_s {
	array_t				models;			// List of particle models to create on this wave
	struct spawnWave_s	*next;
	long				delay;
} spawnWave_t;

typedef struct {
	spawnWave_t			*nextWave;
} emitterSpawnData_t;

typedef struct {
	transform_t			particleTransform;
	emitterSpawnData_t	spawnDataModel;
} emitterModel_t;

typedef struct {
	emitterModel_t		*model;

	transform_t			transform;
	emitterSpawnData_t	spawnData;
	long				lastSpawn;
} emitter_t;

typedef struct {
	particleModel_t*	(*newParticle)(texture_t *texture, float r, float g, float b, float a, float scale, long life, bool useGravity);
	void				(*setParticleTransition)(particleModel_t *model, float r, float g, float b, float a, float scale);

	void				(*setPause)(bool pause);
} _particle_functions;

_particle_functions GLParticles;

typedef struct {
	emitterModel_t*		(*newEmitter)(void);
	emitter_t*			(*instantiate)(emitterModel_t *model);
	void				(*remove)(emitter_t *emitter);

	void				(*createNewWave)(emitterModel_t *emitter, long delay);
	void				(*addNewParticleToLastWave)(emitterModel_t *emitter, particleModel_t *model);
} _emitter_functions;

_emitter_functions GLParticleEmitter;

#ifdef HASNOR_INIT
void initParticleFunctions(void);
#endif

#ifdef HASNOR_ENGINE_INTERNAL
void particles_InitRenderer(void);
void particles_Render(const float viewMatrix[16]);
void particles_Update(const timeStruct_t time);
#endif

#endif
