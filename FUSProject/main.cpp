#include <windows.h>
#include <windowsx.h>
#include <Commctrl.h>

enum MenuNames
{
	MENU_3D_WINDOW,
	MENU_CORONAL,
	MENU_SAGITTAL,
	MENU_AXIAL,
	DEBUG_OPEN_FILE
};

enum TrackBarNames
{
	TRACKBARX = 10,
	TRACKBARY,
	TRACKBARZ,
	TRACKBARALPHA,
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
HWND WINAPI CreateTrackbar(HWND hwndDlg, int posX, int posY, int sizeX, int sizeY, TrackBarNames name, int range);

bool mouseButtonDown;
HINSTANCE globalhInst;
HWND hwndTrackX;
HWND hwndTrackY;
HWND hwndTrackZ;
HWND hwndTrackA;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = WndProc;
	w.hInstance = hInstance;
	w.hbrBackground = GetStockBrush(WHITE_BRUSH);
	w.lpszClassName = L"FUS Project";
	RegisterClass(&w);
	hwnd = CreateWindow(L"FUS Project", L"FUS Project", WS_OVERLAPPEDWINDOW,
		10, 10, 600, 480, NULL, NULL, hInstance, NULL);
	nCmdShow = SW_MAXIMIZE;

	globalhInst = hInstance;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int position;

	switch (message)
	{
	case WM_CREATE:
		AddMenus(hWnd);
		hwndTrackX = CreateTrackbar(hWnd, 1500, 20, 200, 30, TRACKBARX, 50);
		hwndTrackY = CreateTrackbar(hWnd, 1500, 50, 200, 30, TRACKBARY, 50);
		hwndTrackZ = CreateTrackbar(hWnd, 1500, 80, 200, 30, TRACKBARZ, 50);
		hwndTrackA = CreateTrackbar(hWnd, 1500, 110, 200, 30, TRACKBARALPHA, 50);
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
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case DEBUG_OPEN_FILE:
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
		if (LOWORD(wParam) == SB_THUMBPOSITION)
		{
			position = HIWORD(wParam);
			if ((HWND)lParam == hwndTrackX)
			{
				//position = position + 1;
			}
			else if ((HWND)lParam == hwndTrackY)
			{
				//position = position + 1;

			}
			else if ((HWND)lParam == hwndTrackZ)
			{
				//position = position + 2;
			}
			else if ((HWND)lParam == hwndTrackA)
			{
				//position = position + 5;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		
	}
	return 0;
} 

void AddMenus(HWND hwnd) 
{

	HMENU hMenubar;
	HMENU hMenu, hMenuDebug;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();
	hMenuDebug = CreateMenu();

	AppendMenuW(hMenuDebug, MF_STRING, DEBUG_OPEN_FILE, L"&Open File");

	AppendMenuW(hMenu, MF_STRING, MENU_3D_WINDOW, L"&3D Window");
	AppendMenuW(hMenu, MF_STRING, MENU_CORONAL, L"&Coronal Window");
	AppendMenuW(hMenu, MF_STRING, MENU_SAGITTAL, L"&Sagittal Window");
	AppendMenuW(hMenu, MF_STRING, MENU_AXIAL, L"&Axial Window");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Menu");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuDebug, L"&Debug");
	SetMenu(hwnd, hMenubar);
}


HWND WINAPI CreateTrackbar(HWND hwndDlg, int posX, int posY, int sizeX, int sizeY, TrackBarNames name, int range)
{
	HWND hwndTrack = CreateWindowEx(
		0,                               // no extended styles 
		TRACKBAR_CLASS,                  // class name 
		L"Trackbar",                 // title (caption) 
		WS_CHILD |
		WS_VISIBLE | TBS_NOTICKS,			// style 
		posX, posY,                          // position 
		sizeX, sizeY,                         // size 
		hwndDlg,                         // parent window 
		(HMENU)name,							 // control identifier 
		globalhInst,                         // instance 
		NULL                             // no WM_CREATE parameter 
	);

	SendMessage(hwndTrack, TBM_SETRANGE,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)MAKELONG(0, range));  // min. & max. positions

	SendMessage(hwndTrack, TBM_SETPAGESIZE,
		0, (LPARAM)1);                  // new page size 

	SendMessage(hwndTrack, TBM_SETSEL,
		(WPARAM)FALSE,                  // redraw flag 
		(LPARAM)MAKELONG(0, 0));

	SendMessage(hwndTrack, TBM_SETPOS,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)0);

	SetFocus(hwndTrack);

	return hwndTrack;
}

