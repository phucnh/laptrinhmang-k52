#pragma once

class MailMessage
{
private:
	CString _mailId;
	CString _fromAdd;
	CString _toAdd;
	CString _mailHeader;
	CString _mailSubject;
	CString _body;
	CTime _date;

public:
	MailMessage();

	MailMessage(
		CString mailId, 
		CString fromAdd, 
		CString toAdd, 
		CString mailHeader, 
		CString mailSubject,
		CString body,
		CTime date
		);

	~MailMessage(void);
};

class MailHeader
{
public:
	CString  From;
	CString  To;
	CString  Date;
	CString  Subject;
	CString  CC;
	CString  ReplyTo;
	CString	 TextBody;
	BYTE     RealAttach;

	MailHeader(
		CString _from,
		CString _to,
		CString _date,
		CString _subject,
		CString _cc,
		CString _replyTo,
		CString _textBody,
		BYTE _realAttach
		);

	MailHeader(void);
	~MailHeader(void);
};
