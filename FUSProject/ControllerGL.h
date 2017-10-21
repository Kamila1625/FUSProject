#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include "Controller.h"
#include "ModelGL.h"



    class ControllerGL : public Controller
    {
    public:
        ControllerGL(ModelGL* model);
        ~ControllerGL() {};

        int close();                                // close the RC and OpenGL window
        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int create();                               // create RC for OpenGL window and start new thread for rendering
        int paint();
        int size(int w, int h, WPARAM wParam);
        int lButtonDown(WPARAM state, int x, int y);
        int lButtonUp(WPARAM state, int x, int y);
        int rButtonDown(WPARAM state, int x, int y);
        int rButtonUp(WPARAM state, int x, int y);
        int mouseMove(WPARAM state, int x, int y);

    private:
        void runThread();                           // thread for OpenGL rendering

        ModelGL* model;                             //
        std::thread glThread;                       // opengl rendering thread object
        volatile bool loopFlag;                     // rendering loop flag
    };



