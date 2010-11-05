// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::CClientSocket( CDialog* parrentDlg )
{
	this->m_parrent = parrentDlg;

}
CClientSocket::~CClientSocket()
{
}


// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	int nBytesRead = Receive(m_Buffer, MAX_POP3_BUFFER_SIZE-1);
	if (nBytesRead == SOCKET_ERROR) return;

	m_Buffer[nBytesRead] = '\0';
	m_sQueue += m_Buffer;

	while (1)
	{
		int	iPos = m_sQueue.Find('\n');
		if (iPos == -1) break;
		m_ClientRequest = m_sQueue.Left(iPos+1);
		m_sQueue.Delete(0, iPos+1);
		INT code = GetPop3Command(&m_ClientRequest);
		ProcessCommand(code);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class

	CAsyncSocket::OnClose(nErrorCode);
}

void CClientSocket::ProcessCommand( INT _cmdCode )
{
	switch(_cmdCode)
	{
	case USER_CMD:	ProcessUSERCommand(&m_ClientRequest);	break;
	case PASS_CMD:	ProcessPASSCommand(&m_ClientRequest);	break;
	case LIST_CMD:	ProcessLISTCommand(&m_ClientRequest);	break;
	case RETR_CMD:	ProcessRETRCommand(&m_ClientRequest);	break;
	case STAT_CMD:	ProcessSTATCommand(&m_ClientRequest);	break;
	case DELE_CMD:	ProcessDELECommand(&m_ClientRequest);	break;
	case QUIT_CMD:	ProcessQUITCommand(&m_ClientRequest);	break;
	}
}