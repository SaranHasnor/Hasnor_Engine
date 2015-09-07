#include "interface_render.h"

#include <string.h>

void drawRect(staticPlacement_t rect)
{
	glBegin(GL_QUADS);
	glVertex2i(rect.x, rect.y);
	glVertex2i(rect.x+rect.w, rect.y);
	glVertex2i(rect.x+rect.w, rect.y+rect.h);
	glVertex2i(rect.x, rect.y+rect.h);
	glEnd();
}

void renderString(char *s, staticPlacement_t placement, anchor_t alignment, int rasterPos)
{ // Aligment and word wrapping isn't fully supported yet
	int i=0;
	int startX, startY;
	int currentX, currentY;

	switch (alignment)
	{
	case ANCHOR_CENTER: // This doesn't work with line breaks
		startX = placement.x + (placement.w - (strlen(s) * 8)) / 2;
		startY = placement.y + ((placement.h / 2) + 4);
		break;
	default:
		startX = placement.x;
		startY = placement.y + 14;
		break;
	}

	currentX = startX;
	currentY = startY;

	while (s[i] != '\0' || i <= rasterPos)
	{
		glRasterPos2i(currentX, currentY);

		if (s[i] == '\0')
		{ // Means we had a raster to render, just drop it here and stop
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '_');
			break;
		}
		else if (s[i] == '\n' || currentX > placement.x + placement.w)
		{
			currentY += 13;
			currentX = startX;
			
			if (s[i] != '\n')
			{
				i--;
			}
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);
			if (rasterPos == i)
			{
				glRasterPos2i(currentX, currentY);
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '_');
			}
			currentX += 8;
		}
		i++;
	}
}