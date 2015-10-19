#include <stdio.h>
#include <engine.h>
#include <engine_utils.h>
#include <network_server.h>

#include <engine_init.h>
#include <utils_init.h>

#include <utils_matrix.h>

void keyDownFunc(uchar key)
{
	if (key == 27)
	{
		Engine.shutdown();
	}
}

void keyUpFunc(uchar key)
{

}

void mouseDownFunc(ushort button, int x, int y)
{

}

void mouseUpFunc(ushort button, int x, int y)
{

}

void updateCamera(inputStruct_t input)
{
	float velocity[3] = {0,0,0};

	if (input.mouseButtons & INPUT_MOUSELEFT)
	{
		float angle[3];

		angle[0] = (float)input.mouseDelta[1];
		angle[1] = -(float)input.mouseDelta[0];
		angle[2] = 0;

		Engine.Camera.rotate(angle);
	}

	if (input.key_timeHeld('z') > 0)
	{
		velocity[1] += 1.0f;
	}

	if (input.key_timeHeld('s') > 0)
	{
		velocity[1] -= 1.0f;
	}

	if (input.key_timeHeld('d') > 0)
	{
		velocity[0] += 1.0f;
	}

	if (input.key_timeHeld('q') > 0)
	{
		velocity[0] -= 1.0f;
	}

	if (input.key_timeHeld(' ') > 0)
	{
		velocity[2] += 1.0f;
	}

	if (input.key_timeHeld('x') > 0)
	{
		velocity[2] -= 1.0f;
	}

	Engine.Camera.setVelocity(velocity);
}

extern void initSampleInterface();
extern void initSampleParticles();
extern void initSampleMesh();
extern void updateSampleMesh(timeStruct_t time, inputStruct_t input);
void updateFunc(timeStruct_t time, inputStruct_t input)
{
	if (time.currentTime == time.deltaTime)
	{
		initSampleInterface();

		initSampleParticles();

		initSampleMesh();
	}

	updateSampleMesh(time, input);

	updateCamera(input);

	particles_Update(time);
}

extern void drawSampleMesh(float viewMatrix[16]);
void renderFunc(void)
{
	float viewMatrix[16];

	drawAxis();

	Engine.Camera.getViewMatrix(viewMatrix);

	drawSampleMesh(viewMatrix);

	particles_Render(viewMatrix);
}

int main(int argc, char **argv)
{
	engineListener_t listener;

	listener.keyDownFunc = keyDownFunc;
	listener.keyUpFunc = keyUpFunc;
	listener.mouseDownFunc = mouseDownFunc;
	listener.mouseUpFunc = mouseUpFunc;
	listener.renderFunc = renderFunc;
	listener.updateFunc = updateFunc;

	initHasnorUtils();
	initHasnorEngine();

	setupNetwork(3000, 10000);

	Engine.run(argc, argv, 1200, 600, "Test", listener);

	return 0;
}