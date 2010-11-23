
// MFCMailServerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Resource.h"


#define SB_HEIGHT 20
#define TOOLBAR_HEIGHT	42
enum STATUSBAR_PARTS{
	SBP_STATUS = 0,
	SBP_SMTPCONNECTION,
	SBP_POP3CONNECTION,
	SBP_NUMPARTS
};


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

	CStatusBarCtrl	m_wndStatusBar;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
BOOL m_isIconShow;

public:
	BOOL StartMailServer();
	void StopMailServer();
	void WriteLog(CString message);
	afx_msg void OnBnClickedOk();
	CString m_log;
	CListBox m_listBoxCtrl;
	void UpdateStatusbar();
	void InitStatusbar();
	void SBPartsSetting(int cxParent, int cyParent);
};
