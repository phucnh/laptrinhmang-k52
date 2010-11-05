#pragma once

// CClientSocket command target
#define MAX_POP3_BUFFER_SIZE 1024
#define USER_CMD 1
#define PASS_CMD 2
#define LIST_CMD 3
#define RETR_CMD 4
#define STAT_CMD 5
#define DELE_CMD 6
#define QUIT_CMD 7

class CClientSocket : public CAsyncSocket
{
private:
	CDialog* m_parrent;
	CString m_username;
	CString m_password;
	UINT m_totalMail;
	UINT m_totalSize;
	CString	m_sQueue; //Day luu thong tin khi du lieu chua hoan toan duoc gui toi server
	char m_Buffer[MAX_POP3_BUFFER_SIZE]; //Bo dem dung de luu thong tin khi du lieu gui den server
	CString m_ClientRequest; //Du lieu request tu client gui den server
	INT pop3ProcessId;
	BOOL isLocked;

public:
	INT Pop3ProcessId() const { return pop3ProcessId; }
	void Pop3ProcessId(INT val) { pop3ProcessId = val; }

	CClientSocket();
	CClientSocket(CDialog* parrentDlg);

	virtual ~CClientSocket();

	void Initialize();
	void Reply(CString _message);

	INT GetPop3Command(CString* requestMessage);
	void ProcessCommand(INT );

	void ProcessUSERCommand(CString*);
	void ProcessPASSCommand(CString*);
	void ProcessLISTCommand(CString*);
	void ProcessRETRCommand(CString*);
	void ProcessSTATCommand(CString*);
	void ProcessDELECommand(CString*);
	void ProcessQUITCommand(CString*);


	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


