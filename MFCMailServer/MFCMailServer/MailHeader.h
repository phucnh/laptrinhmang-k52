#include "stdafx.h"

#ifndef CLASS_H_
#define CLASS_H_

class MailHeader
{
public:
	CString  sql;
	CRecordset *dataset;

public:
	CString  MessageID;
	CString  From;
	CString  To;
	CString  Date;
	CString  Subject;
	CString  Cc;
	CString  ReplyTo;
	CString	 TextBody;
	CString  MimeVersion;
	CString  ContentType;
	BYTE     RealAttach;
	BOOL	 IsDeleted;
	MailHeader( 
		CString _from,
		CString _to,
		CString _date,
		CString _subject,
		CString _cc,
		CString _replyTo,
		CString _textBody,
		BYTE _realAttach,
		BOOL _isDeleted
		);
	MailHeader(void);
	~MailHeader(void);

	CRecordset* getAllMail(CString username);
	CRecordset* getAllInboxMailByUser(CString username);
	CRecordset* getAllSentMailByUser(CString username);

	MailHeader* getMail(UINT mailID);
	//bool InsertMail(CString from,CString to,CString date,CString subject,CString cc,CString replyto,CString textbody,CString realattach);
	bool deleteMail(UINT mailID);

	//phuc add 20101121
	bool InsertMail(MailHeader* mailHeader);
	//end phuc add 20101121

	INT16 getSizeOfMail(MailHeader* mailHeader);
	INT16 getLinesOfTextBody(MailHeader* mailHeader);
};

#endif