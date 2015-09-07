#include "interface_slider.h"
#include "interface_render.h"

slider_t *newSlider()
{
	slider_t *slider = (slider_t*)mem_alloc(sizeof(slider_t));
	mem_set(slider, 0, sizeof(slider_t));
	return slider;
}

void destroySlider(slider_t *slider)
{
	mem_free(slider);
}

void updateSliderValue(slider_t *slider)
{
	slider->value = slider->min+slider->percentage*(float)(slider->max-slider->min);
	if (slider->dynValue)
	{
		*slider->dynValue = slider->value;
	}
}

void drawSlider(slider_t *slider, staticPlacement_t placement)
{
	glBegin(GL_LINES);
	// Axis
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(placement.x, placement.y-5);
	glVertex2i(placement.x, placement.y+5);

	glVertex2i(placement.x, placement.y);
	glVertex2i(placement.x+placement.w, placement.y);

	glVertex2i(placement.x+placement.w, placement.y-5);
	glVertex2i(placement.x+placement.w, placement.y+5);

	// Cursor
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2i(placement.x+(slider->percentage*placement.w), placement.y-5);
	glVertex2i(placement.x+(slider->percentage*placement.w), placement.y+5);
	glEnd();
}