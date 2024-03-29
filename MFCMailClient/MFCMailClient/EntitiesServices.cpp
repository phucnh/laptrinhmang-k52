#include "StdAfx.h"
#include "EntitiesServices.h"
#include <cstring>
#include <string>

CUserEntitiesServices::CUserEntitiesServices(void)
{
	dal = new CDAL();
}

CUser* CUserEntitiesServices::InsertNewUser( CUser* user )
{	
	CString userid,username,password,emailaddress,displayname;

	CUser* userreturn;
	if (user == NULL)	return NULL;

	sqlCommand1.Format(_T("INSERT INTO [User] (Username, Password, EmailAddress, DisplayName) VALUES ('%s','%s','%s','%s') ;"),
		user->Username(),
		user->Password(),
		user->EmailAddress(),
		user->DisplayName()
		);

	sqlCommand2.Format(_T("SELECT * From [User] where Username='%s'"),
		user->Username());
	
	try
	{	
		
		if (!dal->ExecuteSQL(sqlCommand1)) return NULL;

		dataUser=dal->GetRecordSet(sqlCommand2);
		dataUser->GetFieldValue(_T("UserId"),userid);
		dataUser->GetFieldValue(_T("Username"),username);
		dataUser->GetFieldValue(_T("Password"),password);
		dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
		dataUser->GetFieldValue(_T("DisplayName"),displayname);



	}
	catch (CException* e)
	{
		throw;
		delete e;
	}

	userreturn->Username(username);
	userreturn->UserId(_tstoi(userid)); //CString to UINT
	userreturn->Password(password);
	userreturn->EmailAddress(emailaddress);
	userreturn->DisplayName(displayname);

	return userreturn;
}

CUser* CUserEntitiesServices::GetByUserId( INT userId )
{
	CUser *userreturn = new CUser();
	CString userid,username,password,emailaddress,displayname;
	
	sqlCommand2.Format(_T("SELECT * From [User] where UserId=%d ;"),userId);
	
	try
	{
		dataUser=dal->GetRecordSet(sqlCommand2);
	}
	catch(CException* e)
	{
		throw;
		delete e;
	}

	if(dataUser==NULL) return NULL;	

	else
			{
				dataUser->GetFieldValue(_T("UserId"),userid);
				dataUser->GetFieldValue(_T("Username"),username);
				dataUser->GetFieldValue(_T("Password"),password);
				dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
				dataUser->GetFieldValue(_T("DisplayName"),displayname);
				
				userreturn->Username(username);
				userreturn->UserId(_tstoi(userid)); //CString to UINT
				userreturn->Password(password);
				userreturn->EmailAddress(emailaddress);
				userreturn->DisplayName(displayname);
				
				return userreturn;
			}

	
}

CUser* CUserEntitiesServices::GetByUsername( CString username )
{
	CUser *userreturn = new CUser();
	CString userid,usernamereturn,password,emailaddress,displayname;

	sqlCommand2.Format(_T("select * from User where Username = '%s'"),username);

	try
	{
		dataUser=dal->GetRecordSet(sqlCommand2);
	}
	catch(CException* e)
	{
		throw;
		delete e;
	}

	if((dataUser==NULL) || (dataUser->GetRecordCount() == 0)) return NULL;	
	else
	{


		//dataUser=dal->GetRecordSet(sqlCommand2);
		dataUser->GetFieldValue(_T("UserId"),userid);
		dataUser->GetFieldValue(_T("Username"),usernamereturn);
		dataUser->GetFieldValue(_T("Password"),password);
		dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
		dataUser->GetFieldValue(_T("DisplayName"),displayname);
		
		userreturn->Username(usernamereturn.GetString());
		userreturn->UserId(_tstoi(userid.GetString())); //CString to UINT
		userreturn->Password(password.GetString());
		userreturn->EmailAddress(emailaddress.GetString());
		userreturn->DisplayName(displayname.GetString());

		return userreturn;

	}

}

