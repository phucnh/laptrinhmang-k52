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

CSmtp::CSmtp( CMFCMailServerDlg* parrent )
{
	this->parrentDlg = parrent;
}

CSmtp::~CSmtp()
{
}


// CSmtp member functions

void CSmtp::OnAccept(int nErrorCode)
{
	CSMTPClient* socketClient = new CSMTPClient(parrentDlg);

	if (Accept(*socketClient))
	{
		CString	sMsg("+OK SMTP Mail server connected"); // TODO: Implement message in here
		sMsg.ReleaseBuffer();
		sMsg+="\r\n";
		socketClient->Send(sMsg,sMsg.GetLength(),0);

		//phuc add 20101121
		this->parrentDlg->WriteLog(sMsg);
		//end phuc add 20101121
	}

	CAsyncSocket::OnAccept(nErrorCode);
}
