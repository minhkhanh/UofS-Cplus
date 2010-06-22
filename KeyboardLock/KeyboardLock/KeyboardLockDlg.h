
// KeyboardLockDlg.h : header file
//

#pragma once
#include "afxcmn.h"

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
	CDialog *tab[4];
	int i_NumberOfTab;
	static bool isRightPass;

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
	afx_msg void OnHelpAbout();
	afx_msg void OnFileExit();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	CTabCtrl MyTabControl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

public:
	void EnableTab(int nSel, bool isEnable);

private:
	bool m_bIsEnabled;
};
