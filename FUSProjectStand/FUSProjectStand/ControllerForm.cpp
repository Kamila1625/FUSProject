#include <process.h>
#include <locale>
#include <string>
#include <iostream>

#include "FusExport.h"
#include "ControllerForm.h"
#include "resource.h"

ControllerForm* globalCtrlForEllipse;

// Globals

void __stdcall SendEllipseData(const EllipseData *data)
{
  globalCtrlForEllipse->ReturnEllipseText(data);
}

///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerForm::ControllerForm(Data* d)
{
	data = d;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerForm::create()
{
    // initialize all controls
    initControls(handle);

	globalCtrlForEllipse = this;

    // place the opengl form dialog in right place, bottome of the opengl rendering window
    //RECT rect = {0, 0, 4, 8};
    //::MapDialogRect(glDialog.getHandle(), &rect);
    //int width = MulDiv(300, 4, rect.right);
    //::SetWindowPos(handle, HWND_TOP, 0, 300, 300, 200, 0);

    return 0;
}

void ControllerForm::initControls(HWND handle)
{
	// set all controls
	handle = handle;
	buttonLoadLib.set(handle, ID_LOAD_LIB);
	buttonLoadData.set(handle, ID_LOAD_DATA);
	buttonSendData.set(handle, ID_SEND_DATA);
	buttonSendData2.set(handle, ID_SEND_DATA2);
	buttonStopLib.set(handle, ID_STOP_LIB);
	ellipseText.set(handle, IDC_EDIT1);
	cubeText.set(handle, IDC_EDIT2);
	freqText.set(handle, IDC_EDIT3);
}

void ControllerForm::ReturnEllipseText(const EllipseData* data)
{
	SetForegroundWindow(handle); //������� ������ � �������� ���� ��� ������

	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > conv;
	std::string str;
	str = "(" + std::to_string(data->center[0]) + ", " + std::to_string(data->center[1]) + ", " + std::to_string(data->center[2]) + "), ";
  str = str + "(" + std::to_string(data->scale[0]) + ", " + std::to_string(data->scale[1]) + ", " + std::to_string(data->scale[2]) + "), ";
	str = str + "(" + std::to_string(data->angles[0]) + ", " + std::to_string(data->angles[1]) + ")";

	std::wstring wstr = conv.from_bytes(str);

	ellipseText.setText(const_cast<wchar_t*>(wstr.c_str())); 
}

void ControllerForm::InitLibFunctions()
{
  initFunc = (InitFunction)GetProcAddress(libModule, "_Init@8");
  sendDataFunc = (SendDataFunction)GetProcAddress(libModule, "_SendData@8");
  stopFunc = (StopFunction)GetProcAddress(libModule, "_Stop@0");
}


///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
int ControllerForm::command(int id, int command, LPARAM msg)
{
    //static bool flag = false;
	
    switch(id)
    {
    case ID_LOAD_LIB:
        if(command == BN_CLICKED)
        {
			    libModule = LoadLibraryA("FUSProject.dll");
          InitLibFunctions();
          //char buffer[MAX_PATH];
          //GetModuleFileNameA(NULL, buffer, MAX_PATH);
			    if (libModule == NULL)
			    {
            printf("Error");
            return -1;
			    } 
          else 
          {
            SendEllipseDataFunction func = &SendEllipseData;
            initFunc(handle, func);
          }          
        }
        break;

    case ID_LOAD_DATA:
        if(command == BN_CLICKED)
        {
			    OPENFILENAMEA ofn;
			    char szFile[512];

			    ZeroMemory(&ofn, sizeof(ofn));
			    ofn.lStructSize = sizeof(ofn);
			    ofn.hwndOwner = handle;
			    ofn.lpstrFile = szFile;
			    ofn.lpstrFile[0] = '\0';
			    ofn.nMaxFile = sizeof(szFile);
			    ofn.lpstrFilter = NULL;//L"All\0*.*\0Text\0*.CPT\0";
			    ofn.nFilterIndex = 1;
			    ofn.lpstrFileTitle = NULL;
			    ofn.nMaxFileTitle = 0;
			    ofn.lpstrInitialDir = NULL;
			    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			    int res = 0;
			    if (GetOpenFileNameA(&ofn) == TRUE)
			    {
				    res = data->ReadData(szFile);
			    }
			    //res = res + 1;
        }
        break;

    case ID_SEND_DATA:
        if(command == BN_CLICKED)
        {
			    //some test
			    /*EllipseData dataTest;
			    dataTest.center[0] = 2;
			    dataTest.center[1] = 4;
			    dataTest.center[2] = 8;
			    dataTest.angles[0] = 2.5f;
			    dataTest.angles[1] = 3.18f;

			    //ReturnEllipseText(&dataTest);*/

          sendDataFunc(data->GetDataPointer(), data->GetDataSize());
			    //if (dataToSend == NULL)
			    //{
				   // //some test
				   // EllipseData dataTest;
				   // dataTest.center[0] = 3;
				   // dataTest.center[1] = 4;
				   // dataTest.center[2] = 5;
				   // dataTest.angles[0] = 0.0f;
				   // dataTest.angles[1] = 0.18f;

				   // ReturnEllipseText(&dataTest);
			    //}
        }
        break;

    case ID_STOP_LIB:
        if(command == BN_CLICKED)
        {
		      if (libModule != NULL)
		      {
            stopFunc();
            FreeLibrary(libModule);
		      }
        }
        break;
    }
	
    return 0;
}

void ControllerForm::UpdateModel(int trackbarId, int position)
{
  /*switch (trackbarId)
  {
  case IDC_SLIDER1: // trackbarXLeft
    model->SetClipPlaneL((float)position / 100.0f, -1, -1);
    break;

  case IDC_SLIDER2: // trackbarYLeft
    model->SetClipPlaneL(-1, (float)position / 100.0f, -1);
    break;

  case IDC_SLIDER3: // trackbarZLeft
    model->SetClipPlaneL(-1, -1, (float)position / 100.0f);
    break;

  case IDC_SLIDER4: // trackbarAlpha
    model->SetAlpha((float)position / 100.0f);
    break;

  case IDC_SLIDER5: // trackbarXRight
    model->SetClipPlaneR((float)position / 100.0f, -1, -1);
    break;

  case IDC_SLIDER6: // trackbarYRight          
    model->SetClipPlaneR(-1, (float)position / 100.0f, -1);
    break;

  case IDC_SLIDER7: // trackbarZRight
    model->SetClipPlaneR(-1, -1, (float)position / 100.0f);
    break;

  case IDC_SLIDER8: // trackbarPhi
    model->SetSphereRot((float)position, 400);
    break;

  case IDC_SLIDER9: // trackbarPsy
    model->SetSphereRot(400, (float)position);
    break;

  case IDC_SLIDER10: // trackbarPosX
    model->SetSpherePos((float)position / 500.0f, -1, -1);
    break;

  case IDC_SLIDER11: // trackbarPosY
    model->SetSpherePos(-1, (float)position / 500.0f, -1);
    break;

  case IDC_SLIDER12: // trackbarPosZ
    model->SetSpherePos(-1, -1, (float)position / 500.0f);
    break;

  case IDC_SLIDER13: // trackbarSizeX
    model->SetSphereScale((float)position / 250.0f, -1, -1);
    break;

  case IDC_SLIDER14: // trackbarSizeY
    model->SetSphereScale(-1, (float)position / 250.0f, -1);
    break;

  case IDC_SLIDER15: // trackbarSizeZ
    model->SetSphereScale(-1, -1, (float)position / 250.0f);
    break;
	
  }*/
}

void ControllerForm::updateTrackbars(HWND handle, int position)
{
	/*if (handle == trackbarXLeft.getHandle())
	{
		trackbarXLeft.setPos(position);
	}
	else if (handle == trackbarXRight.getHandle())
	{
		trackbarXRight.setPos(position);
	}
	else if (handle == trackbarYLeft.getHandle())
	{
		trackbarYLeft.setPos(position);
	}
	else if (handle == trackbarYRight.getHandle())
	{
		trackbarYRight.setPos(position);
	}
	else if (handle == trackbarZLeft.getHandle())
	{
		trackbarZLeft.setPos(position);
	}
	else if (handle == trackbarZRight.getHandle())
	{
		trackbarZRight.setPos(position);
	}
	else if (handle == trackbarAlpha.getHandle())
	{
		trackbarAlpha.setPos(position);
	}
	else if (handle == trackbarPhi.getHandle())
	{
		trackbarPhi.setPos(position);
	}
	else if (handle == trackbarPsy.getHandle())
	{
		trackbarPsy.setPos(position);
	}
	else if (handle == trackbarPosX.getHandle())
	{
		trackbarPosX.setPos(position);
	}
	else if (handle == trackbarPosY.getHandle())
	{
		trackbarPosY.setPos(position);
	}
	else if (handle == trackbarPosZ.getHandle())
	{
		trackbarPosZ.setPos(position);
	}
	else if (handle == trackbarSizeX.getHandle())
	{
		trackbarSizeX.setPos(position);
	}
	else if (handle == trackbarSizeY.getHandle())
	{
		trackbarSizeY.setPos(position);
	}
	else if (handle == trackbarSizeZ.getHandle())
	{
		trackbarSizeZ.setPos(position);
	}*/
}