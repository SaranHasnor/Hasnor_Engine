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
	particleModel = GLParticles.newParticle(texture, 1000, true);
	particleModel->startData->green = 0.0f;
	particleModel->startData->scale = 3.0f;
	particleModel->endData->red = 0.0f;
	particleModel->endData->alpha = 0.0f;
	particleModel->endData->scale = 0.0f;
	particleModel->useGravity = true;
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
	particleModel = GLParticles.newParticle(texture, 500, false);
	particleModel->startData->red = 0.0f;
	particleModel->startData->green = 0.0f;
	particleModel->startData->scale = 6.0f;
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