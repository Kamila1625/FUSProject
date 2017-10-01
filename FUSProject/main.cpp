#include "windows.h"
#include "windowsx.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
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
	case WM_LBUTTONUP:
		MessageBox(hWnd, L"Вы кликнули!", L"событие", 0);
		break;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;
		
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		
	}
	return 0;
} 