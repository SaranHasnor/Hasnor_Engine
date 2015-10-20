#include "interface_label.h"
#include "interface_render.h"

label_t *newLabel(void)
{
	label_t *label = newObject(label_t);
	Memory.set(label, 0, sizeof(label_t));
	return label;
}

void destroyLabel(label_t *label)
{
	if (label->text)
	{
		destroy(label->text);
	}
	destroy(label);
}

void drawLabel(label_t *label, staticPlacement_t placement)
{
	glColor3f(1.0f,1.0f,1.0f);
	renderString(label->dynText ? *label->dynText : label->text, placement, label->textAlignment, -1);
}