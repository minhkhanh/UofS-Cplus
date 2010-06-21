
// KeyboardLockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "KeyboardLockDlg.h"
#include "dllLink.h"
#include "Tab_Process_Locker.h"
#include "Tab_Process_Tracker.h"

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
	DDX_Control(pDX, IDC_TAB1, MyTabControl);
}

BEGIN_MESSAGE_MAP(CKeyboardLockDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP_ABOUT, &CKeyboardLockDlg::OnHelpAbout)
	ON_COMMAND(ID_FILE_EXIT, &CKeyboardLockDlg::OnFileExit)
	ON_BN_CLICKED(IDC_BUTTON8, &CKeyboardLockDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &CKeyboardLockDlg::OnBnClickedButton7)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CKeyboardLockDlg::OnTcnSelchangeTab1)
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
	//hHookDll = LoadLibrary(_T("HookCore.dll"));

	//Add them the Tab
	i_NumberOfTab = 2;
	tab[0] = new CTab_Process_Locker;
	tab[1] = new CTab_Process_Tracker;

	tab[0]->Create(IDD_TAB_PROCESS_LOCKER, GetParent());
	tab[1]->Create(IDD_TAB_PROCESS_TRACKER, GetParent());

	TCITEM tie;
	tie.mask = TCIF_TEXT;
	tie.pszText = L"Process Locker";
	MyTabControl.InsertItem(0, &tie);

	tie.mask = TCIF_TEXT;
	tie.pszText = L"Tracker";
	MyTabControl.InsertItem(1, &tie);

	//Xu li giao dien tung Tab
	CRect l_RectClient;
	CRect l_RectWnd;

	int nSel = MyTabControl.GetCurSel();

	MyTabControl.GetClientRect(l_RectClient);
	MyTabControl.AdjustRect(FALSE, l_RectClient);
	MyTabControl.GetWindowRect(l_RectWnd);
	ScreenToClient(l_RectWnd);
	l_RectClient.OffsetRect(l_RectWnd.left, l_RectWnd.top);

	for (int i=0; i<i_NumberOfTab; i++)
		tab[i]->SetWindowPos(&wndTop, l_RectClient.left, l_RectClient.top, l_RectClient.Width(), l_RectClient.Height(), SWP_HIDEWINDOW);

	tab[nSel]->ShowWindow(SW_SHOW);


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

void CKeyboardLockDlg::OnHelpAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg about;
	about.DoModal();
}

void CKeyboardLockDlg::OnFileExit()
{
	// TODO: Add your command handler code here
	EndDialog(-1);
}

void CKeyboardLockDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	CAboutDlg about;
	about.DoModal();
}

void CKeyboardLockDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	EndDialog(-1);
}
void CKeyboardLockDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	//Xu li giao dien tung Tab
	CRect l_RectClient;
	CRect l_RectWnd;

	int nSel = MyTabControl.GetCurSel();

	MyTabControl.GetClientRect(l_RectClient);
	MyTabControl.AdjustRect(FALSE, l_RectClient);
	MyTabControl.GetWindowRect(l_RectWnd);
	ScreenToClient(l_RectWnd);
	l_RectClient.OffsetRect(l_RectWnd.left, l_RectWnd.top);

	for (int i=0; i<i_NumberOfTab; i++)
		tab[i]->SetWindowPos(&wndTop, l_RectClient.left, l_RectClient.top, l_RectClient.Width(), l_RectClient.Height(), SWP_HIDEWINDOW);

	tab[nSel]->ShowWindow(SW_SHOW);
}
