// dllmain.cpp : Defines the entry point for the DLL application.
#ifdef _MANAGED
#pragma managed(push, off)
#endif

#include    "inject.h"

HINSTANCE	 hInstDLL;

#pragma data_seg("SHARED")
HHOOK			hKeyboardHook;
#pragma data_seg()


BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	hInstDLL = (HINSTANCE) hModule;
	hKeyboardHook = NULL;
	return TRUE;
}

LRESULT CALLBACK KeyboardHook(int nCode,   WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)  // do not process message 
		return CallNextHookEx( hKeyboardHook, nCode, wParam, lParam );

	return 1;//CallNextHookEx( hHook, nCode, wParam, lParam );//Goi Hook procedure ke tiep trong Hook Chain	

}
// Install hook
void doSetKeyboardGlobalHook(HWND hWnd)
{
	// Init value for MappedData	

	if (!hKeyboardHook)
		hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardHook, hInstDLL, 0);	

}
// Remove hook
void doRemoveKeyboardGlobalHook(HWND hWnd)
{
	if (hKeyboardHook)
		UnhookWindowsHookEx(hKeyboardHook);
}

void __declspec(dllexport) LockKeyboard(HWND hWnd, BOOL bEnableDisable)
{
	if (!bEnableDisable)
	{
		doRemoveKeyboardGlobalHook(hWnd);
	}
	else
	{
		doSetKeyboardGlobalHook(hWnd);
	}
}


/*****************************************************************
* Enable/Disable Ctrl+Alt+Del and Ctrl+Shift+Esc key sequences. *
* TRUE=Enable, FALSE=Disable                                    *
* (Win 2K).                                                     *
*****************************************************************/
int __declspec(dllexport) LockCtrlAltDel(BOOL bEnableDisable)
{
	static BOOL bInjected = FALSE;

	if (!bEnableDisable)
	{
		if (!bInjected)
		{
			bInjected = Inject();
			return bInjected;
		}
	}
	else
	{
		if (bInjected)
		{
			bInjected = !Eject();
			return !bInjected;
		}
	}

	return 0;
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

