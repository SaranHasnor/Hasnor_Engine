#include "interface_utils.h"

typedef struct {
	bool			value;
	bool			*dynValue;

	// Events
	void			(*onToggle)(bool newValue);
} checkBox_t;

checkBox_t *newCheckBox();
void destroyCheckBox(checkBox_t *checkbox);

void drawCheckBox(checkBox_t *checkBox, staticPlacement_t placement);