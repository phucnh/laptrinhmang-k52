// SMTPClient.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailServer.h"
#include "SMTPClient.h"


// CSMTPClient

CSMTPClient::CSMTPClient()
{
	this->smtpProcessId = -1;
	
	Initialize();
}

CSMTPClient::CSMTPClient( CMFCMailServerDlg* dialog)
{
	this->m_parrent = dialog;
	this->smtpProcessId = -1;

	//phuc add 20101123
	nSmtpConnectionsCount++;
	smtpRequestId++;
	this->smtpProcessId = smtpRequestId;
	this->m_parrent->UpdateStatusbar();

	m_SMTPConnectionsList.AddTail(this);

	//phuc add 20101123
	Initialize();
	//end phuc add 20101123
}
CSMTPClient::~CSMTPClient()
{
	POSITION pos = m_SMTPConnectionsList.Find(this);
	if (pos != NULL) m_SMTPConnectionsList.RemoveAt(pos);

	nSmtpConnectionsCount--;
	this->m_parrent->UpdateStatusbar();
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
	//phuc add 20101123
	CString message;
	message.Format(_T("%s - SMTP connection (ID=%d) closed"), GetCurrentTimeStr(), smtpProcessId);
	m_parrent->WriteLog(message);
	//end phuc add 20101123

	CAsyncSocket::OnClose(nErrorCode);

	//phuc add 20101123
	delete this;
	//end phuc add 20101123
}

INT CSMTPClient::GetSMTPCommand( CString* requestMessage )
{
	/*CString sTemp;
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
	return SMTP_ERROR_CMD;*/
	if (requestMessage->GetLength() < 4) return CMDERROR;	
	requestMessage->TrimLeft();
	requestMessage->TrimRight();

	CString sCmd;
	sCmd = requestMessage->Left(5);	
	sCmd.TrimRight();

	if ((requestMessage->Left(9)).CompareNoCase("mail from") == 0) return 2;
	if ((requestMessage->Left(7)).CompareNoCase("rcpt to") == 0) return 3;

	for (int i=1; i<10; i++)
	{
		if (sCmd.CompareNoCase(SMTP_CMD[i].cmd_name) == 0) return i;
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
	case SMTP_RCPT_CMD:		ProcessRCPTCommand();		break;
	case SMTP_DATA_CMD:		ProcessDATACommand();		break;
	case SMTP_QUIT_CMD:		ProcessQUITCommand();		break;
		// TODO: Add them vao day
	}
}

void CSMTPClient::Reply( CString msg )
{
	Send(msg,msg.GetLength(),0);
}

void CSMTPClient::ProcessERRORCommand()
{
	Reply("502 Unknown command\r\n");
}

void CSMTPClient::ProcessHELOCommand()
{
	CString _returnMsg;

	_returnMsg.Format("250 Welcome %s, Pleased to meet you.\r\n", m_ClientAddress); // TODO: May co the sua cho no pro hon :D

	Reply(_returnMsg);
}

void CSMTPClient::ProcessMAILFROMCommand()
{
	//long add
	CString _returnMsg;
	GetMailFrom();
	_returnMsg.Format("250 sender %s OK...\r\n", m_mailHdr->From);
	Reply(_returnMsg);

}
void CSMTPClient::ProcessRCPTCommand()
{
//long add
	CString _returnMsg;
	GetRCPTTo();
	_returnMsg.Format("250 Recipient %s OK...\r\n", m_mailHdr->To);
	Reply(_returnMsg);
}
void CSMTPClient::ProcessDATACommand()
{
	//long add
	Reply("354 Enter mail, end with "+"."+" on a line by itself\r\n");
	GetDATA();
	if (!m_mailHdr->TextBody.IsEmpty())
	{
		Reply("250 Message accepted for delivery.\r\n");		
	}
}

void CSMTPClient::ProcessQUITCommand()
{
	if (m_ClientRequest.CompareNoCase("quit") != 0)
	{
		ProcessERRORCommand();
		return;
	}
	Reply("221 SMTP Mail Server quited.\r\n"); // TODO: May co the thay tu ngu khac cho no pro hon
	// TODO: Add cac cau lenh dong socket vao cho nay nhe
	CloseSocket();
}

void CSMTPClient::CloseSocket()
{
	CString message;
	message.Format("%s - SMTP connection (ID=%d) closed", GetCurrentTimeStr(), smtpProcessId);
	this->m_parrent->WriteLog(message);
	Close();
}

void CSMTPClient::Initialize()
{
	this->m_ClientRequest = "";
	this->m_ClientAddress = "";
	//phuc add 20101203
	m_mailHdr = new MailHeader();
	//end phuc add 20101203

}
//long add
void CSMTPClient::GetMailFrom()
{
	CString temp;
	temp = m_ClientRequest;
	temp.Delete(0,10);
	//temp.Delete(temp.GetLength()-1, 1);
	temp.TrimLeft();
	temp.TrimRight();
	m_mailHdr->From = temp;

}
void CSMTPClient::GetRCPTTo()
{
	CString temp;
	temp = m_ClientRequest;
	temp.Delete(0,8);
	//temp.Delete(temp.GetLength()-1, 1);
	temp.TrimLeft();
	temp.TrimRight();
	m_mailHdr->To = temp;
}
void CSMTPClient::GetDATA()
{
	CString temp;
	while (1)
	{ 
		if (m_ClientRequest == ".\r\n")
		{
			break;
		}
		temp += m_ClientRequest;
	}
	m_mailHdr->TextBody = temp;
}