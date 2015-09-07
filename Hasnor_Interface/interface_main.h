#ifndef INTERFACE_MAIN_IMPORTED
#define INTERFACE_MAIN_IMPORTED

#include "interface_sidepanel.h"

typedef struct {
	block_t			mainBlock;

	sidePanel_t		sidePanel;
	bool			sidePanelVisible;

	radioGroup_t	*radioGroups;
	uint			nbRadioGroups;

	component_t		**allComponents;			// List of all existing objects sorted by ID for quick access
	uint			nbComponents;				// Note that a destroyed object is merely hidden and its memory is freed, but the empty space in this array remains
} interface_t;

interface_t _interface;			// Main container

typedef struct blockList_s {
	block_t				*block;
	struct blockList_s	*next;
} blockList_t;

blockList_t *_curBlockStack;	// FILO stack of blocks for the interface that we're currently creating

component_t *_curObject;	// Object selected by the user

void *clipboard;
char **renameString;
uint renameStringCursor;

#endif