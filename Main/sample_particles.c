
#include <engine_particles.h>
#include <utils_vector.h>

void initSampleParticles()
{
	texture_t *texture = textureFromPath("textures/flare_white.jpg");
	emitterModel_t *emitterModel;
	particleModel_t *particleModel;
	emitter_t *emitter;
	int i;
	
	particles_InitRenderer();

	emitterModel = particles_newEmitterModel();
	emitterModel->particleTransform.velocity[0] = 5.0f;
	particles_CreateNewWaveForEmitter(emitterModel, 10);
	particleModel = particles_newParticleModel(texture, 1.0f, 0.0f, 1.0f, 1.0f, 3.0f, 1000, true);
	particles_addFinalStateToParticleModel(particleModel, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	particles_AddNewParticleToEmitter(emitterModel, particleModel);
	
	for (i = 0; i < 20; i++)
	{
		emitter = particles_CreateEmitterFromModel(emitterModel);

		Vector.set(emitter->transform.position, 10.0f, 0.0f, 0.0f);

		Vector.set(emitter->transform.rotation,
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f));
	}

	emitterModel = particles_newEmitterModel();
	emitterModel->particleTransform.velocity[0] = 1.0f;
	particles_CreateNewWaveForEmitter(emitterModel, 50);
	particleModel = particles_newParticleModel(texture, 0.0f, 0.0f, 1.0f, 1.0f, 6.0f, 500, false);
	particles_AddNewParticleToEmitter(emitterModel, particleModel);

	for (i = 0; i < 5; i++)
	{
		emitter = particles_CreateEmitterFromModel(emitterModel);

		Vector.set(emitter->transform.position, 10.0f, 0.0f, 0.0f);

		Vector.set(emitter->transform.rotation,
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f));
	}
}