#pragma once

#include "stdafx.h"
#include "Global.h"
#include "MailHeader.h"

//SMTP const
#define MAX_SMTP_BUFFER_SIZE 1024
#define SMTP_ERROR_CMD	0
#define SMTP_HELO_CMD	1
#define SMTP_MAIL_CMD	2
#define SMTP_RCPT_CMD	3
#define SMTP_DATA_CMD	4
#define SMTP_RSET_CMD	5
#define SMTP_VRFY_CMD	6
#define SMTP_NOOP_CMD	7
#define SMTP_QUIT_CMD	8
#define SMTP_HELP_CMD	9
#define SMTP_WAITING_CMD 10
#define SMTP_AFTER_CLIENT_SEND_DATA 11

static CMailServerCommand SMTP_CMD[10] = 
{
	{ NULL,	 SMTP_ERROR_CMD},
	{"helo", SMTP_HELO_CMD},
	{"mail from:", SMTP_MAIL_CMD},
	{"rcpt to:", SMTP_RCPT_CMD},
	{"data", SMTP_DATA_CMD},
	{"rset", SMTP_RSET_CMD},
	{"vrfy", SMTP_VRFY_CMD},
	{"noop", SMTP_NOOP_CMD},
	{"quit", SMTP_QUIT_CMD},	
	{"help", SMTP_HELP_CMD}	
};

// CSMTPClient command target

class CSMTPClient : public CAsyncSocket
{
private:
	//CWinThread* smtpThread;
	CMFCMailServerDlg* m_parrent;
	MailHeader* m_mailHdr; //Mail dinh gui di luu tru vao day
	CString m_ClientAddress;
	CString	m_sQueue; //Day luu thong tin khi du lieu chua hoan toan duoc gui toi server
	char m_Buffer[MAX_SMTP_BUFFER_SIZE]; //Bo dem dung de luu thong tin khi du lieu gui den server
	CString m_ClientRequest; //Du lieu request tu client gui den server
	INT smtpProcessId;
	
	BOOL isLocked;
	INT m_nStatus;

public:

	INT SmtpProcessId() const { return smtpProcessId; }
	void SmtpProcessId(INT val) { smtpProcessId = val; }

	CSMTPClient();
	CSMTPClient(CMFCMailServerDlg*);
	virtual ~CSMTPClient();

	//************************************
	// Method:    Initialize
	// FullName:  CSMTPClient::Initialize
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void Initialize();
	void Reply(CString msg);
	void CloseSocket();

	INT GetSMTPCommand(CString* requestMessage);
	void ProcessCommand(INT );

	void ProcessERRORCommand();
	void ProcessHELOCommand();
	void ProcessMAILFROMCommand();
	void ProcessRCPTCommand(); //Long add
	void ProcessDATACommand(); //Long add
	void ProcessNOOPCommand();
	void ProcessHELPCommand();
	void ProcessQUITCommand();
	//Add them ham de xu ly vao day nhe
	void GetMailFrom();
	//Long add
	void GetRCPTTo(); //Long add
	void GetDATA(); //Long add
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


