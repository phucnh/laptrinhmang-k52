
// MFCMailServerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Resource.h"


// CMFCMailServerDlg dialog
class CMFCMailServerDlg : public CDialog
{
// Construction
public:
	CMFCMailServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCMAILSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
BOOL m_isIconShow;

public:
	BOOL StartMailServer();
	void WriteLog(CString message);
	afx_msg void OnBnClickedOk();
	CString m_log;
	CListBox m_listBoxCtrl;
};
