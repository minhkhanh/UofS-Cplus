
// KeyboardLockDlg.h : header file
//

#pragma once

typedef void (*SetGlobalHook)(HWND hWnd);
typedef void (*RemoveGlobalHook)(HWND hWnd);
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
};
