#if _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#include "gl\include\glew.h"
#include "gl\include\wglew.h"

#pragma comment(lib, "gl\\lib\\glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include "ModelGL.h"



///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ModelGL::ModelGL()
{
  renderer = new CustomRenderer();
}



///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
ModelGL::~ModelGL()
{
}


int ModelGL::Init(HWND hWnd)
{
  renderer->Init(hWnd);

  return 1;
}


HDC ModelGL::GetDC(void)
{
  return renderer->hDC;
}


HGLRC ModelGL::GetRC(void)
{
  return renderer->hRC;
}


void ModelGL::Render(void)
{
  renderer->Render();
}

void ModelGL::SwapBuffers(void)
{
  renderer->CopyFrame();
}

void ModelGL::Close(void)
{
  renderer->Close();
}

void ModelGL::Resize(int w, int h)
{
  renderer->Resize(w, h);
}

void ModelGL::UpdateWheelPos(int wheelPos)
{
  renderer->UpdateWheelPos(wheelPos);
}


void ModelGL::Response()
{
  renderer->Response();
}

void ModelGL::UpdateMouseLButton(bool value)
{
  renderer->mouseLButtonDown = value;
}

void ModelGL::UpdateMouseRButton(bool value)
{
  renderer->mouseRButtonDown = value;
}

void ModelGL::LoadData(char* filename, long dataLen)
{
  renderer->LoadData(filename, true, dataLen);
}

void ModelGL::SetRect(int w, int h)
{
  renderer->w = w;
  renderer->h = h;
}

void ModelGL::SetAlpha(float alpha)
{
  renderer->opacity = alpha;
}

void ModelGL::SetClipPlaneL(float x, float y, float z)
{
  if (x >= 0)
    renderer->clipPlaneXL = x;
  else if (y >= 0)
    renderer->clipPlaneYL = y;
  else if (z >= 0)
    renderer->clipPlaneZL = z;
}

void ModelGL::SetClipPlaneR(float x, float y, float z)
{
  if (x >= 0)
    renderer->clipPlaneXR = x;
  else if (y >= 0)
    renderer->clipPlaneYR = y;
  else if (z >= 0)
    renderer->clipPlaneZR = z;
}

void ModelGL::SetSpherePos(float x, float y, float z)
{
  if (x >= 0)
    renderer->spherePos.x = x;
  else if (y >= 0)
    renderer->spherePos.y = y;
  else if (z >= 0)
    renderer->spherePos.z = z;
}

void ModelGL::SetSphereScale(float x, float y, float z)
{
  if (x >= 0)
    renderer->sphereScale.x = x;
  else if (y >= 0)
    renderer->sphereScale.y = y;
  else if (z >= 0)
    renderer->sphereScale.z = z;
}

void ModelGL::SetSphereRot(float x, float y)
{
  if (x <= 360)
    renderer->sphereRot.x = x;
  else if (y <= 360)
    renderer->sphereRot.y = y;
}

void ModelGL::ChangeCoordinateState(void)
{
  renderer->isCoordShown = !renderer->isCoordShown;
  renderer->CoordSetup();
}

void ModelGL::SendEllipseData()
{
  int pos[3] = { 0 };
  float scale[3] = { 0.0 };
  float angles[2] = { 0.0 };
  renderer->GetEllipseData(pos, scale, angles);
  EllipseData *data = new EllipseData();

  memcpy(data->center, pos, sizeof(int) * 3);
  memcpy(data->scale, scale, sizeof(float) * 3);
  memcpy(data->angles, angles, sizeof(float) * 2);
  (*sendEllipseFunc)(data);
  delete data;
}