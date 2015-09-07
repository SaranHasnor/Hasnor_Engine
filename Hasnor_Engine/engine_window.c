#include "engine_window.h"
#include <GL/glut.h>
#include <utils.h>
#include "engine_interface.h"
#include "engine_camera.h"

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glPointSize(1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	setCameraSize(w, h);
	interface_reshapeWindow(w, h);
}

void createWindow(window_t *window)
{
	const int screenX = glutGet(GLUT_SCREEN_WIDTH);
	const int screenY = glutGet(GLUT_SCREEN_HEIGHT);

	// Initializing GLUT
	glutInitWindowSize(window->width, window->height);
	glutInitWindowPosition((screenX-window->width)/2, (screenY-window->height)/2);
	window->id = glutCreateWindow(window->name);

	initRendering();

	initCamera();
	setCameraSize(window->width, window->height);

	glutReshapeFunc(reshape);
}