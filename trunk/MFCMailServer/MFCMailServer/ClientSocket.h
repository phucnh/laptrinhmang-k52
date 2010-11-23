#include "stdafx.h"
#include "Global.h"
#include "MFCMailServerDlg.h"

// CClientSocket command target
#define MAX_POP3_BUFFER_SIZE 1024
#define CMDERROR 0
#define USER_CMD 1
#define PASS_CMD 2
#define LIST_CMD 3
#define RETR_CMD 4
#define STAT_CMD 5
#define DELE_CMD 6
#define QUIT_CMD 7
#define NOOP_CMD 8
#define RSET_CMD 9
#define TOP_CMD 10

static CMailServerCommand POP3_CMD[11] = 
{
	{NULL, CMDERROR},
	{"user", USER_CMD},
	{"pass", PASS_CMD},
	{"quit", QUIT_CMD},
	{"stat", STAT_CMD},
	{"list", LIST_CMD},
	{"retr", RETR_CMD},
	{"dele", DELE_CMD},
	{"noop", NOOP_CMD},
	{"rset", RSET_CMD},		
	{"top",  TOP_CMD}
};	

#pragma once
#include "MailUser.h"

//Lop socket client de giao tiep voi POP 3

class CClientSocket : public CAsyncSocket
{
private:
	CMFCMailServerDlg* m_parrent;
	MailUser* m_user;
	UINT m_totalMail;
	UINT m_totalSize;
	CString	m_sQueue; //Day luu thong tin khi du lieu chua hoan toan duoc gui toi server
	char m_Buffer[MAX_POP3_BUFFER_SIZE]; //Bo dem dung de luu thong tin khi du lieu gui den server
	CString m_ClientRequest; //Du lieu request tu client gui den server
	INT pop3ProcessId;
	BOOL isLocked;
	CStringList	m_MailboxInfo;

public:
	INT Pop3ProcessId() const { return pop3ProcessId; }
	void Pop3ProcessId(INT val) { pop3ProcessId = val; }

	CClientSocket();
	CClientSocket(CMFCMailServerDlg* parrentDlg);

	virtual ~CClientSocket();

	void Initialize();
	void SendString(const void *lpBuf, int nBufLen, int nFlags);
	void GetMailboxInfo(CString sMailbox);
	CString GetMessageInfo(UINT nID, int nField);
	MailUser* GetUserByUsername(CString username);
	void Reply(CString _message);
	void Reply(LPSTR format,...);


	INT GetPop3Command(CString* requestMessage);
	void ProcessCommand(INT );

	void ProcessERROR();
	void ProcessUSERCommand();
	void ProcessPASSCommand();
	void ProcessLISTCommand();
	void ProcessRETRCommand();
	void ProcessSTATCommand();
	void ProcessDELECommand();
	void ProcessQUITCommand();


	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void CloseSocket();

};


