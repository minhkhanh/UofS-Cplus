// Tab_X.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "Tab_Process_Locker.h"
#include "dllLink.h"

#define IMPORT __declspec(dllimport)
IMPORT void doSetGlobalHook(HWND hWnd);
IMPORT void doRemoveGlobalHook(HWND hWnd);
// CTab_Process_Locker dialog

IMPLEMENT_DYNCREATE(CTab_Process_Locker, CDHtmlDialog)

CTab_Process_Locker::CTab_Process_Locker(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CTab_Process_Locker::IDD, CTab_Process_Locker::IDH, pParent)
{

}

CTab_Process_Locker::~CTab_Process_Locker()
{
}

void CTab_Process_Locker::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lvDenied);
	DDX_Control(pDX, IDB_MONITER, m_buttMonitor);
	DDX_Control(pDX, IDB_UNMONITER, m_buttUnMonitor);
	DDX_Control(pDX, IDB_ADD2LIST, m_buttAdd);
	DDX_Control(pDX, IDB_REMOVELIST, m_buttRemove);
	DDX_Control(pDX, IDC_LOGKEYBOARD, m_chkLogKeyboard);
}

BOOL CTab_Process_Locker::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	m_lvDenied.InsertColumn(0, L"File name", LVCFMT_LEFT, 140);
	LoadListView();
	StatusUnMonitor();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CTab_Process_Locker, CDHtmlDialog)
	ON_BN_CLICKED(IDB_MONITER, &CTab_Process_Locker::OnBnClickedMoniter)
	ON_BN_CLICKED(IDB_UNMONITER, &CTab_Process_Locker::OnBnClickedUnmoniter)
	ON_BN_CLICKED(IDB_ADD2LIST, &CTab_Process_Locker::OnBnClickedAdd2list)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab_Process_Locker::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_LOGKEYBOARD, &CTab_Process_Locker::OnBnClickedLogkeyboard)
	ON_BN_CLICKED(IDB_VIEWLOG, &CTab_Process_Locker::OnBnClickedViewlog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CTab_Process_Locker)
	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



void CTab_Process_Locker::OnBnClickedMoniter()
{
	// TODO: Add your control notification handler code here
	MonitorProcess(TRUE);
	StatusMonitor();
	//LockTaskManager(FALSE);
	//ViewLogKeyLogger();
}

void CTab_Process_Locker::OnBnClickedUnmoniter()
{
	// TODO: Add your control notification handler code here
	MonitorProcess(FALSE);
	StatusUnMonitor();
}

void CTab_Process_Locker::OnBnClickedAdd2list()
{
	// TODO: Add your control notification handler code here

	CFileDialog dlgOpen(true, L"*.exe", L"*.exe",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
		L"Excutable file|*.exe||");

	dlgOpen.DoModal();

	CString strFilePath;
	TCHAR szFileName[128];

	POSITION pos = dlgOpen.GetStartPosition();

	CFile cfOut;
	cfOut.Open(L"DenyList.txt", CFile::modeWrite);
	cfOut.SeekToEnd();

	while (pos != NULL)
	{
		// lấy dần các đường dẫn trong vùng chọn
		strFilePath = dlgOpen.GetNextPathName(pos);

		// thêm tên file vào danh sách từ chối
		GetFileTitle(strFilePath, szFileName, 128);
		cfOut.Write(szFileName, wcslen(szFileName)*sizeof(TCHAR));
		cfOut.Write(L"\r\n", 2*sizeof(TCHAR));

		// thêm tên file vào list view
		m_lvDenied.InsertItem(m_lvDenied.GetItemCount(), szFileName);
	}

	cfOut.Close();
}

void CTab_Process_Locker::LoadListView(void)
{
	vector<TCHAR*> vList;
	GetListDenyFromFile(L"DenyList.txt", vList);
	for (int i=0; i<vList.size(); ++i)
	{
		m_lvDenied.InsertItem(m_lvDenied.GetItemCount(), vList[i]);
		delete vList[i];
	}
	vList.clear();
}

void CTab_Process_Locker::OnBnClickedButtonRemove()
{
	// TODO: Add your control notification handler code here

	POSITION pos = m_lvDenied.GetFirstSelectedItemPosition();

	while (pos != NULL)
	{
		int i = m_lvDenied.GetNextSelectedItem(pos);
		m_lvDenied.DeleteItem(i);
	}
	SaveListView();
}
void CTab_Process_Locker::SaveListView(void)
{
	vector<TCHAR*> vList;
	int i = m_lvDenied.GetNextItem(-1, LVNI_ALL);
	while (i != -1)
	{
		CString cstrItemText = m_lvDenied.GetItemText(i, 0);
		i = m_lvDenied.GetNextItem(i, LVNI_ALL);
		vList.push_back(wcsdup((wchar_t*)cstrItemText.GetBufferSetLength(cstrItemText.GetLength())));
	} 
	SaveListDenyToFile(L"DenyList.txt", vList);
	for (int i=0; i<vList.size(); ++i)
	{
		delete vList[i];
	}
	vList.clear();
}

void CTab_Process_Locker::StatusMonitor()
{
	m_buttMonitor.EnableWindow(FALSE);
	m_buttUnMonitor.EnableWindow(TRUE);
	m_buttAdd.EnableWindow(FALSE);
	m_buttRemove.EnableWindow(FALSE);
}

void CTab_Process_Locker::StatusUnMonitor()
{
	m_buttMonitor.EnableWindow(TRUE);
	m_buttUnMonitor.EnableWindow(FALSE);
	m_buttAdd.EnableWindow(TRUE);
	m_buttRemove.EnableWindow(TRUE);
}
void CTab_Process_Locker::OnBnClickedLogkeyboard()
{
	// TODO: Add your control notification handler code here
	ActiveKeyLogger(m_hWnd, m_chkLogKeyboard.GetCheck());
}

void CTab_Process_Locker::OnBnClickedViewlog()
{
	// TODO: Add your control notification handler code here
	ViewLogKeyLogger();
}
