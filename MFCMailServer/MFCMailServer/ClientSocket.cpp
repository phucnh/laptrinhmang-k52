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

CClientSocket::CClientSocket( CMFCMailServerDlg* parrentDlg )
{
	this->m_parrent = parrentDlg;

	//phuc add 20101122
	nPop3ConnectionsCount++;
	pop3RequestId++;
	this->pop3ProcessId = pop3RequestId;
	this->m_parrent->UpdateStatusbar();

	m_POP3ConnectionsList.AddTail(this);

	//phuc add 20101123
	Initialize();
	//end phuc add 20101123
}
CClientSocket::~CClientSocket()
{
	POSITION pos = m_POP3ConnectionsList.Find(this);
	if (pos != NULL) m_POP3ConnectionsList.RemoveAt(pos);

	nPop3ConnectionsCount--;
	this->m_parrent->UpdateStatusbar();

	if (m_user != NULL)	delete m_user;
}


// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode)
{
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

void CClientSocket::Initialize()
{
	m_ClientRequest = "";
	m_totalMail = 0;
	m_totalSize = 0;
	m_user = NULL;
	m_sStatus = 11;
	m_user = new MailUser();
}

void CClientSocket::OnClose(int nErrorCode)
{
	CString message;
	message.Format(_T("%s - POP3 connection (ID=%d) closed"), GetCurrentTimeStr(), pop3ProcessId);
	this->m_parrent->WriteLog(message);
	CAsyncSocket::OnClose(nErrorCode);

	delete this;
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
	case NOOP_CMD:  ProcessNOOPCommand();	break;
	case RSET_CMD:	ProcessRSETCommand();	break;
	case TOP_CMD:	ProcessTOPCommand();	break;
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

void CClientSocket::ProcessERROR()
{
	Reply("-ERR Unknown command.");
}

void CClientSocket::ProcessUSERCommand()
{
	/*if ((m_user == NULL) &&
		(m_user->_username.IsEmpty())
		)
	{
		Reply("-ERR Username already specified.");
		return;
	}*/
	m_sStatus = USER_CMD;

	CString currentStatus;
	CString sUserInfo = m_ClientRequest;
	sUserInfo.Delete(0,4);
	sUserInfo.TrimLeft();
	sUserInfo.TrimRight();

	if (sUserInfo.IsEmpty())
	{
		currentStatus = "-ERR No username specified.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	m_user = GetUserByUsername(sUserInfo);
	//m_user = new MailUser("PHUC","12345");

	if (m_user != NULL)
	{
		sUserInfo = m_user->_username;
	}

	CString returnMsg;
	returnMsg.Format("+OK %s is a valid mailbox. Please enter password.", sUserInfo);
	m_parrent->WriteLog(returnMsg);
	returnMsg.ReleaseBuffer();
	returnMsg+="\r\n";
	Reply(returnMsg);
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessPASSCommand()
{
	m_sStatus = PASS_CMD;
	CString currentStatus;
	MailHeader* currentMailHeader = new MailHeader();
	CArray<MailHeader,MailHeader&>* testArray = new CArray<MailHeader,MailHeader&>();

	if (m_user != NULL)
		testArray = currentMailHeader->getAllInboxMailByUser(m_user->_username);

	if (testArray != NULL)
		m_totalMail = testArray->GetCount();

	if ((m_user == NULL) ||
		(m_user->_username.IsEmpty())
		)
	{
		currentStatus = "-ERR No user specified. Uses \"USER\" command first.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	CString sPassInfo = m_ClientRequest;
	sPassInfo.Delete(0,4);
	sPassInfo.TrimLeft();
	sPassInfo.TrimRight();

	if (sPassInfo.IsEmpty())
	{
		currentStatus = "-ERR No password specified.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}


	
	CString returnMsg;

	if ((m_user != NULL) && 
		(sPassInfo.Compare(m_user->_password) == 0))
		returnMsg.Format("+OK %s's mailbox has %d messages", m_user->_username, m_totalMail);
	else
		returnMsg.Format("-ERR No password specified.");
	this->m_parrent->WriteLog(returnMsg);
	returnMsg.ReleaseBuffer();
	returnMsg+="\r\n";
	Reply(returnMsg);
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessLISTCommand()
{
	m_sStatus = LIST_CMD;
	INT32 i = 0;
	CString currentStatus;
	/*MailHeader* test1 = new MailHeader();
	test1->Subject = "test1";
	test1->From = "Phuc";
	test1->TextBody = "test test test";
	MailHeader* test2 = new MailHeader();
	test2->Subject = "test2";
	CArray<MailHeader,MailHeader>* testArray = new CArray<MailHeader,MailHeader>();
	testArray->Add(*test1);
	testArray->Add(*test2);
	m_totalMail = testArray->GetCount();*/
	MailHeader* currentMailHeader = new MailHeader();
	CArray<MailHeader,MailHeader&>* testArray = new CArray<MailHeader,MailHeader&>();

	if (m_user != NULL)
		testArray = currentMailHeader->getAllInboxMailByUser(m_user->_username);

	if (testArray != NULL)
		m_totalMail = testArray->GetCount();
	
	m_ClientRequest.Delete(0,4);
	m_ClientRequest.TrimLeft();
	m_ClientRequest.TrimRight();

	UINT nIndex;
	if (m_ClientRequest != "")
	{
		if (strspn(m_ClientRequest, "0123456789") == (UINT)m_ClientRequest.GetLength())
		{
			nIndex = atoi(m_ClientRequest);
			if (nIndex <= 0)
			{
				currentStatus = "-ERR No such message.";
				m_parrent->WriteLog(currentStatus);
				Reply(currentStatus);
				m_sStatus = WAIT_CMD;
				return;
			}
			if (nIndex > m_totalMail)
			{
				currentStatus.Format("-ERR No such message, only %d messages in mailbox.",m_totalMail);
				m_parrent->WriteLog(currentStatus);
				Reply(currentStatus);
				m_sStatus = WAIT_CMD;
				return;
			}
			currentStatus.Format("+OK %d %s",nIndex, GetMessageInfo(nIndex, 2));
			m_parrent->WriteLog(currentStatus);
			Reply(currentStatus);
			m_sStatus = WAIT_CMD;
			return;
		}
		currentStatus.Format("-ERR Please enter only digits (0..9).");
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}
	// Hien thong tin tat ca cac mail trong box
	currentStatus.Format("+OK %d messages", m_totalMail);
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);

	if (m_totalMail > 0)
		for (i=0; i<m_totalMail; i++)
			if (&testArray->ElementAt(i) != NULL)
			{
				MailHeader* currentMailHeader = new MailHeader();
				currentMailHeader = &testArray->ElementAt(i);
				m_totalSize = currentMailHeader->getSizeOfMail(currentMailHeader);
				Reply("\n %d %d", i+1, m_totalSize);
			}

	Reply(".");
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessRETRCommand()
{
	m_sStatus = RETR_CMD;
	MailHeader* requestedMail = new MailHeader();
	/*MailHeader* test1 = new MailHeader();
	test1->Subject = "test1";
	test1->From = "Phuc";
	test1->Subject = "Test Mail";
	test1->TextBody = "test test test";
	test1->IsDeleted = TRUE;
	MailHeader* test2 = new MailHeader();
	test2->Subject = "test2";
	CArray<MailHeader,MailHeader>* testArray = new CArray<MailHeader,MailHeader>();
	testArray->Add(*test1);
	testArray->Add(*test2);
	m_totalMail = testArray->GetCount();*/
	MailHeader* currentMailHeader = new MailHeader();
	CArray<MailHeader,MailHeader&>* testArray = new CArray<MailHeader,MailHeader&>();
	testArray = currentMailHeader->getAllInboxMailByUser(m_user->_username);
	m_totalMail = testArray->GetCount();

	CString currentStatus;
	
	m_ClientRequest.Delete(0, 4);
	m_ClientRequest.TrimLeft();
	m_ClientRequest.TrimRight();

	if (m_ClientRequest == "")
	{
		currentStatus = "-ERR Please enter parameter.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	if (strspn(m_ClientRequest, "0123456789") != (UINT)m_ClientRequest.GetLength())
	{
		currentStatus = "-ERR Please enter only digits (0..9).";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	UINT nMailIndex = atoi(m_ClientRequest);
	INT32 currentMailIndex = nMailIndex;

	if ((nMailIndex <= 0) || (nMailIndex > m_totalMail))
	{
		currentStatus = "-ERR No such message.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	if (&testArray->ElementAt(currentMailIndex-1) != NULL)
	{
		MailHeader* currentMailHeader = new MailHeader();
		currentMailHeader = &testArray->ElementAt(currentMailIndex-1);
		if (currentMailHeader->IsDeleted == TRUE)
		{
			currentStatus.Format("-ERR Sorry, message %d already deleted.", nMailIndex);
			m_parrent->WriteLog(currentStatus);
			Reply(currentStatus);
			m_sStatus = WAIT_CMD;
			return;
		}
	}
	currentStatus.Format("+OK %s octets", GetMessageInfo(nMailIndex, 2));
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);

	//Lay toan bo thong tin cua mail ma nsd muon lay:
	requestedMail = &testArray->GetAt(currentMailIndex-1);
	if (requestedMail != NULL)
	{
		//currentStatus.Append("\n");
		currentStatus.Format("Received: from %s by %s \r\nMessage-ID: %s \nDate: %s \nSubject: %s \nContent: \n%s \n",
									requestedMail->From.Trim(), 
									requestedMail->From.Trim(),
									requestedMail->MessageID.Trim(),
									requestedMail->Date.Trim(),
									requestedMail->Subject.Trim(),
									requestedMail->TextBody.Trim());
		m_parrent->WriteLog(currentStatus);
		
		Reply(currentStatus);
	}
	Reply(".");
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessSTATCommand()
{
	m_sStatus = STAT_CMD;
	/*MailHeader* test1 = new MailHeader();
	test1->Subject = "test1";
	test1->From = "Phuc";
	test1->Subject = "Test Mail";
	test1->TextBody = "test test test";
	MailHeader* test2 = new MailHeader();
	test2->Subject = "test2";
	CArray<MailHeader,MailHeader>* testArray = new CArray<MailHeader,MailHeader>();
	testArray->Add(*test1);
	testArray->Add(*test2);
	m_totalMail = testArray->GetCount();*/
	MailHeader* currentMailHeader = new MailHeader();
	CArray<MailHeader,MailHeader&>* testArray = new CArray<MailHeader,MailHeader&>();
	testArray = currentMailHeader->getAllInboxMailByUser(m_user->_username);
	m_totalMail = testArray->GetCount();

	CString currentStatus;

	if (m_totalMail == 0)
	{
		currentStatus.Format("-ERR No such messages.");
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}
	
	INT32 i = 0;
	if (m_totalMail > 0)
		for (i=0; i<m_totalMail; i++)
			if (&testArray->ElementAt(i) != NULL)
			{
				MailHeader* currentMailHeader = new MailHeader();
				currentMailHeader = &testArray->ElementAt(i);
				m_totalSize = m_totalSize + currentMailHeader->getSizeOfMail(currentMailHeader);
			}

	currentStatus.Format("+OK %d %d", m_totalMail, m_totalSize);
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessDELECommand()
{
	m_sStatus = DELE_CMD;
	MailHeader* requestedMail = new MailHeader();
	/*MailHeader* test1 = new MailHeader();
	test1->Subject = "test1";
	test1->From = "Phuc";
	test1->Subject = "Test Mail";
	test1->TextBody = "test test test";
	MailHeader* test2 = new MailHeader();
	test2->Subject = "test2";
	CArray<MailHeader,MailHeader>* testArray = new CArray<MailHeader,MailHeader>();
	testArray->Add(*test1);
	testArray->Add(*test2);
	m_totalMail = testArray->GetCount();*/
	MailHeader* currentMailHeader = new MailHeader();
	CArray<MailHeader,MailHeader&>* testArray = new CArray<MailHeader,MailHeader&>();
	testArray = currentMailHeader->getAllInboxMailByUser(m_user->_username);
	m_totalMail = testArray->GetCount();

	CString currentStatus;
	INT32 currentMailIndex;
	CString tempClientRequest = m_ClientRequest;
	currentMailIndex = tempClientRequest.Delete(0,5);

	m_ClientRequest.Delete(0, 4);
	m_ClientRequest.TrimLeft();
	m_ClientRequest.TrimRight();
	// Neu khong co tham so
	if (m_ClientRequest == "")
	{
		currentStatus = "-ERR Please enter parameter.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}
	if (strspn(m_ClientRequest, "0123456789") != (UINT)m_ClientRequest.GetLength())
	{
		currentStatus = "-ERR Invalid parameter.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}
	UINT nIndex = atoi(m_ClientRequest);
	if ((nIndex <= 0) || (nIndex > m_totalMail))
	{
		currentStatus = "-ERR No such message.";
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}
	// Kiem tra xem mail thu nIndex da duoc xoa hay chua
	if (&testArray->ElementAt(currentMailIndex) == NULL)
	{
		currentStatus.Format("-ERR Message %d already deleted.",currentMailIndex);
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}
	// Danh dau mail nay se duoc xoa
	/*POSITION	pos;
	CString		sTemp;
	pos = m_MailboxInfo.FindIndex(nIndex-1);
	sTemp = m_MailboxInfo.GetAt(pos);
	sTemp.SetAt(sTemp.GetLength()-2, '1');
	m_MailboxInfo.SetAt(pos, sTemp);*/
	testArray->ElementAt(currentMailIndex-1).IsDeleted = TRUE;
	testArray->RemoveAt(currentMailIndex-1,1);
	currentStatus.Format("+OK Message %d deleted.", currentMailIndex);
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessQUITCommand()
{
	m_sStatus = QUIT_CMD;
	Reply("+OK");
	m_sStatus = WAIT_CMD;
	CloseSocket();
}

void CClientSocket::ProcessNOOPCommand()
{
	m_sStatus = NOOP_CMD;
	CString currentStatus;
	currentStatus.Format("+OK");
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessRSETCommand()
{
	m_sStatus = RSET_CMD;
	MailHeader* requestedMail = new MailHeader();
	/*MailHeader* test1 = new MailHeader();
	test1->Subject = "test1";
	test1->From = "Phuc";
	test1->Subject = "Test Mail";
	test1->TextBody = "test test test";
	MailHeader* test2 = new MailHeader();
	test2->Subject = "test2";
	CArray<MailHeader,MailHeader>* testArray = new CArray<MailHeader,MailHeader>();
	testArray->Add(*test1);
	testArray->Add(*test2);
	m_totalMail = testArray->GetCount();*/
	MailHeader* currentMailHeader = new MailHeader();
	CArray<MailHeader,MailHeader&>* testArray = new CArray<MailHeader,MailHeader&>();
	testArray = currentMailHeader->getAllInboxMailByUser(m_user->_username);
	m_totalMail = testArray->GetCount();

	INT32 i = 0;
	for (i=0;i<m_totalMail;i++)
	{
		if (&testArray->ElementAt(i) != NULL)
		{
			MailHeader* currentMailHeader = new MailHeader();
			currentMailHeader = &testArray->ElementAt(i);
			if (currentMailHeader->IsDeleted == TRUE)
			{
				currentMailHeader->IsDeleted = FALSE;
			}
			m_totalSize = m_totalSize + currentMailHeader->getSizeOfMail(currentMailHeader);
		}
	}
	CString currentStatus;
	currentStatus.Format("+OK Maildrop has %d messages (%d octets)",m_totalMail,m_totalSize);
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);
	m_sStatus = WAIT_CMD;
}

void CClientSocket::ProcessTOPCommand()
{
	m_sStatus = TOP_CMD;
	MailHeader* requestedMail = new MailHeader();
	/*MailHeader* test1 = new MailHeader();
	test1->Subject = "test1";
	test1->From = "Phuc";
	test1->Subject = "Test Mail";
	test1->TextBody = "test test test\n test test\ntest";
	MailHeader* test2 = new MailHeader();
	test2->Subject = "test2";
	CArray<MailHeader,MailHeader>* testArray = new CArray<MailHeader,MailHeader>();
	testArray->Add(*test1);
	testArray->Add(*test2);
	m_totalMail = testArray->GetCount();*/
	MailHeader* currentMailHeader = new MailHeader();
	CArray<MailHeader,MailHeader&>* testArray = new CArray<MailHeader,MailHeader&>();
	testArray = currentMailHeader->getAllInboxMailByUser(m_user->_username);
	m_totalMail = testArray->GetCount();

	CString tempClientRequest = m_ClientRequest;
	CString currentStatus;

	m_ClientRequest.Delete(0,3);
	m_ClientRequest.TrimLeft();
	m_ClientRequest.TrimRight();

	if (m_ClientRequest == "")
	{
		currentStatus.Format("-ERR Not enough parameter.");
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	if (strspn(m_ClientRequest, "0123456789 ") != (UINT)m_ClientRequest.GetLength())
	{
		currentStatus.Format("-ERR Please enter only digits (0..9).");
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	INT16 currentMailIndex;
	INT16 lineCount;
	INT16 i;
	CString sTempString;

	i = m_ClientRequest.Find(" ");
	if (i>0)
	{
		sTempString = m_ClientRequest.Left(i);
		currentMailIndex = atoi(sTempString);
		m_ClientRequest.Delete(0, i);
		lineCount = atoi(m_ClientRequest);
	}
	else
	{
		currentMailIndex = atoi(m_ClientRequest);
		lineCount = 0;
	}

	if ((currentMailIndex <= 0) || (currentMailIndex > m_totalMail))
	{
		currentStatus.Format("-ERR No such messages.");
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	//MailHeader* currentMailHeader = new MailHeader();
	currentMailHeader = &testArray->ElementAt(currentMailIndex-1);
	INT16 totalLinesOfTextBody = currentMailHeader->getLinesOfTextBody(currentMailHeader);

	if (currentMailHeader->IsDeleted == TRUE)
	{
		currentStatus.Format("-ERR Message %d already deleted.",currentMailIndex);
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}
	currentStatus.Format("+OK Top %d of this message: \n",lineCount);
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);
	currentStatus.Format("");

	if ((lineCount == 0) || (lineCount >= totalLinesOfTextBody))
	{
		currentStatus.Format("%s",currentMailHeader->TextBody);
		m_parrent->WriteLog(currentStatus);
		Reply(currentStatus);
		m_sStatus = WAIT_CMD;
		return;
	}

	for (i=0;i<currentMailHeader->TextBody.GetLength();i++)
	{
		currentStatus.AppendChar(currentMailHeader->TextBody.GetAt(i));
		if (currentMailHeader->TextBody.GetAt(i) == '\n')
		{
			lineCount--;
		}
		if (lineCount == 0)
		{
			break;
		}
	}
	
	m_parrent->WriteLog(currentStatus);
	Reply(currentStatus);
	m_sStatus = WAIT_CMD;
}

void CClientSocket::GetMailboxInfo(CString sMailbox)
{
	WIN32_FIND_DATA	FindFileData;
	HANDLE			hFindFile;
	CString			sTemp;
	BOOL			bContinue;

	m_totalMail = 0;
	m_totalSize = 0;
	m_MailboxInfo.RemoveAll();

	sTemp.Format("%s%s\\*.*", sMailStoreDirectory, sMailbox);
	hFindFile = FindFirstFile(sTemp, &FindFileData);
	bContinue = (hFindFile != NULL);
	while (bContinue)
	{
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			UINT nSize = FindFileData.nFileSizeHigh * MAXDWORD + FindFileData.nFileSizeLow;
			m_totalMail ++;
			m_totalSize += nSize;
			CString sTemp;
			sTemp.Format("%s%s\\%s;%d;0;", sMailStoreDirectory, sMailbox, FindFileData.cFileName, nSize);
			m_MailboxInfo.AddTail(sTemp);
		}
		bContinue = FindNextFile(hFindFile, &FindFileData);
	}
	FindClose(hFindFile);
}

CString CClientSocket::GetMessageInfo(UINT nID, int nField)
{
	POSITION	pos;
	CString		sTemp;

	pos = m_MailboxInfo.FindIndex(nID-1);
	if (pos == NULL) return _T("");
	sTemp = m_MailboxInfo.GetAt(pos);

	int i=-1, j=0, k=0;
	do
	{
		i = sTemp.Find(';', j);
		if (i >= 0)
		{
			if (++k == nField) return sTemp.Mid(j, i-j);
			j = i+1;
		}
	}
	while (i >= 0);

	return _T("");
}

#define TIME_OUT_MS	10000
void CClientSocket::SendString(const void *lpBuf, int nBufLen, int nFlags)
{
	DWORD	dwStartTime;
	int		iReturn = 0;

	dwStartTime = GetTickCount();
	do
	{
		iReturn = Send(lpBuf, nBufLen, 0);
	}
	while ((iReturn < nBufLen) && (GetTickCount() - dwStartTime < TIME_OUT_MS));

	//	m_pDlg->m_sLog += (LPSTR)lpBuf;
	//	m_pDlg->UpdateData(FALSE);
}

void CClientSocket::Reply(LPSTR format,...)
{
	CString	sMsg;
	va_list	argptr;

	va_start(argptr, format);
	vsprintf(sMsg.GetBuffer(1024), format, argptr);
	va_end(argptr);
	sMsg.ReleaseBuffer();
	sMsg+="\r\n";
	Send(sMsg, strlen(sMsg), 0);
}


void CClientSocket::CloseSocket()
{
	CString message;
	message.Format(_T("%s - POP3 connection (ID=%d) closed"), GetCurrentTimeStr(),pop3ProcessId);
	this->m_parrent->WriteLog(message);
	Close();
}

void CClientSocket::Reply( CString _message )
{
	Send(_message,_message.GetLength(),0);
}

MailUser* CClientSocket::GetUserByUsername( CString username )
{
	MailUser* currentUser = new MailUser();
	currentUser = currentUser->GetUserByUsername(username);

	return currentUser;
}

