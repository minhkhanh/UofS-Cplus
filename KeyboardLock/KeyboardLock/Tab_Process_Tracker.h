#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CTab_Process_Tracker dialog

class CTab_Process_Tracker : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CTab_Process_Tracker)

public:
	CTab_Process_Tracker(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_Process_Tracker();
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
	BOOL m_bIsMonitored;
	HANDLE m_hThread;
	CListCtrl m_lvTracked;
	CButton m_btnSave;

	struct _PROCESS_LVITEM
	{
		DWORD dwProcID;
		HANDLE hProc;

		_PROCESS_LVITEM()
		{
			dwProcID = 0;
			hProc = NULL;
		}

		~_PROCESS_LVITEM()
		{
			dwProcID = 0;

			if(hProc)
				CloseHandle(hProc);
			hProc = NULL;
		}
	};

	void ClearLVData(void);
	void UpdateProcessList(void);
	void AddProcessToList(DWORD dwProcID);
	void UpdateProcessTime(void);

public:
	afx_msg void OnBnClickedMonitor();
	afx_msg void OnBnClickedSave();
	static DWORD WINAPI RunThread(void* lpVoid);
	afx_msg void OnDestroy();
private:
	CButton m_btnOpen;
public:
	afx_msg void OnBnClickedOpen();
};
