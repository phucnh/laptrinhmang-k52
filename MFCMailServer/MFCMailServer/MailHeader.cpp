#include "StdAfx.h"
#include "MailHeader.h"
#include <string>
#include <cstring>
#include "MailUser.h"

MailHeader::MailHeader( 
					   CString _from,
					   CString _to,
					   CString _date,
					   CString _subject,
					   CString _cc,
					   CString _replyTo,
					   CString _textBody,
					   BYTE _realAttach,
					   BOOL _isDeleted
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
	this->IsDeleted = _isDeleted;
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
	if (dal->IsOpen()) dal->Close();
}
CArray<MailHeader,MailHeader&>* MailHeader::getAllMail()
{
	sql.Format(_T("Select * from MailHeader ;"));
	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
	CRecordset *dataMail;
	

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttachString,MessageId;
	BYTE RealAttach;
	INT UserId,GroupId,MailId;
	MailHeader mailheader;
		
	try
	{
		dataMail=dal->GetRecordSet(sql);
		if(dataMail==NULL) return NULL;

	
			else
				{
					
					while(!dataMail->IsEOF())
					{
							
						dataMail->GetFieldValue(_T("MessageID"),MessageId);
						dataMail->GetFieldValue(_T("From"),From);
						dataMail->GetFieldValue(_T("To"),To);
						dataMail->GetFieldValue(_T("Subject"),Subject);
						dataMail->GetFieldValue(_T("Cc"),Cc);
						dataMail->GetFieldValue(_T("Date"),Date);
						dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
						dataMail->GetFieldValue(_T("TextBody"),TextBody);
					/*	dataMail->GetFieldValue(_T("MimeVersion",MimeVersion);
						dataMail->GetFieldValue(_T("ContendType",ContendType);*/

						dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
					/*	dataMail->GetFieldValue(_T("GroupId",GroupId);
						dataMail->GetFieldValue(_T("UserId",UserId);
						dataMail->GetFieldValue(_T("MailId",MailId);*/

						//convert String to BYTE
						RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

						//mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);
						 mailheader.Cc=Cc;
						 mailheader.From=From;
						 mailheader.Date=Date;
						 mailheader.Subject=Subject;
						 mailheader.RealAttach=RealAttach;
						 mailheader.ReplyTo=ReplyTo;
						 mailheader.TextBody=TextBody;
						 mailheader.To=To;

						listMailHeader->Add(mailheader);


						dataMail->MoveNext();

						

					}

				return listMailHeader;
				}

		}
	 catch(CException* e)
		 {
				throw;
				e->Delete();

		 }

	 //Da sua lai dung, test o nut dong  Server
}
CArray<MailHeader,MailHeader&>* MailHeader::getAllInboxMailByUser(CString username)
{
//	sql.Format(_T("Select * FROM MailHeader  where [To] like '%'%s'%' ;"),username);
	sql.Format(_T("Select * FROM MailHeader  where [To]= '%s' ;"),username);
	
	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
	CRecordset *dataMail;
	

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttachString,MessageId;
	BYTE RealAttach;
	INT UserId,GroupId,MailId;
	MailHeader mailheader;
		
	try
	{
		dataMail=dal->GetRecordSet(sql);
		if(dataMail==NULL) return NULL;

	
			else
				{
					int number=dataMail->GetRecordCount();
					while(!dataMail->IsEOF())
					{
							
						dataMail->GetFieldValue(_T("MessageID"),MessageId);
						dataMail->GetFieldValue(_T("From"),From);
						dataMail->GetFieldValue(_T("To"),To);
						dataMail->GetFieldValue(_T("Subject"),Subject);
						dataMail->GetFieldValue(_T("Cc"),Cc);
						dataMail->GetFieldValue(_T("Date"),Date);
						dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
						dataMail->GetFieldValue(_T("TextBody"),TextBody);
					/*	dataMail->GetFieldValue(_T("MimeVersion",MimeVersion);
						dataMail->GetFieldValue(_T("ContendType",ContendType);*/

						dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
					/*	dataMail->GetFieldValue(_T("GroupId",GroupId);
						dataMail->GetFieldValue(_T("UserId",UserId);
						dataMail->GetFieldValue(_T("MailId",MailId);*/

						//convert String to BYTE
						RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

						//mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);
						 mailheader.Cc=Cc;
						 mailheader.From=From;
						 mailheader.Date=Date;
						 mailheader.Subject=Subject;
						 mailheader.RealAttach=RealAttach;
						 mailheader.ReplyTo=ReplyTo;
						 mailheader.TextBody=TextBody;
						 mailheader.To=To;
						 

						listMailHeader->Add(mailheader);


						dataMail->MoveNext();

						

					}

				return listMailHeader;
				}

		}
	 catch(CException* e)
		 {
				throw;
				e->Delete();

		 }
	
	//Loi cau lenh Sql: he thong dung luon
	 //DANG: Da sua xong 6
}
CArray<MailHeader,MailHeader&>* MailHeader::getAllSentMailByUser(CString  username)
{
	
	 MailUser* mailU=new MailUser();
	 INT UserId2=mailU->getIdFromUsername(username);

	if(UserId2==NULL) return NULL;
	sql.Format(_T("Select *  FROM MailHeader  where UserId= %d ;"),UserId2);
	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
	CRecordset *dataMail;
	

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttachString,MessageId;
	BYTE RealAttach;
	INT UserId,GroupId,MailId;
	MailHeader mailheader;
		
	try
	{
		dataMail=dal->GetRecordSet(sql);
		if(dataMail==NULL) return NULL;
	
			else
				{
					
					while(!dataMail->IsEOF())
					{
							
						dataMail->GetFieldValue(_T("MessageID"),MessageId);
						dataMail->GetFieldValue(_T("From"),From);
						dataMail->GetFieldValue(_T("To"),To);
						dataMail->GetFieldValue(_T("Subject"),Subject);
						dataMail->GetFieldValue(_T("Cc"),Cc);
						dataMail->GetFieldValue(_T("Date"),Date);
						dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
						dataMail->GetFieldValue(_T("TextBody"),TextBody);
					/*	dataMail->GetFieldValue(_T("MimeVersion",MimeVersion);
						dataMail->GetFieldValue(_T("ContendType",ContendType);*/

						dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
					/*	dataMail->GetFieldValue(_T("GroupId",GroupId);
						dataMail->GetFieldValue(_T("UserId",UserId);
						dataMail->GetFieldValue(_T("MailId",MailId);*/

						//convert String to BYTE
						RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

						//mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);
						 mailheader.Cc=Cc;
						 mailheader.From=From;
						 mailheader.Date=Date;
						 mailheader.Subject=Subject;
						 mailheader.RealAttach=RealAttach;
						 mailheader.ReplyTo=ReplyTo;
						 mailheader.TextBody=TextBody;
						 mailheader.To=To;

						listMailHeader->Add(mailheader);


						dataMail->MoveNext();

						

					}

				return listMailHeader;
				}

		}
	 catch(CException* e)
		 {
				throw;
				e->Delete();

		 }

	 //Loi lay du lieu tu db voi cau lenh sql: dataMail ko co du lieu nao
	  // DANG: Da sua xong , Trung Kiem tra ho to cai
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
				dataset->GetFieldValue(_T("Cc"),cc);
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
		
		//Dung!
}



