#include "interface_textfield.h"
#include "interface_render.h"

#include <utils_bytestream.h>
#include <string.h>

textField_t *newTextField()
{
	textField_t *textField = (textField_t*)mem_alloc(sizeof(textField_t));
	mem_set(textField, 0, sizeof(textField_t));
	return textField;
}

void destroyTextField(textField_t *textField)
{
	mem_free(textField->value);
	mem_free(textField);
}

void updateFieldText(textField_t *field)
{ // value -> text (overwrite the value instead of reallocating it every time please :( )
	if (field->text)
	{
		mem_free(field->text);
	}

	if (field->type == FIELDTYPE_INT)
	{
		field->text = strFromInt(*((int*)(*field->value)));
	}
	else if (field->type == FIELDTYPE_FLOAT)
	{
		field->text = strFromFloat(*((float*)(*field->value)));
	}
	else if (field->type == FIELDTYPE_BINARY)
	{ // NEEDS TESTING
		bytestream bs;
		bytestream_init(&bs, (uint)field->max);
		field->text = bytestream_toString(&bs);
		bytestream_destroy(&bs);
	}
	else
	{
		field->text = quickString((char*)(*field->value));
	}
}

void updateFieldValue(textField_t *field)
{ // text -> value
	if (field->type == FIELDTYPE_INT)
	{
		int value = parseInt(field->text);
		if (value < field->min)
			value = field->min;
		if (value > field->max)
			value = field->max;
		*((int*)(*field->value)) = value;
	}
	else if (field->type == FIELDTYPE_FLOAT)
	{
		float value = (float)parseFloat(field->text);
		if (value < field->min)
			value = field->min;
		if (value > field->max)
			value = field->max;
		*((float*)(*field->value)) = value;
	}
	else if (field->type == FIELDTYPE_BINARY)
	{ // Compile the binary data (we expect that the size of the container did not change...)
		uint i = 0;
		byte counter = 0;
		uint size = mem_size(*field->value);
		if (!size) size = 1;
		while (i < 9*size) // Make sure we don't go out of bounds
		{
			if (field->text[i] == ' ' || field->text[i] == '\0')
			{
				((byte*)*field->value)[i/9] = counter;
				counter = 0;
				if (field->text[i] == '\0')
				{
					break;
				}
			}
			else
			{
				counter = counter * 2;
				if (field->text[i] == '1') counter += 1;
			}
			i++;
		}
	}
	else
	{
		strcpy_safe((char*)(*field->value), field->text);
	}
}

void drawTextField(textField_t *field, staticPlacement_t placement)
{
	if (!field->selected)
	{
		updateFieldText(field);
	}

	// Border
	if (field->editable)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else
	{
		glColor3f(0.5f, 0.5f, 0.5f);
	}
	drawRect(placement);

	// Text
	if (field->text)
	{
		glColor3f(0.0f,0.0f,0.0f);
		renderString(field->text, placement, ANCHOR_CENTER, field->selected ? strlen(field->text) : -1);
	}
}