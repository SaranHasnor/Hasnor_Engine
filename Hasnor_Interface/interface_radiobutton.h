#include "interface_utils.h"

typedef struct {
	uint			id;

	// Events
	void			(*onSelectionChanged)(uint index);
} radioGroup_t;

typedef struct {
	bool			value;
	bool			*dynValue;

	radioGroup_t	*group;
} radioButton_t;

radioButton_t *newRadioButton();
void destroyRadioButton(radioButton_t *radioButton);

void drawRadioButton(radioButton_t *rButton, staticPlacement_t placement);