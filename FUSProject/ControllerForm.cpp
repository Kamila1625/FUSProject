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
    static bool flag = false;

    switch(id)
    {
    /*case IDC_ANIMATE:
        if(command == BN_CLICKED)
        {
            flag = !flag;
            model->animate(flag);
            view->animate(flag);
        }
        break;

    case IDC_FILL:
        if(command == BN_CLICKED)
        {
            model->setDrawMode(0);
        }
        break;

    case IDC_WIREFRAME:
        if(command == BN_CLICKED)
        {
            model->setDrawMode(1);
        }
        break;

    case IDC_POINT:
        if(command == BN_CLICKED)
        {
            model->setDrawMode(2);
        }
        break;*/
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
    if(trackbarHandle)
    {
        // get control ID
        int trackbarId = ::GetDlgCtrlID(trackbarHandle);

        switch(LOWORD(wParam))
        {
        case TB_THUMBTRACK:     // user dragged the slider
            updateTrackbars(trackbarHandle, position);
			/*if (trackbarId == IDC_SLIDER1)
				model->setBackgroundRed(position / 255.0f);
			else if (trackbarId == IDC_SLIDER2)
				model->setBackgroundGreen(position / 255.0f);
			else if (trackbarId == IDC_SLIDER3)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER4)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER5)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER6)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER7)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER8)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER9)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER10)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER11)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER12)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER13)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER14)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER15)
				model->setBackgroundBlue(position / 255.0f);*/
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
			/*if (trackbarId == IDC_SLIDER1)
				model->setBackgroundRed(position / 255.0f);
			else if (trackbarId == IDC_SLIDER2)
				model->setBackgroundGreen(position / 255.0f);
			else if (trackbarId == IDC_SLIDER3)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER4)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER5)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER6)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER7)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER8)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER9)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER10)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER11)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER12)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER13)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER14)
				model->setBackgroundBlue(position / 255.0f);
			else if (trackbarId == IDC_SLIDER15)
				model->setBackgroundBlue(position / 255.0f);*/
            break;
        }
    }

    return 0;
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
	}//
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

	radioCube.set(handle, IDC_RADIO2);
	radioSlice.set(handle, IDC_RADIO1);

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

	radioCube.check();

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

