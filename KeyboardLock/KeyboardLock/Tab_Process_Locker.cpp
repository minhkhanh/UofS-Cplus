// Tab_X.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "Tab_Process_Locker.h"


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
}

BOOL CTab_Process_Locker::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	m_lvDenied.InsertColumn(0, L"File name");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CTab_Process_Locker, CDHtmlDialog)
	ON_BN_CLICKED(IDB_MONITER, &CTab_Process_Locker::OnBnClickedMoniter)
	ON_BN_CLICKED(IDB_UNMONITER, &CTab_Process_Locker::OnBnClickedUnmoniter)
	ON_BN_CLICKED(IDB_ADD2LIST, &CTab_Process_Locker::OnBnClickedAdd2list)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab_Process_Locker::OnBnClickedButtonRemove)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CTab_Process_Locker)
	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CTab_Process_Locker message handlers

//HRESULT CTab_Process_Locker::OnButtonOK(IHTMLElement* /*pElement*/)
//{
//	OnOK();
//	return S_OK;
//}
//
//HRESULT CTab_Process_Locker::OnButtonCancel(IHTMLElement* /*pElement*/)
//{
//	OnCancel();
//	return S_OK;
//}

void CTab_Process_Locker::OnBnClickedMoniter()
{
	// TODO: Add your control notification handler code here
	MonitorProcess(TRUE);
}

void CTab_Process_Locker::OnBnClickedUnmoniter()
{
	// TODO: Add your control notification handler code here
	MonitorProcess(FALSE);
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
		m_lvDenied.InsertItem(0, szFileName);
	}

	cfOut.Close();
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
}
