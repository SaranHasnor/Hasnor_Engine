#include "interface_label.h"
#include "interface_render.h"

label_t *newLabel()
{
	label_t *label = (label_t*)mem_alloc(sizeof(label_t));
	mem_set(label, 0, sizeof(label_t));
	return label;
}

void destroyLabel(label_t *label)
{
	if (label->dynText)
	{
		mem_free(label->dynText);
	}
	if (label->text)
	{
		mem_free(label->text);
	}
	mem_free(label);
}

void drawLabel(label_t *label, staticPlacement_t placement)
{
	glColor3f(1.0f,1.0f,1.0f);
	renderString(label->dynText ? *label->dynText : label->text, placement, label->textAlignment, -1);
}