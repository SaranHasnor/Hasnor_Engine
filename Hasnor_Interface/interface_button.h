#include "interface_utils.h"

typedef struct {
	char			*text;
	char			**dynText;

	// Events
	void			(*onClick)(void);
} button_t;

button_t *newButton();
void destroyButton(button_t *button);

void drawButton(button_t *button, staticPlacement_t placement);