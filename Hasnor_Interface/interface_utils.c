#include "interface_utils.h"

placement_t staticPlacement(int x, int y, int w, int h)
{
	placement_t placement;
	staticPlacement_t *p = &placement.placement.staticPos;

	placement.type = PLACEMENT_STATIC;
	p->x = x;
	p->y = y;
	p->w = w;
	p->h = h;

	return placement;
}

placement_t dynamicPlacement(int left, int top, int right, int bottom)
{
	placement_t placement;
	dynamicPlacement_t *p = &placement.placement.dynamicPos;

	placement.type = PLACEMENT_DYNAMIC;
	p->left = left;
	p->top = top;
	p->right = right;
	p->bottom = bottom;

	return placement;
}

placement_t relativePlacement(float x, float y, float w, float h)
{
	placement_t placement;
	relativePlacement_t *p = &placement.placement.relativePos;

	placement.type = PLACEMENT_RELATIVE;
	p->x = x;
	p->y = y;
	p->w = w;
	p->h = h;

	return placement;
}

void interface_updateAbsolutePosition(position_t *position, staticPlacement_t bounds)
{
	/*int curTime = glutGet(GLUT_ELAPSED_TIME);
	if (position->animation.startTime <= curTime && position->animation.startTime + position->animation.duration >= curTime)
	{
		float lerpFactor = (curTime - position->animation.startTime) / (float)position->animation.duration;
		
		
	}
	else*/
	{
		if (position->placement.type == PLACEMENT_STATIC)
		{
			position->screenPos.x = bounds.x + position->placement.placement.staticPos.x;
			position->screenPos.y = bounds.y + position->placement.placement.staticPos.y;
			position->screenPos.w = position->placement.placement.staticPos.w;
			position->screenPos.h = position->placement.placement.staticPos.h;
		}
		else if (position->placement.type == PLACEMENT_DYNAMIC)
		{
			position->screenPos.x = bounds.x + position->placement.placement.dynamicPos.left;
			position->screenPos.y = bounds.y + position->placement.placement.dynamicPos.right;
			position->screenPos.w = bounds.w - (position->placement.placement.dynamicPos.left + position->placement.placement.dynamicPos.right);
			position->screenPos.h = bounds.h - (position->placement.placement.dynamicPos.top + position->placement.placement.dynamicPos.bottom);
		}
		else if (position->placement.type == PLACEMENT_RELATIVE)
		{
			position->screenPos.x = (int)(bounds.x + position->placement.placement.relativePos.x * bounds.w);
			position->screenPos.y = (int)(bounds.y + position->placement.placement.relativePos.y * bounds.h);
			position->screenPos.w = (int)(position->placement.placement.relativePos.w * bounds.w);
			position->screenPos.h = (int)(position->placement.placement.relativePos.h * bounds.h);
		}
	}
}