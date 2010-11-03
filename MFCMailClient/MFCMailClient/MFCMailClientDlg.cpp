
// MFCMailClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "MFCMailClientDlg.h"
#include "NewMailDlg.h"
#include "AccountDlg.h"

#include "MailMessage.h"

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
public:

	
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


// CMFCMailClientDlg dialog




CMFCMailClientDlg::CMFCMailClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCMailClientDlg::IDD, pParent)
	, m_MailMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCMailClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_GroupTree);
	DDX_Control(pDX, IDC_LIST3, m_ListMail);
	DDX_Text(pDX, IDC_EDIT1, m_MailMessage);
	DDX_Control(pDX, IDC_LIST2, m_AddList);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonNewMail);
	DDX_Control(pDX, IDC_BUTTON2, m_btnDeleteMail);
	DDX_Control(pDX, IDC_BUTTON3, m_btnReply);
	DDX_Control(pDX, IDC_BUTTON4, m_btnForward);
	DDX_Control(pDX, IDC_BUTTON5, m_btnInbox);
	DDX_Control(pDX, IDC_BUTTON6, m_btnTrash);
}

BEGIN_MESSAGE_MAP(CMFCMailClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MESSAGE_NEWMESSAGE, &CMFCMailClientDlg::OnMessageNewmessage)
	ON_COMMAND(ID_ACOUNT_ACCOUNT, &CMFCMailClientDlg::OnAcountAccount)
	ON_COMMAND(ID_MESSAGE_CHECKMAIL, &CMFCMailClientDlg::OnMessageCheckmail)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CMFCMailClientDlg::OnLvnItemchangedList3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCMailClientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCMailClientDlg message handlers

void CMFCMailClientDlg::CreateListMailColumn()
{
	m_ListMail.ModifyStyle(m_ListMail.GetStyle(),WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_AUTOARRANGE);
	m_ListMail.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	m_ListMail.InsertColumn(0, "From",LVCFMT_LEFT, 130);
	m_ListMail.InsertColumn(1, "Subject",LVCFMT_LEFT, 270);
	m_ListMail.InsertColumn(2, "Date",LVCFMT_LEFT, 90);

	//lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE;
	//lvColumn.cx = 30;
	//lvColumn.pszText = "From";
	//lvColumn.cchTextMax = strlen(lvColumn.pszText);
	//lvColumn.iImage = 1;
	////m_ListMail.InsertColumn(1, &lvColumn);
	//m_ListMail.InsertColumn(0,"From",200);
	//m_ListMail.InsertColumn(1,"Subject",450);
	//m_ListMail.InsertColumn(2,"Date",100);

	//lvColumn.cx = 200;
	//lvColumn.pszText = "Subject";
	//lvColumn.cchTextMax = strlen(lvColumn.pszText);
	//lvColumn.iImage = 2;
	////m_ListMail.InsertColumn(2, &lvColumn);

	//lvColumn.cx = 90;
	//lvColumn.pszText = "Date";
	//lvColumn.cchTextMax = strlen(lvColumn.pszText);
	//lvColumn.iImage = 3;
	////m_ListMail.InsertColumn(3, &lvColumn);

}

BOOL CMFCMailClientDlg::OnInitDialog()
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

	CreateGroupTree();
	CreateListMailColumn();
	SetIconToMenuButton();

	ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCMailClientDlg::SetIconToMenuButton()
{
	HICON hIcn= (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_NEWMAIL),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_buttonNewMail.SetIcon(hIcn);

	hIcn = (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_DELETE),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnDeleteMail.SetIcon(hIcn);

	hIcn = (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_REPLY),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnReply.SetIcon(hIcn);

	hIcn = (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_MAILFORWARD),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnForward.SetIcon(hIcn);

	hIcn = (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_INBOX),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnInbox.SetIcon(hIcn);

	hIcn = (HICON)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDI_ICON_TRASH),
		IMAGE_ICON,
		32,32, // use actual size
		LR_DEFAULTCOLOR
		);
	m_btnTrash.SetIcon(hIcn);
}

void CMFCMailClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCMailClientDlg::OnPaint()
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
HCURSOR CMFCMailClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCMailClientDlg::OnMessageNewmessage()
{
	CNewMailDlg newMailDlg;
	newMailDlg.DoModal();
}



void CMFCMailClientDlg::OnAcountAccount()
{
	CAccountDlg accountDlg;
	accountDlg.DoModal();
}

void CMFCMailClientDlg::CreateGroupTree()
{

	/*m_GroupTree.ModifyStyle(m_GroupTree.GetStyle(),WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
		| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
		| TVS_DISABLEDRAGDROP);*/

	CImageList m_ImageList;
	m_ImageList.Create(IDB_BITMAP_EMAIL,16,0,RGB(0,128,128));
	m_GroupTree.SetImageList(&m_ImageList, TVSIL_NORMAL);

	HTREEITEM item;
	HTREEITEM childitem;

	CString rootText;
	//rootText.Format("%s Mail Box",CMFCMailClientApp::m_Username);
	item = m_GroupTree.InsertItem("Mail",0,0);
	childitem = m_GroupTree.InsertItem("Inbox",1,1,item);
	childitem = m_GroupTree.InsertItem("Trash",3,3,item);
}

void CMFCMailClientDlg::OnMessageCheckmail()
{
	globalMailList.RemoveAll();
	globalPop3.GetAllMail(globalMailList);
	
	if (globalMailList.GetCount() == 0)
	{
		m_ListMail.DeleteAllItems();
		return;
	}

	UpdateData(TRUE);
	m_ListMail.DeleteAllItems();
	

	for (int i=0;i<globalMailList.GetCount();i++)
	{
		/*LVITEM _listMailItem;
		_listMailItem.mask = LVIF_TEXT;
		_listMailItem.iItem = i;
		_listMailItem.iSubItem = 0;
		_listMailItem.pszText = _mailArray.GetAt(i).Subject.GetBuffer(_mailArray.GetAt(i).Subject.GetLength());

		m_ListMail.InsertItem(&_listMailItem);*/
		CString subject;
		subject.Format("%s",globalMailList.GetAt(i).Subject);
		int nIndex = m_ListMail.InsertItem(i,globalMailList[i].From);
		m_ListMail.SetItemText(nIndex,1,globalMailList[i].Subject);
		m_ListMail.SetItemText(nIndex,2,globalMailList[i].Date);
	}
	UpdateData(FALSE);
}

void CMFCMailClientDlg::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	POSITION pos = m_ListMail.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			UINT _selectedItem = m_ListMail.GetNextSelectedItem(pos);
			MailHeader _mailHeader = globalMailList.GetAt(_selectedItem);

			CString _view;
			_view.Format("From: %s\r\nTo: %s\r\nCc: %s\r\nDate: %s\r\nSubject: %s\r\n\r\n%s",
				_mailHeader.From,
				_mailHeader.To,
				_mailHeader.Cc,
				_mailHeader.Date,
				_mailHeader.Subject,
				_mailHeader.TextBody);

			m_MailMessage = _view.GetString();
		}
	}

	UpdateData(FALSE);
}

void CMFCMailClientDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
}
