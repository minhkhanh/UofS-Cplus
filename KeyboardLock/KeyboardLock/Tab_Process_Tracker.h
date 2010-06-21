#pragma once
#include "afxwin.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// Tab_Process_Tracker dialog

class Tab_Process_Tracker : public CDHtmlDialog
{
	DECLARE_DYNCREATE(Tab_Process_Tracker)

public:
	Tab_Process_Tracker(CWnd* pParent = NULL);   // standard constructor
	virtual ~Tab_Process_Tracker();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_TAB_PROCESS_TRACKER, IDH = IDR_HTML_TAB_PROCESS_TRACKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedButtonMonitor();
private:
	CButton m_btnMonitor;
public:
	afx_msg void OnBnClickedMonitor();
};
