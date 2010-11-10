#pragma once

#include "MailMessage.h"

#define MAX_SMTP_BUFFER_SIZE 1024
# define SMTP_ERROR_CMD	0
# define SMTP_HELO_CMD	1
# define SMTP_MAIL_CMD	2
# define SMTP_RCPT_CMD	3
# define SMTP_DATA_CMD	4
# define SMTP_RSET_CMD	5
# define SMTP_VRFY_CMD	6
# define SMTP_NOOP_CMD	7
# define SMTP_QUIT_CMD	8
# define SMTP_HELP_CMD	9

static COMMAND SMTP_CMD[] = 
{
	{ NULL,	 SMTP_ERROR_CMD},
	{"helo", SMTP_HELO_CMD},
	{"mail", SMTP_MAIL_CMD},
	{"rcpt", SMTP_RCPT_CMD},
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
	CDialog* m_parrent;
	MailHeader* m_mailHdr; //Mail dinh gui di luu tru vao day
	CString m_ClientAddress;
	CString	m_sQueue; //Day luu thong tin khi du lieu chua hoan toan duoc gui toi server
	char m_Buffer[MAX_SMTP_BUFFER_SIZE]; //Bo dem dung de luu thong tin khi du lieu gui den server
	CString m_ClientRequest; //Du lieu request tu client gui den server
	INT smtpProcessId;
	
	BOOL isLocked;

public:
	INT SmtpProcessId() const { return smtpProcessId; }
	void SmtpProcessId(INT val) { smtpProcessId = val; }

	CSMTPClient();
	CSMTPClient(CDialog*);
	virtual ~CSMTPClient();

	void Reply(CString msg);

	INT GetSMTPCommand(CString* requestMessage);
	void ProcessCommand(INT );

	void ProcessERRORCommand();
	void ProcessHELOCommand();
	void ProcessMAILFROMCommand();
	void ProcessQUITCommand();
	//Add them ham de xu ly vao day nhe

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

