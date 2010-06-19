#pragma once

class ProcessInfo
{
public:
	DWORD dwPID;
	wchar_t * pszName;
	ProcessInfo(void)
	{
		pszName = NULL;
	}

	virtual ~ProcessInfo(void)
	{
		delete pszName;
	}
};
