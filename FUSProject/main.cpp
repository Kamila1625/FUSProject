#include <windows.h>
#include <windowsx.h>
#include <Commctrl.h>

#define COMPILE_AS_DLL
#define DLL_EXPORT

#include "FusExport.h"

#include "resource.h"
#include "proc.h" 
#include "WindowMain.h"
#include "WindowGL.h"
#include "FormDialog.h"
#include "ControllerMain.h"
#include "ControllerForm.h"
#include "ControllerGL.h"

int mainMessageLoop(HACCEL hAccelTable = 0);

#ifdef COMPILE_AS_DLL

HINSTANCE injectedModuleHandle; //Injected Modules Handle
ControllerGL *glCtrl; // Global GL Controller

DWORD WINAPI CreateFUSWindow()
{    
  INITCOMMONCONTROLSEX commonCtrls;
  commonCtrls.dwSize = sizeof(commonCtrls);
  commonCtrls.dwICC = ICC_BAR_CLASSES;        // trackbar is in this class
  ::InitCommonControlsEx(&commonCtrls);
  
  ControllerMain mainCtrl;
  WindowMain mainWin(injectedModuleHandle, L"FUS Project", 0, &mainCtrl);
  //mainWin.setMenuName(MAKEINTRESOURCE(IDR_MENU1));
  mainWin.setWidth(800);
  mainWin.setHeight(800);
  mainWin.setWindowStyleEx(WS_EX_WINDOWEDGE);

  // create a window and show
  //SetWindowPos(mainWin.getHandle(), 0, 0, 0, 570, 310, SWP_NOZORDER);
  mainWin.create();

  ModelGL *modelGL = glCtrl->GetModel();  

  WindowGL glWin(injectedModuleHandle, L"WindowGL", mainWin.getHandle(), glCtrl);
  glWin.setClassStyle(CS_OWNDC);
  glWin.setWindowStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
  glWin.setWidth(900);
  glWin.setHeight(1000);
  //SetWindowPos(glWin.getHandle(), 0, 0, 0, 300, 500, SWP_NOZORDER);
  glWin.create();

  ControllerForm formCtrl(modelGL);

  FormDialog dialogWin(injectedModuleHandle, IDD_DIALOG1, mainWin.getHandle(), &formCtrl);
  dialogWin.setWidth(500);
  dialogWin.setHeight(989);
  //SetWindowPos(dialogWin.getHandle(), 0, 300, 0, 450, 900, SWP_NOZORDER);
  dialogWin.create();

  //SetWindowPos(glWin.getHandle(), 0, 0, 0, 300, 500, SWP_NOZORDER);
  //SetWindowPos(dialogWin.getHandle(), 0, 300, 0, 450, 900, SWP_NOZORDER);
  //SetWindowPos(mainWin.getHandle(), 0, 0, 0, 450, 900, SWP_NOZORDER);

  globalhInst = injectedModuleHandle;
  //hwndMain = mainWin.getHandle();
  //hwndGL = glWin.getHandle();
  //hwndForm = dialogWin.getHandle();

  mainCtrl.setGLHandle(glWin.getHandle());
  mainCtrl.setFormHandle(dialogWin.getHandle());
  glCtrl->SetWinGLHandle(glWin.getHandle());

  RECT rect1;
  GetWindowRect(glWin.getHandle(), &rect1);

  // place the opengl form dialog in right place, bottome of the opengl rendering window
  ::SetWindowPos(dialogWin.getHandle(), 0, rect1.right + 20, rect1.top, 500, 989, 0);

  // compute height of all sub-windows
  int height = 0;
  RECT rect;
  GetWindowRect(glWin.getHandle(), &rect);      // get size of glWin
  height += rect.bottom - rect.top;
  // ::GetWindowRect(glDialog.getHandle(), &rect);   // get size of glDialog
  // height += rect.bottom - rect.top;
  // ::GetWindowRect(statusHandle, &rect);           // get size of status bar
  //  height += rect.bottom - rect.top;

  int width = 0;
  ::GetWindowRect(glWin.getHandle(), &rect);      // get size of glWin
  width += rect.right - rect.left;
  ::GetWindowRect(dialogWin.getHandle(), &rect);   // get size of glDialog
  width += rect.right - rect.left;
  //::GetWindowRect(statusHandle, &rect);           // get size of status bar
  //width += rect.right - rect.left;

  // resize main window, so all sub windows are fit into the client area of main window
  DWORD style = (DWORD)::GetWindowLongPtr(mainWin.getHandle(), GWL_STYLE);       // get current window style
  DWORD styleEx = (DWORD)::GetWindowLongPtr(mainWin.getHandle(), GWL_EXSTYLE);   // get current extended window style
  rect.left = 0;
  rect.right = width;
  rect.top = 0;
  rect.bottom = height;
  ::AdjustWindowRectEx(&rect, style, TRUE, styleEx);
  ::SetWindowPos(mainWin.getHandle(), 0, 0, 20, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);  

  glWin.show();
  dialogWin.show();
  mainWin.show();
  //nCmdShow = SW_MAXIMIZE;
  //mainWin.show(SW_MAXIMIZE);


  int exitCode;
  HACCEL hAccelTable = 0;
  exitCode = mainMessageLoop(hAccelTable);
  
  delete glCtrl;

  return exitCode;
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reasonForCall, LPVOID lpReserved)
{
  // Perform actions based on the reason for calling.
  switch (reasonForCall)
  {
  case DLL_PROCESS_ATTACH:
    injectedModuleHandle = hModule;    
    break;

  case DLL_THREAD_ATTACH:
    // Do thread-specific initialization.
    break;

  case DLL_THREAD_DETACH:
    // Do thread-specific cleanup.
    break;

  case DLL_PROCESS_DETACH:
    // Perform any necessary cleanup.
    break;
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH.  
}


// Global functions

FUS_API bool __stdcall Init(HWND hWnd, SendEllipseDataFunction sendEllipseFunc)
{
  glCtrl = new ControllerGL();
  glCtrl->SetModel(new ModelGL());
  glCtrl->SetInitData(hWnd, sendEllipseFunc);
  CreateFUSWindow();    

  return true;
}

FUS_API void __stdcall SendData(void *data, long dataLen)
{    
  glCtrl->SendData((char *)data, dataLen);    
}

FUS_API void __stdcall Stop()
{

}

#endif // !COMPILE_AS_LIB

#ifndef COMPILE_AS_DLL

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX commonCtrls;
	commonCtrls.dwSize = sizeof(commonCtrls);
	commonCtrls.dwICC = ICC_BAR_CLASSES;        // trackbar is in this class
	::InitCommonControlsEx(&commonCtrls);
  
	ControllerMain mainCtrl;

	WindowMain mainWin(hInstance, L"FUS Project", 0, &mainCtrl);
	//mainWin.setMenuName(MAKEINTRESOURCE(IDR_MENU1));
	mainWin.setWidth(500);
	mainWin.setHeight(600);
	mainWin.setWindowStyleEx(WS_EX_WINDOWEDGE);

	// create a window and show
	//SetWindowPos(mainWin.getHandle(), 0, 0, 0, 570, 310, SWP_NOZORDER);
	mainWin.create();


	ModelGL modelGL;
	ControllerGL glCtrl(&modelGL);

	WindowGL glWin(hInstance, L"WindowGL", mainWin.getHandle(), &glCtrl);
	glWin.setClassStyle(CS_OWNDC);
	glWin.setWindowStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	glWin.setWidth(300);
	glWin.setHeight(300);
	//SetWindowPos(glWin.getHandle(), 0, 0, 0, 300, 500, SWP_NOZORDER);
	glWin.create();

	ControllerForm formCtrl(&modelGL);

	FormDialog dialogWin(hInstance, IDD_DIALOG1, mainWin.getHandle(), &formCtrl);
	dialogWin.setWidth(500);
	dialogWin.setHeight(989);
	//SetWindowPos(dialogWin.getHandle(), 0, 300, 0, 450, 900, SWP_NOZORDER);
	dialogWin.create();

	//SetWindowPos(glWin.getHandle(), 0, 0, 0, 300, 500, SWP_NOZORDER);
	//SetWindowPos(dialogWin.getHandle(), 0, 300, 0, 450, 900, SWP_NOZORDER);
	//SetWindowPos(mainWin.getHandle(), 0, 0, 0, 450, 900, SWP_NOZORDER);
	
	globalhInst = hInstance;
	//hwndMain = mainWin.getHandle();
	//hwndGL = glWin.getHandle();
	//hwndForm = dialogWin.getHandle();

	mainCtrl.setGLHandle(glWin.getHandle());
	mainCtrl.setFormHandle(dialogWin.getHandle());

	RECT rect1;
	GetWindowRect(glWin.getHandle(), &rect1);

	// place the opengl form dialog in right place, bottome of the opengl rendering window
	::SetWindowPos(dialogWin.getHandle(), 0, rect1.right + 20, rect1.top, 500, 989, 0);

	// compute height of all sub-windows
	int height = 0;
	RECT rect;
	GetWindowRect(glWin.getHandle(), &rect);      // get size of glWin
	height += rect.bottom - rect.top;
	// ::GetWindowRect(glDialog.getHandle(), &rect);   // get size of glDialog
	// height += rect.bottom - rect.top;
	// ::GetWindowRect(statusHandle, &rect);           // get size of status bar
	//  height += rect.bottom - rect.top;

	int width = 0;
	::GetWindowRect(glWin.getHandle(), &rect);      // get size of glWin
	width += rect.right - rect.left;
	::GetWindowRect(dialogWin.getHandle(), &rect);   // get size of glDialog
	width += rect.right - rect.left;
	//::GetWindowRect(statusHandle, &rect);           // get size of status bar
	//width += rect.right - rect.left;

	// resize main window, so all sub windows are fit into the client area of main window
	DWORD style = (DWORD)::GetWindowLongPtr(mainWin.getHandle(), GWL_STYLE);       // get current window style
	DWORD styleEx = (DWORD)::GetWindowLongPtr(mainWin.getHandle(), GWL_EXSTYLE);   // get current extended window style
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;
	::AdjustWindowRectEx(&rect, style, TRUE, styleEx);
	::SetWindowPos(mainWin.getHandle(), 0, 0, 100, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

	glWin.show();
	dialogWin.show();
	//nCmdShow = SW_MAXIMIZE;
	mainWin.show(SW_MAXIMIZE);


	int exitCode;
	HACCEL hAccelTable = 0;
	exitCode = mainMessageLoop(hAccelTable);

	return exitCode;
}

#endif // !COMPILE_AS_LIB

int mainMessageLoop(HACCEL hAccelTable)
{
	HWND activeHandle;
	MSG msg;

	while (::GetMessage(&msg, 0, 0, 0) > 0)  // loop until WM_QUIT(0) received
	{
		// determine the activated window is dialog box
		// skip if messages are for the dialog windows
		activeHandle = GetActiveWindow();
		if (::GetWindowLongPtr(activeHandle, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) // WS_EX_CONTROLPARENT is automatically added by CreateDialogBox()
		{
			if (::IsDialogMessage(activeHandle, &msg))
				continue;   // message handled, back to while-loop
		}

		// now, handle window messages
		if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;                 // return nExitCode of PostQuitMessage()
}

