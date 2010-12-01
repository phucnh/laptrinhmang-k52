
// stdafx.cpp : source file that includes just the standard includes
// MFCMailClient.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

//Global variables
CUser globalUser;
CPop3 globalPop3;
CSMTP globalSmtp;
CArray<MailHeader,MailHeader> globalMailList;
CString globalUsername;
CString globalPassword;
CString globalServerIP;
BOOL globalIsConnected;


