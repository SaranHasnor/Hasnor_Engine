#include "interface_label.h"
#include "interface_button.h"
#include "interface_textfield.h"
#include "interface_list.h"
#include "interface_slider.h"
#include "interface_checkbox.h"
#include "interface_radiobutton.h"

typedef enum {
	COMP_INACTIVE,
	COMP_LABEL,
	COMP_BUTTON,
	COMP_TEXTFIELD,
	COMP_LIST,
	COMP_SLIDER,
	COMP_CHECKBOX,
	COMP_RADIOBUTTON
} componentType_t;

typedef union {
	slider_t		*slider;
	button_t		*button;
	list_t			*list;
	label_t			*label;
	textField_t		*textField;
	checkBox_t		*checkBox;
	radioButton_t	*radioButton;
} gen_component_u;

typedef struct {
	gen_component_u	object;
	componentType_t	type;
} gen_component_t;

typedef struct {
	uint			id;

	position_t		position;

	gen_component_t	gen_component;
} component_t;

component_t *newComponent(componentType_t type, uint id);
void destroyComponent(component_t *object);

void drawObject(const component_t *object);