CUser* CUserEntitiesServices::DeleteUserById( INT userID )
{

	CUser *userreturn = new CUser();
	CString userid,username,password,emailaddress,displayname;

	sqlCommand1.Format(_T("SELECT * From User where UserId=%d ;"),userID);
	sqlCommand2.Format(_T("Delete From User where UserId=%d  ; "),userID);

	try
	{
		dataUser=dal->GetRecordSet(sqlCommand1);
	}
	catch(CException* e)
	{
		throw;
		delete e;
	}

	if(dataUser==NULL) return NULL;	

	else
	{


		//dataUser=dal->GetRecordSet(sqlCommand1);
		dataUser->GetFieldValue(_T("UserId"),userid);
		dataUser->GetFieldValue(_T("Username"),username);
		dataUser->GetFieldValue(_T("Password"),password);
		dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
		dataUser->GetFieldValue(_T("DisplayName"),displayname);
		//creat user info
		userreturn->Username(username);
		userreturn->UserId(_tstoi(userid)); //CString to UINT
		userreturn->Password(password);
		userreturn->EmailAddress(emailaddress);
		userreturn->DisplayName(displayname);
		//delete User
		dal->ExecuteSQL(sqlCommand2);
		
		return userreturn;

	}

}

CUser* CUserEntitiesServices::DeleteUserByUsername( CString username )
{
	CUser *userreturn = new CUser();
	CString userid,usernamereturn,password,emailaddress,displayname;

	sqlCommand1.Format(_T("SELECT * From User where Username='%s' ;"),username);
	sqlCommand2.Format(_T("Delete from User where UserId='%s' ;"),username);

	try
	{
		dataUser=dal->GetRecordSet(sqlCommand1);
	}
	catch(CException* e)
	{
		throw;
		delete e;
	}

	if(dataUser==NULL) return NULL;	

	else
	{


		
		dataUser->GetFieldValue(_T("UserId"),userid);
		dataUser->GetFieldValue(_T("Username"),usernamereturn);
		dataUser->GetFieldValue(_T("Password"),password);
		dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
		dataUser->GetFieldValue(_T("DisplayName"),displayname);
		//fill user info
		userreturn->Username(usernamereturn);
		userreturn->UserId(_tstoi(userid)); //CString to UINT
		userreturn->Password(password);
		userreturn->EmailAddress(emailaddress);
		userreturn->DisplayName(displayname);
		//delete User
		dal->ExecuteSQL(sqlCommand2);

		return userreturn;

	}

}

BOOL CUserEntitiesServices::UpdateUserById(INT userId,CUser* user )
{

	sqlCommand1.Format(_T("Update [User] Set Username='%s' , Password='%s', EmailAddress='%s' , DisplayName='%s' where UserId=%d ;"),
		user->Username(),
		user->Password(),
		user->EmailAddress(),
		user->DisplayName(),
		userId
		);

	try
	{
		
		dal->ExecuteSQL(sqlCommand1);
		return TRUE;
	}
	catch (CException* e)
	{
		return FALSE;
		throw;
		delete e;
	}


}

BOOL CUserEntitiesServices::UpdateUserByUsername(CString username,CUser* user )
{
	sqlCommand1.Format(_T("Update [User] set Password='%s', EmailAddress='%s' , DisplayName='%s' where Username='%s'  ; "),
		user->Username(),
		user->Password(),
		user->EmailAddress(),
		user->DisplayName(),
		username
		);

	try
	{

		dal->ExecuteSQL(sqlCommand1);
		return TRUE;
	}
	catch (CException* e)
	{
		return FALSE;
		throw;
		delete e;
	}


}

BOOL CUserEntitiesServices::Login( CString username, CString password )
{

	sqlCommand1.Format(_T("SELECT * FROM [User] where Username='%s' and Password='%s';"),
		username,
		password);

	try
	{
	
		dataUser=dal->GetRecordSet(sqlCommand1);
		
	}
	catch (CException* e)
	{
		throw;
		e->Delete();
		return FALSE;
	}
	int numberRecord=0;

	while (!dataUser->IsEOF())
	{
		numberRecord += 1;
		dataUser->MoveNext();
	}

	if(numberRecord==1)
		 return TRUE;
	else
		 return FALSE;

}

