// NewMailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "NewMailDlg.h"
#include "SMTP.h"
#include "Mime.h"
#include "MimeCode.h"


// CNewMailDlg dialog

IMPLEMENT_DYNCREATE(CNewMailDlg, CDHtmlDialog)

CNewMailDlg::CNewMailDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CNewMailDlg::IDD, CNewMailDlg::IDH, pParent)
	, m_From(_T(""))
	, m_To(_T(""))
	, m_Subject(_T(""))
	, m_TextBody(_T(""))
	, m_serverip(0)
{

}

CNewMailDlg::~CNewMailDlg()
{
}

void CNewMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_From);
	DDX_Text(pDX, IDC_EDIT2, m_To);
	DDX_Text(pDX, IDC_EDIT3, m_Subject);
	DDX_Text(pDX, IDC_EDIT4, m_TextBody);
	DDX_Control(pDX, IDC_IPADDRESS1, m_serverIP);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_serverip);
}

BOOL CNewMailDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CNewMailDlg, CDHtmlDialog)
	ON_BN_CLICKED(IDOK, &CNewMailDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CNewMailDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CNewMailDlg message handlers

HRESULT CNewMailDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CNewMailDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CNewMailDlg::OnBnClickedOk()
{

	UpdateData(TRUE);

	CString _serverIP;
	_serverIP.Format("%d",m_serverip);

		CSMTP _smtp(_serverIP,25);
		_smtp.Connect();
		MailHeader msg;
		msg.From = m_From;
		msg.To = m_To;
		msg.Subject = m_Subject;
		msg.TextBody = m_TextBody;
		//long 20101108


		CMimeMessage msgmime;
		//msgmime.SetMailMime(from,to,Cc,subject,filePath,textBody);
		//su dung ham nay de tao mime
		
		/*CMimeEnvironment::SetAutoFolding(true); 
		int nSize = msgmime.GetLength();
		char* pBuff = new char[nSize];
		nSize = msgmime.Store(pBuff, nSize);
		msg.TextBody = pBuff;*/
		_smtp.SendMessage(&msg);
		AfxMessageBox("Success",MB_OK);
		UpdateData(FALSE);
		//long add
}

INT_PTR CNewMailDlg::DoModal(MailHeader* mailHdr,CString prefixSubject)
{
	if (mailHdr != NULL)
		this->m_Subject = prefixSubject + mailHdr->Subject;
	return CDHtmlDialog::DoModal();
}

INT_PTR CNewMailDlg::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDHtmlDialog::DoModal();
}