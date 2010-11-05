
// stdafx.cpp : source file that includes just the standard includes
// MFCMailServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

INT pop3RequestId;
INT smtpRequestId;
CString logMessage;

CPop3* serverPop3Socket;
INT nPop3ConnectionsCount;

INT settingPop3Port = 110;
INT settingSmtpPort = 25;