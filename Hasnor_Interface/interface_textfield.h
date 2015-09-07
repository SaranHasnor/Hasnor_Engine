#include "interface_utils.h"

typedef struct {
	void			**value;
	fieldType_t		type;
	float			min, max;	// Used to cap the value of numbers and the length of texts

	char			*text;		// Actual displayed text

	bool			editable;

	anchor_t		textAlignment;

	bool			selected;

	// Events
	void			(*onSelected)(void);
	void			(*onDeselected)(bool cancelled);
} textField_t;

textField_t *newTextField();
void destroyTextField(textField_t *textField);
void updateFieldText(textField_t *field);
void updateFieldValue(textField_t *field);

void drawTextField(textField_t *field, staticPlacement_t placement);