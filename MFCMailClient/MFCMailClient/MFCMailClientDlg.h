
// MFCMailClientDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Mime.h"


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
	void CreateAttachListColumn();
	DECLARE_MESSAGE_MAP()
public:
	HTREEITEM inboxTreeNode;
	HTREEITEM sentTreeNode;
	HTREEITEM trashTreeNode;

	CMimeMessage* _curentMIME;

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

	void Checkmail();
	void BindMailToListBox(CArray<MailHeader,MailHeader&>* listMail);
	void BindToAttachmentList(CMimeMessage* _mime);
	void ReplySelectedMail();
	void ForwardMessage();
	void UpdateStatusbar();
	void InitStatusbar();
	void SBPartsSetting(int cxParent, int cyParent);
	afx_msg void OnMessageReplymessage();
	afx_msg void OnMessageForwardmessage();
	afx_msg void OnBnClickedButton4();


	void testLogin();
	void testInsertNewUser();
	void testDeleteUserById();	
	void testGetUserById();
	void testGetUserByUsername();
	void testUpdateUserById();
	void testUpdateUserByUsername();
	void testChangePassword();
	void testGetAllMail();
	void testGetMailByMailId();
	void testGetMailByUserId();
	void testGetMailByGroupId();
	void testGetByUserIdGroupId();
	void testGetMailByUserIdGroupId();
	void testInsertNewMail();
	
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_lstAttachControl;
	afx_msg void OnHdnItemdblclickListAttachlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListAttachlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton6();
};
