#ifndef INTERFACE_UTILS_IMPORTED
#define INTERFACE_UTILS_IMPORTED

#include <utils.h>

typedef enum {
	ANCHOR_TOP_LEFT,
	ANCHOR_TOP,
	ANCHOR_TOP_RIGHT,
	ANCHOR_LEFT,
	ANCHOR_CENTER,
	ANCHOR_RIGHT,
	ANCHOR_BOTTOM_LEFT,
	ANCHOR_BOTTOM,
	ANCHOR_BOTTOM_RIGHT
} anchor_t;

typedef struct {
	int				x, y;
	int				w, h;
} staticPlacement_t;

typedef struct {
	int				left;
	int				top;
	int				right;
	int				bottom;
} dynamicPlacement_t;

typedef struct {
	float			x, y;
	float			w, h;
} relativePlacement_t;

typedef union {
	staticPlacement_t	staticPos;
	dynamicPlacement_t	dynamicPos;
	relativePlacement_t	relativePos;
} placement_u;

typedef enum {
	PLACEMENT_STATIC,
	PLACEMENT_DYNAMIC,
	PLACEMENT_RELATIVE
} placementType_t;

typedef struct {
	placement_u		placement;
	placementType_t	type;
} placement_t;

typedef struct {
	placement_t		origin;
	placement_t		destination;

	int				startTime;
	int				duration;
} placementAnimation_t;

typedef struct {
	placement_t				placement;
	placementAnimation_t	animation;

	staticPlacement_t		screenPos;	// Position on the screen in pixels, only value actually checked by rendering
} position_t;

typedef enum {
	FIELDTYPE_TEXT,
	FIELDTYPE_INT,
	FIELDTYPE_FLOAT,
	FIELDTYPE_SPECIAL,	// TODO: Take a specific set of characters
	FIELDTYPE_BINARY	// All data is displayed as binary
} fieldType_t;


placement_t staticPlacement(int x, int y, int w, int h);
placement_t dynamicPlacement(int left, int top, int right, int bottom);
placement_t relativePlacement(float x, float y, float w, float h);

void interface_updateAbsolutePosition(position_t *position, staticPlacement_t bounds);

#endif