
// stdafx.cpp : source file that includes just the standard includes
// MFCMailServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "Smtp.h"

INT pop3RequestId = 0;
INT smtpRequestId = 0;
CString logMessage;

CPop3* serverPop3Socket;
INT nPop3ConnectionsCount = 0;

CSmtp* serverSmtpSocket;
INT nSmtpConnectionsCount = 0;

INT settingPop3Port = 110;
INT settingSmtpPort = 25;

CSMTPProcessList m_SMTPConnectionsList;
CPOP3ProcessList m_POP3ConnectionsList;

//Bien cua DAL
DAL* dal;

CString GetCurrentTimeStr()
{
	CTime	tDateTime = CTime::GetCurrentTime();
	return	tDateTime.Format("%m/%d/%Y %H:%M:%S");
}


