#pragma once

#include "CustomRenderer.h"
#include "FusExport.h"

class ModelGL
{
  
public:
  CustomRenderer *renderer;
  SendEllipseDataFunction sendEllipseFunc;

  ModelGL();
  ~ModelGL();

  int Init(HWND hWnd);

  HDC GetDC(void);

  HGLRC GetRC(void);

  void Render(void);

  void SwapBuffers(void);

  void Close(void);

  void Resize(int w, int h);

  void UpdateWheelPos(int wheelPos);

  void Response();

  void UpdateMouseLButton(bool value);

  void UpdateMouseRButton(bool value);

  void LoadData(char* filename, long dataLen = 0);

  void SetRect(int w, int h);

  void SetAlpha(float alpha);

  void SetClipPlaneL(float x, float y, float z);

  void SetClipPlaneR(float x, float y, float z);

  void SetSpherePos(float x, float y, float z);

  void SetSphereScale(float x, float y, float z);

  void SetSphereRot(float x, float y);

  void ChangeCoordinateState(void);  

  void SendEllipseData();
};

