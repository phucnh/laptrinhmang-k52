// Pop3.cpp : implementation file
//

#include "stdafx.h"
#include "Pop3.h"
#include "EntitiesServices.h"
#include <afx.h>


// CPop3

CPop3::CPop3( LPCTSTR _serverIp,UINT _serverPort, CString _username, CString _password )
{
	ASSERT(_serverIp != NULL);
	AfxSocketInit();

	serverIP = _serverIp;
	serverPort = _serverPort;
	username = _username;
	password = _password;
	isConnected = FALSE;
}

CPop3::CPop3()
{
	AfxSocketInit();
}
CPop3::~CPop3()
{
	if (_instance != NULL)
		delete _instance;

	Disconnect();
}

void CPop3::Connect()
{
	char _receiveMessage[DEFAULT_LENGTH_RECEIVE_MESSAGE];
	CString strCommand;

	if (!serverSocket.Create())
	{
		isConnected = FALSE;
		return;
	}

	//Connect
	if (!serverSocket.Connect(serverIP,serverPort))
	{
		isConnected = FALSE;
		return;
	}

	if (
		!serverSocket.Receive(
		_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE)||
		!this->ReceiveMessageIsOK(_receiveMessage)
		){
		isConnected = FALSE;
		return;
	}

	//Send username
	strCommand.Format("USER %s\r\n",username);
	serverSocket.Send((LPCTSTR)(strCommand),strCommand.GetLength(),0);
	if (!serverSocket.Receive(
		_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE)
		||!this->ReceiveMessageIsOK(_receiveMessage)
		){
		isConnected = FALSE;
		return;
	}

	//Send Password
	strCommand.Format("PASS %s\r\n",password);
	serverSocket.Send((LPCTSTR)(strCommand),strCommand.GetLength(),0);
	if (!serverSocket.Receive(
		_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE)||
		!this->ReceiveMessageIsOK(_receiveMessage)
		){
		Disconnect();
		isConnected = FALSE;
		return;
	}

	isConnected = TRUE;
}

BOOLEAN CPop3::ReceiveMessageIsOK( char* _receiveMessage )
{
	return (
		_receiveMessage[0] == '+' &&
		_receiveMessage[1] == 'O' &&
		_receiveMessage[2] == 'K'
		);
}

void CPop3::Disconnect()
{
	CString quitStr;
	char _receiveMessage[DEFAULT_LENGTH_RECEIVE_MESSAGE];

	if (!isConnected)
		Connect();

	quitStr.Format("QUIT\r\n");
	serverSocket.Send(quitStr,quitStr.GetLength(),0);
	/*if (!serverSocket.Receive(_receiveMessage,1024)||!this->ReceiveMessageIsOK(_receiveMessage)){
		isConnected = FALSE;
		return;
	}*/
	// TODO : Co van de voi lenh quit

	//serverSocket.Close();

	isConnected = FALSE;
}

CString CPop3::GetHeaderItem( CString Hdr,const CString& sName ) const
{
	CString sField;
	CString sHeader=CString(_T("\r\n"));

	sHeader += Hdr;
	CString sUpCaseHeader=CString(sHeader);

	sUpCaseHeader.MakeUpper();
	CString sUpCaseName(sName);

	sUpCaseName.MakeUpper();

	CString sFind(CString(_T("\r\n")) + sUpCaseName + _T(":"));
	int nFindLength = sFind.GetLength();//do dai cua chuoi can tim
	int nFindStart = sUpCaseHeader.Find(sFind);//vi tri tim thay chuoi

	if (nFindStart == -1)
	{
		return _T("");
	}
	else
	{
		nFindStart += (3 + sName.GetLength());
		BOOL bFoundEnd = FALSE;
		int i = nFindStart;
		int nLength = sHeader.GetLength();
		do
		{
			TCHAR c1 = _T('\0');
			if (i < nLength)
				c1 = sHeader.GetAt(i);
			TCHAR c2 = _T('\0');
			if (i < (nLength-1))
				c2 = sHeader.GetAt(i+1);
			TCHAR c3 = _T('\0');
			if (i < (nLength-2))
				c3 = sHeader.GetAt(i+2);

			if ((c1 == _T('\0')) ||
				((c1 == _T('\r')) && (c2 == _T('\n')) && (c3 != _T(' ')) && c3 != _T('\t')))
			{
				bFoundEnd = TRUE;
			}
			else
			{
				++i;
			}
		}
		while (!bFoundEnd);

		sField = CString(sHeader.Mid(nFindStart, i - nFindStart));
		int nEOL = sField.Find(_T("\r\n"));
		while (nEOL != -1)
		{
			sField = sField.Left(nEOL) + sField.Right(sField.GetLength() - nEOL - 2);
			nEOL = sField.Find(_T("\r\n"));
		}

		int nTab = sField.Find(_T('\t'));
		while (nTab != -1)
		{
			sField = sField.Left(nTab) + _T(' ') + sField.Right(sField.GetLength() - nTab - 1);
			nTab = sField.Find(_T('\t'));
		}

		sField.TrimLeft();
		sField.TrimRight();
	}

	return sField;
}

