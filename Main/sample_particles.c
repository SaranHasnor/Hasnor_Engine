
#include <engine_particles.h>

void initSampleParticles()
{
	texture_t *texture = textureFromPath("textures/flare_white.jpg");
	emitterModel_t *emitterModel;
	particleModel_t *particleModel;
	emitter_t *emitter;
	
	particles_InitRenderer();

	emitterModel = particles_newEmitterModel();
	emitterModel->particleTransform.velocity[0] = 5.0f;
	particles_CreateNewWaveForEmitter(emitterModel, 10);
	particleModel = particles_newParticleModel(texture, 1.0f, 1.0f, 1.0f, 1.0f, 1000, true);
	particles_addFinalStateToParticleModel(particleModel, 1.0f, 1.0f, 1.0f, 0.0f);
	particles_AddNewParticleToEmitter(emitterModel, particleModel);
	
	emitter = particles_CreateEmitterFromModel(emitterModel);
	emitter->transform.rotation[0] = randomValueBetween(-100.0f, 100.0f);
	emitter->transform.rotation[1] = randomValueBetween(-100.0f, 100.0f);
	emitter->transform.rotation[2] = randomValueBetween(-100.0f, 100.0f);
}