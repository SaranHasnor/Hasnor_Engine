#include "interface_block.h"

void updateBlockLayout(block_t *block)
{
	uint i;

	for (i = 0; i < block->nbComponents; i++)
	{
		component_t *childComponent = block->components[i];

		interface_updateAbsolutePosition(&childComponent->position, block->position.screenPos);
	}

	for (i = 0; i < block->nbChildren; i++)
	{
		block_t *childBlock = block->children[i];

		interface_updateAbsolutePosition(&childBlock->position, block->position.screenPos);

		updateBlockLayout(childBlock);
	}
}

void drawBlock(const block_t *block)
{
	uint i;
	
	for (i = 0; i < block->nbComponents; i++)
	{
		drawObject(block->components[i]);
	}

	for (i = 0; i < block->nbChildren; i++)
	{
		drawBlock(block->children[i]);
	}
}