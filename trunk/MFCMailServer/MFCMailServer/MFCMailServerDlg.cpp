
// MFCMailServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include <stdarg.h>
#include "MFCMailServer.h"
#include "MFCMailServerDlg.h"
#include "Pop3.h"
#include "Smtp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	StartMailServer();
}


void CMFCMailServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_LBString(pDX, IDC_LIST1, m_log);
	DDX_Control(pDX, IDC_LIST1, m_listBoxCtrl);
}

BEGIN_MESSAGE_MAP(CMFCMailServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMFCMailServerDlg::OnBnClickedOk)
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

	WriteLog("======================================================");
	WriteLog("%s - SMTP Server started. Listening on port 25");
	WriteLog("%s - POP3 Server started. Listening on port 110");
}

void CMFCMailServerDlg::WriteLog( CString message )
{
	UpdateData(TRUE);
	m_listBoxCtrl.AddString(message);
	UpdateData(FALSE);
}