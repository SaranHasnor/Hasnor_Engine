#define HASNOR_ENGINE_INTERNAL

#include "engine_window.h"
#include <GL/glut.h>
#include <utils.h>
#include "engine_interface.h"
#include "engine_camera.h"

typedef struct {
	int			id;
	const char	*name;
	int			width, height;
} window_t;

window_t _window;

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
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	setCameraSize((float)w, (float)h);
	interface_reshapeWindow(w, h);
}

void createWindow(const char *name, int width, int height)
{
	const int screenX = glutGet(GLUT_SCREEN_WIDTH);
	const int screenY = glutGet(GLUT_SCREEN_HEIGHT);

	_window.width = width;
	_window.height = height;
	_window.name = name;

	glutInitWindowSize(width, height);
	glutInitWindowPosition((screenX-width)/2, (screenY-height)/2);
	_window.id = glutCreateWindow(name);

	initRendering();

	initCamera();
	setCameraSize((float)width, (float)height);

	glutReshapeFunc(reshape);
}

int getWindowWidth()
{
	return _window.width;
}

int getWindowHeight()
{
	return _window.height;
}

void initWindowFunctions(_window_functions *Window)
{
	Window->getWidth = getWindowWidth;
	Window->getHeight = getWindowHeight;
}

#undef HASNOR_ENGINE_INTERNAL
