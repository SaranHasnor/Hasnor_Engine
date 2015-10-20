#ifndef HASNOR_WINDOW_IMPORTED
#define HASNOR_WINDOW_IMPORTED

/*
engine_window

- Window creation and management
*/

typedef struct {
	int	(*getWidth)(void);
	int	(*getHeight)(void);
} _window_functions;

#ifdef HASNOR_ENGINE_INTERNAL
void createWindow(const char *name, int width, int height);
#endif

#ifdef HASNOR_INIT
void initWindowFunctions(_window_functions *Window);
#endif

#endif