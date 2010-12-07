// UserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "UserDlg.h"


// CUserDlg dialog

IMPLEMENT_DYNAMIC(CUserDlg, CDialog)

CUserDlg::CUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDlg::IDD, pParent)
	, m_sUsername(_T(""))
	, m_sPassword(_T(""))
{

}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sUsername);
	DDX_Text(pDX, IDC_EDIT2, m_sPassword);
	DDX_Control(pDX, IDC_LIST1, m_lsbUserList);
}


BEGIN_MESSAGE_MAP(CUserDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUserDlg::OnBnClickedButton1)
	ON_LBN_DBLCLK(IDC_LIST1, &CUserDlg::OnLbnDblclkList1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUserDlg::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON2, &CUserDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

//CArray<MailUser,MailUser>* CUserDlg::GetAllUser()
//{
//	CArray<MailUser,MailUser>* mailUsers = new CArray<MailUser,MailUser>();
//	MailUser* _mailUser = new MailUser();
//	// TODO: implement more code in here
//
//	return mailUsers;
//}

void CUserDlg::BindAllUserToListBox()
{
	UpdateData(TRUE);

	m_lsbUserList.ResetContent();

	MailUser* mailU=new MailUser();
	CString userStr;
	CArray<MailUser,MailUser&>* mailUsers =mailU->GetAllUsers();

	int numberUser=mailUsers->GetCount();

	if ((mailUsers != NULL) &&numberUser!=NULL)
		{

			//CString result;
			//result=mailUsers->GetAt(1)._username;
		for (int i=0;i<numberUser;i++)
		{
		
			userStr.Format("%s",mailUsers->GetAt(i)._username);
			m_lsbUserList.AddString(userStr);
		}
	}
	UpdateData(FALSE);
}
// CUserDlg message handlers

void CUserDlg::OnBnClickedButton1()
{
	m_sOldUsername = m_sUsername;

	UpdateData(TRUE);

	MailUser* mailUser = new MailUser();
	mailUser->_username = m_sUsername;
	mailUser->_password = m_sPassword;


	if (!mailUser->UpdateUserByUsername(m_sOldUsername,mailUser))	return;

	/*if (isInsertMode)
	{
	mailUser->_username = m_sUsername;
	mailUser->_password = m_sPassword;

	if (!mailUser->InsertNewUser(mailUser))	return;
	}
	else
	{*/
		
	/*}*/


	BindAllUserToListBox();
	UpdateData(FALSE);
}

void CUserDlg::OnLbnDblclkList1()
{
//	POSITION pos = m_lsbUserList;
	int _index = m_lsbUserList.GetCurSel();
	MailUser* mailUser = new MailUser();
	m_lsbUserList.GetText(_index,m_sUsername);
	// TODO : Implement listbox double click in here
}

BOOL CUserDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	BindAllUserToListBox();

	return TRUE;
}

void CUserDlg::OnLbnSelchangeList2()
{
	UpdateData(TRUE);
	int _index = m_lsbUserList.GetCurSel();
	MailUser* mailUser = new MailUser();
	m_lsbUserList.GetText(_index,m_sUsername);
	mailUser = mailUser->GetUserByUsername(m_sUsername);
	m_sPassword = mailUser->_password;
	UpdateData(FALSE);

}

void CUserDlg::OnBnClickedButton2()
{
  UpdateData();
  MailUser* mailUser = new MailUser();
  if(mailUser->CheckUsernameExist(m_sUsername))
	  try
		{
			mailUser=new MailUser();
			mailUser->_username=m_sUsername;
			mailUser->_password=m_sPassword;
			mailUser->DeleteUser(mailUser);
			BindAllUserToListBox();
			m_sUsername="";
			m_sPassword="";
			UpdateData(FALSE);
		}
	  catch (CException* e)
	  {
		  throw;
		  e->Delete();
	  }
}
