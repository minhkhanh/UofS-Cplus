#pragma once

class Wchar
{
public:
	wchar_t * pszText;
	Wchar(void)
	{
		pszText = NULL;
	}

	Wchar(wchar_t *p)
	{
		pszText = p;
	}

	virtual ~Wchar(void)
	{
		delete pszText;
	}
};
