// Smtp.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "Smtp.h"
#include "smtpclient.h"


// CSmtp

CSmtp::CSmtp()
{
}

CSmtp::CSmtp( CDialog* parrent )
{
	this->parrentDlg = parrent;
}

CSmtp::~CSmtp()
{
}


// CSmtp member functions

void CSmtp::OnAccept(int nErrorCode)
{
	CSMTPClient socketClient;

	if (Accept(socketClient))
	{
		CString	sMsg("+OK SMTP Mail server connected"); // TODO: Implement message in here
		sMsg.ReleaseBuffer();
		sMsg+="\r\n";
		socketClient.Send(sMsg,sMsg.GetLength(),0);
	}

	CAsyncSocket::OnAccept(nErrorCode);
}
