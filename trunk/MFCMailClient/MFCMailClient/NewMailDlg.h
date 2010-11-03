#pragma once
#include "afxcmn.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CNewMailDlg dialog

class CNewMailDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CNewMailDlg)

public:
	CNewMailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewMailDlg();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_NEW_MAIL_DIALOG, IDH = IDR_HTML_NEWMAILDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	CString m_From;
	CString m_To;
	CString m_Subject;
	CString m_TextBody;
	CIPAddressCtrl m_serverIP;
	DWORD m_serverip;
};
