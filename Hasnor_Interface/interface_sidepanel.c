#include "interface_sidepanel.h"
#include "interface_render.h"


void drawSidePanel(sidePanel_t menu)
{
	 // Side menu background
	glColor3f(0.2f, 0.2f, 0.1f);
	//drawRect(

	// TODO
}

// Side menu transition animation, a bit dirty but it works
uint prevMenu = 0;
uint nextMenu = 0;

/*void doTransition(int value)
{
	float percentage = 1.0f - (float)(value-glutGet(GLUT_ELAPSED_TIME))/(float)(value-_interface.sidePanel.transTime);

	if (percentage < 1.0f && ((_interface.sidePanel.targX != _interface.sidePanel.x) || (_interface.sidePanel.targY != _interface.sidePanel.y)))
	{
		_interface.sidePanel.x = _interface.sidePanel.x + atLeastOne(percentage*(_interface.sidePanel.targX - _interface.sidePanel.x));
		_interface.sidePanel.y = _interface.sidePanel.y + atLeastOne(percentage*(_interface.sidePanel.targY - _interface.sidePanel.y));

		glutTimerFunc(10, doTransition, value);
	}
	else
	{

		if (_interface.sidePanel.menus[prevMenu].onExit)
		{
			_interface.sidePanel.menus[prevMenu].onExit(nextMenu);
		}
		
		prevMenu = nextMenu;

		_interface.sidePanel.x = _interface.sidePanel.targX;
		_interface.sidePanel.y = _interface.sidePanel.targY;
		_interface.sidePanel.transTime = 0;
	}
	
	glutSetWindow(2);
	glutPostRedisplay();
	glutSetWindow(1);
}

void interface_showMenu(uint menu)
{
	if (menu > _interface.sidePanel.nbMenus)
	{
		return;
	}

	if (_interface.sidePanel.transTime)
	{ // Already switching menus
		return;
	}

	nextMenu = menu;

	if (_interface.sidePanel.menus[nextMenu].onEnter)
	{
		_interface.sidePanel.menus[nextMenu].onEnter(prevMenu);
	}

	_interface.sidePanel.targX = _interface.sidePanel.menus[menu].content.position.screenPos.x;
	_interface.sidePanel.targY = _interface.sidePanel.menus[menu].content.position.screenPos.y;
	_interface.sidePanel.transTime = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(10, doTransition, _interface.sidePanel.transTime + 500);
}*/