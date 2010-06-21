#include "utilities.h"
#include "stdafx.h"
#include <vector>
using namespace std;

void WriteNewFileByLine( wchar_t* szPath, vector<wchar_t*> &vList )
{
	CFile cfOut;
	if (!cfOut.Open(szPath, CFile::modeCreate|CFile::modeWrite)) return;
	for (int i=0; i<vList.size(); ++i)
	{
		cfOut.Write(vList[i], wcslen(vList[i])*sizeof(wchar_t));
		cfOut.Write(L"\r\n", 2*sizeof(wchar_t));
	}
	cfOut.Close();
}

void ReadFileByLine( wchar_t* szPath, vector<wchar_t*> &vList )
{
	CFile cfOut;
	if (!cfOut.Open(L"DenyList.txt", CFile::modeRead)) return;
	for (int i=0; i<vList.size(); ++i) delete vList[i];
	vList.clear();
	int cb = cfOut.GetLength();
	int nChar = cb / sizeof(TCHAR);
	CString cstrTmp;
	if (nChar > 4)
	{
		cstrTmp.GetBufferSetLength(nChar);
		cfOut.Read(cstrTmp.GetBuffer(), cb);
		CString cstrToken;
		int pos = 0;
		cstrToken = cstrTmp.Tokenize(L".exe", pos);
		while (cstrToken != "\r\n")
		{
			cstrToken.Append(L".exe");
			int i=0;
			while (i<cstrToken.GetLength())
			{
				if ((cstrToken[i]==10)||cstrToken[i]==13)
				{
					cstrToken.Delete(i,1);
				}
				else
				{
					++i;
				}
			}
			vList.push_back(wcsdup((wchar_t*)cstrToken.GetBufferSetLength(cstrToken.GetLength())));
			cstrToken = cstrTmp.Tokenize(L".exe", pos);
		}
	}
	cfOut.Close();
}