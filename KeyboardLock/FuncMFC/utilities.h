#pragma once


#define EXPORT __declspec(dllexport)

void EXPORT WriteNewFileByLine(wchar_t* szPath, vector<wchar_t*> &vList);
void EXPORT ReadFileByLine(wchar_t* szPath, vector<wchar_t*> &vList);