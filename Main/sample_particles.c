
#include <engine.h>
#include <utils_vector.h>

void initSampleParticles(void)
{
	texture_t *texture = Engine.Render.Texture.fromPath("textures/flare_white.jpg");
	emitterModel_t *emitterModel;
	particleModel_t *particleModel;
	emitter_t *emitter;
	int i;
	
	particles_InitRenderer();

	emitterModel = Engine.Render.Particles.newEmitter();
	emitterModel->particleTransform.velocity[0] = 5.0f;
	Engine.Render.Particles.createNewWaveForEmitter(emitterModel, 10);
	particleModel = Engine.Render.Particles.newParticle(texture, 1.0f, 0.0f, 1.0f, 1.0f, 3.0f, 1000, true);
	Engine.Render.Particles.setParticleTransition(particleModel, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	Engine.Render.Particles.addNewParticleToEmitter(emitterModel, particleModel);
	
	for (i = 0; i < 20; i++)
	{
		emitter = Engine.Render.Particles.instantiateEmitter(emitterModel);

		Vector.set(emitter->transform.position, 10.0f, 0.0f, 0.0f);

		Vector.set(emitter->transform.rotation,
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f));
	}

	emitterModel = Engine.Render.Particles.newEmitter();
	emitterModel->particleTransform.velocity[0] = 1.0f;
	Engine.Render.Particles.createNewWaveForEmitter(emitterModel, 50);
	particleModel = Engine.Render.Particles.newParticle(texture, 0.0f, 0.0f, 1.0f, 1.0f, 6.0f, 500, false);
	Engine.Render.Particles.addNewParticleToEmitter(emitterModel, particleModel);

	for (i = 0; i < 5; i++)
	{
		emitter = Engine.Render.Particles.instantiateEmitter(emitterModel);

		Vector.set(emitter->transform.position, 10.0f, 0.0f, 0.0f);

		Vector.set(emitter->transform.rotation,
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f),
			randomValueBetween(-100.0f, 100.0f));
	}
}