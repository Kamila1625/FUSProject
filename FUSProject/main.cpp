#include <windows.h>
#include <windowsx.h>
#include <Commctrl.h>

#include "resource.h"
#include "proc.h" 
#include "WindowMain.h"
#include "WindowGL.h"
#include "FormDialog.h"
#include "ControllerMain.h"
#include "ControllerForm.h"
#include "ControllerGL.h"


int mainMessageLoop(HACCEL hAccelTable = 0);

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
	//glWin.setWidth(300);
	//glWin.setHeight(300);
	//SetWindowPos(glWin.getHandle(), 0, 10, 10, 300, 300, SWP_NOZORDER);
	glWin.create();

	ControllerForm formCtrl(&modelGL);

	FormDialog dialogWin(hInstance, IDD_DIALOG1, mainWin.getHandle(), &formCtrl);
	//dialogWin.setWidth(500);
	//dialogWin.setHeight(300);
	//SetWindowPos(dialogWin.getHandle(), 0, 310, 10, 560, 300, SWP_NOZORDER);
	dialogWin.create();

	SetWindowPos(glWin.getHandle(), 0, 0, 0, 300, 500, SWP_NOZORDER);
	SetWindowPos(dialogWin.getHandle(), 0, 300, 0, 450, 900, SWP_NOZORDER);
	SetWindowPos(mainWin.getHandle(), 0, 0, 0, 450, 900, SWP_NOZORDER);
	

	glWin.show();
	dialogWin.show();
	nCmdShow = SW_MAXIMIZE;
	mainWin.show(nCmdShow);
	

	globalhInst = hInstance;
	hwndMain = mainWin.getHandle();
	hwndGL = glWin.getHandle();
	hwndForm = dialogWin.getHandle();

	int exitCode;
	HACCEL hAccelTable = 0;
	exitCode = mainMessageLoop(hAccelTable);

	return exitCode;
}

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

