#include "ControllerForm.h"
#include "resource.h"
#include "proc.h" 
#include "StandDialog.h"

#include <windows.h>
#include <windowsx.h>

int mainMessageLoop(HACCEL hAccelTable = 0);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX commonCtrls;
	commonCtrls.dwSize = sizeof(commonCtrls);
	commonCtrls.dwICC = ICC_BAR_CLASSES;        // trackbar is in this class
	::InitCommonControlsEx(&commonCtrls);

	Data d;
	ControllerForm formCtrl(&d);
	StandDialog dlWnd(hInstance, IDD_DIALOG1, 0, &formCtrl);
	dlWnd.setWidth(300);
	dlWnd.setHeight(200);
	dlWnd.create();
	dlWnd.show();

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