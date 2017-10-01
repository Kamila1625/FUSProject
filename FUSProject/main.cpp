#include "windows.h"
#include "windowsx.h"

enum MenuNames
{
	MENU_3D_WINDOW,
	MENU_CORONAL,
	MENU_SAGITTAL,
	MENU_AXIAL,
	DEBUG_OPEN_FILE
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);

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
	switch (message)
	{
	case WM_CREATE:
		AddMenus(hWnd);
		break;
	//case WM_LBUTTONUP:
		//MessageBox(hWnd, L"Вы кликнули!", L"Cобытие", 0);
		//break;
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
			ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
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