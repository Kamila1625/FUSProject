#pragma once  

#include <windows.h>

struct EllipseData
{
  int center[3];
  float scale[3];
  float angles[2];
};

typedef void(__stdcall *SendEllipseDataFunction)(const EllipseData*);

#ifdef DLL_EXPORT  
#define FUS_API __declspec(dllexport)   
#else  
#define FUS_API __declspec(dllimport)   

typedef bool(__stdcall *InitFunction)(HWND, SendEllipseDataFunction);
typedef void(__stdcall *SendDataFunction)(void *, long);
typedef void(__stdcall *StopFunction)(void);

#endif  

#ifdef __cplusplus
extern "C" {
#endif

  FUS_API bool __stdcall Init(HWND hWnd, SendEllipseDataFunction sendEllipseFunc);
  FUS_API void __stdcall SendData(void *data, long dataLen);
  FUS_API void __stdcall Stop();

#ifdef __cplusplus
}
#endif