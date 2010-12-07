
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

//Bien toan cuc:
extern CStringList		AccountList;
extern CString			sMailStoreDirectory;


// CMFCMailServerDlg dialog
class CMFCMailServerDlg : public CDialog
{
// Construction
public:
	CMFCMailServerDlg(CWnd* pParent = NULL);	// standard constructor
	CListBox	m_lstLog;
	void SetIconToButton();

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
	afx_msg void OnAccountsManageaccounts();
	afx_msg void OnAccountsNewaccount();
	CButton m_btnStartServer;
	CButton m_btnStopServer;
	CButton m_btnSettings;
	CButton m_btnUsers;
	CButton m_btnExit;
	afx_msg void OnBnClickedButton1();
	 void testInsertNewMail();
	 afx_msg void OnBnClickedButton2();
	 afx_msg void OnBnClickedButton4();
	 afx_msg void OnBnClickedButton3();
	 bool SystemTrayMessage(DWORD dwMsg,CString aStrToolTip,int nResId, HWND m_hWnd);
	 afx_msg void OnBnClickedButton6();
};