CString CUserEntitiesServices::ChangePassWord( INT userId,CString newpass )
{
	CString oldpass;

	sqlCommand1.Format(_T("Select * from [User] where UserId=%d"),userId);
	sqlCommand2.Format(_T("Update [User] set Password='%s' where UserId=%d  ;"),newpass,userId);


	
	try
	{
		dataUser=dal->GetRecordSet(sqlCommand1);
		if(dataUser==NULL)
			return NULL;
		else
		{

			dataUser->GetFieldValue(_T("Password"),oldpass);
			dal->ExecuteSQL(sqlCommand2);
			return oldpass;
		}

		


	}
	catch (CException* e)
	{
		throw;
		e->Delete();
	}
	

}




CArray<MailHeader,MailHeader&>* CMailHeaderServices::GetAllMail()
{

	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
		
	sqlCommand1.Format(_T("Select * from MailHeader ;"));

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContentType,RealAttachString,MessageId;
	BYTE RealAttach;
	CString UserId,GroupId,MailId;
	MailHeader mailheader;
	
		
	try
	{
		dataMail=dal->GetRecordSet(sqlCommand1);
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
						dataMail->GetFieldValue(_T("SentDate"),Date);
						dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
						dataMail->GetFieldValue(_T("TextBody"),TextBody);
						dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
						dataMail->GetFieldValue(_T("ContentType"),ContentType);

						dataMail->GetFieldValue(_T("RealAttach)"),RealAttachString);
						dataMail->GetFieldValue(_T("GroupId"),GroupId);
						dataMail->GetFieldValue(_T("UserId"),UserId);
						dataMail->GetFieldValue(_T("MailId"),MailId);

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


}

MailHeader* CMailHeaderServices::GetMailByMailId( INT mailId )
{

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,RealAttachString,MessageId,MimeVersion,ContentType;
	BYTE RealAttach;
	
	MailHeader* mailheader=new MailHeader();

	sqlCommand1.Format(_T("SELECT * FROM MailHeader WHERE MailId=%d;"),mailId);

	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	else
		{
		dataMail->GetFieldValue(_T("MessageID"),MessageId);
	 	dataMail->GetFieldValue(_T("From"),From);
		dataMail->GetFieldValue(_T("To"),To);
		dataMail->GetFieldValue(_T("Subject"),Subject);
		dataMail->GetFieldValue(_T("Cc"),Cc);
		dataMail->GetFieldValue(_T("SentDate"),Date);
		dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
		dataMail->GetFieldValue(_T("TextBody"),TextBody);
		dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
		dataMail->GetFieldValue(_T("ContentType"),ContentType);

		dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
		//convert String to BYTE
		
		RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

		mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);
		
		return mailheader;
		

		}




}

