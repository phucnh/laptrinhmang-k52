
// MFCMailServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include <stdarg.h>
#include "MFCMailServer.h"
#include "MFCMailServerDlg.h"
#include "Pop3.h"
#include "Smtp.h"
#include "UserDlg.h"
#include "NewUserDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStringList		AccountList;
CString			sMailStoreDirectory;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CString GetAccountInfo(CString sUsername, int iIndex)
{
	CString sReturn = "";
	for (POSITION pos = AccountList.GetHeadPosition(); pos != NULL;)
	{
		CString sTemp = AccountList.GetNext(pos);
		int i = sTemp.Find(";");
		int j = sTemp.Find(";", i+1);
		if ((i>0) && (j>0))
		{
			CString s1 = sTemp.Left(i);
			CString s2 = sTemp.Mid(i+1, j-i-1);
			CString s3 = sTemp.Mid(j+1, sTemp.GetLength()-j-2);
			if (s2.CompareNoCase(sUsername) == 0)
			{
				switch (iIndex)
				{
				case 1 : return s1;
				case 2 : return s2;
				case 3 : return s3;
				}
			}
		}
	}
	return sReturn;
}

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFCMailServerDlg dialog




CMFCMailServerDlg::CMFCMailServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCMailServerDlg::IDD, pParent)
	, m_log(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//AfxSocketInit();
	//StartMailServer();
}


void CMFCMailServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_LBString(pDX, IDC_LIST1, m_log);
	DDX_Control(pDX, IDC_LIST1, m_listBoxCtrl);

	//StartMailServer();
	DDX_Control(pDX, IDC_BUTTON1, m_btnStartServer);
	DDX_Control(pDX, IDC_BUTTON2, m_btnStopServer);
	DDX_Control(pDX, IDC_BUTTON3, m_btnSettings);
	DDX_Control(pDX, IDC_BUTTON4, m_btnUsers);
	DDX_Control(pDX, IDC_BUTTON6, m_btnExit);
}

BEGIN_MESSAGE_MAP(CMFCMailServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMFCMailServerDlg::OnBnClickedOk)
	ON_COMMAND(ID_ACCOUNTS_MANAGEACCOUNTS, &CMFCMailServerDlg::OnAccountsManageaccounts)
	ON_COMMAND(ID_ACCOUNTS_NEWACCOUNT, &CMFCMailServerDlg::OnAccountsNewaccount)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCMailServerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCMailServerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCMailServerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCMailServerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCMailServerDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMFCMailServerDlg message handlers

BOOL CMFCMailServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	// TODO: Add extra initialization here
	m_isIconShow = FALSE;

	InitStatusbar();
	SetIconToButton();

	StartMailServer();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCMailServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCMailServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCMailServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCMailServerDlg::OnBnClickedOk()
{
	//OnOK();
}

BOOL CMFCMailServerDlg::StartMailServer()
{
	serverPop3Socket = new CPop3(this);
	if (!serverPop3Socket->Create(settingPop3Port))
	{
		AfxMessageBox(_T("Can not create POP3 server socket."));
		return FALSE;
	}
	else
	{
		if (!serverPop3Socket->Listen(100))
		{
			AfxMessageBox(_T("Can not listen from POP3 server."));
			return FALSE;
		}
	}

	serverSmtpSocket = new CSmtp(this);
	if (!serverSmtpSocket->Create(settingSmtpPort))
	{
		AfxMessageBox(_T("Can not create SMTP server socket."));
		return FALSE;
	}
	else
	{
		if(!serverSmtpSocket->Listen(100))
		{
			AfxMessageBox(_T("Can not listen from SMTP server"));
			return FALSE;
		}
	}

	pop3RequestId = 0;
	nPop3ConnectionsCount = 0;

	UpdateStatusbar();

	CString message;
	message.Format(_T("%s - SMTP Server started. Listening on port 25"),GetCurrentTimeStr());
	WriteLog(message);
	message.Format(_T("%s - POP3 Server started. Listening on port 110"),GetCurrentTimeStr());
	WriteLog(message);

	return TRUE;
}

void CMFCMailServerDlg::WriteLog( CString message )
{
	UpdateData(TRUE);
	m_listBoxCtrl.AddString(message);
	UpdateData(FALSE);
}

void CMFCMailServerDlg::UpdateStatusbar()
{
	CString	strSMTPConnections;
	CString	strPOP3Connections;

	strSMTPConnections.Format("SMTP Connections : %i ", nSmtpConnectionsCount);
	strPOP3Connections.Format("POP3 Connections : %i ", nPop3ConnectionsCount);

	m_wndStatusBar.SetText(strSMTPConnections, SBP_SMTPCONNECTION, 0);
	m_wndStatusBar.SetText(strPOP3Connections, SBP_POP3CONNECTION, 0);
	m_wndStatusBar.SetText("Mail Server", SBP_STATUS, SBT_NOBORDERS);
}

void CMFCMailServerDlg::InitStatusbar()
{
	CRect rcDialog;
	GetWindowRect(&rcDialog);
	rcDialog.top = rcDialog.bottom - SB_HEIGHT;

	m_wndStatusBar.Create(WS_CHILD | WS_BORDER | WS_VISIBLE, rcDialog, this, AFX_IDW_STATUS_BAR);

	SBPartsSetting(rcDialog.Width(), rcDialog.Height());

	UpdateStatusbar();
}

