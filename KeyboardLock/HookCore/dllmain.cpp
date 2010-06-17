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

LRESULT CALLBACK KeyboardHookProc(int nCode,   WPARAM wParam, LPARAM lParam)
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
		hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHookProc, hInstDLL, 0);	

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
* Lock Ctrl+Alt+Del and Ctrl+Shift+Esc key sequences.           *
* TRUE=Lock, FALSE=UnLock                                       *
* (Win 2K).                                                     *
*****************************************************************/
int __declspec(dllexport) LockCtrlAltDel(BOOL bEnableDisable)
{
	static BOOL bInjected = FALSE;

	if (bEnableDisable)
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

/****************************************
* Lock Task Manager (CTRL+ALT+DEL).    *
* TRUE=Lock, FALSE=UnLock              *
* (Win NT/2K/XP)                       *
****************************************/
int __declspec(dllexport) LockTaskManager(BOOL bEnableDisable)
{
#define KEY_DISABLETASKMGR  "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
#define VAL_DISABLETASKMGR  "DisableTaskMgr"

	HKEY    hKey;
	DWORD   val;
	LONG	r;

	if (RegOpenKey(HKEY_CURRENT_USER, KEY_DISABLETASKMGR, &hKey) != ERROR_SUCCESS)
		if (RegCreateKey(HKEY_CURRENT_USER, KEY_DISABLETASKMGR, &hKey) != ERROR_SUCCESS)
			return 0;

	if (!bEnableDisable) // Enable
	{
		r = RegDeleteValue(hKey, VAL_DISABLETASKMGR);
	}
	else                // Disable
	{
		val = 1;
		r = RegSetValueEx(hKey, VAL_DISABLETASKMGR, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
	}

	RegCloseKey(hKey);

	return (r == ERROR_SUCCESS ? 1 : 0);
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