CArray<MailHeader,MailHeader&>* CMailHeaderServices::GetMailByUserId( INT userId )
{
	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
		
	sqlCommand1.Format(_T("Select * from MailHeader where UserId=%d ;"),userId);

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContentType,RealAttachString,MessageId;
	BYTE RealAttach;
	CString UserId,GroupId,MailId;
	MailHeader mailheader;
		
	try
	{
		dataMail=dal->GetRecordSet(sqlCommand1);
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
						dataMail->GetFieldValue(_T("SentDate"),Date);
						dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
						dataMail->GetFieldValue(_T("TextBody"),TextBody);
						dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
						dataMail->GetFieldValue(_T("ContentType"),ContentType);

						dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
						dataMail->GetFieldValue(_T("GroupId"),GroupId);
						dataMail->GetFieldValue(_T("UserId"),UserId);
						dataMail->GetFieldValue(_T("MailId"),MailId);

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
//DANG : Đã test cẩn thận , chạy tốt


}

CArray<MailHeader,MailHeader&>* CMailHeaderServices::GetMailByGroupId( INT groupId )
{
	sqlCommand1.Format(_T("Select * FROM MailHeader  where GroupId= %d ;"),groupId);
	
	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
	CRecordset *dataMail;
	

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContentType,RealAttachString,MessageId;
	BYTE RealAttach;
	CString UserId,GroupId,MailId;
	MailHeader mailheader;
		
	try
	{
		dataMail=dal->GetRecordSet(sqlCommand1);
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
						dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
						dataMail->GetFieldValue(_T("ContentType"),ContentType);

						dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
						dataMail->GetFieldValue(_T("GroupId"),GroupId);
						dataMail->GetFieldValue(_T("UserId"),UserId);
						dataMail->GetFieldValue(_T("MailId"),MailId);

						//convert String to BYTE
						RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

						
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

}




CArray<MailHeader,MailHeader&>* CMailHeaderServices::GetMailByUserIdGroupId( INT userId, INT groupId )
{
	sqlCommand1.Format(_T("Select * FROM MailHeader  where UserId= %d and GroupId=%d  ;"),userId,groupId);
	
	CArray<MailHeader,MailHeader&>* listMailHeader=new CArray<MailHeader,MailHeader&>();
	CRecordset *dataMail;
	

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContentType,RealAttachString,MessageId;
	BYTE RealAttach;
	CString UserId,GroupId,MailId;
	MailHeader mailheader;
		
	try
	{
		dataMail=dal->GetRecordSet(sqlCommand1);
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
						dataMail->GetFieldValue(_T("SentDate"),Date);
						dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
						dataMail->GetFieldValue(_T("TextBody"),TextBody);
						dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
						dataMail->GetFieldValue(_T("ContentType"),ContentType);

						dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
						dataMail->GetFieldValue(_T("GroupId"),GroupId);
						dataMail->GetFieldValue(_T("UserId"),UserId);
						dataMail->GetFieldValue(_T("MailId"),MailId);

						//convert String to BYTE
						RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

						//mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);
						 
						 mailheader.From=From;
						 mailheader.To=To;
						 mailheader.Date=Date;
						 mailheader.MessageID=MessageId;
					   	 mailheader.Subject=Subject;
					   	 mailheader.Cc=Cc;
						 mailheader.RealAttach=RealAttach;
						 mailheader.ReplyTo=ReplyTo;
						 mailheader.TextBody=TextBody;
						 
						 mailheader.GroupId=atoi(GroupId);
						 mailheader.UserId=atoi(UserId);
						 mailheader.MailId=atoi(MailId);
						 mailheader.MimeVersion=MimeVersion;
						 mailheader.ContentType=ContentType;
						 

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

}

MailHeader* CMailHeaderServices::InsertNewMail( MailHeader* mailHeader )
{
	
	mailHeader->GroupId = 1;

	sqlCommand1.Format(_T("INSERT INTO MailHeader (MessageID, [From], [To], SentDate,Subject,Cc,ReplyTo,TextBody,MimeVersion,ContentType,RealAttach,UserId,GroupId) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d);"),
		mailHeader->MessageID,
		mailHeader->From,
		mailHeader->To,
		mailHeader->Date.GetString(),
		mailHeader->Subject,
		mailHeader->Cc,
		mailHeader->ReplyTo,
		mailHeader->TextBody,
		mailHeader->MimeVersion,
		mailHeader->ContentType,
		mailHeader->RealAttach,
		mailHeader->UserId,
		mailHeader->GroupId
		);

	try
	{
		if(dal->ExecuteSQL(sqlCommand1))
			return mailHeader;
		else 
			return NULL;
		
	}
	catch (CDBException* e)
	{
		throw;
	}
	


}

CMailHeaderServices::CMailHeaderServices( void )
{
	this->dal = new CDAL();

}
