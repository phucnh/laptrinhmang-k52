// NewMailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "NewMailDlg.h"
#include "SMTP.h"
#include "Mime.h"
#include "MimeCode.h"
#include "EntitiesServices.h"


// CNewMailDlg dialog

IMPLEMENT_DYNCREATE(CNewMailDlg, CDHtmlDialog)

CNewMailDlg::CNewMailDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CNewMailDlg::IDD, CNewMailDlg::IDH, pParent)
	, m_From(_T(""))
	, m_To(_T(""))
	, m_Subject(_T(""))
	, m_TextBody(_T(""))
	, m_serverip(0)
	, m_sCc(_T(""))
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
	DDX_Text(pDX, IDC_EDIT5, m_sCc);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_lstFileList);
}

BOOL CNewMailDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CNewMailDlg, CDHtmlDialog)
	ON_BN_CLICKED(IDOK, &CNewMailDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADDFILE, &CNewMailDlg::OnBnClickedButtonAddfile)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CNewMailDlg::OnBnClickedButtonRemove)
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

		if (m_lstFileList.GetCount() == 0)
			msgmime.SetMailMime(msg.From,msg.To,msg.Cc,msg.Subject,"",msg.TextBody);
		else
		{
			for (int i=0; i<m_lstFileList.GetCount() - 1;i++)
			{
				CString _tempPath;
				m_lstFileList.GetDlgItemText(i,_tempPath);
				msgmime.SetMailMime(msg.From,msg.To,msg.Cc,msg.Subject,_tempPath,msg.TextBody);
			}
		}
		//msgmime.SetMailMime(msg.From,msg.To,msg.Cc,msg.Subject,filePath,msg.TextBody);
		//su dung ham nay de tao mime
		
		/*CMimeEnvironment::SetAutoFolding(true); 
		int nSize = msgmime.GetLength();
		char* pBuff = new char[nSize];
		nSize = msgmime.Store(pBuff, nSize);
		msg.TextBody = pBuff;*/
		_smtp.SendMessage(&msg);

		//CMailHeaderServices* mailHeaderService = new CMailHeaderServices();
		//msg.GroupId = 3; //Group Sent mail
		//msg.UserId = globalUser.UserId();
		//mailHeaderService->InsertNewMail(&msg);

		//if (mailHeaderService != NULL)	delete mailHeaderService;

		AfxMessageBox("Success",MB_OK);
		UpdateData(FALSE);
		//long add
}

INT_PTR CNewMailDlg::DoModal(MailHeader* mailHdr,CString prefixSubject)
{
	if (mailHdr != NULL)
	{
		this->m_From = mailHdr->From;
		this->m_To = mailHdr->To;
		this->m_sCc = mailHdr->Cc;
		this->m_TextBody = mailHdr->TextBody;
		this->m_Subject = prefixSubject + mailHdr->Subject;
	}
	return CDHtmlDialog::DoModal();
}

INT_PTR CNewMailDlg::DoModal()
{
	return CDHtmlDialog::DoModal();
}

void CNewMailDlg::OnBnClickedButtonAddfile()
{
	CFileDialog fOpenDlg(TRUE, "*", "All Files", OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		"All Files (*.*)|*.*||", this);

	CString fileName;
	fOpenDlg.GetOFN().lpstrFile = fileName.GetBuffer(_MAX_PATH);
	fOpenDlg.GetOFN().nMaxFile = _MAX_PATH;
	fOpenDlg.m_pOFN->lpstrTitle="Attach File";
	fOpenDlg.m_pOFN->lpstrInitialDir="c:";

	INT_PTR nResult = fOpenDlg.DoModal();

	if(nResult == IDOK)
	{
		if (!m_lstFileList.FindString(0,fileName))
			AfxMessageBox(_T("File is already in list"),MB_OK);
		else
			m_lstFileList.AddString(fileName);
	}
}

void CNewMailDlg::OnBnClickedButtonRemove()
{
	int pos = -1;
	pos = m_lstFileList.GetCurSel();

	if (pos < 0)
	{
		AfxMessageBox(_T("Select one path"));
	}
	else
	{
		m_lstFileList.DeleteString(pos);
	}
}