void CMFCMailServerDlg::SBPartsSetting(int cxParent, int cyParent)
{
	int	nWidths[SBP_NUMPARTS];

	CString	strSMTPConnections;
	CString	strPOP3Connections;
	strSMTPConnections.Format("SMTP Connections : %i ", 0x00);
	strPOP3Connections.Format("POP3 Connections : %i ", 0x00);

	int nSMTPConnectionWidth = m_wndStatusBar.GetDC()->GetTextExtent(strSMTPConnections).cx;
	int nPOP3ConnectionWidth = m_wndStatusBar.GetDC()->GetTextExtent(strPOP3Connections).cx;
	//int nTimerWidth = m_wndStatusBar.GetDC()->GetTextExtent(CString("88:88 PM    ")).cx;

	nWidths[SBP_STATUS] = cxParent - 50 - nSMTPConnectionWidth- nPOP3ConnectionWidth;
	nWidths[SBP_SMTPCONNECTION] = nSMTPConnectionWidth + nWidths[SBP_STATUS];
	nWidths[SBP_POP3CONNECTION] = nPOP3ConnectionWidth + nWidths[SBP_SMTPCONNECTION];

	m_wndStatusBar.SetMinHeight(SB_HEIGHT);
	m_wndStatusBar.SetParts(SBP_NUMPARTS, nWidths);
}

void CMFCMailServerDlg::StopMailServer()
{
	if (serverSmtpSocket)
	{
		serverSmtpSocket->Close();
		delete serverSmtpSocket;
		serverSmtpSocket = NULL;

		CString message;
		message.Format(_T("%s - SMTP Server stopped."),GetCurrentTimeStr());
		WriteLog("%s - SMTP Server stopped.");
	}

	if (serverPop3Socket)
	{
		serverPop3Socket->Close();
		delete serverPop3Socket;
		serverPop3Socket = NULL;

		CString message;
		message.Format(_T("%s - POP3 Server stopped."),GetCurrentTimeStr());
		WriteLog("%s - SMTP Server stopped.");
	}

	UpdateStatusbar();
}

void CMFCMailServerDlg::OnDestroy()
{
	CDialog::OnDestroy();
	StopMailServer();
}

void CMFCMailServerDlg::OnAccountsManageaccounts()
{
	CUserDlg userDlg;
	userDlg.DoModal();
}

void CMFCMailServerDlg::OnAccountsNewaccount()
{
	CNewUserDlg newUserDlg;
	newUserDlg.DoModal();
}

void CMFCMailServerDlg::SetIconToButton()
{
	HICON hIcn= (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_STARTSERVER),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnStartServer.SetIcon(hIcn);

	hIcn= (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_STOPSERVER),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnStopServer.SetIcon(hIcn);

	hIcn= (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_SETTINGS),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnSettings.SetIcon(hIcn);

	hIcn= (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_USERS),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnUsers.SetIcon(hIcn);

	hIcn= (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_EXIT),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnExit.SetIcon(hIcn);
}

void CMFCMailServerDlg::OnBnClickedButton1()
{
	StartMailServer();
	/*CWnd* pWnd = GetDlgItem(IDC_BUTTON2); 
	pWnd->EnableWindow(TRUE);
	this->EnableWindow(FALSE);*/
}
void CMFCMailServerDlg::testInsertNewMail()
{
 MailUser* mail=new MailUser();
 mail->_username="dangbka";
 mail->_password="123456";
 try
 {
	  mail->InsertNewUser(mail);
 
 }
 catch (CException* e)
 {
	 throw;
 }
}

void CMFCMailServerDlg::OnBnClickedButton2()
{
	StopMailServer();
	/*CWnd* pWnd = GetDlgItem(IDC_BUTTON1);
	pWnd->EnableWindow(TRUE);
	this->EnableWindow(FALSE);*/
}

void CMFCMailServerDlg::OnBnClickedButton4()
{
	OnAccountsNewaccount();
}

void CMFCMailServerDlg::OnBnClickedButton3()
{
	OnAccountsManageaccounts();
}

void CMFCMailServerDlg::OnBnClickedButton6()
{
	//this->OnDestroy();
	CString username = "abc";
	MailHeader* test = new MailHeader();

	//test GETALLMAIL()
	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
	listMailHeader = test->getAllInboxMailByUser(username);
	INT resultTest = listMailHeader->GetCount();
	
	CString result,temp;
	result.Format("Cac CC  cua tat ca mail la :");

	for(int i=0;i<listMailHeader->GetSize();i++)
	{
		temp.Format("%s",listMailHeader->GetAt(i).Cc);
		result+=temp;
	}
	
	m_listBoxCtrl.AddString(result);

	//test GetMailSentByUSername
	CString result2,temp2;
	result2.Format("Cac CC  cua tat ca mail cua user dang la :");
	m_listBoxCtrl.AddString(result2);
	
	CArray<MailHeader,MailHeader&>* listMailHeader2=new CArray<MailHeader,MailHeader&>();
	 listMailHeader2=test->getAllSentMailByUser("dang");
	 for(int i=0;i<listMailHeader2->GetSize();i++)
	 {
		 m_listBoxCtrl.AddString(listMailHeader2->GetAt(i).Cc);
	 }
	 
	 CString result3,temp3;
	 result2.Format("Cac CC  cua mailinbox cua user dang@dang.com la :");
	 m_listBoxCtrl.AddString(result2);

	 CArray<MailHeader,MailHeader&>* listMailHeader3=new CArray<MailHeader,MailHeader&>();
	 listMailHeader3=test->getAllInboxMailByUser("dang@dang.com");
	 for(int i=0;i<listMailHeader3->GetSize();i++)
	 {
		 m_listBoxCtrl.AddString(listMailHeader3->GetAt(i).Cc);
	 }
	
	 //int resul2 = listMailHeader->GetCount();
	//bool isDeleted = test->deleteMail(3);
		

	//test delete Mail
		//MailHeader* mailH =new MailHeader();
		//if(mailH->deleteMail(2))
		// AfxMessageBox("OK");
		//else
		// AfxMessageBox("False");
}
