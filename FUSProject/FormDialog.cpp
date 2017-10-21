#include "FormDialog.h"
#include "proc.h"

FormDialog::FormDialog(HINSTANCE hInst, WORD id, HWND hParent, Controller* ctrl) : handle(0), instance(hInst), controller(ctrl), id(id), parentHandle(hParent), x(0), y(0)
{

}

FormDialog::~FormDialog()
{

}

HWND FormDialog::create()
{
	handle = ::CreateDialogParam(instance, MAKEINTRESOURCE(id), parentHandle, FormDialogProc, (LPARAM)controller);
	return handle;
}

void FormDialog::show(int cmdShow)
{
	::ShowWindow(handle, cmdShow);
	::UpdateWindow(handle);
}
