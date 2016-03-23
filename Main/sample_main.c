#include <stdio.h>
#include <engine.h>
#include <engine_utils.h>
#include <network_server.h>

#include <engine_init.h>
#include <utils_init.h>

#include <utils_matrix.h>
#include <utils_quaternion.h>
#include <utils_math.h>

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

void debugQuaternions(void)
{
	float quat[4];
	float mat[16];
	float pitch = Math.deg2rad(45.0f),
		yaw = Math.deg2rad(75.0f),
		roll = Math.deg2rad(10.0f);

	Matrix.rotation(mat, pitch, yaw, roll);

	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
		mat[0], mat[4], mat[8],  mat[12],
		mat[1], mat[5], mat[9],  mat[13],
		mat[2], mat[6], mat[10], mat[14],
		mat[3], mat[7], mat[11], mat[15]);

	Quaternion.fromMatrix(quat, mat);

	printf("%f %f %f %f\n\n",
		quat[0], quat[1], quat[2], quat[3]);

	Matrix.fromQuaternion(mat, quat);

	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
		mat[0], mat[4], mat[8],  mat[12],
		mat[1], mat[5], mat[9],  mat[13],
		mat[2], mat[6], mat[10], mat[14],
		mat[3], mat[7], mat[11], mat[15]);

	printf("===================================================\n\n");

	Quaternion.fromEuler(quat, pitch, yaw, roll);
	printf("%f %f %f %f\n\n",
		quat[0], quat[1], quat[2], quat[3]);

	Matrix.fromQuaternion(mat, quat);

	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
		mat[0], mat[4], mat[8],  mat[12],
		mat[1], mat[5], mat[9],  mat[13],
		mat[2], mat[6], mat[10], mat[14],
		mat[3], mat[7], mat[11], mat[15]);

	Quaternion.fromMatrix(quat, mat);

	printf("%f %f %f %f\n\n",
		quat[0], quat[1], quat[2], quat[3]);
}

extern void initSampleInterface(void);
extern void initSampleParticles(void);
extern void initSampleMesh(void);
void initFunc(void)
{
	initSampleInterface();

	initSampleParticles();

	initSampleMesh();
}

extern void updateSampleMesh(const timeStruct_t time, const inputStruct_t input);
void updateFunc(const timeStruct_t time, const inputStruct_t input)
{
	updateSampleMesh(time, input);

	updateCamera(input);
}

extern void drawSampleMesh(const float viewMatrix[16]);
void renderFunc(const float viewMatrix[16])
{
	drawAxis();

	drawSampleMesh(viewMatrix);
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
	listener.initFunc = initFunc;

	initHasnorUtils();
	initHasnorEngine();

	setupNetwork(3000, 10000);

	Engine.run(argc, argv, 1200, 600, "Test", listener);

	return 0;
}
