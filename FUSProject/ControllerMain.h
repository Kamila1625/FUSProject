#pragma once

#include "Controller.h"


    class ControllerMain :public Controller
    {
    public:
        ControllerMain();
        ~ControllerMain() {};

        void setGLHandle(HWND handle)               { glHandle = handle; }
        void setFormHandle(HWND handle)             { formHandle = handle; }

        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int close();
        int create();                               // for WM_CRERATE
        int destroy();
        int size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

    private:
        HWND glHandle;                              // handle of gl window
        HWND formHandle;                            // handle of form dialog

    };

