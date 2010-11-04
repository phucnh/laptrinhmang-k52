// Pop3.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "Pop3.h"


// CPop3
CPop3::CPop3( CDialog* parrentDlg )
{
	CAsyncSocket::CAsyncSocket();
	this->parrentDlg = parrentDlg;
}
CPop3::~CPop3()
{
}

// CPop3 member functions

void CPop3::OnAccept(int nErrorCode)
{
	CAsyncSocket socketClient;
	if (Accept(socketClient))
	{
		CString	sMsg("+OK Mail server ready");
		sMsg.ReleaseBuffer();
		sMsg+="\r\n";
		this->Send(sMsg,sMsg.GetLength());
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
