#include "interface_component.h"

typedef struct block_s {
	position_t		position;

	struct block_s	**children;
	uint			nbChildren;

	component_t		**components;
	uint			nbComponents;
} block_t;

void updateBlockLayout(block_t *block);
void drawBlock(const block_t *block);