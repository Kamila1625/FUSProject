#pragma once

#include "Controller.h"
#include "ModelGL.h"
#include "Controls.h"

enum EditTypeText
{
	CUBE_TYPE,
	SPHERE_TYPE, 
	NULL_TYPE,
};

class ControllerForm : public Controller
{
public:
  ControllerForm(ModelGL* model);
  ~ControllerForm() {};

  int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
  int create();                               // for WM_CREATE
  int hScroll(WPARAM wParam, LPARAM lParam);  // for WM_HSCROLL

  void SetEditText(EditTypeText type);

	void updateTrackbars(HWND handle, int position);
	void initControls(HWND handle);

  void UpdateModel(int trackbarId, int position);

private:
  ModelGL* model;                             // pointer to model component

	Button addSphere;
	Button delSphere;

	Button addCube;
	Button delCube;

	Button buttonCube;
	Button buttonSlices;

	Button buttonCoord;

    Button buttonScan;

	TextBox editCubeSphere;
	TextBox scanFreqText;

	//RadioButton radioSlice;
	//RadioButton radioCube;

	Trackbar trackbarFreq;

	Trackbar trackbarXLeft;
	Trackbar trackbarXRight;
	Trackbar trackbarYLeft;
	Trackbar trackbarYRight;
	Trackbar trackbarZLeft;
	Trackbar trackbarZRight;
	Trackbar trackbarAlpha;

	Trackbar trackbarPhi;
	Trackbar trackbarPsy;
	Trackbar trackbarPosX;
	Trackbar trackbarPosY;
	Trackbar trackbarPosZ;
	Trackbar trackbarSizeX;
	Trackbar trackbarSizeY;
	Trackbar trackbarSizeZ;
};


