#ifndef HASNOR_ENGINE_CALLBACKS_IMPORTED
#define HASNOR_ENGINE_CALLBACKS_IMPORTED

#include <utils_types.h>

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

	void	(*mouseDownFunc)(ushort button, int x, int y);
	void	(*mouseUpFunc)(ushort button, int x, int y);

	void	(*initFunc)(void);
	void	(*updateFunc)(const timeStruct_t time, const inputStruct_t input);
	void	(*renderFunc)(const float viewMatrix[16]);
} engineListener_t;

#endif