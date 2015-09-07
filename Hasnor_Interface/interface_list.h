#include "interface_utils.h"

typedef struct {
	char			*value;
	char			**dynValue;
} listEntry_t;

typedef struct {
	listEntry_t		*entries;
	uint			nbEntries;

	uint			selectIndex;
	uint			startIndex;				// Index of the first entry to display (for scrolling)
	uint			maxDisplayedEntries;	// Helper variable, counts how many entries can be fit in the list at once

	anchor_t		textAlignment;

	bool			selected;

	// Events
	void			(*onSelect)(uint index);
} list_t;

list_t *newList();
void clearList(list_t *list);
void destroyList(list_t *list);

void listSelect(list_t *list, uint index);

void drawList(list_t *list, staticPlacement_t placement);