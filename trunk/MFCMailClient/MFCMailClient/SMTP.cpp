#include "stdafx.h"
#include "SMTP.h"
#include "string.h"
#include "EntitiesServices.h"
#include <afxstr.h>
#include "GlobalFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSMTP::CSMTP( LPCTSTR szSMTPServerName, UINT nPort )
{
	ASSERT( szSMTPServerName != NULL );
	AfxSocketInit();
	//m_sMailerName = _T( "WC Mail" );
	m_IpAddress = szSMTPServerName;
	m_Port = nPort;
	m_Connected = FALSE;
	m_Error = _T( "OK" );
}

CSMTP::CSMTP()
{
	AfxSocketInit();
}

BOOL CSMTP::Connect()
{
	CString sHello;
	TCHAR local_host[ 80 ];	
	if( m_Connected )
		return TRUE;

	if( !m_Server.Create() )
	{
		m_Error = _T( "Unable to create the socket." );
		return FALSE;
	}
	if( !m_Server.Connect((LPCTSTR)m_IpAddress,m_Port) )
	{
		m_Error = _T( "Unable to connect to server" );
		m_Server.Close();
		return FALSE;
	}
	if( !get_response( SERVER_AVAILABLE ) )
	{
		m_Error = _T( "Server didn't respond." );
		m_Server.Close();
		return FALSE;
	}
	gethostname( local_host, 80 );
	sHello.Format( "HELO %s\r\n", local_host );
	m_Server.Send( (LPCTSTR)sHello, sHello.GetLength() );
	if( !get_response( GENERIC_SUCCESS ) )
	{
		m_Server.Close();
		return FALSE;
	}
	m_Connected = TRUE;
	return TRUE;
}

BOOL CSMTP::Disconnect()
{
	BOOL ret;
	if( !m_Connected )
		return TRUE;
	CString sQuit = _T( "QUIT\r\n" );
	m_Server.Send( (LPCTSTR)sQuit, sQuit.GetLength() );

	ret = get_response( QUIT_SUCCESS );
	m_Server.Close();

	m_Connected = FALSE;
	return ret;
}

CSMTP::~CSMTP()
{
	if( m_Connected )
		Disconnect();
}

CString CSMTP::GetServerHostName()
{
	return m_IpAddress;
}

UINT CSMTP::GetPort()
{
	return m_Port;
}

BOOL CSMTP::get_response( UINT response_expected )
{
	CString sResponse;
	UINT response;
	//response_code* pResp;	// Shorthand

	if( m_Server.Receive( response_buf, RESPONSE_BUFFER_SIZE ) == SOCKET_ERROR )
	{
		m_Error = _T( "Socket Error" );
		return FALSE;
	}
	response_buf[3] = '\0';
	response = atoi(response_buf);
	
	switch(response_expected)
	{
		case SERVER_AVAILABLE:
			if( response != SERVER_AVAILABLE )
			{
				return FALSE;
			}
			break;
		case CONNECT_SUCCESS:
			if( response != CONNECT_SUCCESS )
			{
				return FALSE;
			}
			break;
	}
	return TRUE;
}

CString CSMTP::GetLastError()
{
	return m_Error;
}

