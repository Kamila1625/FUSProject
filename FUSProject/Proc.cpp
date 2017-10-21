#include "proc.h"
#include <windowsx.h>
#include <Commctrl.h>
#include "resource.h"

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int position;
	PAINTSTRUCT ps;
	HDC hdc;
	LRESULT returnValue = 0;

	// find controller associated with window handle
	static Controller *ctrl;
	ctrl = (Controller*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (message == WM_NCCREATE)  // Non-Client Create
	{
		// WM_NCCREATE message is called before non-client parts(border,
		// titlebar, menu,etc) are created. This message comes with a pointer
		// to CREATESTRUCT in lParam. The lpCreateParams member of CREATESTRUCT
		// actually contains the value of lpPraram of CreateWindowEX().
		// First, retrieve the pointrer to the controller specified when
		// Win::Window is setup.
		ctrl = (Controller*)(((CREATESTRUCT*)lParam)->lpCreateParams);
		ctrl->setHandle(hWnd);

		// Second, store the pointer to the Controller into GWLP_USERDATA,
		// so, other messege can be routed to the associated Controller.
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ctrl);

		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	// check NULL pointer, because GWLP_USERDATA is initially 0, and
	// we store a valid pointer value when WM_NCCREATE is called.
	if (!ctrl)
		return ::DefWindowProc(hWnd, message, wParam, lParam);


	switch (message)
	{

	//case WM_NCCREATE:
		//SetWindowLongPtr(hWnd, GWLP_USERDATA, NULL);
		//break;
	case WM_CREATE:
		break;
	case WM_SIZE:
	{
		RECT rect;
		GetWindowRect(hwndForm, &rect);
		int formHeight = rect.bottom - rect.top;
		int formWidth = rect.right - rect.left;

		GetWindowRect(hwndGL, &rect);
		int glWidth = rect.right - rect.left;
		int glHeight = rect.bottom - rect.top;

		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		// resize the height of glWin and reposition glDialog & status bar
		//int glHeight = height - formHeight;
		SetWindowPos(hwndGL, 0, 10, 10, width - formWidth, height, SWP_NOZORDER);
		//::SetWindowPos(formHandle, 0, 0, glHeight, width, formHeight, SWP_NOZORDER);
		SetWindowPos(hwndForm, 0, width - formWidth, 10, formWidth, formHeight, SWP_NOZORDER);
		InvalidateRect(hwndForm, 0, TRUE);      // force to repaint
		//::SendMessage(statusHandle, WM_SIZE, 0, 0); // automatically resize width, so send 0s
		//::InvalidateRect(statusHandle, 0, FALSE);   // force to repaint

													// display OpenGL window dimension on the status bar
	}
		break;
	case WM_PAINT:

		break;
	case WM_CLOSE:
		returnValue = ctrl->close();
		break;
	
	case WM_LBUTTONDOWN:
		mouseButtonDown = true;
		break;
	case WM_LBUTTONUP:
		mouseButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		if (mouseButtonDown)
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			//��������, � ����� ����� ���� ��������� ����

		}
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_DEBUG_OPENFILE:
			OPENFILENAME ofn;
			HANDLE hf;
			char szFile[100];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = (LPWSTR)szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = NULL;//L"All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn) == TRUE)
			{
				hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
			}

			break;
		}

		break;
	case WM_HSCROLL:
		returnValue = ctrl->hScroll(wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return returnValue;
}


LRESULT CALLBACK WndProcGL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT returnValue = 0;        // return value
	// find controller associated with window handle
	static Controller *ctrl;
	ctrl = (Controller*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (message == WM_NCCREATE)  // Non-Client Create
	{
		// WM_NCCREATE message is called before non-client parts(border,
		// titlebar, menu,etc) are created. This message comes with a pointer
		// to CREATESTRUCT in lParam. The lpCreateParams member of CREATESTRUCT
		// actually contains the value of lpPraram of CreateWindowEX().
		// First, retrieve the pointrer to the controller specified when
		// Win::Window is setup.
		ctrl = (Controller*)(((CREATESTRUCT*)lParam)->lpCreateParams);
		ctrl->setHandle(hWnd);

		// Second, store the pointer to the Controller into GWLP_USERDATA,
		// so, other messege can be routed to the associated Controller.
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ctrl);

		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	// check NULL pointer, because GWLP_USERDATA is initially 0, and
	// we store a valid pointer value when WM_NCCREATE is called.
	if (!ctrl)
		return ::DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{

	//case WM_NCCREATE:
		//SetWindowLongPtr(hWnd, GWLP_USERDATA, NULL);
		//break;
	case WM_CREATE:
		returnValue = ctrl->create();
		break;
	case WM_PAINT:
	/*{
		PAINTSTRUCT ps;
		HDC hdc = GetDC(hWnd);
		BeginPaint(hWnd, &ps);
		GetWindowRect(hWnd, &window);
		Rectangle(ps.hdc, window.left + 5, window.top + 5, window.right - 10, window.bottom - 10);
		DefWindowProc(hWnd, message, wParam, lParam);
	}*/

		break;

	case WM_CLOSE:
		returnValue = ctrl->close();
		break;
	case WM_SIZE:
	{
		//int width = LOWORD(lParam);
		//int height = HIWORD(lParam);
		//SetWindowPos(hWnd, 0, 0, 0, width, height, SWP_NOZORDER);
		//InvalidateRect(hwndForm, 0, TRUE);
		returnValue = ctrl->size(LOWORD(lParam), HIWORD(lParam), (int)wParam);
	}
		break;
	case WM_LBUTTONDOWN:
		returnValue = ctrl->lButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); // state, x, y
		break;

	case WM_LBUTTONUP:
		returnValue = ctrl->lButtonUp(wParam, LOWORD(lParam), HIWORD(lParam));   // state, x, y
		break;

	case WM_RBUTTONDOWN:
		returnValue = ctrl->rButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); // state, x, y
		break;

	case WM_RBUTTONUP:
		returnValue = ctrl->rButtonUp(wParam, LOWORD(lParam), HIWORD(lParam));   // state, x, y
		break;


	case WM_MOUSEMOVE:
		returnValue = ctrl->mouseMove(wParam, LOWORD(lParam), HIWORD(lParam));  // state, x, y
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return returnValue;
}

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
		ctrl->hScroll(wParam, lParam);
		break;

	case WM_VSCROLL:
		break;

	case WM_SIZE:
		break;

	default:
		return false;
	}
	return true;
}