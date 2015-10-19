
/*
engine_window

- Window creation and management
*/

typedef struct {
	int		id;
	char	*name;
	int		width, height;
} window_t;

#ifdef HASNOR_ENGINE_INTERNAL
void createWindow(window_t *window);
#endif