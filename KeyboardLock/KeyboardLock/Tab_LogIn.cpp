// Tab_LogIn.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "KeyboardLockDlg.h"
#include "Tab_LogIn.h"


// Tab_LogIn dialog

IMPLEMENT_DYNAMIC(Tab_LogIn, CDialog)

Tab_LogIn::Tab_LogIn(CWnd* pParent /*=NULL*/)
	: CDialog(Tab_LogIn::IDD, pParent)
{
	PassAccess = L"anhkhoa";
}

Tab_LogIn::~Tab_LogIn()
{
}

void Tab_LogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, EditBox_Pass);
}


BEGIN_MESSAGE_MAP(Tab_LogIn, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &Tab_LogIn::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Tab_LogIn::OnBnClickedButton2)
END_MESSAGE_MAP()


// Tab_LogIn message handlers

void Tab_LogIn::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString temp;
	EditBox_Pass.GetWindowTextW(temp);
	
	if (temp == PassAccess)
	{
		CKeyboardLockDlg::isRightPass = true;
		MessageBox(L"Access successfully!", L"Notice", MB_OK);
	}
	else
		MessageBox(L"Access denied!", L"Notice", MB_OK);

	EditBox_Pass.SetWindowTextW(L"");
}

void Tab_LogIn::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (CKeyboardLockDlg::isRightPass == true)
	{
		MessageBox(L"Log out successfully", L"Notice", MB_OK);
		CKeyboardLockDlg::isRightPass = false;
	}
}
