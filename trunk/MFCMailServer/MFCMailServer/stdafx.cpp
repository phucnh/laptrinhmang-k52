
// stdafx.cpp : source file that includes just the standard includes
// MFCMailServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "Smtp.h"

INT pop3RequestId;
INT smtpRequestId;
CString logMessage;

CPop3* serverPop3Socket;
INT nPop3ConnectionsCount;

CSmtp* serverSmtpSocket;
INT nSmtpConnectionsCount;

INT settingPop3Port = 110;
INT settingSmtpPort = 25;

CSMTPProcessList m_SMTPConnectionsList;
CPOP3ProcessList m_POP3ConnectionsList;

//Bien cua DAL
DAL* dal;

CString GetCurrentTimeStr()
{
	CTime	tDateTime = CTime::GetCurrentTime();;
	return	tDateTime.Format("%m/%d/%Y %H:%M:%S");
}


