#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CAccountDlg dialog

class CAccountDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CAccountDlg)

public:
	CAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAccountDlg();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_ACCOUNT_DIALOG, IDH = IDR_HTML_ACCOUNTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedOk();
	DWORD m_ServerIP;
	CString m_UserName;
	CString m_Password;
};
