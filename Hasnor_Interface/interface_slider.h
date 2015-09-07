#include "interface_utils.h"

typedef struct {
	float			value;
	float			*dynValue;

	float			min, max;
	float			percentage;
	
	// Events
	void			(*onSelect)(uint index);
} slider_t;

slider_t *newSlider();
void destroySlider(slider_t *slider);
void updateSliderValue(slider_t *slider);

void drawSlider(slider_t *slider, staticPlacement_t placement);