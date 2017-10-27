#pragma once
#include <windows.h>
#include "Controller.h"
#include "ControllerMain.h"

static bool mouseButtonDown;
static HINSTANCE globalhInst;

//static HWND hwndMain;
//static HWND hwndGL;
//static HWND hwndForm;

static RECT window;

static HBRUSH  hbrBkgnd;

enum TrackBarNames
{
	TRACKBARX = 10,
	TRACKBARY,
	TRACKBARZ,
	TRACKBARALPHA,
};

//HWND WINAPI CreateTrackbar(HWND hwndDlg, int posX, int posY, int sizeX, int sizeY, TrackBarNames name, int range);

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcGL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK FormDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);