#include "WindowGL.h"
#include "proc.h"

#include <sstream>
#include <iostream>
#include <cstring>

WindowGL::WindowGL(HINSTANCE hInst, const wchar_t* name, HWND hParent, Controller* ctrl) : handle(0), instance(hInst), controller(ctrl), winStyle(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN),
																		 winStyleEx(WS_EX_CLIENTEDGE), x(CW_USEDEFAULT), y(CW_USEDEFAULT),
																		 width(CW_USEDEFAULT), height(CW_USEDEFAULT),
																		 parentHandle(hParent), menuHandle(0)
{
	// copy string
	wcsncpy_s(this->title, name, MAX_STRING - 1);
	wcsncpy_s(this->className, name, MAX_STRING - 1);

	winClass.cbSize = sizeof(WNDCLASSEX);

	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WndProcGL;
	winClass.hInstance = instance;
	winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winClass.lpszClassName = name;

	//RegisterClass(&winClass);
}

WindowGL::~WindowGL()
{
	UnregisterClass(className, instance);
}

HWND WindowGL::create()
{
	// register a window class
	if (!::RegisterClassEx(&winClass)) return 0;

	handle = ::CreateWindowEx(winStyleEx,           // window border with a sunken edge
		className,            // name of a registered window class
		title,                // caption of window
		winStyle,             // window style
		x,                    // x position
		y,                    // y position
		width,                // witdh
		height,               // height
		parentHandle,         // handle to parent window
		menuHandle,           // handle to menu
		instance,             // application instance
		(LPVOID)controller);  // window creation data

				//this->show(SW_SHOWDEFAULT);                     // make it visible  

	return handle;
}



///////////////////////////////////////////////////////////////////////////////
// show the window on the screen
///////////////////////////////////////////////////////////////////////////////
void WindowGL::show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}
