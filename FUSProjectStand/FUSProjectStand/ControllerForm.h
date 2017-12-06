#pragma once
#include "Controls.h"
#include "Controller.h"
//#include "ModelGL.h"
#include "Ellipse.h"
#include "Data.h"

class ControllerForm : public Controller
{
public:
  //ControllerForm(ModelGL* model);
	ControllerForm(Data* d);
  ~ControllerForm() {};

  int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
  int create();                               // for WM_CREATE
  //int hScroll(WPARAM wParam, LPARAM lParam);  // for WM_HSCROLL

	void updateTrackbars(HWND handle, int position);
	void initControls(HWND handle);

  void UpdateModel(int trackbarId, int position);

  void ReturnEllipseText(EllipseData* data);

private:
	//ModelGL* model;                             // pointer to model component
	//HWND hwndForm;                               // handle of form dialog
	Data* data;
	Button buttonLoadLib;
	Button buttonLoadData;
	Button buttonSendData;
	Button buttonStopLib;
	TextBox ellipseText;
	HINSTANCE libModule;
};


