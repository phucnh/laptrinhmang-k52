#pragma once


// CNewUserDlg dialog

class CNewUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewUserDlg)

public:
	CNewUserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewUserDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEWUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_sUsername;
	CString m_sPassword;
	afx_msg void OnBnClickedOk();
};
