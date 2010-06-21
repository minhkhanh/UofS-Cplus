#pragma once
#include "afxwin.h"


// Tab_LogIn dialog

class Tab_LogIn : public CDialog
{
	DECLARE_DYNAMIC(Tab_LogIn)

public:
	Tab_LogIn(CWnd* pParent = NULL);   // standard constructor
	virtual ~Tab_LogIn();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit EditBox_Pass;
	afx_msg void OnBnClickedButton1();
};
