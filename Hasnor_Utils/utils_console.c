#include "utils_console.h"
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

void console_setVisible(bool visible)
{
	HWND consoleWindow = GetConsoleWindow();
	ShowWindow(consoleWindow, visible);
}

void console_print(const char* format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	vprintf(format, arguments);
	va_end(arguments);
}

void initConsoleFunctions(void)
{
	Console.setVisible = console_setVisible;
	Console.print = console_print;
}
