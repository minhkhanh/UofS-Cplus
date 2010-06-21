
// KeyboardLock.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "dllLink.h"
#include <fstream>
using namespace std;


// CKeyboardLockApp:
// See KeyboardLock.cpp for the implementation of this class
//

class CKeyboardLockApp : public CWinAppEx
{
public:
	CKeyboardLockApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CKeyboardLockApp theApp;