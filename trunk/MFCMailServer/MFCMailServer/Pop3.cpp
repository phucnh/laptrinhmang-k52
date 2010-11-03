// Pop3.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "Pop3.h"


// CPop3

CPop3::CPop3()
{
	AfxSocketInit();
	m_socket.Create(this->m_hostPort,SOCK_STREAM);
}

CPop3::~CPop3()
{
}

void CPop3::ListenFromClient()
{
	if (m_socket.Listen(110))
	{
		CSocket sockRev;
		if (m_socket.Accept(sockRev))
			AfxMessageBox(_T("Connected"));
	}
	m_socket.Close();
}

// CPop3 member functions
