#include "StdAfx.h"
#include "EntitiesServices.h"

#pragma region "CUserServices"
CUserEntitiesServices::CUserEntitiesServices(void)
{
}

CUser* CUserEntitiesServices::InsertNewUser( CUser* user )
{	
	CString userid,username,password,emailaddress,displayname;

	CUser* userreturn;
	if (user == NULL)	return NULL;

	sqlCommand1.Format(_T("INSERT INTO [User] (Username, Password, EmailAddress, DisplayName) VALUES (%s,%s,%s,%s)"),
		user->Username(),
		user->Password(),
		user->EmailAddress(),
		user->DisplayName()
		);
	sqlCommand2.Format(_T("SELECT * From [User] where Username='%s' and EmailAddress='%s'"),
		user->Username(),
		user->EmailAddress());
	
	try
	{	
		
		dal->ExecuteSQL(sqlCommand1);

		dataUser=dal->GetRecordSet(sqlCommand2);
		dataUser->GetFieldValue(_T("UserID"),userid);
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
	userreturn->UserId(atoi(userid));
	userreturn->Password(password);
	userreturn->EmailAddress(emailaddress);
	userreturn->DisplayName(displayname);

	return userreturn;
}

CUser* CUserEntitiesServices::GetByUserId( INT userId )
{
	CUser *userreturn;
	CString userid,username,password,emailaddress,displayname;
	
	sqlCommand2.Format(_T("SELECT * From [User] where UserID=%d"),userId);
	
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

			
			dataUser=dal->GetRecordSet(sqlCommand2);
			dataUser->GetFieldValue(_T("UserID"),userid);
			dataUser->GetFieldValue(_T("Username"),username);
			dataUser->GetFieldValue(_T("Password"),password);
			dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
			dataUser->GetFieldValue(_T("DisplayName"),displayname);
			
			return userreturn;

			}

	}

CUser* CUserEntitiesServices::GetByUsername( CString _username )
{
	CUser *userreturn;
	CString userid,username,password,emailaddress,displayname;

	sqlCommand2.Format(_T("SELECT * From [User] where Username='%s';"),_username);

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


		dataUser=dal->GetRecordSet(sqlCommand2);
		dataUser->GetFieldValue(_T("UserID"),userid);
		dataUser->GetFieldValue(_T("Username"),username);
		dataUser->GetFieldValue(_T("Password"),password);
		dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
		dataUser->GetFieldValue(_T("DisplayName"),displayname);

		return userreturn;

	}

}

CUser* CUserEntitiesServices::DeleteUserById( INT userID )
{

	CUser *userreturn;
	CString userid,username,password,emailaddress,displayname;

	sqlCommand1.Format(_T("SELECT * From [User] where UserID=%d ;"),userID);
	sqlCommand2.Format(_T("Delete from [User] where UserID=%d"),userID);

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


		dataUser=dal->GetRecordSet(sqlCommand2);
		dataUser->GetFieldValue(_T("UserID"),userid);
		dataUser->GetFieldValue(_T("Username"),username);
		dataUser->GetFieldValue(_T("Password"),password);
		dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
		dataUser->GetFieldValue(_T("DisplayName"),displayname);
		//delete User
		dal->ExecuteSQL(sqlCommand2);

		return userreturn;

	}

}

CUser* CUserEntitiesServices::DeleteUserByUsername( CString _username )
{
	CUser *userreturn;
	CString userid,username,password,emailaddress,displayname;

	sqlCommand1.Format(_T("SELECT * From [User] where Username='%s' ;"),_username);
	sqlCommand2.Format(_T("Delete from [User] where UserID='%s' ;"),_username);

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


		
		dataUser->GetFieldValue(_T("UserID"),userid);
		dataUser->GetFieldValue(_T("Username"),username);
		dataUser->GetFieldValue(_T("Password"),password);
		dataUser->GetFieldValue(_T("EmailAddress"),emailaddress);
		dataUser->GetFieldValue(_T("DisplayName"),displayname);
		//delete User
		dal->ExecuteSQL(sqlCommand2);

		return userreturn;

	}

}

