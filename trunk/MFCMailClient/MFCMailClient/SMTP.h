#if !defined(AFX_SMTP_H__55DE48CB_BEA4_11D1_870E_444553540000__INCLUDED_)
#define AFX_SMTP_H__55DE48CB_BEA4_11D1_870E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxsock.h>
#include <atltime.h> // MFC time extensions
#include <afxtempl.h>
#include "MailMessage.h"
#include "Mime.h"

#define SMTP_PORT 25		// Standard port for SMTP servers
#define RESPONSE_BUFFER_SIZE 3

#define GENERIC_SUCCESS 0
#define SYSTEM_STATUS 211
#define HELP_MESSAGE 214
#define SERVER_AVAILABLE 220
#define QUIT_SUCCESS 221
#define CONNECT_SUCCESS 250
#define INPUT_DATA 354
#define SERVER_NA 421
#define MAILBOX_NA 450
#define PROCESS_ERROR 451
#define SYSTEM_ERRSTORAGE 452
#define SYNTAX_ERROR 500
#define SYNERROR_ARGUMENT 501
#define COMMAND_NOTIMPLEMENT 502
#define COMMAND_BADSEQUENCE 503
#define COMMAND_PARAMNOTIMPL 504
#define MAILBOX_NOTFOUND 550
#define USER_NOTLOCAL 551
#define EXCEEDED_STORAGEALLOCATION 552
#define MAILBOX_NAMENOTALLOWED 553
#define TRANSACTION_FAILED 554

class CSMTP
{
public:
	CSMTP( LPCTSTR szSMTPServerName, UINT nPort = SMTP_PORT );

	//phuc add 20101028
	CSMTP( );
	// end phuc add 20101028
	virtual ~CSMTP();

	void SetServerProperties( LPCTSTR sServerHostName, UINT nPort = SMTP_PORT );
	BOOL Connect();
	CString GetServerHostName();
	UINT GetPort();
	BOOL Disconnect();
	CString GetLastError();
	virtual BOOL FormatMailMessage( MailHeader* msg );
	BOOL SendMessage( MailHeader* msg );
	BOOL SendMessage(MailHeader* msg, CMimeMessage* mime);

private:
	BOOL get_response( UINT response_expected );
	BOOL transmit_message( MailHeader* msg );
	BOOL transmit_message(MailHeader* msg, CMimeMessage* mime);
	CString prepare_body( MailHeader* msg );
	BOOL prepare_header( MailHeader* msg);

	CString m_Error;
	BOOL m_Connected;

	UINT m_Port;
	CString m_IpAddress;
	CSocket m_Server;
protected:
	struct response_code
	{
		UINT nResponse;		// Response we're looking for
		TCHAR* sMessage;	// Error message if we don't get it
	};

	
	TCHAR response_buf[ RESPONSE_BUFFER_SIZE ];
	static response_code response_table[];
};



#endif