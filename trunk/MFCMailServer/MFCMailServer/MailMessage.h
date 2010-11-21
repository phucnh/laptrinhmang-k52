#pragma once

class MailHeader
{
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

	CRecordset* MailHeader::getAllMail(CString username);
	CRecordset* MailHeader::getAllInboxMailByUser(CString username);
	CRecordset* MailHeader::getAllSendedMailByUser(CString username);

	MailHeader* getMail(UINT mailID);
	bool InsertMail(CString from,CString to,CString date,CString subject,CString cc,CString replyto,CString textbody,CString realattach);
	bool deleteMail(UINT mailID);

	//phuc add 20101121
	bool InsertMail(MailHeader* mailHeader);
	//end phuc add 20101121
};
