#pragma once
#include "afxwin.h"
#include "resource.h"
#include "MailUser.h"


// CUserDlg dialog

class CUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserDlg)

public:
	CUserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_sOldUsername;
	CString m_sUsername;
	CString m_sPassword;
	CListBox m_lsbUserList;

	BOOL isInsertMode;

	CArray<MailUser,MailUser>* GetAllUser();
	void BindAllUserToListBox();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLbnDblclkList1();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButton2();
};
