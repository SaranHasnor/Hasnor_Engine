#include "interface_checkbox.h"
#include "interface_render.h"

checkBox_t *newCheckBox()
{
	checkBox_t *checkbox = (checkBox_t*)mem_alloc(sizeof(checkBox_t));
	mem_set(checkbox , 0, sizeof(checkBox_t));
	return checkbox;
}

void destroyCheckBox(checkBox_t *checkbox)
{
	mem_free(checkbox);
}

void drawCheckBox(checkBox_t *checkBox, staticPlacement_t placement)
{
	// Border
	glColor3f(1.0f,1.0f,1.0f);
	drawRect(placement);

	if (checkBox->value)
	{ // Check sign
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex2i(placement.x, placement.y);
		glVertex2i(placement.x+placement.w, placement.y+placement.h);

		glVertex2i(placement.x+placement.w, placement.y);
		glVertex2i(placement.x, placement.y+placement.h);
		glEnd();
	}
}