#include <stdio.h>

#define EXPORT __declspec(dllexport)
void GetCurrentPath();
void EXPORT SetCurrentPath(wchar_t * psz);