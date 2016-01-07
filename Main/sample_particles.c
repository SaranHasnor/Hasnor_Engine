#include <engine.h>
#include <utils_vector.h>
#include <utils_math.h>

void initSampleParticles(void)
{
	texture_t *texture = GLTexture.fromPath("textures/flare_white.jpg");
	emitterModel_t *emitterModel;
	particleModel_t *particleModel;
	emitter_t *emitter;
	int i;

	emitterModel = GLParticleEmitter.newEmitter();
	emitterModel->particleTransform.velocity[0] = 5.0f;
	GLParticleEmitter.createNewWave(emitterModel, 10);
	particleModel = GLParticles.newParticle(texture, 1.0f, 0.0f, 1.0f, 1.0f, 3.0f, 1000, true);
	GLParticles.setParticleTransition(particleModel, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	GLParticleEmitter.addNewParticleToLastWave(emitterModel, particleModel);
	
	for (i = 0; i < 20; i++)
	{
		emitter = GLParticleEmitter.instantiate(emitterModel);

		Vector.set(emitter->transform.position, 10.0f, 0.0f, 0.0f);

		Vector.set(emitter->transform.rotation,
			Math.randomInterval(-100.0f, 100.0f),
			Math.randomInterval(-100.0f, 100.0f),
			Math.randomInterval(-100.0f, 100.0f));
	}

	emitterModel = GLParticleEmitter.newEmitter();
	emitterModel->particleTransform.velocity[0] = 1.0f;
	GLParticleEmitter.createNewWave(emitterModel, 50);
	particleModel = GLParticles.newParticle(texture, 0.0f, 0.0f, 1.0f, 1.0f, 6.0f, 500, false);
	GLParticleEmitter.addNewParticleToLastWave(emitterModel, particleModel);

	for (i = 0; i < 5; i++)
	{
		emitter = GLParticleEmitter.instantiate(emitterModel);

		Vector.set(emitter->transform.position, 10.0f, 0.0f, 0.0f);

		Vector.set(emitter->transform.rotation,
			Math.randomInterval(-100.0f, 100.0f),
			Math.randomInterval(-100.0f, 100.0f),
			Math.randomInterval(-100.0f, 100.0f));
	}
}