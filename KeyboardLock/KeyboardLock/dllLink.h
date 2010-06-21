#include <vector>
#include <fstream>
using namespace std;

#define IMPORT __declspec(dllimport)
void IMPORT LockMouse(HWND hWnd, BOOL bEnableDisable);
void IMPORT LockKeyboard(HWND hWnd, BOOL bEnableDisable);
int IMPORT LockTaskManager(BOOL bEnableDisable);
void IMPORT MonitorProcess(BOOL b);
void IMPORT ActiveKeyLogger(HWND hWnd, BOOL bEnableDisable);
//void IMPORT ViewLogKeyLogger();
void IMPORT GetListDenyFromFile(wchar_t* pszPath, vector<wchar_t*> &vList);

void IMPORT SaveListDenyToFile(wchar_t* pszPath, vector<wchar_t*> &vList);
void IMPORT MiliSecsToSystemTime(DWORD dwMiliSecs, SYSTEMTIME *stResult);
DWORD IMPORT GetFileTimeDifference(FILETIME* pFileTimeMin, FILETIME* pFileTimeMax);
BOOL IMPORT GetDateString( LPFILETIME pFt, TCHAR * pszDate, unsigned cbIn);
BOOL IMPORT GetTimeString( LPFILETIME pFt, TCHAR * pszTime, unsigned cbIn);