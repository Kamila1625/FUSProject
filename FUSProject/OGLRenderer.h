#ifndef _OGLRENDERER_H_
#define _OGLRENDERER_H_

#define GLEW_STATIC

#include <windows.h>

#include "gl\include\glew.h"
#include "gl\include\wglew.h"

#pragma comment(lib, "gl\\lib\\glew32s.lib")
#pragma comment(lib, "opengl32.lib")

class OGLRenderer
{
public:
  HWND hWnd;                                /* Window descriptor */
  HDC hDC;                                  /* Memory context */
  HGLRC hRC;                                /* Build context */
  INT w, h;                                 /* Output sreen sizes */

  double globalTime;                        /* Time from which system starts( without stops ) */
  double globalDelta;                       /* Time from last frame( without stops ) */
  double time;                              /* Time from which system starts( with stops ) */
  double delta;                             /* Time from last frame( with stops ) */
  
  bool isFullScreen;                        /* Full screen flag */

  double fps;                               /* Frames-per-seconds */
  byte kbd[256];                            /* Keyboard state array */
  double mx, my, dmx, dmy;                  /* Mouse coordinates and difference between now and previous frame */
                                              
  __int64 startTime, oldTime, lastTime, timesPerSecond, pauseTime; /* Time counters */
  __int64 frameCount;                       /* Frame counter */
  
  virtual void CustomInit(void) = 0;        /* User initializing function */
  virtual void CustomRender(void) = 0;      /* User building-frame function */
  virtual void CustomResponse(void) = 0;    /* User response function */
  virtual void CustomReshape(void) = 0;     /* Changing window sizes user function */
  virtual void CustomClose(void) = 0;       /* User closing function */

  OGLRenderer()
  {
  }

  
  /* System initialization function */
  void Init(HWND hWnd);

  /* System closing function */
  void Close(void);

  /* Changing window sizes function */
  void Resize(int w, int h);

  /* System reaction function */
  void Response(void);

  /* System frame building function */
  void Render(void);

  /* Frame counting function */
  void CopyFrame(void);

  /* Full screen function */
  void SetFullScreen(bool goToFS);

  /* Reset rendering context */
  void ResetRenderingContext(bool cleanUp = false);
};

#endif /* _OGLRENDERER_H_ */