BOOL CUserEntitiesServices::UpdateUserById(INT userId,CUser* user )
{

	sqlCommand1.Format(_T("Update [User] set Username='%s' , Password='%s', EmailAddess='%s' , DisplayName='%s' where UserID=%d ;"),
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
	sqlCommand1.Format(_T("Update [User] set Password='%s', EmailAddess='%s' , DisplayName='%s' where Username='%s'; "),
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

	sqlCommand1.Format(_T("SELECT * FROM [User] where Usernam='%s' and Password='%s';"),
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
	if(dataUser->GetRecordCount()==1)
		 return TRUE;
	else
		 return FALSE;

}

CString* CUserEntitiesServices::ChangePassWord( INT userId,CString newpass )
{
	CString oldpass;

	sqlCommand1.Format(_T("Select * from [User] where UserId=%d"),userId);
	sqlCommand2.Format(_T("Update [User] set Password='%s' where UserId=%d"),newpass,userId);

	try
	{
		dataUser = dal->GetRecordSet(sqlCommand1);
		if(dataUser == NULL)
			return NULL;
		else
		{
			dataUser->GetFieldValue(_T("Password"),oldpass);
			dal->ExecuteSQL(sqlCommand2);
		}
		return &oldpass;
	}
	catch (CException* e)
	{
		throw;
		e->Delete();
	}
	

}
#pragma endregion "CUserServices"

#pragma region "MailHeaderServices"
CArray<MailHeader,MailHeader>* CMailHeaderServices::GetAllMail()
{
	CArray<MailHeader,MailHeader>* listMailHeader = new CArray<MailHeader,MailHeader>();
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttach,UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail;"));
	dataMail = dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	
	while(!dataMail->IsEOF())
	{
		dataMail->GetFieldValue(_T("MessageId"),MessageId);
		dataMail->GetFieldValue(_T("From"),From);
		dataMail->GetFieldValue(_T("To"),To);
		dataMail->GetFieldValue(_T("Subject"),Subject);
		dataMail->GetFieldValue(_T("Cc"),Cc);
		dataMail->GetFieldValue(_T("Date"),Date);
		dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
		dataMail->GetFieldValue(_T("TextBody"),TextBody);
		dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
		dataMail->GetFieldValue(_T("ContendType"),ContendType);
		dataMail->GetFieldValue(_T("RealAttach"),RealAttach);
		dataMail->GetFieldValue(_T("GroupId"),GroupId);
		dataMail->GetFieldValue(_T("UserId"),UserId);
		dataMail->GetFieldValue(_T("MailId"),MailId);

		mailheader = new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,atoi(RealAttach));

		listMailHeader->Add(*mailheader);
	}

	return listMailHeader;
}

MailHeader* CMailHeaderServices::GetByMailId( INT mailId )
{

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,RealAttach,MimeVersion,ContentType;
	
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail WHERE MailID=%d;"),mailId);
	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	else
		{
	 	dataMail->GetFieldValue(_T("From"),From);
		dataMail->GetFieldValue(_T("To"),To);
		dataMail->GetFieldValue(_T("Subject"),Subject);
		dataMail->GetFieldValue(_T("Cc"),Cc);
		dataMail->GetFieldValue(_T("Date"),Date);
		dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
		dataMail->GetFieldValue(_T("TextBody"),TextBody);
		dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
		dataMail->GetFieldValue(_T("ContendType"),ContentType);

		dataMail->GetFieldValue(_T("RealAttach"),RealAttach);

		mailheader = new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,atoi(RealAttach));
		
		return mailheader;
		}
}

CArray<MailHeader,MailHeader>* CMailHeaderServices::GetByUserId( INT userId )
{
	CArray<MailHeader,MailHeader>* listMailHeader = new CArray<MailHeader,MailHeader>();
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContentType,RealAttach,UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail where USerId=%d ;"),userId);
	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	
	while(!dataMail->IsEOF())
	{
			
		dataMail->GetFieldValue(_T("MessageId"),MessageId);
		dataMail->GetFieldValue(_T("From"),From);
		dataMail->GetFieldValue(_T("To"),To);
		dataMail->GetFieldValue(_T("Subject"),Subject);
		dataMail->GetFieldValue(_T("Cc"),Cc);
		dataMail->GetFieldValue(_T("Date"),Date);
		dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
		dataMail->GetFieldValue(_T("TextBody"),TextBody);
		dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
		dataMail->GetFieldValue(_T("ContendType"),ContentType);

		dataMail->GetFieldValue(_T("RealAttach"),RealAttach);
		dataMail->GetFieldValue(_T("GroupId"),GroupId);
		dataMail->GetFieldValue(_T("UserId"),UserId);
		dataMail->GetFieldValue(_T("MailId"),MailId);

		mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,atoi(RealAttach));

		listMailHeader->Add(*mailheader);
	}

	return listMailHeader;
}

CArray<MailHeader,MailHeader>* CMailHeaderServices::GetByGroupId( INT groupId )
{
	CArray<MailHeader,MailHeader>* listMailHeader = new CArray<MailHeader,MailHeader>();
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttach,UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail where GroupId=%d;"),groupId);
	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	
	while(!dataMail->IsEOF())
	{
			
		dataMail->GetFieldValue(_T("MessageId"),MessageId);
		dataMail->GetFieldValue(_T("From"),From);
		dataMail->GetFieldValue(_T("To"),To);
		dataMail->GetFieldValue(_T("Subject"),Subject);
		dataMail->GetFieldValue(_T("Cc"),Cc);
		dataMail->GetFieldValue(_T("Date"),Date);
		dataMail->GetFieldValue(_T("ReplyTo"),ReplyTo);
		dataMail->GetFieldValue(_T("TextBody"),TextBody);
		dataMail->GetFieldValue(_T("MimeVersion"),MimeVersion);
		dataMail->GetFieldValue(_T("ContendType"),ContendType);

		dataMail->GetFieldValue(_T("RealAttach"),RealAttach);
		dataMail->GetFieldValue(_T("GroupId"),GroupId);
		dataMail->GetFieldValue(_T("UserId"),UserId);
		dataMail->GetFieldValue(_T("MailId"),MailId);

		mailheader = new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,atoi(RealAttach));

		listMailHeader->Add(*mailheader);
	}

	return listMailHeader;
}

#pragma endregion "MailHeaderServices"
