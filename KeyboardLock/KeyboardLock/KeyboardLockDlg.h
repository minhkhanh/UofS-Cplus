
// KeyboardLockDlg.h : header file
//

#pragma once

//typedef void (*LockKeyboard)(HWND hWnd, BOOL b);
////typedef void (*LockMouse)(HWND hWnd, BOOL b);
//typedef void (*LockCtrlAltDel)(BOOL b);
//typedef void (*LockTaskManager)(BOOL b);

// CKeyboardLockDlg dialog
class CKeyboardLockDlg : public CDialog
{
// Construction
public:
	CKeyboardLockDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KEYBOARDLOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HMODULE hHookDll;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHelpAbout();
	afx_msg void OnFileExit();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
};
