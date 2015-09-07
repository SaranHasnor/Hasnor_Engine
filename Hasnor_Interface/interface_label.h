#include "interface_utils.h"

typedef struct {
	char			*text;
	char			**dynText;

	anchor_t		textAlignment;
} label_t;

label_t *newLabel();
void destroyLabel(label_t *label);

void drawLabel(label_t *label, staticPlacement_t placement);