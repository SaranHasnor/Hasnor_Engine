#include "interface_button.h"
#include "interface_render.h"

button_t *newButton()
{
	button_t *button = (button_t*)mem_alloc(sizeof(button_t));
	mem_set(button, 0, sizeof(button_t));
	return button;
}

void destroyButton(button_t *button)
{
	if (button->text)
	{
		mem_free(button->text);
	}
	mem_free(button);
}

void drawButton(button_t *button, staticPlacement_t placement)
{
	// Border
	glColor3f(0.0f, 0.0f, 0.0f);
	drawRect(placement);

	// Text
	glColor3f(1.0f,1.0f,1.0f);
	renderString(button->dynText ? *button->dynText : button->text, placement, ANCHOR_CENTER, -1);
}