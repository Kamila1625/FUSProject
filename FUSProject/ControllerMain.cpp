#include <windows.h>
#include <commctrl.h>                   // common controls
#include <sstream>
#include "ControllerMain.h"
#include "resource.h"

#include "ControllerGL.h"

bool CALLBACK enumerateChildren(HWND childHandle, LPARAM lParam);



ControllerMain::ControllerMain() : hwndGL(0), hwndForm(0)
{

}

int ControllerMain::command(int id, int cmd, LPARAM msg)
{
    /*switch(id)
    {
    case ID_FILE_EXIT:
        ::PostMessage(handle, WM_CLOSE, 0, 0);
        break;
		
    }*/

    return 0;
}

int ControllerMain::close()
{
   
    // close all child windows first
    ::EnumChildWindows(handle, (WNDENUMPROC)enumerateChildren, (LPARAM)WM_CLOSE);

    ::DestroyWindow(handle);    // close itself
    return 0;
}



int ControllerMain::destroy()
{
    ::PostQuitMessage(0);       // exit the message loop
 
    return 0;
}



int ControllerMain::create()
{
    return 0;
}


int ControllerMain::size(int width, int height, WPARAM wParam)
{

	RECT rect;
	GetWindowRect(hwndForm, &rect);
	int formHeight = rect.bottom - rect.top;
	int formWidth = rect.right - rect.left;

	GetWindowRect(hwndGL, &rect);
	int glWidth = rect.right - rect.left;
	int glHeight = rect.bottom - rect.top;

	SetWindowPos(hwndGL, 0, 0, 0, width - formWidth, height, SWP_NOZORDER);
	SetWindowPos(hwndForm, 0, width - formWidth, 0, formWidth, height, SWP_NOZORDER);
	InvalidateRect(hwndForm, 0, TRUE);      // force to repaint											 

  return 0;
}

bool CALLBACK enumerateChildren(HWND handle, LPARAM lParam)
{
    if(lParam == WM_CLOSE)
    {
        ::SendMessage(handle, WM_CLOSE, 0, 0);      // close child windows
    }

    return true;
}

void ControllerMain::SendData(char *fileName, long dataLen)
{
  ControllerGL *ctrl = (ControllerGL *)GetWindowLongPtr(hwndGL, GWLP_USERDATA);
  /* Check */
  HGLRC cont = wglGetCurrentContext();

  ctrl->SendData(fileName, dataLen);
}