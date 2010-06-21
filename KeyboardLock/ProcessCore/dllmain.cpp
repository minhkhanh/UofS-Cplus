// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "dllmain.h"

#pragma data_seg("SHARED")
vector<PROCESSINFO> vProcessList;
HANDLE hMonitorProcessThread;
bool bStopMonitorProcessThread;
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
	return TRUE;
}
BOOL KillProcess(DWORD pid)
{    
	HANDLE hProc = NULL;
	BOOL bRet = FALSE;

	// OpenProcess voi flag PROCESS_TERMINATE
	hProc = OpenProcess(
		PROCESS_TERMINATE,
		FALSE,
		pid
		);
	if (!hProc)
	{
		return bRet;
	}

	// Kill process
	bRet = TerminateProcess(hProc, 0);

	// Clean up
	CloseHandle(hProc);
	return bRet;
}

DWORD CALLBACK DenyProcessThread(void *lpVoid)
{
	vector<wchar_t*> vFileExeList;
	GetListDenyFromFile(_T("DenyList.txt"), vFileExeList);
	while(!bStopMonitorProcessThread)
	{
		UpdateProcessList(vProcessList);
		for (int i=0; i<vFileExeList.size(); ++i)
		{
			for (int j=0; j<vProcessList.size(); ++j)
			{
				if (!wcscmp(vProcessList[j].szName, vFileExeList[i]))
				{
					KillProcess(vProcessList[j].dwPID);
					UpdateProcessList(vProcessList);
					break;
				}
			}
		}
		Beep(750,300);
		Sleep(500);
	}
	for (int i=0; i<vFileExeList.size(); ++i)
	{
		delete vFileExeList[i];
	}
	return TRUE;
}
void EXPORT MonitorProcess(BOOL b)
{
	if (b)
	{
		if (hMonitorProcessThread)
		{
			return;
		}
		hMonitorProcessThread = CreateThread(NULL,0,DenyProcessThread,NULL,0,NULL);
		bStopMonitorProcessThread = false;
	} 
	else
	{
		if (hMonitorProcessThread)
		{
			bStopMonitorProcessThread = true;
			Sleep(600);
			CloseHandle(hMonitorProcessThread);
			hMonitorProcessThread = NULL;
		}
	}
}

void UpdateProcessList(vector<PROCESSINFO> &vList)
{
	DWORD aProcesses[1024];
	DWORD cbNeeded = 0;
	vList.clear();
	if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return;

	// Calculate how many process IDs were returned
	DWORD cProcesses = cbNeeded / sizeof(DWORD);

	for(int i = 0; i < cProcesses; i++ )
	{
		//if (FindPIdInList(aProcesses[i], vList))
		//{
		//	continue;
		//}
		PROCESSINFO temp;
		temp.dwPID = aProcesses[i];
		HMODULE hMod;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE, aProcesses[i]);
		if(!hProcess) continue;
		if(EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded))
			GetModuleBaseName( hProcess, hMod, temp.szName,sizeof(temp.szName));
		vList.push_back(temp);
	}
}
bool FindPIdInList(DWORD dwPID, vector<PROCESSINFO> &vList)
{
	for (int i=0; i<vList.size(); ++i)
	{
		if (dwPID==vList[i].dwPID)
		{
			return true;
		}
	}
	return false;
}
void EXPORT GetListDenyFromFile(wchar_t* pszPath, vector<wchar_t*> &vList)
{
	vList.clear();
	FILE * f;
	if ((f = _wfopen(pszPath, _T("rt"))) == NULL)
		return; 
	char pTemp[INPUT_MAX_LEN];
	int indexCell = 0;
	while (fgets(pTemp, INPUT_MAX_LEN, f))
	{
		wchar_t * temp = UTF8_to_WChar(pTemp);
		temp[wcslen(temp)-1] = temp[wcslen(temp)];   // bo ky tu xuong dong 
		vList.push_back(temp);
	}
	fclose(f);
}

wchar_t * UTF8_to_WChar(const char *string)
{
	long b=0,
		c=0;
	if ((unsigned char)string[0]==BOM8A && (unsigned char)string[1]==BOM8B && (unsigned char)string[2]==BOM8C)
		string+=3;
	for (const char *a=string;*a;a++)
		if (((unsigned char)*a)<128 || (*a&192)==192)
			c++;
	wchar_t *res=new wchar_t[c+1];
	res[c]=0;
	for (unsigned char *a=(unsigned char*)string;*a;a++){
		if (!(*a&128))
			//Byte represents an ASCII character. Direct copy will do.
			res[b]=*a;
		else if ((*a&192)==128)
			//Byte is the middle of an encoded character. Ignore.
			continue;
		else if ((*a&224)==192)
			//Byte represents the start of an encoded character in the range
			//U+0080 to U+07FF
			res[b]=((*a&31)<<6)|a[1]&63;
		else if ((*a&240)==224)
			//Byte represents the start of an encoded character in the range
			//U+07FF to U+FFFF
			res[b]=((*a&15)<<12)|((a[1]&63)<<6)|a[2]&63;
		else if ((*a&248)==240){
			//Byte represents the start of an encoded character beyond the
			//U+FFFF limit of 16-bit integers
			res[b]='?';
		}
		b++;
	}
	return res;
}

void EXPORT MiliSecsToSystemTime(DWORD dwMiliSecs, SYSTEMTIME *stResult)
{
	//stResult->wDay = dwMiliSecs / 86400000;
	//dwMiliSecs = dwMiliSecs % 86400000;

	stResult->wHour = dwMiliSecs / 3600000;
	dwMiliSecs = dwMiliSecs % 3600000;

	stResult->wMinute = dwMiliSecs / 60000;
	dwMiliSecs = dwMiliSecs % 60000;

	stResult->wSecond = dwMiliSecs / 1000;
	dwMiliSecs = dwMiliSecs % 1000;

	stResult->wMilliseconds = dwMiliSecs;
}

DWORD EXPORT GetFileTimeDifference(FILETIME* pFileTimeMin, FILETIME* pFileTimeMax)
{
	LONGLONG llCreate = *(LONGLONG *)pFileTimeMin;
	LONGLONG llExit = *(LONGLONG *)pFileTimeMax;

	DWORD dwCreate, dwExit;

	dwCreate = (DWORD)(llCreate / 10000);
	dwExit = (DWORD)(llExit / 10000);

	return dwExit - dwCreate;
}

BOOL EXPORT GetDateString( LPFILETIME pFt, TCHAR * pszDate, unsigned cbIn)
{
	FILETIME ftLocal;
	SYSTEMTIME st;

	if(!FileTimeToLocalFileTime(pFt, &ftLocal))
		return FALSE;

	if(!FileTimeToSystemTime(&ftLocal, &st))
		return FALSE;

	TCHAR szTemp[12];

	wsprintf(szTemp, L"%02u/%02u/%04u",	st.wMonth, st.wDay, st.wYear);
	lstrcpyn(pszDate, szTemp, cbIn); 
	return TRUE;
}

BOOL EXPORT GetTimeString( LPFILETIME pFt, TCHAR * pszTime, unsigned cbIn)
{
	FILETIME ftLocal;
	SYSTEMTIME st;

	if(!FileTimeToLocalFileTime( pFt, &ftLocal))
		return FALSE;

	if(!FileTimeToSystemTime( &ftLocal, &st))
		return FALSE;

	TCHAR szTemp[12];

	wsprintf(szTemp, L"%02u:%02u:%02u", st.wHour, st.wMinute, st.wSecond);
	lstrcpyn(pszTime, szTemp, cbIn); 

	return TRUE;
}