bool MailHeader::deleteMail(INT mailID)
{
	sql.Format(_T("Delete * FROM MailHeader where MailId= %d ; "),mailID);
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

	//Loi: cau lenh Sql sai -> bao loi
	//DANG: Da sua lai  va test
}

bool MailHeader::InsertMail(MailHeader* mailH)
{
	int _IsDeleted;

	if (mailH->IsDeleted = TRUE)
		_IsDeleted = 1;
	else
	{
		if (mailH->IsDeleted = FALSE)
		{
			_IsDeleted = 0;
		}
	}
	mailH->Date = GetCurrentTimeStr();
	sql.Format("INSERT INTO MailHeader ([From],To,[Date],Subject,Cc,ReplyTo,TextBody,RealAttach,IsDeleted) VALUES('%s','%s',#%s#,'%s','%s','%s','%s',%d,%d);",
		mailH->From,
		mailH->To,
		mailH->Date,
		mailH->Subject,
		mailH->Cc,
		mailH->ReplyTo,
		mailH->TextBody,
		mailH->RealAttach,
		_IsDeleted
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

INT16 MailHeader::getSizeOfMail( MailHeader* mailHeader )
{
	INT16 m_nTotalSize;
	m_nTotalSize = mailHeader->Cc.GetLength()
		+ mailHeader->ContentType.GetLength()
		+ mailHeader->Date.GetLength()
		+ mailHeader->From.GetLength()
		+ mailHeader->MessageID.GetLength()
		+ mailHeader->MimeVersion.GetLength()
		+ mailHeader->RealAttach
		+ mailHeader->ReplyTo.GetLength()
		+ mailHeader->Subject.GetLength()
		+ mailHeader->TextBody.GetLength()
		+ mailHeader->To.GetLength();
	return m_nTotalSize;
}

INT16 MailHeader::getLinesOfTextBody( MailHeader* mailHeader )
{
	INT16 totalLines;
	CString currentTextBody = mailHeader->TextBody;
	INT16 i = 0;
	INT16 lengthOfTextBody = currentTextBody.GetLength();
	if (currentTextBody != "")
	{
		totalLines = 1;
	}
	for (i=0;i<=lengthOfTextBody;i++)
	{
		if (currentTextBody.GetAt(i) == '\n')
		{
			totalLines++;
		}
	}
	return totalLines;
}