MailHeader CPop3::AnalysisHeader( CString _headerMessage )
{
	MailHeader _mailHeader("","","","","","","",FALSE);

	_mailHeader.To = GetHeaderItem(_headerMessage,"To");
	_mailHeader.From = GetHeaderItem(_headerMessage,"From");
	_mailHeader.Subject = GetHeaderItem(_headerMessage,"Subject");
	_mailHeader.Date = GetHeaderItem(_headerMessage,"Date");
	_mailHeader.Cc = GetHeaderItem(_headerMessage,"CC");
	_mailHeader.ReplyTo = GetHeaderItem(_headerMessage,"Reply-To");
	_mailHeader.RealAttach = FALSE;
	//_mailHeader.TextBody= ReadMessageBody(_headerMessage);

	return _mailHeader;
}

void CPop3::SendSTATCommand(char* _receiveMessage)
{
	_receiveMessage[0] = '\0';

	if (!isConnected)
		Connect();

	CString strCommand;

	//Send STAT
	strCommand.Format("STAT\r\n");
	serverSocket.Send(strCommand,strCommand.GetLength(),0);

	INT _bytesRead = serverSocket.Receive(_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE);

	if(!_bytesRead)
		return;

	_receiveMessage[_bytesRead] = '\0';
}

INT CPop3::GetNumberOfMail()
{
	INT _numberOfMail;
	char _receiveMessage[DEFAULT_LENGTH_RECEIVE_MESSAGE];
	_receiveMessage[0] = '\0'; 

	SendSTATCommand(_receiveMessage);

	if (_receiveMessage == NULL)
		return -1;

	CString temp("");

	for (int i=4;_receiveMessage[i]!=' ';i++)
	{
		temp.Append(&_receiveMessage[i],1);
	}
	_numberOfMail = atoi(temp.GetString());

	return _numberOfMail;
}

void CPop3::SendLISTCommand( char* _receiveMessage )
{
	_receiveMessage[0] = '\0';

	if (!isConnected)
		Connect();

	CString strCommand;

	//Send STAT
	strCommand.Format("LIST\r\n");
	serverSocket.Send(strCommand,strCommand.GetLength(),0);

	INT _bytesRead = serverSocket.Receive(_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE);

	if(!_bytesRead)
		return;

	_receiveMessage[_bytesRead] = '\0';
}

void CPop3::SendRETRCommand(char* _receiveMessage, INT _mailNumber)
{
	_receiveMessage[0] = '\0';

	if (!isConnected)
		Connect();

	CString strCommand;

	strCommand.Format("RETR %d\r\n",_mailNumber);
	INT _bytesRead = serverSocket.Receive(_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE);

	if(!_bytesRead)
		return;

	_receiveMessage[_bytesRead] = '\0';
}

//void CPop3::SendDELECommand( char* _receiveMessage, INT _mailNumber )
//{
//	_receiveMessage[0] = '\0';
//
//	if (!isConnected)
//		return;
//
//	CString strCommand;
//
//	//Send STAT
//	strCommand.Format("DELE %d\r\n",_mailNumber);
//	serverSocket.Send(strCommand,strCommand.GetLength(),0);
//	if(!serverSocket.Receive(_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE))
//		return;
//}

BOOL CPop3::DeleteMail( INT _mailNumber )
{
	if (!isConnected)
		Connect();

	char _receiveMessage[DEFAULT_LENGTH_RECEIVE_MESSAGE];
	CString strCommand;

	strCommand.Format("DELE %d\r\n",_mailNumber);
	serverSocket.Send(strCommand,strCommand.GetLength(),0);
	if (!serverSocket.Receive(
		_receiveMessage,DEFAULT_LENGTH_RECEIVE_MESSAGE)
		||!this->ReceiveMessageIsOK(_receiveMessage)
		){
		return FALSE;	
	}

	return TRUE;
}

