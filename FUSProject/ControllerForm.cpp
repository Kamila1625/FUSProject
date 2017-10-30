#include <process.h>                              
#include "ControllerForm.h"
#include "resource.h"




///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerForm::ControllerForm(ModelGL* model) : model(model)
{
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerForm::create()
{
    // initialize all controls
    initControls(handle);

    // place the opengl form dialog in right place, bottome of the opengl rendering window
    //RECT rect = {0, 0, 4, 8};
    //::MapDialogRect(glDialog.getHandle(), &rect);
    //int width = MulDiv(300, 4, rect.right);
    //::SetWindowPos(handle, HWND_TOP, 0, 300, 300, 200, 0);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
int ControllerForm::command(int id, int command, LPARAM msg)
{
    //static bool flag = false;

    switch(id)
    {
    case ID_ADD:
        if(command == BN_CLICKED)
        {
            
        }
        break;

    case ID_DEL:
        if(command == BN_CLICKED)
        {
            
        }
        break;

    case ID_CUBE:
        if(command == BN_CLICKED)
        {
            
        }
        break;

    case ID_SLICES:
        if(command == BN_CLICKED)
        {
            
        }
        break;

	case ID_SK:
		if (command == BN_CLICKED)
		{
			
		}
		break;
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle horizontal scroll notification
///////////////////////////////////////////////////////////////////////////////
int ControllerForm::hScroll(WPARAM wParam, LPARAM lParam)
{
    // check if the message comming from trackbar
    HWND trackbarHandle = (HWND)lParam;

    int position = HIWORD(wParam);              // current tick mark position
    if (trackbarHandle)
    {
        // get control ID
        int trackbarId = ::GetDlgCtrlID(trackbarHandle);

        switch(LOWORD(wParam))
        {
        case TB_THUMBTRACK:     // user dragged the slider
          updateTrackbars(trackbarHandle, position);
          UpdateModel(trackbarId, position);             			                          
          break;

        case TB_THUMBPOSITION:  // by WM_LBUTTONUP
            break;

        case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
            break;

        case TB_LINEDOWN:       // by VK_LEFT, VK_UP
            break;

        case TB_TOP:            // by VK_HOME
            break;

        case TB_BOTTOM:         // by VK_END
            break;

        case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
            break;

        case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
            break;

        case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
            position = (int)::SendMessage(trackbarHandle, TBM_GETPOS, 0, 0);
            updateTrackbars(trackbarHandle, position);			
            UpdateModel(trackbarId, position);
            break;
        }
    }

    return 0;
}

void ControllerForm::UpdateModel(int trackbarId, int position)
{
  switch (trackbarId)
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

  default:
    break;
  }
}


void ControllerForm::updateTrackbars(HWND handle, int position)
{
	if (handle == trackbarXLeft.getHandle())
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
	}
}

void ControllerForm::initControls(HWND handle)
{
	// set all controls

	addSphere.set(handle, ID_ADD);
	delSphere.set(handle, ID_DEL);

	buttonCube.set(handle, ID_CUBE);
	buttonSlices.set(handle, ID_SLICES);

	buttonCoord.set(handle, ID_SK);

	//radioCube.set(handle, IDC_RADIO2);
	//radioSlice.set(handle, IDC_RADIO1);

	trackbarXLeft.set(handle, IDC_SLIDER1);
	trackbarXRight.set(handle, IDC_SLIDER5);
	trackbarYLeft.set(handle, IDC_SLIDER2);
	trackbarYRight.set(handle, IDC_SLIDER6);
	trackbarZLeft.set(handle, IDC_SLIDER3);
	trackbarZRight.set(handle, IDC_SLIDER7);
	trackbarAlpha.set(handle, IDC_SLIDER4);

	trackbarPhi.set(handle, IDC_SLIDER8);
	trackbarPsy.set(handle, IDC_SLIDER9);
	trackbarPosX.set(handle, IDC_SLIDER10);
	trackbarPosY.set(handle, IDC_SLIDER11);
	trackbarPosZ.set(handle, IDC_SLIDER12);
	trackbarSizeX.set(handle, IDC_SLIDER13);
	trackbarSizeY.set(handle, IDC_SLIDER14);
	trackbarSizeZ.set(handle, IDC_SLIDER15);

	//radioCube.check();

	trackbarPhi.setRange(-90, 90);
	trackbarPsy.setRange(-90, 90);
	trackbarPosX.setRange(0, 500);
	trackbarPosY.setRange(0, 500);
	trackbarPosZ.setRange(0, 500);
	trackbarSizeX.setRange(0, 500);
	trackbarSizeY.setRange(0, 500);
	trackbarSizeZ.setRange(0, 500);

	trackbarXLeft.setRange(0, 100);
	trackbarXRight.setRange(0, 100);
	trackbarYLeft.setRange(0, 100);
	trackbarYRight.setRange(0, 100);
	trackbarZLeft.setRange(0, 100);
	trackbarZRight.setRange(0, 100);
	trackbarAlpha.setRange(0, 100);

	trackbarXLeft.setPos(0);
	trackbarXRight.setPos(100);
	trackbarYLeft.setPos(0);
	trackbarYRight.setPos(100);
	trackbarZLeft.setPos(0);
	trackbarZRight.setPos(100);
	trackbarAlpha.setPos(0);

	trackbarPhi.setPos(0);
	trackbarPsy.setPos(0);
	trackbarPosX.setPos(249);
	trackbarPosY.setPos(249);
	trackbarPosZ.setPos(249);
	trackbarSizeX.setPos(249);
	trackbarSizeY.setPos(249);
	trackbarSizeZ.setPos(249);


}

