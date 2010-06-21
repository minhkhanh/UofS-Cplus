// Tab_X.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "Tab_Process_Locker.h"
#include "dllLink.h"


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

	m_lvDenied.InsertColumn(0, L"File name", LVCFMT_LEFT, 140);
	LoadListView();

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
		m_lvDenied.InsertItem(m_lvDenied.GetItemCount(), szFileName);
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

	SaveListView();
}

void CTab_Process_Locker::LoadListView(void)
{
	CFile cfOut;
	cfOut.Open(L"DenyList.txt", CFile::modeRead);

	m_lvDenied.DeleteAllItems();

	int cb = cfOut.GetLength();
	int nChar = cb / sizeof(TCHAR);
	CString cstrTmp;
	if (nChar > 4)
	{
		cstrTmp.GetBufferSetLength(nChar);
		cfOut.Read(cstrTmp.GetBuffer(), cb);

		CString cstrToken;
		int pos = 0;
		cstrToken = cstrTmp.Tokenize(L".exe", pos);
		while (cstrToken != "\r\n")
		{
			m_lvDenied.InsertItem(m_lvDenied.GetItemCount(), cstrToken + ".exe");
			cstrToken = cstrTmp.Tokenize(L".exe", pos);
		}
	}

	cfOut.Close();
}

void CTab_Process_Locker::SaveListView(void)
{
	CFile cfOut;
	cfOut.Open(L"DenyList.txt", CFile::modeCreate|CFile::modeWrite);

	TCHAR *szFileName;
	int i;
	while (i = m_lvDenied.GetNextItem(-1, LVNI_ALL) != -1)
	{
		CString cstrItemText = m_lvDenied.GetItemText(i,0);

		szFileName = cstrItemText.GetBufferSetLength(cstrItemText.GetLength());
		cfOut.Write(szFileName, wcslen(szFileName)*sizeof(TCHAR));
		cfOut.Write(L"\r\n", 2*sizeof(TCHAR));
	}

	cfOut.Close();
}
