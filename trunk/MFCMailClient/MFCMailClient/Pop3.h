#include <afxsock.h>
#include "MailMessage.h"

#pragma once

#define DEFAULT_POP3_PORT 110
#define DEFAULT_LENGTH_RECEIVE_MESSAGE 1025

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

	CPop3(LPCTSTR _serverIp,UINT _serverPort, CString _username, CString _password);

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
	// Method:    Analysis Mail Header
	// FullName:  CPop3::AnalysisHeader
	// Access:    public 
	// Returns:   MailHeader*
	// Qualifier: Analysis Mail Header, return null if error
	// Parameter: INT _mailNumber
	//************************************
	MailHeader ReadMail(INT _mailNumber);
	CString ReadMessageBody(CString _header);
	void GetAllMail(CArray<MailHeader,MailHeader> &_mailArray);
	CString GetHeaderItem(CString Hdr,const CString& sName) const;
	MailHeader AnalysisHeader(CString Hdr);

	BOOL DeleteMail(INT _mailNumber);

	virtual ~CPop3(void);
};


