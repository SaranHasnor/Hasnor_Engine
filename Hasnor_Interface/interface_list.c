#include "interface_list.h"
#include "interface_render.h"

#include <string.h>

list_t *newList()
{
	list_t *list = (list_t*)mem_alloc(sizeof(list_t));
	mem_set(list, 0, sizeof(list_t));
	return list;
}

void clearList(list_t *list)
{
	if (list->entries)
	{
		uint i;
		for (i = 0; i < list->nbEntries; i++)
		{
			listEntry_t *entry = &list->entries[i];
			if (entry->dynValue)
			{
				mem_free(entry->dynValue);
			}
			if (entry->value)
			{
				mem_free(entry->value);
			}
		}
		mem_free(list->entries);
		list->entries = NULL;
		list->nbEntries = 0;
	}
}

void destroyList(list_t *list)
{
	clearList(list);
	mem_free(list);
}

void listSelect(list_t *list, uint index)
{
	if (index >= list->nbEntries)
	{
		if (list->nbEntries)
		{ // Just lower it...
			index = list->nbEntries-1;
		}
		else
		{
			return;
		}
	}

	if (list->onSelect)
	{
		list->onSelect(index);
	}
	list->selectIndex = index;
}

void drawList(list_t *list, staticPlacement_t placement)
{
	uint i;

	// Border
	glColor3f(1.0f, 1.0f, 1.0f);
	drawRect(placement);

	// Buttons
	// - Top
	if (list->startIndex > 0)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2i(placement.x, placement.y);
	glVertex2i(placement.x+placement.w, placement.y);
	glVertex2i(placement.x+placement.w, placement.y+20);
	glVertex2i(placement.x, placement.y+20);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2i(placement.x+(placement.w-8)/2,placement.y+14);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '^'); // TODO: Draw an actual arrow

	// - Bottom
	if (list->startIndex+list->maxDisplayedEntries < list->nbEntries)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2i(placement.x, placement.y+placement.h-20);
	glVertex2i(placement.x+placement.w, placement.y+placement.h-20);
	glVertex2i(placement.x+placement.w, placement.y+placement.h);
	glVertex2i(placement.x, placement.y+placement.h);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2i(placement.x+(placement.w-8)/2,placement.y+placement.h-9);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'v'); // TODO: Draw an actual arrow

	// List
	for (i=list->startIndex; i<list->nbEntries && i-list->startIndex < list->maxDisplayedEntries; i++)
	{
		const int entryHeight = 15;
		staticPlacement_t entryPlacement;
		char *text = list->entries[i].dynValue ? *list->entries[i].dynValue : list->entries[i].value;

		entryPlacement.x = placement.x + 2;
		entryPlacement.y = placement.y + 20 + entryHeight * (i-list->startIndex);
		entryPlacement.w = placement.w - 4;
		entryPlacement.h = entryHeight;
		
		if (i == list->selectIndex)
		{ // Selected option
			glColor3f(0.0f,0.0f,0.75f);
			glBegin(GL_QUADS);
			drawRect(entryPlacement);
			glEnd();
			glColor3f(1.0f,1.0f,1.0f);
		}
		else
		{
			glColor3f(0.0f,0.0f,0.0f);
		}

		if (!text)
		{
			renderString("Unnamed entry", entryPlacement, ANCHOR_TOP_LEFT, -1);
		}
		else if (text[0] != '\0' || list->selected)
		{
			renderString(text, entryPlacement, ANCHOR_TOP_LEFT, list->selected ? strlen(text) : -1);
		}
		else
		{
			renderString("(empty)", entryPlacement, ANCHOR_TOP_LEFT, -1);
		}
	}
}