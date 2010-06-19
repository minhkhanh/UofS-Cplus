#define IMPORT __declspec(dllimport)
void IMPORT LockMouse(HWND hWnd, BOOL bEnableDisable);
void IMPORT LockKeyboard(HWND hWnd, BOOL bEnableDisable);
int IMPORT LockTaskManager(BOOL bEnableDisable);