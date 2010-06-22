// Tab_LogIn.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "KeyboardLockDlg.h"
#include "Tab_LogIn.h"

void SetRegPassword(LPWSTR lpszNewPass)
{
	RegSetKeyValue(HKEY_CURRENT_USER, L"Software\\Security", L"Password", REG_SZ, lpszNewPass, sizeof(TCHAR)*wcslen(lpszNewPass));
}

void GetRegPassword(LPWSTR lpszPass)
{
	DWORD pLong;
	HKEY hKey;
	DWORD dwTmp;

	RegOpenKey(HKEY_CURRENT_USER, L"Software\\Security", &hKey);
	if (RegQueryValueEx(hKey, L"Password", 0, &dwTmp, (BYTE*)lpszPass, &pLong) != ERROR_SUCCESS)
	{
		wsprintf(lpszPass, L"%s", L"1234");
		RegSetKeyValue(HKEY_CURRENT_USER, L"Software\\Security", L"Password", REG_SZ, lpszPass, sizeof(TCHAR)*wcslen(lpszPass));
		return;
	}
}


// Tab_LogIn dialog

IMPLEMENT_DYNAMIC(Tab_LogIn, CDialog)

Tab_LogIn::Tab_LogIn(CWnd* pParent /*=NULL*/)
	: CDialog(Tab_LogIn::IDD, pParent)
{	
}

Tab_LogIn::~Tab_LogIn()
{
}

void Tab_LogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, EditBox_Pass);
	DDX_Control(pDX, IDC_BUTTON3, m_btnSetNewPass);

	m_btnSetNewPass.EnableWindow(FALSE);
}


BEGIN_MESSAGE_MAP(Tab_LogIn, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &Tab_LogIn::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Tab_LogIn::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Tab_LogIn::OnBnClickedButton3)
END_MESSAGE_MAP()

//BOOL Tab_LogIn::OnInitDialog()
//{
//	CHandle hTransaction;
//
//	hTransaction.Attach ( CreateTransaction ( NULL, 0, TRANSACTION_DO_NOT_PROMOTE,
//		0, 0, 0, NULL ) );
//
//	if (hTransaction == NULL)
//		return TRUE;
//
//	WriteRegString ( HKEY_CURRENT_USER, L"software\\security", L"Password",
//		L"anhkhoa", hTransaction  );
//
//	CommitTransaction(hTransaction);
//
//	return TRUE;
//}
// Tab_LogIn message handlers

void Tab_LogIn::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString temp;
	EditBox_Pass.GetWindowText(temp);

	LPWSTR password = new TCHAR[128];
	GetRegPassword(password);
	
	if (temp == password)
	{
		CKeyboardLockDlg::isRightPass = true;
		MessageBox(L"Access successfully!", L"Notice", MB_OK);

		m_btnSetNewPass.EnableWindow(TRUE);
	}
	else
		MessageBox(L"Access denied!", L"Notice", MB_OK);

	EditBox_Pass.SetWindowText(L"");
}

void Tab_LogIn::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (CKeyboardLockDlg::isRightPass == true)
	{
		MessageBox(L"Log out successfully", L"Notice", MB_OK);
		CKeyboardLockDlg::isRightPass = false;
		m_btnSetNewPass.EnableWindow(FALSE);
	}
}

void Tab_LogIn::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	if (CKeyboardLockDlg::isRightPass == true)
	{
		LPWSTR lpszNewPass = new TCHAR[128];
		EditBox_Pass.GetWindowText(lpszNewPass, 128);

		SetRegPassword(lpszNewPass);
		MessageBox(L"Change password successfully");

		EditBox_Pass.SetWindowText(L"");
	}
	else
		MessageBox(L"You must login first!");
}
