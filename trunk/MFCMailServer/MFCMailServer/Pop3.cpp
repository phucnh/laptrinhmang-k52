// Pop3.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "Pop3.h"
#include "ClientSocket.h"


// CPop3
CPop3::CPop3( CMFCMailServerDlg* parrentDlg ) : CAsyncSocket()
{
	this->parrentDlg = parrentDlg;

}
CPop3::~CPop3()
{
}

// CPop3 member functions

void CPop3::OnAccept(int nErrorCode)
{
	CClientSocket* socketClient = new CClientSocket(this->parrentDlg);
	if (Accept(*socketClient))
	{
		CString	sMsg("+OK Mail server ready"); //TODO: Implement message in here
		sMsg.ReleaseBuffer();
		sMsg+="\r\n";
		socketClient->Send(sMsg,sMsg.GetLength(),0);

		//phuc add 20101121
		this->parrentDlg->WriteLog(sMsg);
		//end phuc add 20101121
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