BOOL CSMTP::SendMessage(MailHeader * msg)
{
	ASSERT( msg != NULL );

	msg->Date = GetCurrentTimeStr();

	if( !m_Connected )
	{
		m_Error = _T( "Must be connect" );
		return FALSE;
	}
	/*if( FormatMailMessage( msg ) == FALSE )
	{
		return FALSE;
	}*/
	if( transmit_message( msg ) == FALSE )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSMTP::SendMessage( MailHeader* msg, CMimeMessage* mime )
{
	ASSERT(msg != NULL);
	ASSERT(mime!= NULL);

	msg->Date = GetCurrentTimeStr();

	if (!m_Connected)
	{
		m_Error = _T("Must be connect");
		return FALSE;
	}
	if( transmit_message( msg, mime ) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}
BOOL CSMTP::FormatMailMessage( MailHeader * msg )
{
	ASSERT( msg != NULL );

	if( prepare_header( msg ) == FALSE )
	{
		return FALSE;
	}
	if( msg->TextBody.Right( 2 ) != "\r\n" )
		msg->TextBody += "\r\n";
	return TRUE;
}

void CSMTP::SetServerProperties( LPCTSTR sServerHostName, UINT nPort)
{
	ASSERT( sServerHostName != NULL );
	if( sServerHostName == NULL )
		return;
	m_IpAddress = sServerHostName;
	m_Port = nPort;
}

BOOL CSMTP::prepare_header(MailHeader * msg)
{
	CTime currentDate = GetCurrentTime();
	ASSERT( msg != NULL );
	CString sTo;
	CString sDate;
	CString sSubject = msg->Subject;	
	CString sMessageId = "";
	CString sSender = msg->From.Left(msg->From.Find("@"));
	sMessageId.Format("<%s@%s>",currentDate.Format("%d%m%y.%H%M%S"),sSender);

	CString sHeader = "";	
	
	msg->Date = currentDate.Format("%a, %d %b %y %H:%M:%S %Z");
	// Format: Mon, 01 Jun 2010 01:10:30 GMT
	sDate = msg->Date;
	sHeader.Format( "Date: %s\r\n"\
		"From: %s\r\n"\
		"To: %s\r\n"\
		"Subject: %s\r\n"
		"Message-ID: %s\r\n",
		(LPCTSTR)sDate,
		(LPCTSTR)msg->From,
		(LPCTSTR)msg->To,
		(LPCTSTR)sSubject,
		(LPCTSTR)sMessageId);
	return TRUE;
}

CString CSMTP::prepare_body(MailHeader * msg)
{
	ASSERT( msg != NULL );
	CString sTemp;
	CString sCooked = "";
	LPTSTR szBad = "\r\n.\r\n";
	LPTSTR szGood = "\r\n..\r\n";
	int nPos;
	int nStart = 0;
	int nBadLength = strlen( szBad );
	sTemp = msg->TextBody;
	if( sTemp.Left( 3 ) == ".\r\n" )
		sTemp = "." + sTemp;
	
	while( (nPos = sTemp.Find( szBad )) > -1 )
	{
		sCooked = sTemp.Mid( nStart, nPos );
		sCooked += szGood;
		sTemp = sCooked + sTemp.Right( sTemp.GetLength() - (nPos + nBadLength) );
	}
	return sTemp;
}

BOOL CSMTP::transmit_message(MailHeader* msg)
{
	CString sFrom;
	CString sTo;
	CString sTemp;
	CString sEmail;

	ASSERT( msg != NULL );
	if( !m_Connected )
	{
		m_Error = _T( "Must be connected" );
		return FALSE;
	}

	// gui MAIL 
	sFrom.Format( "MAIL From: <%s>\r\n", (LPCTSTR)msg->From );
	m_Server.Send( (LPCTSTR)sFrom, sFrom.GetLength() );
	Sleep(300);
	if( !get_response( GENERIC_SUCCESS ) )
		return FALSE;
	
	// gui RCPT
	sFrom.Format( "RCPT To: <%s>\r\n", (LPCTSTR)msg->To );
	m_Server.Send( (LPCTSTR)sFrom, sFrom.GetLength() );
	Sleep(300);
		if( !get_response( GENERIC_SUCCESS ) )
			return FALSE;

	// gui DATA 
	sTemp = "DATA\r\n";
	m_Server.Send( (LPCTSTR)sTemp, sTemp.GetLength() );
	/*if( !get_response( INPUT_DATA ) )
	{
		return FALSE;
	}*/
	// Send the header
	Sleep(300);
	CString _messageFormat;
	_messageFormat.Format("Message-ID: %s\r\nFrom: %s\r\nTo: %s\r\nCc: %s\r\nDate: %s\r\nSubject: %s\r\nMime-Version: %s\r\nContent-Type: %s\r\n\n%s",
		msg->MessageID,
		msg->From,
		msg->To,
		msg->Cc,
		msg->Date,
		msg->Subject,
		msg->MimeVersion,
		msg->ContentType,
		msg->TextBody);
	m_Server.Send(_messageFormat,_messageFormat.GetLength());
	//Sleep(300);
	//m_Server.Send( (LPCTSTR)msg->Subject, msg->Subject.GetLength() );
	// Send the body
	//sTemp = prepare_body( msg );
	//m_Server.Send( (LPCTSTR)sTemp, sTemp.GetLength() );

	Sleep(300);

	// Them dau . de ket thuc mail
	sTemp = "\r\n.\r\n";
	m_Server.Send( (LPCTSTR)sTemp, sTemp.GetLength() );
	if( !get_response( GENERIC_SUCCESS ) )
	{
		return FALSE;
	}

	CMailHeaderServices* mailHeadrService = new CMailHeaderServices();
	msg->UserId = globalUser.UserId();
	msg->GroupId = 3;
	mailHeadrService->InsertNewMail(msg);

	if (mailHeadrService != NULL) delete mailHeadrService;

	return TRUE;
}

BOOL CSMTP::transmit_message( MailHeader* msg, CMimeMessage* mime )
{
	CString sFrom;
	CString sTo;
	CString sTemp;
	CString sEmail;

	ASSERT( msg != NULL );
	if( !m_Connected )
	{
		m_Error = _T( "Must be connected" );
		return FALSE;
	}

	sFrom.Format( "MAIL From: <%s>\r\n", (LPCTSTR)msg->From );
	m_Server.Send( (LPCTSTR)sFrom, sFrom.GetLength() );
	Sleep(300);
	if( !get_response( GENERIC_SUCCESS ) )
		return FALSE;

	sFrom.Format( "RCPT To: <%s>\r\n", (LPCTSTR)msg->To );
	m_Server.Send( (LPCTSTR)sFrom, sFrom.GetLength() );
	Sleep(300);
		if( !get_response( GENERIC_SUCCESS ) )
			return FALSE;

	sTemp = "DATA\r\n";
	m_Server.Send( (LPCTSTR)sTemp, sTemp.GetLength() );
	/*if( !get_response( INPUT_DATA ) )
	{
		return FALSE;
	}*/
	// Send the header
	Sleep(300);
	CString _mailMessage(mime->ConvertToString());
	m_Server.Send(_mailMessage,_mailMessage.GetLength());
	Sleep(300);
	//m_Server.Send( (LPCTSTR)msg->Subject, msg->Subject.GetLength() );
	// Send the body
	//sTemp = prepare_body( msg );
	//m_Server.Send( (LPCTSTR)sTemp, sTemp.GetLength() );

	Sleep(300);

	sTemp = "\r\n.\r\n";
	m_Server.Send( (LPCTSTR)sTemp, sTemp.GetLength() );
	if( !get_response( GENERIC_SUCCESS ) )
	{
		return FALSE;
	}

	CMailHeaderServices* mailHeadrService = new CMailHeaderServices();
	msg->UserId = globalUser.UserId();
	msg->GroupId = 3;
	msg->TextBody = mime->GetTextBody();
	mailHeadrService->InsertNewMail(msg);
	if (mailHeadrService != NULL) delete mailHeadrService;

	return TRUE;
}