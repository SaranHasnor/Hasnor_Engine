#include <GL/glut.h>
#include "engine_input.h"
#include "engine_interface.h"
#include <utils.h>

typedef struct inputKey_s {
	unsigned char		key;
	int					pressTime;
	bool				onInterface;

	struct inputKey_s	*next;
} inputKey_t;

uint _mouseButtons;
int _mousePos[2];
int _lastUpdatePos[2];
inputKey_t *_heldKeys = NULL;

engineListener_t _inputListener;

bool _clickOnInterface = false;

inputKey_t *heldKey(unsigned char key)
{
	inputKey_t *curKey = _heldKeys;
	while (curKey)
	{
		if (curKey->key == key)
		{
			return curKey;
		}
		curKey = curKey->next;
	}
	return NULL;
}

void appendHeldKey(unsigned char key, int currentTime, bool onInterface)
{
	inputKey_t **curKey = &_heldKeys;
	
	while (*curKey)
	{
		if ((*curKey)->key == key)
		{ // Already have it
			return;
		}
		else if (charsEqualCaseInsensitive((*curKey)->key, key))
		{ // Already have it but the case is different
			(*curKey)->key = key;
			return;
		}
		curKey = &(*curKey)->next;
	}
	
	*curKey = (inputKey_t*)mem_alloc(sizeof(inputKey_t));
	(*curKey)->key = key;
	(*curKey)->pressTime = currentTime;
	(*curKey)->onInterface = onInterface;
	(*curKey)->next = NULL;
}

bool deleteHeldKey(unsigned char key)
{ // Returns true if the key was on the screen, false if it was on the interface
	inputKey_t **curKey = &_heldKeys;
	while (*curKey)
	{
		if (charsEqualCaseInsensitive((*curKey)->key, key))
		{
			inputKey_t *temp = *curKey;
			bool onScreen = !temp->onInterface;
			*curKey = temp->next;
			mem_free(temp);
			return onScreen;
		}
		curKey = &(*curKey)->next;
	}

	// There are plenty of reasons why this may happen, so it shouldn't be considered an error
	return false;
}

int keyTimeHeld(unsigned char key)
{
	inputKey_t *curKey = _heldKeys;
	while (curKey)
	{
		//if (curKey->key == key)
		if (charsEqualCaseInsensitive(curKey->key, key))
		{
			if (curKey->onInterface)
			{
				return 0;
			}
			return curKey->pressTime;
		}
		curKey = curKey->next;
	}
	return 0;
}

void motion(int x, int y)
{
	_mousePos[0] = x;
	_mousePos[1] = y;

	if (_clickOnInterface)
	{
		interface_clickHeld(x, y);
	}
}

void mouse(int button, int state, int x, int y)
{
	ushort actualButton;
	if (button == GLUT_LEFT_BUTTON)
	{
		actualButton = INPUT_MOUSELEFT_BUTTON;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		actualButton = INPUT_MOUSERIGHT_BUTTON;
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		actualButton = INPUT_MOUSEMIDDLE_BUTTON;
	}
	else
	{ // ???
		assert(0);
		actualButton = -1;
	}

	if (state == GLUT_DOWN)
	{
		if (interface_clickDown(x, y))
		{
			_clickOnInterface = true;
			return;
		}

		_mouseButtons |= (1 << actualButton);
	}
	else
	{
		if (_clickOnInterface)
		{
			_clickOnInterface = false;

			interface_clickUp(x, y);
		}

		_mouseButtons &= ~(1 << actualButton);
	}

	// Might as well
	_mousePos[0] = x;
	_mousePos[1] = y;

	if (state == GLUT_DOWN)
	{
		_inputListener.mouseDownFunc(actualButton, x, y);
	}
	else
	{
		_inputListener.mouseUpFunc(actualButton, x, y);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	bool onInterface = interface_keyPressed(key, x, y);

	appendHeldKey(key, glutGet(GLUT_ELAPSED_TIME), onInterface);

	if (!onInterface)
	{
		_inputListener.keyDownFunc(key);
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	if (deleteHeldKey(key))
	{
		_inputListener.keyUpFunc(key);
	}
}

void initInput(engineListener_t listener)
{
	_inputListener = listener;

	_lastUpdatePos[0] = _lastUpdatePos[1] = 0;

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);
}

void updateInput(inputStruct_t *input)
{
	input->mouseButtons = _mouseButtons;

	input->mousePos[0] = _mousePos[0];
	input->mousePos[1] = _mousePos[1];

	input->mouseDelta[0] = _mousePos[0] - _lastUpdatePos[0];
	input->mouseDelta[1] = _mousePos[1] - _lastUpdatePos[1];

	_lastUpdatePos[0] = _mousePos[0];
	_lastUpdatePos[1] = _mousePos[1];

	input->key_timeHeld = keyTimeHeld; // Not really necessary to do that every frame...
}