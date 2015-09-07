#include <stdio.h>
#include <engine.h>
#include <engine_utils.h>
#include <engine_render.h>
#include <network_server.h>

#include <utils_matrix.h>

mesh_t *_testMesh;

void keyDownFunc(uchar key)
{
	if (key == 27)
	{
		engine_shutdown();
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

		engine_rotateCamera(angle);
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

	engine_setCameraVelocity(velocity);
}

extern void createInterface();
void updateFunc(timeStruct_t time, inputStruct_t input)
{
	if (time.currentTime == time.deltaTime)
	{
		createInterface();

		_testMesh = newMesh();
		addFace();
		addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
		addFace();
		addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		addVertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		updateMeshGeometry(_testMesh);
	}

	_testMesh->origin[2] += 0.1f * time.deltaTimeSeconds;
	mat_rotation(_testMesh->rotation, _testMesh->origin[2], 0.0f, 0.0f, false);

	updateCamera(input);
}

void renderFunc(void)
{
	float viewMatrix[16];

	drawAxis();

	engine_getViewMatrix(viewMatrix);

	renderMesh(_testMesh, viewMatrix);
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

	setupNetwork(3000, 10000);

	engine_run(argc, argv, 1200, 600, "Test", listener);

	return 0;
}