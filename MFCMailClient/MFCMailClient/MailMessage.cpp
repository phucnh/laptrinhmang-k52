#include "StdAfx.h"
#include "MailMessage.h"

#pragma region MailHeader
MailHeader::MailHeader( 
					   CString _from,
					   CString _to,
					   CString _date,
					   CString _subject,
					   CString _cc,
					   CString _replyTo,
					   CString _textBody,
					   BYTE _realAttach 
					   )
{
	this->MimeVersion = "1.0";
	this->From = _from;
	this->To = _to;
	this->Date = _date;
	this->Subject = _subject;
	this->Cc = _cc;
	this->ReplyTo = _replyTo;
	this->TextBody = _textBody;
	this->RealAttach = _realAttach;
}

MailHeader::MailHeader( void )
{
	this->MimeVersion = "1.0";
	this->From = "";
	this->To = "";
	this->Date = "";
	this->Subject = "";
	this->Cc = "";
	this->ReplyTo = "";
	this->TextBody = "";
	this->RealAttach = FALSE;
}

MailHeader::~MailHeader( void )
{

}
#pragma endregion MailHeader