// AccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "AccountDlg.h"
#include "Pop3.h"
#include "MailMessage.h"
#include "MFCMailClientDlg.h"


// CAccountDlg dialog

IMPLEMENT_DYNCREATE(CAccountDlg, CDHtmlDialog)

CAccountDlg::CAccountDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CAccountDlg::IDD, CAccountDlg::IDH, pParent)
	, m_ServerIP(0)
	, m_UserName(_T(""))
	, m_Password(_T(""))
{

}

CAccountDlg::~CAccountDlg()
{
}

void CAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ServerIP);
	DDX_Text(pDX, IDC_EDIT1, m_UserName);
	DDX_Text(pDX, IDC_EDIT2, m_Password);
}

BOOL CAccountDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CAccountDlg, CDHtmlDialog)
	
	ON_BN_CLICKED(IDOK, &CAccountDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CAccountDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CAccountDlg message handlers

HRESULT CAccountDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CAccountDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


void CAccountDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CString _serverIP;
	_serverIP.Format("%d",m_ServerIP);
	CString _username = m_UserName;
	CString _password = m_Password;

	CPop3::GetInstance(_serverIP,110,_username,_password)->Connect();
	OnCancel();
	UpdateData(FALSE);
}
