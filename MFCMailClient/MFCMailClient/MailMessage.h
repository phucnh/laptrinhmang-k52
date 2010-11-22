#pragma once

public class MailHeader
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
	INT		 UserId;

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
