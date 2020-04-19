#include <windows.h>
#include <windowsx.h>
#include <dinput.h>

#include "defines.h"

extern LPDIRECTINPUT lpDirectInputObject;
extern LPDIRECTINPUTDEVICE mKeyboard;
extern unsigned char keystate[256];

BOOL InitDirectInput(HWND hwnd)
{
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDirectInputObject, NULL)))
		return FALSE;    

	if (FAILED(lpDirectInputObject->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL)))
		return FALSE;  

	if (FAILED(mKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return FALSE;  

	if (FAILED(mKeyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE)))
		return FALSE;

	if (FAILED(mKeyboard->Acquire()))
		return FALSE;

	return TRUE;
}

void GetKeystate()
{
	mKeyboard->GetDeviceState(sizeof(unsigned char[256]), (LPVOID)keystate);
}
