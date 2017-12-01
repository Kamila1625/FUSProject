#include "StandDialog.h"
#include "proc.h"

StandDialog::StandDialog(HINSTANCE hInst, WORD id, HWND hParent, Controller* ctrl) : handle(0), instance(hInst), controller(ctrl), id(id), parentHandle(hParent), x(0), y(0)
{

}

StandDialog::~StandDialog()
{

}

HWND StandDialog::create()
{
	handle = ::CreateDialogParam(instance, MAKEINTRESOURCE(id), parentHandle, FormDialogProc, (LPARAM)controller);
	return handle;
}

void StandDialog::show(int cmdShow)
{
	::ShowWindow(handle, cmdShow);
	::UpdateWindow(handle);
}
