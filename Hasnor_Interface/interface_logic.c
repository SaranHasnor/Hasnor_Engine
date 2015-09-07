#include "interface_main.h"
#include "interface_logic.h"
#include "interface_render.h"

#include <utils_bytestream.h>
#include <string.h>
#include <stdlib.h>

void interface_init()
{
	renameString = NULL;

	_curObject = NULL;

	_curBlockStack = (blockList_t*)mem_alloc(sizeof(blockList_t));
	_curBlockStack->block = &_interface.mainBlock;
	_curBlockStack->next = NULL;

	_interface.mainBlock.position.screenPos.x = 0;
	_interface.mainBlock.position.screenPos.y = 0;
	_interface.mainBlock.position.screenPos.w = 100;
	_interface.mainBlock.position.screenPos.h = 100;

	_interface.allComponents = NULL;
	_interface.nbComponents = 0;
	_interface.radioGroups = NULL;
	_interface.nbRadioGroups = 0;

	_interface.sidePanel.menus = NULL;
	_interface.sidePanel.nbMenus = 0;
	_interface.sidePanel.targX = 0;
	_interface.sidePanel.targY = 0;
	_interface.sidePanel.x = 0;
	_interface.sidePanel.y = 0;
	_interface.sidePanel.transTime = 0;
	_interface.sidePanel.width = 200;
}

void _unselectRadioGroup(radioGroup_t *group)
{
	uint i;
	for (i = 0; i < _interface.nbComponents; i++)
	{
		if (_interface.allComponents[i]->gen_component.type == COMP_RADIOBUTTON)
		{
			radioButton_t *rButton = _interface.allComponents[i]->gen_component.object.radioButton;

			if (rButton->group == group)
			{
				rButton->value = false;
				if (rButton->dynValue)
				{
					*rButton->dynValue = false;
				}
			}
		}
	}
}

void _dropFocus()
{ // Drop the focus of the current object
	renameString = NULL;
	renameStringCursor = 0;
	if (_curObject->gen_component.type == COMP_TEXTFIELD)
	{
		_curObject->gen_component.object.textField->selected = false;
	}
	_curObject = NULL;
}

bool interface_clickDown(int x, int y)
{ // Returns true if the event was intercepted by the interface, false otherwise
	uint i;

	if (renameString)
	{ // Cancel it FIXME: Don't do that if we clicked on the object?
		if (_curObject->gen_component.type == COMP_TEXTFIELD)
		{
			textField_t *field = _curObject->gen_component.object.textField;
			_dropFocus();
			if (field->onDeselected) field->onDeselected(true);
		}
		else
		{
			_dropFocus();
		}
		return true;
	}
	
	// Check for interactions with components (in opposite order compared to render)
	i = _interface.nbComponents;
	while (i > 0)
	{
		component_t *object = _interface.allComponents[--i];
		staticPlacement_t *objectPos = &object->position.screenPos;
		int left = objectPos->x;
		int right = objectPos->x + objectPos->w;
		int top = objectPos->y;
		int bottom = objectPos->y + objectPos->h;

		if (isInRange(left, x, right) && isInRange(top, y, bottom))
		{
			switch (object->gen_component.type)
			{
			case COMP_BUTTON:
				{
					button_t *button = object->gen_component.object.button;
					if (button->onClick)
					{
						button->onClick();
						return true;
					}
				}
				break;
			case COMP_TEXTFIELD:
				{
					textField_t *field = object->gen_component.object.textField;
					if (field->editable)
					{
						_curObject = object;
						renameString = &field->text;
						field->selected = true;
						if (field->onSelected) field->onSelected();
						return true;
					}
				}
				break;
			case COMP_LIST:
				{
					list_t *list = object->gen_component.object.list;
					if (y < top+20)
					{
						if (list->startIndex > 0)
							list->startIndex--;
					}
					else if (y > bottom-20)
					{
						if (list->startIndex+list->maxDisplayedEntries < list->nbEntries)
							list->startIndex++;
					}
					else
					{
						listSelect(list, list->startIndex+(y-top-20)/15);
					}
					return true;
				}
				break;
			case COMP_SLIDER:
				{
					slider_t *slider = object->gen_component.object.slider;
					slider->percentage = (x - objectPos->x) / (float)objectPos->w;
					updateSliderValue(slider);
					_curObject = object;
					return true;
				}
				break;
			case COMP_CHECKBOX:
				{
					checkBox_t *checkBox = object->gen_component.object.checkBox;
					checkBox->value = !checkBox->value;
					if (checkBox->dynValue)
					{
						*checkBox->dynValue = checkBox->value;
					}
					return true;
				}
				break;
			case COMP_RADIOBUTTON:
				{
					radioButton_t *rButton = object->gen_component.object.radioButton;
					if (!rButton->value)
					{
						_unselectRadioGroup(rButton->group);
						rButton->value = true;
						if (rButton->dynValue) *rButton->dynValue = true;
						rButton->group->onSelectionChanged(object->id);
					}
					return true;
				}
				break;
			default:
				break;
			}
		}
	}

	return false;
}

