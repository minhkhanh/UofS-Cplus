// FuncMFC.h : main header file for the FuncMFC DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFuncMFCApp
// See FuncMFC.cpp for the implementation of this class
//

class CFuncMFCApp : public CWinApp
{
public:
	CFuncMFCApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
