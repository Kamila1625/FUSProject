#include "proc.h"

INT_PTR CALLBACK FormDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// find controller associated with window handle
	static Controller *ctrl;
	ctrl = (Controller*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);

	// WM_INITDIALOG message is called before displaying the dialog box.
	// lParam contains the value of dwInitParam of CreateDialogBoxParam(),
	// which is the pointer to the Controller.
	if (msg == WM_INITDIALOG)
	{
		// First, retrieve the pointrer to the controller associated with
		// the current dialog box.
		ctrl = (Controller*)lParam;
		ctrl->setHandle(hwnd);

		// Second, store the pointer to the Controller into GWL_USERDATA,
		// so, other messege can be routed to the associated Controller.
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)ctrl);

		// When WM_INITDIALOG is called, all controls in the dialog are created.
		// It is good time to initalize the appearance of controls here.
		// NOTE that we don't handle WM_CREATE message for dialogs because
		// the message is sent before controls have been create, so you cannot
		// access them in WM_CREATE message handler.
		ctrl->create();

		return true;
	}

	// check NULL pointer, because GWL_USERDATA is initially 0, and
	// we store a valid pointer value when WM_NCCREATE is called.
	if (!ctrl)
		return false;


	switch (msg)
	{
		//case WM_INITDIALOG:
		//SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
		//break;
	case WM_COMMAND:
		ctrl->command(LOWORD(wParam), HIWORD(wParam), lParam);
		DefWindowProc(hwnd, msg, wParam, lParam);
		break;
		//case WM_CTLCOLORSTATIC:
		/*{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(255, 255, 255));

		if (hbrBkgnd == NULL)
		{
		hbrBkgnd = CreateSolidBrush(RGB(255, 255, 255));
		}
		return (INT_PTR)hbrBkgnd;
		}*/
		//break;
	case WM_PAINT:
		DefWindowProc(hwnd, msg, wParam, lParam);
		break;

	case WM_HSCROLL:
		//ctrl->hScroll(wParam, lParam);
		break;

	case WM_VSCROLL:
		break;

	case WM_MOUSEWHEEL:
		DefWindowProc(hwnd, msg, wParam, lParam);
		break;

	case WM_SIZE:
		ctrl->size(LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		CloseWindow(hwnd);
		break;
	default:
		return false;
	}
	return true;
}