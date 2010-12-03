
// MFCMailClientDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMFCMailClientDlg dialog
class CMFCMailClientDlg : public CDialog
{
// Construction
public:
	CStatusBarCtrl	m_wndStatusBar;

	CMFCMailClientDlg(CWnd* pParent = NULL);	// standard constructor
	void SetIconToMenuButton();

// Dialog Data
	enum { IDD = IDD_MFCMAILCLIENT_DIALOG };

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

	void CreateGroupTree();
	void CreateListMailColumn();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMessageNewmessage();
	afx_msg void OnAcountAccount();
	CTreeCtrl m_GroupTree;
	afx_msg void OnMessageCheckmail();
	CListCtrl m_ListMail;
	CString m_MailMessage;
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_AddList;
	CButton m_buttonNewMail;
	CButton m_btnDeleteMail;
	afx_msg void OnBnClickedButton2();
	CButton m_btnReply;
	CButton m_btnForward;
	CButton m_btnInbox;
	CButton m_btnTrash;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	void OnNewMail();
	afx_msg void OnBnClickedButton5();

	void ReplySelectedMail();
	void ForwardMessage();
	void UpdateStatusbar();
	void InitStatusbar();
	void SBPartsSetting(int cxParent, int cyParent);
	afx_msg void OnMessageReplymessage();
	afx_msg void OnMessageForwardmessage();
	afx_msg void OnBnClickedButton4();

	void Dang_TestInsertNewMail();
};
