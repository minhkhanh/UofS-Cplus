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
	//virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit EditBox_Pass;
	afx_msg void OnBnClickedButton1();
public:
	CString PassAccess;
	afx_msg void OnBnClickedButton2();
private:
	CButton m_btnSetNewPass;
public:
	afx_msg void OnBnClickedButton3();
};
