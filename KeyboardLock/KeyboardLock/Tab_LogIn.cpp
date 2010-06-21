// Tab_LogIn.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "KeyboardLockDlg.h"
#include "Tab_LogIn.h"
#include <ktmw32.h>
#pragma comment(lib, "ktmw32.lib")

bool ReadRegString( HKEY hkeyParent, LPCTSTR szSubkey, LPCTSTR szValueName,
				   LPTSTR szData, HANDLE hTransaction )
{
	HKEY hKey;
	CRegKey reg;
	LONG lRet;

	lRet = RegCreateKeyTransacted (
		hkeyParent, szSubkey, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &hKey, NULL,
		hTransaction, NULL );

	if ( ERROR_SUCCESS != lRet )
		return false;

	reg.Attach ( hKey );

	ULONG cb;
	lRet = reg.QueryStringValue( szValueName, szData, &cb);

	return ERROR_SUCCESS == lRet;
}

bool WriteRegString ( HKEY hkeyParent, LPCTSTR szSubkey, LPCTSTR szValueName,
					 LPCTSTR szData, HANDLE hTransaction )
{
	HKEY hKey;
	CRegKey reg;
	LONG lRet;

	lRet = RegCreateKeyTransacted (
		hkeyParent, szSubkey, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL,
		hTransaction, NULL );

	if ( ERROR_SUCCESS != lRet )
		return false;

	reg.Attach ( hKey );

	lRet = reg.SetStringValue ( szValueName, szData );

	return ERROR_SUCCESS == lRet;
}

void SetRegPassword(LPCTSTR lpszNewPass)
{
	CHandle hTransaction;

	hTransaction.Attach ( CreateTransaction ( NULL, 0, TRANSACTION_DO_NOT_PROMOTE,
		0, 0, 0, NULL ) );

	if (hTransaction == NULL)
		return;

	WriteRegString ( HKEY_CURRENT_USER, L"software\\security", L"Password",
		lpszNewPass, hTransaction  );

	CommitTransaction(hTransaction);
}

void GetRegPassword(LPTSTR lpszPass)
{
	CHandle hTransaction;

	hTransaction.Attach ( CreateTransaction ( NULL, 0, TRANSACTION_DO_NOT_PROMOTE,
		0, 0, 0, NULL ) );

	if (hTransaction == NULL)
		return;

	ReadRegString(HKEY_CURRENT_USER, L"software\\security", L"Password",
		lpszPass, hTransaction  );

	CommitTransaction(hTransaction);
}



// Tab_LogIn dialog

IMPLEMENT_DYNAMIC(Tab_LogIn, CDialog)

Tab_LogIn::Tab_LogIn(CWnd* pParent /*=NULL*/)
	: CDialog(Tab_LogIn::IDD, pParent)
{
	SetRegPassword(L"anhkhoa");
}

Tab_LogIn::~Tab_LogIn()
{
}

void Tab_LogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, EditBox_Pass);
	DDX_Control(pDX, IDC_BUTTON3, m_btnSetNewPass);
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

	LPTSTR password = new TCHAR[128];
	GetRegPassword(password);
	
	if (temp == password)
	{
		CKeyboardLockDlg::isRightPass = true;
		MessageBox(L"Access successfully!", L"Notice", MB_OK);
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
	}
}

void Tab_LogIn::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
}
