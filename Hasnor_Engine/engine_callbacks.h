#ifndef HASNOR_CALLBACKS_IMPORTED
#define HASNOR_CALLBACKS_IMPORTED

#include <utils_types.h>

/*
engine_callbacks

- Input events
*/

#define INPUT_MOUSELEFT_BUTTON		0
#define INPUT_MOUSERIGHT_BUTTON		1
#define INPUT_MOUSEMIDDLE_BUTTON	2

#define INPUT_MOUSELEFT		(1 << INPUT_MOUSELEFT_BUTTON)
#define INPUT_MOUSERIGHT	(1 << INPUT_MOUSERIGHT_BUTTON)
#define INPUT_MOUSEMIDDLE	(1 << INPUT_MOUSEMIDDLE_BUTTON)

typedef struct {
	int		mousePos[2];
	int		mouseDelta[2];

	uint	mouseButtons;

	int		(*key_timeHeld)(unsigned char key);
} inputStruct_t;

typedef struct {
	long	currentTime;
	long	deltaTime;
	float	deltaTimeSeconds;
} timeStruct_t;

typedef struct {
	void	(*keyDownFunc)(uchar key);
	void	(*keyUpFunc)(uchar key);

	void	(*mouseDownFunc)(ushort button);
	void	(*mouseUpFunc)(ushort button);

	void	(*updateFunc)(timeStruct_t time, inputStruct_t input);
	void	(*renderFunc)(void);
} engineListener_t;

#endif