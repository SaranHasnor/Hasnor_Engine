#include "interface_component.h"

component_t *newComponent(componentType_t type, uint id)
{
	component_t *newComp;

	newComp = (component_t*)mem_alloc(sizeof(component_t));

	newComp->gen_component.type = type;

	switch (type)
	{
	case COMP_LABEL:
		newComp->gen_component.object.label = newLabel();
		break;
	case COMP_BUTTON:
		newComp->gen_component.object.button = newButton();
		break;
	case COMP_TEXTFIELD:
		newComp->gen_component.object.textField = newTextField();
		break;
	case COMP_LIST:
		newComp->gen_component.object.list = newList();
		break;
	case COMP_SLIDER:
		newComp->gen_component.object.slider = newSlider();
		break;
	case COMP_CHECKBOX:
		newComp->gen_component.object.checkBox = newCheckBox();
		break;
	case COMP_RADIOBUTTON:
		newComp->gen_component.object.radioButton = newRadioButton();
		break;
	default:
		printf("Error: unknown object type: %i\n", type);
		break;
	}

	newComp->id = id;
	
	mem_set(&newComp->position, 0, sizeof(position_t));
	
	return newComp;
}

void destroyComponent(component_t *object)
{
	switch (object->gen_component.type)
	{
	case COMP_LABEL:
		destroyLabel(object->gen_component.object.label);
		break;
	case COMP_BUTTON:
		destroyButton(object->gen_component.object.button);
		break;
	case COMP_TEXTFIELD:
		destroyTextField(object->gen_component.object.textField);
		break;
	case COMP_LIST:
		destroyList(object->gen_component.object.list);
		break;
	case COMP_SLIDER:
		destroySlider(object->gen_component.object.slider);
		break;
	case COMP_CHECKBOX:
		destroyCheckBox(object->gen_component.object.checkBox);
		break;
	case COMP_RADIOBUTTON:
		destroyRadioButton(object->gen_component.object.radioButton);
		break;
	default:
		break;
	}

	object->gen_component.object.label = NULL;
	object->gen_component.type = COMP_INACTIVE;
}

void drawObject(const component_t *object)
{
	switch (object->gen_component.type)
	{
	case COMP_LABEL:
		drawLabel(object->gen_component.object.label, object->position.screenPos);
		break;
	case COMP_BUTTON:
		drawButton(object->gen_component.object.button, object->position.screenPos);
		break;
	case COMP_TEXTFIELD:
		drawTextField(object->gen_component.object.textField, object->position.screenPos);
		break;
	case COMP_LIST:
		drawList(object->gen_component.object.list, object->position.screenPos);
		break;
	case COMP_SLIDER:
		drawSlider(object->gen_component.object.slider, object->position.screenPos);
		break;
	case COMP_CHECKBOX:
		drawCheckBox(object->gen_component.object.checkBox, object->position.screenPos);
		break;
	case COMP_RADIOBUTTON:
		drawRadioButton(object->gen_component.object.radioButton, object->position.screenPos);
		break;
	default:
		break;
	}
}