void interface_clickHeld(int x, int y)
{
	slider_t *slider;

	if (!_curObject || _curObject->gen_component.type != COMP_SLIDER)
	{
		return;
	}

	slider = _curObject->gen_component.object.slider;
	slider->percentage = (x - _curObject->position.screenPos.x) / (float) _curObject->position.screenPos.w;

	if (slider->percentage < 0)
		slider->percentage = 0;
	if (slider->percentage > 1)
		slider->percentage = 1;

	updateSliderValue(slider);
}

void interface_clickUp(int x, int y)
{
	if (_curObject && _curObject->gen_component.type == COMP_SLIDER)
	{
		_curObject = NULL;
	}
}

bool _isAcceptedSpecialChar(char c)
{
	return (c == '_' ||
		c == '-' ||
		c == '+' ||
		c == '/' ||
		c == '*' ||
		c == '[' ||
		c == ']' ||
		c == '(' ||
		c == ')' ||
		c == '{' ||
		c == '}' ||
		c == '|' ||
		c == ',' ||
		c == ':' ||
		c == ';' ||
		c == '.' ||
		c == '!' ||
		c == '?' ||
		c == '<' ||
		c == '>' ||
		c == '^' ||
		c == '&' ||
		c == '#' ||
		c == '$' ||
		c == '%');
}


bool interface_keyPressed(unsigned char key, int x, int y)
{ // Returns true if the event was intercepted by the interface
	uint pos;
	uint len;

	if (!renameString)
		return false;

	if (!_curObject)
		return false;

	len = strlen(*renameString);
	pos = len;//renameStringCursor;

	if (key == 8)
	{ // Backspace
		if (len > 0)
		{
			(*renameString)[pos-1] = '\0';
			*renameString = (char*)mem_realloc(*renameString, sizeof(char)*(pos));
		}
	}
	else if (key == 13)
	{ // Enter
		if (len > 0)
		{
			if (_curObject->gen_component.type == COMP_TEXTFIELD)
			{
				textField_t *field = _curObject->gen_component.object.textField;
				updateFieldValue(field);
				_dropFocus();
				if (field->onDeselected) field->onDeselected(false);
			}
			else if (_curObject->gen_component.type == COMP_LIST)
			{
				list_t *list = _curObject->gen_component.object.list;
				int index = list->selectIndex;
				strcpy_safe(list->entries[index].dynValue ? *list->entries[index].dynValue : list->entries[index].value, *renameString);
				_dropFocus();
				list->onSelect(index); // Update it	
			}
		}
	}
	else if (key == 27)
	{ // Escape
		if (_curObject->gen_component.type == COMP_TEXTFIELD)
		{
			textField_t *field = _curObject->gen_component.object.textField;
			_dropFocus();
			if (field->onDeselected) field->onDeselected(true);
		}
		else
		{
			_dropFocus();
		}
	}
	else
	{ // Normal input
		if (_curObject->gen_component.type == COMP_TEXTFIELD && _curObject->gen_component.object.textField->type != FIELDTYPE_TEXT)
		{ // Apply rules to select what we can type
			textField_t *field = _curObject->gen_component.object.textField;
			if (field->type == FIELDTYPE_BINARY)
			{ // Only two choices here really
				if (key != '0' && key != '1')
				{
					return true;
				}
			}
			else
			{
				if (key < '0' || key > '9')
				{
					if (key == '.' && field->type != FIELDTYPE_FLOAT)
					{
						return true;
					}
				}

				if ((field->max > 0 && pos >= nbDigits(field->max)) || pos >= 8)
				{
					return true;
				}
			}
		}
		else
		{ // Take most things
			if ((key < 'a' || key > 'z') &&
				(key < 'A' || key > 'Z') &&
				(key < '0' || key > '9') &&
				!_isAcceptedSpecialChar(key) &&
				((key != ' ') || pos == 0)) // Can't start with a space
			{
				return true;
			}

			if (pos >= 15)
			{
				return true;
			}
		}
		
		*renameString = (char*)mem_realloc(*renameString, sizeof(char)*(pos+2));
		(*renameString)[pos] = key;
		(*renameString)[pos+1] = '\0';
	}

	return true;
}

void interface_update(float deltaTime)
{
	// :D
}

void interface_render()
{
	drawBlock(&_interface.mainBlock);

	if (_interface.sidePanelVisible)
	{
		drawSidePanel(_interface.sidePanel);
	}
}

void interface_reshapeWindow(int width, int height)
{
	uint i;

	_interface.mainBlock.position.screenPos.w = width;
	_interface.mainBlock.position.screenPos.h = height;

	updateBlockLayout(&_interface.mainBlock);

	for (i = 0; i < _interface.sidePanel.nbMenus; i++)
	{
		_interface.sidePanel.menus[i].content.position.screenPos.h = height;

		updateBlockLayout(&_interface.sidePanel.menus[i].content);
	}

}
