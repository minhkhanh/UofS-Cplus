#pragma once
#include "afxcmn.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CTab_Process_Locker dialog

class CTab_Process_Locker : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CTab_Process_Locker)

public:
	CTab_Process_Locker(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_Process_Locker();
	// Overrides
	//HRESULT OnButtonOK(IHTMLElement *pElement);
	//HRESULT OnButtonCancel(IHTMLElement *pElement);

	// Dialog Data
	enum { IDD = IDD_TAB_PROCESS_LOCKER, IDH = IDR_HTML_TAB_X };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedMoniter();
	afx_msg void OnBnClickedUnmoniter();
	afx_msg void OnBnClickedAdd2list();

private:
	CListCtrl m_lvDenied;
	CListCtrl m_buttMonitor;
	CListCtrl m_buttUnMonitor;
	CListCtrl m_buttAdd;
	CListCtrl m_buttRemove;
	CButton m_chkLogKeyboard;
public:
	afx_msg void OnBnClickedButtonRemove();
private:
	void UpdateListView(void);
	void LoadListView();
	void SaveListView(void);
	void StatusMonitor();
	void StatusUnMonitor();
public:
	afx_msg void OnBnClickedLogkeyboard();
	afx_msg void OnBnClickedViewlog();
};
