#include "StdAfx.h"
#include "MailMessage.h"
#include <string>
#include <cstring>

CString  sql;
CRecordset *dataset;

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

CRecordset* MailHeader::getAllMail(CString username)
{
	sql.Format(_T("Select * from MailHeader where [From]='%s' or [To] like '%'%s'%' ;"),username,username);
	return dal->GetRecordSet(sql);

}
CRecordset* MailHeader::getAllInboxMailByUser(CString username)
{
	sql.Format(_T("Select * from MailHeader where [To] like '%'%s'%' ;"),username);
	return dal->GetRecordSet(sql);

}
CRecordset* MailHeader::getAllSendedMailByUser(CString  username)
{
	sql.Format(_T("Select * from MailHeader where [From]='%s'"),username);
	return dal->GetRecordSet(sql);

}

MailHeader* MailHeader::getMail(UINT mailID)
{
	MailHeader *mailh=new MailHeader();
	sql.Format(_T("Select * from MailHeader where MailID=%d"),mailID);
	CString from, to, date, subject, cc, replyto, textbody, realattach;

	dataset = dal->GetRecordSet(sql);

	dataset->GetFieldValue(_T("From"),from);
	dataset->GetFieldValue(_T("To"),to);
	dataset->GetFieldValue(_T("Date"),date);
	dataset->GetFieldValue(_T("Subject"),subject);
	dataset->GetFieldValue(_T("CC"),cc);
	dataset->GetFieldValue(_T("ReplyTo"),replyto);
	dataset->GetFieldValue(_T("TextBody"),textbody);
	dataset->GetFieldValue(_T("RealAttach"),realattach);

	mailh->From = from;
	mailh->Cc = cc;
	mailh->To = to;
	mailh->Date = date;
	mailh->ReplyTo = replyto;
	mailh->TextBody = textbody;
	mailh->Subject = subject;
	mailh->RealAttach = atoi(realattach);

	return mailh;
}

bool MailHeader::InsertMail(CString from,CString to,CString date,CString subject,CString cc,CString replyto,CString textbody,CString realattach)
{
	/*CString from,to,date,subject,cc,replyto,textbody,realattach;

	from=mailheader->_from;
	to=mailheader->_to;
	date=mailheader->_date;
	subject=mailheader->_subject;
	cc=mailheader->_cc;
	replyto=mailheader->_replyto;
	textbody=mailheader->_textbody;
	realattach=mailheader->_realattach;*/

	sql.Format(_T("Insert Into MailHeader values('%s','%s','%s','%s','%s','%s','%s','%s')"),from,to,date,subject,cc,replyto,textbody,realattach);

	return dal->ExecuteSQL(sql);
}

bool MailHeader::deleteMail(UINT mailID)
{
	sql.Format(_T("Delete MailHeader where MailID=%d"),mailID);
	return dal->ExecuteSQL(sql);
}

bool MailHeader::InsertMail( MailHeader* mailHeader )
{
	sql.Format(_T("Insert into MailHeader values('%s','%s','%s','%s','%s','%s','%s','%s')"),
		mailHeader->From,
		mailHeader->To,
		mailHeader->Date,
		mailHeader->Subject,
		mailHeader->Cc,
		mailHeader->ReplyTo,
		mailHeader->TextBody,
		mailHeader->RealAttach
		);

	return dal->ExecuteSQL(sql);
}
#pragma endregion MailHeader