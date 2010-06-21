// Tab_Process_Tracker.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardLock.h"
#include "Tab_Process_Tracker.h"


// Tab_Process_Tracker dialog

IMPLEMENT_DYNCREATE(Tab_Process_Tracker, CDHtmlDialog)

Tab_Process_Tracker::Tab_Process_Tracker(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(Tab_Process_Tracker::IDD, Tab_Process_Tracker::IDH, pParent)
{

}

Tab_Process_Tracker::~Tab_Process_Tracker()
{
}

void Tab_Process_Tracker::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_MONITOR, m_btnMonitor);
}

BOOL Tab_Process_Tracker::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(Tab_Process_Tracker, CDHtmlDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &Tab_Process_Tracker::OnBnClickedButtonMonitor)
	ON_BN_CLICKED(IDB_MONITOR, &Tab_Process_Tracker::OnBnClickedMonitor)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(Tab_Process_Tracker)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// Tab_Process_Tracker message handlers

HRESULT Tab_Process_Tracker::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT Tab_Process_Tracker::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void Tab_Process_Tracker::OnBnClickedButtonMonitor()
{
	// TODO: Add your control notification handler code here

	// chuyển đổi text trên button
	CString cstrBtnText;
	m_btnMonitor.GetWindowText(cstrBtnText);
	if (cstrBtnText == "Moniter")
		m_btnMonitor.SetWindowText(L"Unmonitor");
	else
		m_btnMonitor.SetWindowText(L"Monitor");


}

void Tab_Process_Tracker::OnBnClickedMonitor()
{
	// TODO: Add your control notification handler code here


}
