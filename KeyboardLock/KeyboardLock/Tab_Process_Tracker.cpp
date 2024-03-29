// CTab_Process_Tracker.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "Tab_Process_Tracker.h"
#include "dllLink.h"
#include <Psapi.h>

// CTab_Process_Tracker dialog

IMPLEMENT_DYNCREATE(CTab_Process_Tracker, CDHtmlDialog)

CTab_Process_Tracker::CTab_Process_Tracker(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CTab_Process_Tracker::IDD, CTab_Process_Tracker::IDH, pParent)
{

}

CTab_Process_Tracker::~CTab_Process_Tracker()
{
}

void CTab_Process_Tracker::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_MONITOR, m_btnMonitor);
	DDX_Control(pDX, IDC_LIST1, m_lvTracked);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON2, m_btnOpen);
}

BOOL CTab_Process_Tracker::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	m_lvTracked.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_lvTracked.InsertColumn(0,L"#",LVCFMT_RIGHT,30,0);
	m_lvTracked.InsertColumn(1,L"PID",LVCFMT_RIGHT,40,1);	
	m_lvTracked.InsertColumn(2,L"Name",LVCFMT_LEFT,130,2);
	m_lvTracked.InsertColumn(3,L"Start",LVCFMT_RIGHT,120,3);
	m_lvTracked.InsertColumn(4,L"End",LVCFMT_RIGHT,120,4);
	m_lvTracked.InsertColumn(5,L"Length",LVCFMT_RIGHT,80,5);

	m_bIsMonitored = false;
	m_btnSave.EnableWindow(FALSE);

	//m_hThread = CreateThread(NULL,0,RunThread,this,CREATE_SUSPENDED,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CTab_Process_Tracker, CDHtmlDialog)
	ON_BN_CLICKED(IDB_MONITOR, &CTab_Process_Tracker::OnBnClickedMonitor)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab_Process_Tracker::OnBnClickedSave)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDB_OPEN, &CTab_Process_Tracker::OnBnClickedOpen)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CTab_Process_Tracker)
	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()


void CTab_Process_Tracker::OnBnClickedMonitor()
{
	// TODO: Add your control notification handler code here

	// chuyển đổi text trên button

	m_bIsMonitored = !m_bIsMonitored;
	if (m_bIsMonitored == true)
	{
		m_btnMonitor.SetWindowText(L"Unmonitor");
		m_btnSave.EnableWindow(FALSE);

		m_hThread = CreateThread(NULL,0,RunThread,this,0,NULL);
	}
	else
	{
		m_btnMonitor.SetWindowText(L"Monitor");
		m_btnSave.EnableWindow(TRUE);

		TerminateThread(m_hThread, 0);
	}
}

DWORD WINAPI CTab_Process_Tracker::RunThread(void* lpVoid)
{
	CTab_Process_Tracker *pDlg = (CTab_Process_Tracker *)lpVoid;

	pDlg->ClearLVData();

	while (pDlg->m_bIsMonitored)
	{
		pDlg->UpdateProcessList();
		Sleep(500);
	}

	//ExitThread(0);

	return 0;
}

void CTab_Process_Tracker::UpdateProcessList(void)
{
	// lấy danh sách các process id
	DWORD arrProcessID[1024], cbNeeded;
	if(!EnumProcesses(arrProcessID, sizeof(arrProcessID), &cbNeeded))
		return;

	// tính ra số lượng process id
	DWORD nProcess = cbNeeded / sizeof(DWORD);

	// xử lý từng process mới nhận được
	for(unsigned i = 0; i < nProcess; i++ )
		AddProcessToList(arrProcessID[i]);
}

void CTab_Process_Tracker::AddProcessToList(DWORD dwProcID)
{
	// cập nhật thời gian vào list view
	UpdateProcessTime();

	int nLVItem = m_lvTracked.GetItemCount();

	// lấy HANDLE của process bằng hàm OpenProcess từ ID của process
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE, dwProcID);
	if (!hProcess)
		return;

	for (int i = 0; i < nLVItem; i++)
	{
		_PROCESS_LVITEM *pOldPI = (_PROCESS_LVITEM *)m_lvTracked.GetItemData(i);

		// nếu process id truyền vào giống id của process cũ
		//thì kiểm tra nó có kết thúc chưa, nếu chưa thì khỏi xử lý nữa
		if (pOldPI->dwProcID == dwProcID)
		{
			CString strEndTime = m_lvTracked.GetItemText(i,4);

			if(strEndTime.TrimRight() == "")	// cột End time rỗng
				return;
		}
	}

	_PROCESS_LVITEM *pProcessItem;				// struct lưu vào list view item

	// các thông tin để đưa vào list view:
	TCHAR szItemNo[MAX_PATH];					// số thứ tự cột "#"
	TCHAR szProcessName[MAX_PATH] = L"";		// cột tên process, mặc định là "Unknown"
	TCHAR szProcessID[MAX_PATH+64];				// cột process ID

	HMODULE hModule;
	DWORD cbNeeded;	

	if(EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
		GetModuleBaseName(hProcess, hModule, szProcessName,sizeof(szProcessName));
	if (szProcessName[0] == NULL)		// process have no names
		return;

	wsprintf(szProcessID, L"%u", dwProcID);
	wsprintf(szItemNo,L"%d", nLVItem);

	// đưa thông tin vào struct
	pProcessItem = new _PROCESS_LVITEM();
	pProcessItem->dwProcID	= dwProcID;
	pProcessItem->hProc	= hProcess;

	m_lvTracked.InsertItem(nLVItem,szItemNo);
	m_lvTracked.SetItemText(nLVItem,1,szProcessID);
	m_lvTracked.SetItemText(nLVItem,2,szProcessName);

	// lưu struct trên vào list view item
	m_lvTracked.SetItemData(nLVItem,(DWORD)pProcessItem);
}