MailHeader CPop3::ReadMail( INT _mailNumber )
{
	MailHeader mailHeader;

	if (!isConnected)
		Connect();

	char _receiveMessage[DEFAULT_LENGTH_RECEIVE_MESSAGE];
	CString strCommand;

	strCommand.Format("RETR %d\r\n",_mailNumber);
	serverSocket.Send(strCommand,strCommand.GetLength(),0);
	Sleep(300);
	int _bytesRead = 0;

	/*HINSTANCE hInst;

	CString sDriver;
	CString sExePath;

	TCHAR szFullPath[_MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

	GetModuleFileName(hInst, szFullPath, MAX_PATH);
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);
	sExePath.Format("%s%s", szDrive, szDir);

	CFile _tempFile;
	_tempFile.Open(_T(sExePath + "\\TempFile\\temp.eml"),CFile::modeCreate|CFile::modeWrite);*/
	do 
	{
		_bytesRead = serverSocket.Receive(_receiveMessage,sizeof(_receiveMessage));
		//_tempFile.Write(_receiveMessage,sizeof(_receiveMessage));
	} while (_bytesRead == DEFAULT_LENGTH_RECEIVE_MESSAGE);

	//_tempFile.Write(CFile::end,sizeof(CFile::end));
	//_tempFile.Close();
	
	_receiveMessage[_bytesRead] = '\0';
	if(!ReceiveMessageIsOK(_receiveMessage))
		return mailHeader;

	CString _messageHeader(_receiveMessage);

	mailHeader.To = GetHeaderItem(_messageHeader,"To");
	mailHeader.From = GetHeaderItem(_messageHeader,"From");
	mailHeader.Subject = GetHeaderItem(_messageHeader,"Subject");
	mailHeader.Date = GetHeaderItem(_messageHeader,"Date");
	mailHeader.Cc = GetHeaderItem(_messageHeader,"CC");
	mailHeader.ReplyTo = GetHeaderItem(_messageHeader,"Reply-To");
	mailHeader.TextBody = ReadMessageBody(_messageHeader);

	mailHeader.UserId = globalUser.UserId();

	CMailHeaderServices* mailHeaderService = new CMailHeaderServices();
	mailHeaderService->InsertNewMail(&mailHeader);

	if (mailHeaderService != NULL)	delete mailHeaderService;

	return mailHeader;
}



INT64 CPop3::GetMailTotalSize()
{
	INT64 _mailTotalSize;
	char _receiveMessage[DEFAULT_LENGTH_RECEIVE_MESSAGE];
	_receiveMessage[0] = '\0'; 

	SendSTATCommand(_receiveMessage);

	if (_receiveMessage == NULL)
		return -1;

	CString temp("");

	INT j = 0;

	for (j=4;_receiveMessage[j]!=' ';j++);

	for (INT i=j;_receiveMessage[i]!='\0';i++)
	{
		temp.Append(&_receiveMessage[i],1);
	}
	_mailTotalSize = atoi(temp.GetString());

	return _mailTotalSize;
}

CString CPop3::ReadMessageBody( CString _header )
{
	CString _messageBody("");
	INT count = _header.Find("Message-ID:");

	if (!count)
		return _messageBody;

	while(_header.GetAt(count)!= '\n')
	{
		count++;
	}

	count++;

	while(_header.GetAt(count)!= '\0')
	{
		_messageBody += _header.GetAt(count);
		count++;
	}

	//Remove '.' and 2 ' ' character
	_messageBody.Delete(_messageBody.GetLength()-3,3);

	return _messageBody;
}

void CPop3::GetAllMail(CArray<MailHeader,MailHeader> &_mailArray)
{
	if (!isConnected)
		Connect();

	INT _numberOfMails = GetNumberOfMail();
	
	if (!_numberOfMails)
		return;

	for (INT i=1;i<=_numberOfMails;i++)
		_mailArray.Add(ReadMail(i));

	return;
}

CPop3* CPop3::GetInstance( LPCTSTR _serverIp,UINT _serverPort, CString _username, CString _password )
{
	if (_instance == NULL)
		return (_instance = new CPop3(_serverIp,110,_username,_password));
	
	return _instance;
}

CPop3* CPop3::GetInstance()
{
	if (_instance == NULL)
		return (_instance = new CPop3("127.0.0.1",110,"",""));

	return _instance;
}

CPop3* CPop3::_instance;