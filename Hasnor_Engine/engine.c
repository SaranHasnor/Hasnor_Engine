#include "engine.h"
#include "engine_window.h"
#include "engine_input.h"
#include "engine_interface.h"
#include "engine_camera.h"

#include <utils.h>

#include <GL\glew.h>
#include <GL\glut.h>

window_t window;
engineListener_t _listener;

void update(int prevTime)
{
	const int curTime = glutGet(GLUT_ELAPSED_TIME);
	timeStruct_t time;
	inputStruct_t input;

	time.currentTime = curTime;
	time.deltaTime = curTime-prevTime;
	time.deltaTimeSeconds = time.deltaTime / 1000.0f;

	//printf("FPS: %f\n", 1.0f / time.deltaTimeSeconds);

	interface_update(time.deltaTimeSeconds);

	updateInput(&input);
	
	glutSetWindow(window.id);
	updateGLCamera(time, input);
	_listener.updateFunc(time, input);

	glutTimerFunc(10, update, curTime);
	glutPostRedisplay();
}

void display(void)
{
	// Frame initialization
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);

	// Rendering
	positionGLCameraForRender();
	_listener.renderFunc();

	// Interface
	positionGLCameraForInterface();
	interface_render();

	// Frame saving and rendering
	glutSwapBuffers();
	glFlush();
}

void engine_run(int argc, char **argv, int windowWidth, int windowHeight, char *windowName, engineListener_t listener)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	window.width = windowWidth;
	window.height = windowHeight;
	window.name = quickString(windowName);

	createWindow(&window);

	glewInit();

	interface_init();

	glutDisplayFunc(display);
	
	initInput(listener);

	_listener = listener;

	update(0);

	glutMainLoop();
}

void engine_shutdown()
{
	// Force the engine to shutdown
	quit(); // temp
}