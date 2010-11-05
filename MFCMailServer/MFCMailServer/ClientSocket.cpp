// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
	this->pop3ProcessId = -1;
}

CClientSocket::CClientSocket( CDialog* parrentDlg )
{
	this->m_parrent = parrentDlg;
	this->pop3ProcessId = -1;

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
	case CMDERROR:	ProcessERROR();	break;
	case USER_CMD:	ProcessUSERCommand();	break;
	case PASS_CMD:	ProcessPASSCommand();	break;
	case LIST_CMD:	ProcessLISTCommand();	break;
	case RETR_CMD:	ProcessRETRCommand();	break;
	case STAT_CMD:	ProcessSTATCommand();	break;
	case DELE_CMD:	ProcessDELECommand();	break;
	case QUIT_CMD:	ProcessQUITCommand();	break;
	}
}

INT CClientSocket::GetPop3Command( CString* requestMessage )
{
	if (requestMessage->GetLength() < 4) return CMDERROR;	
	requestMessage->TrimLeft();
	requestMessage->TrimRight();

	CString sCmd;
	sCmd = requestMessage->Left(5);	
	sCmd.TrimRight();

	if ((requestMessage->Left(3)).CompareNoCase("top") == 0) return 10;

	for (int i=1; i<10; i++)
	{
		if (sCmd.CompareNoCase(POP3_CMD[i].cmd_name) == 0) return i;
	}
	return CMDERROR;
}

void CClientSocket::ProcessUSERCommand()
{

}

void CClientSocket::ProcessERROR()
{

}

void CClientSocket::ProcessPASSCommand()
{

}

void CClientSocket::ProcessLISTCommand()
{

}

void CClientSocket::ProcessRETRCommand()
{

}

void CClientSocket::ProcessSTATCommand()
{

}

void CClientSocket::ProcessDELECommand()
{

}

void CClientSocket::ProcessQUITCommand()
{

}