void CTab_Process_Tracker::UpdateProcessTime(void)
{
	FILETIME ftCreate, ftExit, ftKernel, ftUser;

	int nLVItem = m_lvTracked.GetItemCount();

	for(int i = 0; i < nLVItem; i++)
	{
		_PROCESS_LVITEM *pProcessItem = (_PROCESS_LVITEM *)m_lvTracked.GetItemData(i);
		if(!pProcessItem->hProc)
			continue;

		if(GetProcessTimes(pProcessItem->hProc, &ftCreate, &ftExit, &ftKernel, &ftUser))
		{
			// chuỗi ngày giờ
			TCHAR szEndColText[128];
			TCHAR szLengthColText[128];

			TCHAR szFileDate[32] = { 0 };
			TCHAR szFileTime[32] = { 0 };

			CString cszText = m_lvTracked.GetItemText(i,3);		// Start
			if (cszText.Trim() == "")
			{
				if(!ftCreate.dwHighDateTime && !ftCreate.dwLowDateTime)
				{
					wcscpy_s(szFileDate, L"");
					wcscpy_s(szFileTime, L"");
				}
				else
				{
					GetDateString(&ftCreate, szFileDate, sizeof(szFileDate));
					GetTimeString(&ftCreate, szFileTime, sizeof(szFileTime));
				}

				wsprintf(szEndColText, L"%s %s", szFileDate, szFileTime);

				m_lvTracked.SetItemText(i,3,szEndColText);
				m_lvTracked.SetItemText(i,5,L"");
			}

			cszText = m_lvTracked.GetItemText(i,4);				// End
			if (cszText.Trim() == "")
			{
				if(!ftExit.dwHighDateTime && !ftExit.dwLowDateTime)
				{
					wcscpy_s(szFileDate, L"");
					wcscpy_s(szFileTime, L"");
					wcscpy_s(szLengthColText, L"");
				}
				else
				{
					GetDateString(&ftExit, szFileDate, sizeof(szFileDate));
					GetTimeString(&ftExit, szFileTime, sizeof(szFileTime));

					DWORD dwResult = GetFileTimeDifference(&ftCreate, &ftExit);

					SYSTEMTIME stLength;
					MiliSecsToSystemTime(dwResult, &stLength);
					wsprintf(szLengthColText, L"%02u:%02u:%02u", stLength.wHour, stLength.wMinute, stLength.wSecond);
				}

				wsprintf(szEndColText, L"%s %s", szFileDate, szFileTime);

				m_lvTracked.SetItemText(i,4,szEndColText);
				m_lvTracked.SetItemText(i,5,szLengthColText);
			}
		}
	}
}

void CTab_Process_Tracker::ClearLVData(void)
{
	int nLVItem = m_lvTracked.GetItemCount();
	_PROCESS_LVITEM *piProcessItem;

	for(int i = 0; i < nLVItem; ++i)
	{
		piProcessItem = (_PROCESS_LVITEM *)m_lvTracked.GetItemData(0);
		delete piProcessItem;

		m_lvTracked.DeleteItem(0);
	}
}

void CTab_Process_Tracker::OnDestroy()
{
	CDHtmlDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CTab_Process_Tracker::OnBnClickedOpen()
{
	// TODO: Add your control notification handler code here

	CFileDialog dlgOpen(true, L"*.log", L"*.log",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Log file|*.log||");

	if (dlgOpen.DoModal() == IDCANCEL)
		return;

	CString cstrFilePath = dlgOpen.GetPathName();

	CStdioFile csfIn;
	csfIn.Open(cstrFilePath, CStdioFile::modeRead);

	CString cstrLine;
	csfIn.ReadString(cstrLine);		// đọc dòng headers
	csfIn.ReadString(cstrLine);
	int i = 0;
	while (cstrLine != "")
	{
		CString cstrToken;
		int pos = 0;
		for (int j = 0; j < 6; ++j)
		{
			cstrToken = cstrLine.Tokenize(L"\t\t", pos);
			m_lvTracked.SetItemText(i, j, cstrToken);
		}

		++i;
	}

	csfIn.Close();
}

void CTab_Process_Tracker::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here

	CFileDialog dlgSave(false, L"*.log", L"*.log",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Log file|*.log||");

	if (dlgSave.DoModal() == IDCANCEL)
		return;

	CString cstrFileName = dlgSave.GetPathName();
	CFile cfOut;
	cfOut.Open(cstrFileName, CFile::modeCreate|CFile::modeWrite);

	TCHAR *szFileName;
	int i = m_lvTracked.GetNextItem(-1, LVNI_ALL);
	cfOut.Write(L"No\t\tPID\t\tProcess Name\t\tStart\t\tEnd\t\tLength\r\n", sizeof(TCHAR)*45);
	while (i != -1)
	{
		for (int j = 0; j < 6; ++j)
		{
			CString cstrItemText = m_lvTracked.GetItemText(i,j);

			szFileName = cstrItemText.GetBufferSetLength(cstrItemText.GetLength());
			cfOut.Write(szFileName, wcslen(szFileName)*sizeof(TCHAR));
			cfOut.Write(L"\t\t", 2*sizeof(TCHAR));
		}

		cfOut.Write(L"\r\n", 2*sizeof(TCHAR));

		i = m_lvTracked.GetNextItem(i, LVNI_ALL);
	}

	cfOut.Close();
}