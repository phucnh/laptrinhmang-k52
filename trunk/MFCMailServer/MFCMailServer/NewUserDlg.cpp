// NewUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "NewUserDlg.h"
#include "MailUser.h"


// CNewUserDlg dialog

IMPLEMENT_DYNAMIC(CNewUserDlg, CDialog)

CNewUserDlg::CNewUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewUserDlg::IDD, pParent)
	, m_sUsername(_T(""))
	, m_sPassword(_T(""))
{

}

CNewUserDlg::~CNewUserDlg()
{
}

void CNewUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sUsername);
	DDX_Text(pDX, IDC_EDIT2, m_sPassword);
}


BEGIN_MESSAGE_MAP(CNewUserDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewUserDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewUserDlg message handlers

void CNewUserDlg::OnBnClickedOk()
{
	MailUser* mailUser = new MailUser();

	mailUser->_username = m_sUsername;
	mailUser->_password = m_sPassword;

	mailUser->InsertNewUser(mailUser);
	OnOK();
}
