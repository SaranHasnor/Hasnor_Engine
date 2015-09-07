
/*
engine_window

- Window creation and management
*/

typedef struct {
	int		id;
	char	*name;
	int		width, height;
} window_t;

void createWindow(window_t *window);