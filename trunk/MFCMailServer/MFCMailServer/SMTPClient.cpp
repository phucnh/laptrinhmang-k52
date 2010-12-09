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

	while ((m_nStatus == SMTP_WAITING_CMD) ||
		(m_nStatus == SMTP_AFTER_CLIENT_SEND_DATA))
	{
		if (m_nStatus == SMTP_AFTER_CLIENT_SEND_DATA)
		{
			m_sQueue.Format("");
			m_ClientRequest.Format("");
			m_Buffer[0] = '\0';
			m_nStatus = SMTP_WAITING_CMD;
			break;
		}

		int	iPos = m_sQueue.Find('\n');
		if (iPos == -1) break;
		m_ClientRequest = m_sQueue.Left(iPos+1);
		m_sQueue.Delete(0, iPos+1);
		INT code = GetSMTPCommand(&m_ClientRequest);
		ProcessCommand(code);
	}
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

	if ((requestMessage->Left(10)).CompareNoCase("mail from:") == 0) 
	{
		if (requestMessage->GetLength() < 11) 
		{
			return CMDERROR;
		}
		else
		{
			return 2;
		}
		
	}
	if ((requestMessage->Left(8)).CompareNoCase("rcpt to:") == 0) 
	{
		if (requestMessage->GetLength() < 9) 
		{
			return CMDERROR;
		}
		else
		{
			return 3;
		}
	}

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
	case SMTP_NOOP_CMD:		ProcessNOOPCommand();		break;
	case SMTP_HELP_CMD:		ProcessHELPCommand();		break;
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
	m_nStatus = SMTP_ERROR_CMD;
	CString returnMsg(_T("502 Unknown command\r\n"));
	m_parrent->WriteLog(returnMsg);
	Reply(returnMsg);
	m_nStatus = SMTP_WAITING_CMD;
}

void CSMTPClient::ProcessHELOCommand()
{
	m_nStatus = SMTP_HELO_CMD;
	CString _returnMsg;

	_returnMsg.Format("250 Welcome %s, Nice to meet you.\r\n", m_ClientAddress);
	m_parrent->WriteLog(_returnMsg);
	Reply(_returnMsg);
	m_nStatus = SMTP_WAITING_CMD;
}

void CSMTPClient::ProcessMAILFROMCommand()
{
	//long add
	m_nStatus = SMTP_MAIL_CMD;
	CString _returnMsg;
	GetMailFrom();
	_returnMsg.Format("250 sender %s OK...\r\n", m_mailHdr->From);
	m_parrent->WriteLog(_returnMsg);
	Reply(_returnMsg);
	m_nStatus = SMTP_WAITING_CMD;

}
void CSMTPClient::ProcessRCPTCommand()
{
//long add
	m_nStatus = SMTP_RCPT_CMD;
	CString _returnMsg;
	GetRCPTTo();
	MailUser* user = new MailUser();
	user = user->GetUserByUsername(m_mailHdr->To);

	if (user == NULL)
	{
		_returnMsg.Format("544 User %s unknown\r\n", m_mailHdr->To);
	}
	else
	{
		_returnMsg.Format("250 Recipient %s OK...\r\n", m_mailHdr->To);
	}
	m_parrent->WriteLog(_returnMsg);
	Reply(_returnMsg);
	m_nStatus = SMTP_WAITING_CMD;

	if (user != NULL) delete user;
}
void CSMTPClient::ProcessDATACommand()
{
	//long add
	m_nStatus = SMTP_DATA_CMD;
	Reply("354 Enter mail, end with "+"."+" on a line by itself\r\n");
	m_Buffer[0] = '\0';
	m_sQueue.Format("");
	m_nStatus = SMTP_DATA_CMD;
	GetDATA();
	m_nStatus = SMTP_AFTER_CLIENT_SEND_DATA;
	if (!m_mailHdr->TextBody.IsEmpty())
	{
		CString _returnMsg("250 Message accepted for delivery.\r\n");
		m_parrent->WriteLog(_returnMsg);
		Reply(_returnMsg);		
	}
	//m_nStatus = SMTP_WAITING_CMD;

}
void CSMTPClient::ProcessNOOPCommand()
{
	m_nStatus = SMTP_NOOP_CMD;
	Reply("250 OK \r\n");
	m_nStatus = SMTP_WAITING_CMD;

}
void CSMTPClient::ProcessHELPCommand()
{
	m_nStatus = SMTP_HELP_CMD;
	Reply("214-This is SMTP server\r\n214-Following commands are suppported:\r\n214-HELO   MAIL   RCPT\r\n214-DATA   NOOP\r\n214-QUIT\r\n214-Nhom lap trinh mang 44\r\n214-End of HELP\r\n");
	m_nStatus = SMTP_WAITING_CMD;
}
void CSMTPClient::ProcessQUITCommand()
{
	m_nStatus = SMTP_QUIT_CMD;
	if (m_ClientRequest.CompareNoCase("quit") != 0)
	{
		ProcessERRORCommand();
		return;
	}

	CString _returnMsg("221 SMTP Mail Server quited.\r\n");
	m_parrent->WriteLog(_returnMsg);
	Reply(_returnMsg);
	CloseSocket();
}

void CSMTPClient::CloseSocket()
{
	CString message;
	message.Format("%s - SMTP connection (ID=%d) closed", GetCurrentTimeStr(), smtpProcessId);
	this->m_parrent->WriteLog(message);
	Close();

	m_mailHdr->InsertMail(m_mailHdr);
}

void CSMTPClient::Initialize()
{
	this->m_ClientRequest = "";
	this->m_ClientAddress = "";
	//phuc add 20101203
	m_mailHdr = new MailHeader();
	//end phuc add 20101203
	m_nStatus = SMTP_WAITING_CMD;

	/*try
	{
		smtpThread = AfxBeginThread(RUNTIME_CLASS(CWinThread),THREAD_PRIORITY_NORMAL);
	}
	catch (CException* e)
	{
		AfxMessageBox(_T("Can not create thread"));
		throw;
	}*/

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
	int nBytesRead;
	while (1)
	{		
		Sleep(5000);
		nBytesRead = Receive(m_Buffer, MAX_SMTP_BUFFER_SIZE-1);

		if (nBytesRead <1 ) continue;

		m_Buffer[nBytesRead] = '\0';
		m_sQueue.Append(m_Buffer);

		if(nBytesRead < 3) continue;

		if (nBytesRead > 3)
			if ((m_Buffer[nBytesRead-4]=='\n')&&
				(m_Buffer[nBytesRead-3]=='.') &&
				(m_Buffer[nBytesRead-2]=='\r')&&
				(m_Buffer[nBytesRead-1]=='\n'))
			{
				//m_sQueue.Delete(nBytesRead-3,3);
				break;
			}
		if (nBytesRead == 3)
			if ((m_Buffer[nBytesRead-3]=='.') &&
				(m_Buffer[nBytesRead-2]=='\r')&&
				(m_Buffer[nBytesRead-1]=='\n'))
			{
				//m_sQueue.Delete(nBytesRead-3,3);
				break;
			}
		
	}
	m_ClientRequest = m_sQueue;
	m_mailHdr->TextBody = m_ClientRequest;
	//m_ClientRequest.Delete(0,m_ClientRequest.GetLength());
	return;
}