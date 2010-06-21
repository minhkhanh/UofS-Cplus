#include "psapi.h"
#include <tchar.h>
#include <vector>
using namespace std;

#define INPUT_MAX_LEN 260
#define BOM8A 0xEF
#define BOM8B 0xBB
#define BOM8C 0xBF

struct PROCESSINFO
{
	DWORD dwPID;
	wchar_t szName[INPUT_MAX_LEN];
};

wchar_t * UTF8_to_WChar(const char *string);
bool FindPIdInList(DWORD dwPID, vector<PROCESSINFO> &vList);
void UpdateProcessList(vector<PROCESSINFO> &vList);
DWORD CALLBACK DenyProcessThread(void *lpVoid);
BOOL KillProcess(DWORD pid);
void EXPORT GetListDenyFromFile(wchar_t* pszPath, vector<wchar_t*> &vList);
void EXPORT MiliSecsToSystemTime(DWORD dwMiliSecs, SYSTEMTIME *stResult);
void EXPORT SaveListDenyToFile(wchar_t* pszPath, vector<wchar_t*> &vList);
DWORD EXPORT GetFileTimeDifference(FILETIME* pFileTimeMin, FILETIME* pFileTimeMax);
BOOL EXPORT GetDateString( LPFILETIME pFt, TCHAR * pszDate, unsigned cbIn);
void IMPORT ReadFileByLine(wchar_t* szPath, vector<wchar_t*> &vList);
BOOL EXPORT GetTimeString( LPFILETIME pFt, TCHAR * pszTime, unsigned cbIn);void IMPORT WriteNewFileByLine(wchar_t* szPath, vector<wchar_t*> &vList);