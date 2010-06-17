
// KeyboardLockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "KeyboardLockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CKeyboardLockDlg dialog




CKeyboardLockDlg::CKeyboardLockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyboardLockDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyboardLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKeyboardLockDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CKeyboardLockDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CKeyboardLockDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKeyboardLockDlg message handlers

BOOL CKeyboardLockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	hHookDll = LoadLibrary(_T("HookCore.dll"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeyboardLockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0)  == SC_CLOSE)
	{
		this->DestroyWindow();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyboardLockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyboardLockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKeyboardLockDlg::OnBnClickedOk()
{
	if (hHookDll)
	{
		LockKeyboard lpFuncLockKeyboard = (LockKeyboard)GetProcAddress(hHookDll, ("LockKeyboard"));
		if (lpFuncLockKeyboard)
		{
			//lpFuncLockKeyboard(m_hWnd, TRUE);
		}
		LockMouse lpFuncLockMouse = (LockMouse)GetProcAddress(hHookDll, ("LockMouse"));
		if (lpFuncLockMouse)
		{
			lpFuncLockMouse(m_hWnd, TRUE);
		}
		//LockCtrlAltDel lpFuncCtrlAltDel = (LockCtrlAltDel)GetProcAddress(hHookDll, ("LockCtrlAltDel"));
		//if (lpFuncCtrlAltDel)
		//{
		//	lpFuncCtrlAltDel(TRUE);
		//}
		LockTaskManager lpFuncLockTaskManager = (LockTaskManager)GetProcAddress(hHookDll, ("LockTaskManager"));
		if (lpFuncLockTaskManager)
		{
			lpFuncLockTaskManager(TRUE);
		}
	}
}

void CKeyboardLockDlg::OnBnClickedCancel()
{
	if (hHookDll)
	{
		LockKeyboard lpFunc = (LockKeyboard)GetProcAddress(hHookDll, ("LockKeyboard"));
		if (lpFunc)
		{
			//lpFunc(m_hWnd, FALSE);
		}
		LockMouse lpFuncLockMouse = (LockMouse)GetProcAddress(hHookDll, ("LockMouse"));
		if (lpFuncLockMouse)
		{
			lpFuncLockMouse(m_hWnd, FALSE);
		}
		//LockCtrlAltDel lpFuncCtrlAltDel = (LockCtrlAltDel)GetProcAddress(hHookDll, ("LockCtrlAltDel"));
		//if (lpFuncCtrlAltDel)
		//{
		//	lpFuncCtrlAltDel(FALSE);
		//}
		LockTaskManager lpFuncLockTaskManager = (LockTaskManager)GetProcAddress(hHookDll, ("LockTaskManager"));
		if (lpFuncLockTaskManager)
		{
			lpFuncLockTaskManager(FALSE);
		}
	}
}
