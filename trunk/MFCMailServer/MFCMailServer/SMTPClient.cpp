// SMTPClient.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "SMTPClient.h"


// CSMTPClient

CSMTPClient::CSMTPClient()
{
	this->smtpProcessId = -1;
}

CSMTPClient::CSMTPClient( CDialog* dialog)
{
	this->m_parrent = dialog;
	this->smtpProcessId = -1;
}
CSMTPClient::~CSMTPClient()
{
}


// CSMTPClient member functions

void CSMTPClient::OnReceive(int nErrorCode)
{
	int nBytesRead = Receive(m_Buffer, MAX_SMTP_BUFFER_SIZE-1);
	if (nBytesRead == SOCKET_ERROR) return;

	m_Buffer[nBytesRead] = '\0';
	m_sQueue += m_Buffer;

	while (1)
	{
		int	iPos = m_sQueue.Find('\n');
		if (iPos == -1) break;
		m_ClientRequest = m_sQueue.Left(iPos+1);
		m_sQueue.Delete(0, iPos+1);
		INT code = GetSMTPCommand(&m_ClientRequest);
		ProcessCommand(code);
	}
	CAsyncSocket::OnReceive(nErrorCode);

	CAsyncSocket::OnReceive(nErrorCode);
}

void CSMTPClient::OnClose(int nErrorCode)
{
	CAsyncSocket::OnClose(nErrorCode);
}

INT CSMTPClient::GetSMTPCommand( CString* requestMessage )
{
	CString sTemp;
	CString	sRequestTemp = *requestMessage;

	if (requestMessage->GetLength() < 4) return SMTP_ERROR_CMD;
	requestMessage->TrimLeft();
	requestMessage->TrimRight();
	sTemp = requestMessage->Left(5);
	sTemp.TrimRight();
	for (int i=1; i<10; i++)
	{
		if (sTemp.CompareNoCase(SMTP_CMD[i].cmd_name) == 0)
		{
			SMTP_CMD[i].cmd_code = i;
			if (SMTP_CMD[i].cmd_name == "mail" || SMTP_CMD[i].cmd_name == "rcpt")
			{
				requestMessage->Delete(0, 4);
				requestMessage->TrimLeft();
				sTemp = sTemp + " " + *requestMessage;
				*requestMessage = sTemp;
				if (SMTP_CMD[i].cmd_name == "mail")
				{
					sTemp  = requestMessage->Left(9);
					if (requestMessage->GetLength() > 9) requestMessage->Delete(0, 9);
					else requestMessage->Delete(0, requestMessage->GetLength());
					requestMessage->TrimLeft();
					sTemp = sTemp + *requestMessage;
					*requestMessage = sTemp;
					sTemp  = requestMessage->Left(10);
					if (sTemp.CompareNoCase("mail from:") != 0)	return SMTP_ERROR_CMD;
				}
				else
				{
					sTemp  = requestMessage->Left(7);
					if (requestMessage->GetLength() > 7) requestMessage->Delete(0, 7);
					else requestMessage->Delete(0, requestMessage->GetLength());
					requestMessage->TrimLeft();
					sTemp = sTemp + *requestMessage;
					*requestMessage = sTemp;
					sTemp  = requestMessage->Left(8);
					if (sTemp.CompareNoCase("rcpt to:") != 0) return SMTP_ERROR_CMD;
				}
			}
			return SMTP_CMD[i].cmd_code;
		}
	}	
	return SMTP_ERROR_CMD;
}

void CSMTPClient::ProcessCommand( INT cmdCode)
{
	switch (cmdCode)
	{
	case SMTP_HELO_CMD:		ProcessHELOCommand();		break;
	case SMTP_ERROR_CMD:	ProcessERRORCommand();		break;
	case SMTP_MAIL_CMD:		ProcessMAILFROMCommand();	break;
	case SMTP_QUIT_CMD:		ProcessQUITCommand();		break;
		// TODO: Add them vao day
	}
}

void CSMTPClient::Reply( CString msg )
{
	Send(&msg,msg.GetLength(),0);
}

void CSMTPClient::ProcessERRORCommand()
{

}

void CSMTPClient::ProcessHELOCommand()
{
	CString _returnMsg;
	_returnMsg.Format("250 %s Please to meet you.", m_ClientAddress); // TODO: May co the sua cho no pro hon :D
	Reply(_returnMsg);
}

void CSMTPClient::ProcessMAILFROMCommand()
{

}

void CSMTPClient::ProcessQUITCommand()
{
	if (m_ClientRequest.CompareNoCase("quit") != 0)
	{
		ProcessERRORCommand();
		return;
	}
	Reply("221 SMTP Mail Server quited."); // TODO: May co the thay tu ngu khac cho no pro hon
	// TODO: Add cac cau lenh dong socket vao cho nay nhe
}