#include <afxsock.h>
#include "MailMessage.h"

#pragma once

#define DEFAULT_POP3_PORT 110
#define DEFAULT_LENGTH_RECEIVE_MESSAGE 30000

// CPop3 command target

class CPop3 : public CWnd
{
private:
	CString serverIP;
	UINT serverPort;
	CString username;
	CString password;
	CSocket serverSocket;
	BOOL isConnected;
	CString errorString;
	static CPop3* _instance;

	//************************************
	// Method:    Check Receive Message Is OK or Not
	// FullName:  CPop3::ReciveMessageIsOK
	// Access:    private 
	// Returns:   BOOLEAN
	// Qualifier:
	// Parameter: char * result
	//************************************
	BOOLEAN ReceiveMessageIsOK(char *result);

public:
	CPop3(LPCTSTR _serverIp,UINT _serverPort, CString _username, CString _password);
	CPop3();

	static CPop3* GetInstance(LPCTSTR _serverIp,UINT _serverPort, CString _username, CString _password);
	static CPop3* GetInstance();

	CString ServerIP() const { return serverIP; }
	void ServerIP(CString val) { serverIP = val; }

	UINT ServerPort() const { return serverPort; }
	void ServerPort(UINT val) { serverPort = val; }

	CString Username() const { return username; }
	void Username(CString val) { username = val; }

	CString Password() const { return password; }
	void Password(CString val) { password = val; }

	BOOL IsConnected() const { return isConnected; }
	void IsConnected(BOOL val) { isConnected = val; }

	void Connect();
	void Disconnect();

	/*void CheckMail();*/

	void SendSTATCommand(char* _receiveMessage);
	void SendLISTCommand(char* _receiveMessage);
	void SendRETRCommand(char* _receiveMessage, INT _mailNumber);
	
	INT GetNumberOfMail();
	INT64 GetMailTotalSize();

	//************************************
	// Method:    ReadMail
	// FullName:  CPop3::ReadMail
	// Access:    public 
	// Returns:   MailHeader
	// Qualifier:
	// Parameter: INT _mailNumber
	//************************************
	MailHeader ReadMail(INT _mailNumber);
	//************************************
	// Method:    ReadMessageBody
	// FullName:  CPop3::ReadMessageBody
	// Access:    public 
	// Returns:   CString
	// Qualifier:
	// Parameter: CString _header
	//************************************
	CString ReadMessageBody(CString _header);
	//************************************
	// Method:    GetAllMail
	// FullName:  CPop3::GetAllMail
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CArray<MailHeader
	// Parameter: MailHeader> & _mailArray
	//************************************
	void GetAllMail(CArray<MailHeader,MailHeader> &_mailArray);
	//************************************
	// Method:    GetHeaderItem
	// FullName:  CPop3::GetHeaderItem
	// Access:    public 
	// Returns:   CString
	// Qualifier: const
	// Parameter: CString Hdr
	// Parameter: const CString & sName
	//************************************
	CString GetHeaderItem(CString Hdr,const CString& sName) const;
	//************************************
	// Method:    AnalysisHeader
	// FullName:  CPop3::AnalysisHeader
	// Access:    public 
	// Returns:   MailHeader
	// Qualifier:
	// Parameter: CString Hdr
	//************************************
	MailHeader AnalysisHeader(CString Hdr);

	//************************************
	// Method:    DeleteMail
	// FullName:  CPop3::DeleteMail
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: INT _mailNumber
	//************************************
	BOOL DeleteMail(INT _mailNumber);

	virtual ~CPop3(void);
};


