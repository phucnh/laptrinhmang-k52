// Pop3.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "Pop3.h"
#include "ClientSocket.h"


// CPop3
CPop3::CPop3( CDialog* parrentDlg ) : CAsyncSocket()
{
	this->parrentDlg = parrentDlg;
}
CPop3::~CPop3()
{
}

// CPop3 member functions

void CPop3::OnAccept(int nErrorCode)
{
	CClientSocket socketClient;
	if (Accept(socketClient))
	{
		CString	sMsg("+OK Mail server ready"); //TODO: Implement message in here
		sMsg.ReleaseBuffer();
		sMsg+="\r\n";
		socketClient.Send(sMsg,sMsg.GetLength(),0);
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
