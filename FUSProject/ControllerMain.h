#pragma once

#include "Controller.h"


    class ControllerMain :public Controller
    {
    public:
        ControllerMain();
        ~ControllerMain() {};

        void setGLHandle(HWND handle)               { hwndGL = handle; }
        void setFormHandle(HWND handle)             { hwndForm = handle; }

        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int close();
        int create();                               // for WM_CRERATE
        int destroy();
        int size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

        void SendData(char *fileName);
		
    private:
		HWND hwndGL;                              // handle of gl window
		HWND hwndForm;                            // handle of form dialog


    };

