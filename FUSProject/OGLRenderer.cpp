#include <string.h>
#include <assert.h>

#include "OGLRenderer.h"

//static OGLRenderer renderer;

/* System initialization function */
void OGLRenderer::Init(HWND hWnd)
{
  int mode;
  LARGE_INTEGER time;
  PIXELFORMATDESCRIPTOR pd = {0};

  assert(hWnd != NULL);

  //memset(&renderer, 0, sizeof(renderer));

  /* Initing contexts */
  hDC = GetDC(hWnd);
  assert(hDC != NULL);

  this->hWnd = hWnd;

  mode = GetPixelFormat(hDC);
  DescribePixelFormat(hDC, mode, sizeof(PIXELFORMATDESCRIPTOR), &pd);

  /* Pixel format */
  pd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pd.nVersion = 1;
  pd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pd.iPixelType = PFD_TYPE_RGBA;
  pd.cColorBits = 32;
  pd.cDepthBits = 24;
  pd.cStencilBits = 8;

  /* Appopriate format for pixel */
  mode = ChoosePixelFormat(hDC, &pd);
  DescribePixelFormat(hDC, mode, sizeof(PIXELFORMATDESCRIPTOR), &pd);

  if (!SetPixelFormat(hDC, mode, &pd))
  {
    MessageBox(hWnd, L"Too bad!....", L"ERROR", MB_OK | MB_ICONSTOP);
    exit(0);
  }

  /* Build-up context */
  HGLRC tempHRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, tempHRC);

  /* Default sizes of window */
  w = 800;
  h = 600;    

  /* Starting time counters */
  QueryPerformanceCounter(&time);
  startTime = oldTime = lastTime = time.QuadPart;
  QueryPerformanceFrequency(&time);
  timesPerSecond = time.QuadPart;

  /* Setup GLEW */
  GLenum error = glewInit(); 
  if (error != GLEW_OK) 
  {
    MessageBox(hWnd, L"Too bad! GLEW is done for...", L"ERROR", MB_OK | MB_ICONSTOP);
    exit(0);
  }

  int attributes[] = 
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,                              // MAJOR version of OpenGL
    WGL_CONTEXT_MINOR_VERSION_ARB, 2,                              // MINOR version of OpenGL
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // forward compatible context
    0
  };

  /* If the OpenGL 3.x context creation extension is available */
  if (wglewIsSupported("WGL_ARB_create_context") == 1) 
  { 
    hRC = wglCreateContextAttribsARB(hDC, NULL, attributes);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempHRC); 
    wglMakeCurrent(hDC, hRC);
  }
  else // OpenGL ver. is < 2.0
  {
    hRC = tempHRC;
  }  

  /* User function */
  CustomInit();
}


/* System closing function */
void OGLRenderer::Close(void)
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hWnd, hDC);

  CustomClose();
}


/* Changing window sizes function */
void OGLRenderer::Resize(int w, int h)
{
  this->w = w;
  this->h = h;
  
  glViewport(0, 0, w, h);  

  CustomReshape();
}


/* System reaction function */
void OGLRenderer::Response(void)
{
  INT i;
  POINT pt;
  LARGE_INTEGER time;

  /* Set main time counters */
  QueryPerformanceCounter(&time);
  globalTime = (double)(time.QuadPart - startTime) / timesPerSecond;
  globalDelta = (double)(time.QuadPart - oldTime) / timesPerSecond;

  /* FPS counting */
  if (time.QuadPart - lastTime > timesPerSecond)
  {
    fps = (double)frameCount / (time.QuadPart - lastTime) * timesPerSecond;
    lastTime = time.QuadPart;
    frameCount = 0;
  }

  delta = globalDelta;
  oldTime = time.QuadPart;

  /* Keyboard */
  GetKeyboardState(kbd);
  for (i = 0; i < 256; i++)
    kbd[i] >>= 7;

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  dmx = pt.x - mx;
  dmy = pt.y - my;
  mx = pt.x;
  my = pt.y;

  /* User function */  
  CustomResponse();
}


/* System frame building function */
void OGLRenderer::Render(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0, 0, 0, 0);

  /* User function */  
  CustomRender();  
}


/* Frame counting function */
void OGLRenderer::CopyFrame(void)
{
  SwapBuffers(hDC);
  frameCount++;
}


/* Full screen function */
void OGLRenderer::SetFullScreen(bool goToFS)
{
  RECT rc;
  DWORD style;
  static RECT saveRC;

  if (goToFS == isFullScreen)
    return;
  
  if ((isFullScreen = goToFS) != FALSE)
  {
    GetWindowRect(hWnd, &saveRC);

    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

    style = GetWindowLong(hWnd, GWL_STYLE);
    AdjustWindowRect(&rc, style, FALSE);
    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOREDRAW | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);
  }
  else
    SetWindowPos(hWnd, HWND_TOP, saveRC.left, saveRC.top, saveRC.right - saveRC.left, saveRC.bottom - saveRC.top,
      SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);
}
