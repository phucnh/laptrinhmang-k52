#include "StdAfx.h"
#include "MailHeader.h"
#include <string>
#include <cstring>

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

	dal = new DAL();
}

MailHeader::~MailHeader( void )
{
	if (dal != NULL) delete dal;
}
CRecordset* MailHeader::getAllMail(CString username)
{
	sql.Format(_T("Select * from MailHeader where [From]='%s' or [To] like '%'%s'%' ;"),username,username);
	try
	{
		dal->GetRecordSet(sql);
		if(dal==NULL) return NULL;

	}
	catch (CException* e)
	{
		throw;
		e->Delete();
	}
	

}
CRecordset* MailHeader::getAllInboxMailByUser(CString username)
{
	sql.Format(_T("Select * from MailHeader where [To] like '%'%s'%' ;"),username);
	CRecordset* dataset;
	try
	{
		 dataset = dal->GetRecordSet(sql);
		 if(dataset==NULL) return NULL;
		 return dataset;
	}
	catch(CException* e)
	{
		throw;
		e->Delete();
	}

}
CRecordset* MailHeader::getAllSentMailByUser(CString  username)
{
	sql.Format(_T("Select * from MailHeader where [From]='%s'"),username);
	CRecordset* dataset;
	try
	{
		dataset = dal->GetRecordSet(sql);
		if(dataset==NULL) return NULL;
		return dataset;
	}
	catch(CException* e)
	{
		throw;
		e->Delete();
	}
}

MailHeader* MailHeader::getMail(UINT mailID)
{
	MailHeader *mailh=new MailHeader();
	sql.Format(_T("Select * from MailHeader where MailID=%d"),mailID);
	CString from, to, date, subject, cc, replyto, textbody, realattach;

	try
		{
			dataset = dal->GetRecordSet(sql);
			if(dataset==NULL) return NULL;
			else
			{

			
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
		}
			catch(CException* ee)
			{
				throw;
				ee->Delete();
			}

	}



bool MailHeader::deleteMail(UINT mailID)
{
	sql.Format(_T("Delete MailHeader where MailID=%d"),mailID);
	try
	{
		if(dal->ExecuteSQL(sql))
			return true;
		else 
			return false;
	}
	catch(CException* e)
	{
		throw;
		e->Delete();
	}
}

bool MailHeader::InsertMail(MailHeader* mailH)
{
	sql.Format("Insert into MailHeader values('%s','%s','%s','%s','%s','%s','%s','%s') ;",
		mailH->From,
		mailH->To,
		mailH->Date,
		mailH->Subject,
		mailH->Cc,
		mailH->ReplyTo,
		mailH->TextBody,
		mailH->RealAttach
		);

	try
	{
		if(dal->ExecuteSQL(sql))
			return true;
		else
			 return false;
	}
	catch(CException* e)	
	{
		throw;
		e->Delete();

	}
}