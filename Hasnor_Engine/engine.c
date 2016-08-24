#define HASNOR_ENGINE_INTERNAL

#include "engine.h"
#include "engine_window.h"
#include "engine_input.h"
#include "engine_interface.h"
#include "engine_camera.h"
#include "engine_particles.h"

#include <utils.h>
#include <utils_program.h>

#include <GL\glew.h>
#include <GL\glut.h>

engineListener_t _listener;

void update(int prevTime)
{
	const int curTime = glutGet(GLUT_ELAPSED_TIME);
	timeStruct_t time;
	inputStruct_t input;

	time.currentTime = curTime;
	time.deltaTime = curTime-prevTime;
	time.deltaTimeSeconds = time.deltaTime / 1000.0f;

	//Console.print("FPS: %f\n", 1.0f / time.deltaTimeSeconds);

	interface_update(time.deltaTimeSeconds);

	updateInput(&input);
	
	updateGLCamera(time, input);
	_listener.updateFunc(time, input);
	particles_Update(time);

	glutTimerFunc(10, update, curTime);
	glutPostRedisplay();
}

void display(void)
{
	float viewMatrix[16];

	// Frame initialization
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Rendering
	positionGLCameraForRender();
	CameraInternal->getViewMatrix(viewMatrix);
	particles_Render(viewMatrix);
	_listener.renderFunc(viewMatrix);

	// Interface
	glDepthMask(GL_FALSE);
	positionGLCameraForInterface();
	interface_render();

	// Frame saving and rendering
	glutSwapBuffers();
	glFlush();
}

void engine_run(int argc, char **argv, int windowWidth, int windowHeight, const char *windowName, engineListener_t listener)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	createWindow(windowName, windowWidth, windowHeight);

	glewInit();

	interface_init();

	particles_InitRenderer();

	glutDisplayFunc(display);
	
	initInput(listener);

	_listener = listener;

	_listener.initFunc();

	update(0);

	glutMainLoop();
}

void engine_shutdown(void)
{ // TODO: Actually stop the loop
	Program.close();
}

void initEngineFunctions(void)
{
	Engine.run = engine_run;
	Engine.shutdown = engine_shutdown;
}

#undef HASNOR_ENGINE_INTERNAL
