#include "interface_block.h"

typedef struct {
	block_t			content;

	// Events
	void			(*onEnter)(uint origin);
	void			(*onExit)(uint destination);
} sideMenu_t;

typedef struct {
	// Current view pos
	int				x, y;

	// Transition data
	int				transTime;
	int				targX, targY;

	int				width;

	// Menus
	sideMenu_t		*menus;
	uint			nbMenus;
} sidePanel_t;

void drawSidePanel(sidePanel_t menu);