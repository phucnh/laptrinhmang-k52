#include "StdAfx.h"
#include "EntitiesServices.h"

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
		dataUser->GetFieldValue(L"UserID",userid);
		dataUser->GetFieldValue(L"Username",username);
		dataUser->GetFieldValue(L"Password",password);
		dataUser->GetFieldValue(L"EmailAddress",emailaddress);
		dataUser->GetFieldValue(L"DisplayName",displayname);



	}
	catch (CException* e)
	{
		throw;
		delete e;
	}

	userreturn->Username(username)
	userreturn->UserId(userid);
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
			dataUser->GetFieldValue(L"UserID",userid);
			dataUser->GetFieldValue(L"Username",username);
			dataUser->GetFieldValue(L"Password",password);
			dataUser->GetFieldValue(L"EmailAddress",emailaddress);
			dataUser->GetFieldValue(L"DisplayName",displayname);
			
			return userreturn;

			}

	}

CUser* CUserEntitiesServices::GetByUsername( CString username )
{
	CUser *userreturn;
	CString userid,username,password,emailaddress,displayname;

	sqlCommand2.Format(_T("SELECT * From [User] where Username='%s';"),username);

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
		dataUser->GetFieldValue(L"UserID",userid);
		dataUser->GetFieldValue(L"Username",username);
		dataUser->GetFieldValue(L"Password",password);
		dataUser->GetFieldValue(L"EmailAddress",emailaddress);
		dataUser->GetFieldValue(L"DisplayName",displayname);

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
		dataUser->GetFieldValue(L"UserID",userid);
		dataUser->GetFieldValue(L"Username",username);
		dataUser->GetFieldValue(L"Password",password);
		dataUser->GetFieldValue(L"EmailAddress",emailaddress);
		dataUser->GetFieldValue(L"DisplayName",displayname);
		//delete User
		dal->ExecuteSQL(sqlCommand2);

		return userreturn;

	}

}

CUser* CUserEntitiesServices::DeleteUserByUsername( CString username )
{
	CUser *userreturn;
	CString userid,username,password,emailaddress,displayname;

	sqlCommand1.Format(_T("SELECT * From [User] where Username='%s' ;"),username);
	sqlCommand2.Format(_T("Delete from [User] where UserID='%s' ;"),username);

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


		
		dataUser->GetFieldValue(L"UserID",userid);
		dataUser->GetFieldValue(L"Username",username);
		dataUser->GetFieldValue(L"Password",password);
		dataUser->GetFieldValue(L"EmailAddress",emailaddress);
		dataUser->GetFieldValue(L"DisplayName",displayname);
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
	CString* oldpass;

	sqlCommand1.Format(_T("Select * from [User] where UserId=%d"),userId);
	sqlCommand2.Format(_T("Update [User] set Password='%s' where UserId=%d"),newpass,userId);


	
	try
	{
		dataUser=dal->GetRecordSet(sqlCommand1);
		if(dataUser==NULL)
			return NULL;
		else
		{

			dataUser->GetFieldValue(L"Password",oldpass);
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




CArray<MailHeader,&MailHeader>* CMailHeaderServices::GetAllMail()
{
	CArray<MailHeader,&MailHeader> listMailHeader;
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType;
	BYTE RealAttach;
	INT UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail;"));
	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	
	while(!dataMail->IsEOF())
	{
			
		//dataMail->GetFieldValue(L"MessageId",MessageId);
		dataMail->GetFieldValue(L"From",From);
		dataMail->GetFieldValue(L"To",To);
		dataMail->GetFieldValue(L"Subject",Subject);
		dataMail->GetFieldValue(L"Cc",Cc);
		dataMail->GetFieldValue(L"Date",Date);
		dataMail->GetFieldValue(L"ReplyTo",ReplyTo);
		dataMail->GetFieldValue(L"TextBody",TextBody);
	/*	dataMail->GetFieldValue(L"MimeVersion",MimeVersion);
		dataMail->GetFieldValue(L"ContendType",ContendType);*/

		dataMail->GetFieldValue(L"RealAttach",RealAttach);
	/*	dataMail->GetFieldValue(L"GroupId",GroupId);
		dataMail->GetFieldValue(L"UserId",UserId);
		dataMail->GetFieldValue(L"MailId",MailId);*/

		mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);

		listMailHeader.Add(mailheader);

		

	}

	return listMailHeader;



}

MailHeader* CMailHeaderServices::GetByMailId( INT mailId )
{

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody;
	BYTE RealAttach;
	
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail WHERE MailID=%d;"),mailId);
	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	else
		{
	 	dataMail->GetFieldValue(L"From",From);
		dataMail->GetFieldValue(L"To",To);
		dataMail->GetFieldValue(L"Subject",Subject);
		dataMail->GetFieldValue(L"Cc",Cc);
		dataMail->GetFieldValue(L"Date",Date);
		dataMail->GetFieldValue(L"ReplyTo",ReplyTo);
		dataMail->GetFieldValue(L"TextBody",TextBody);
	/*	dataMail->GetFieldValue(L"MimeVersion",MimeVersion);
		dataMail->GetFieldValue(L"ContendType",ContendType);*/

		dataMail->GetFieldValue(L"RealAttach",RealAttach);

		mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);
		
		return mailheader;
		

		}




}

CArray<MailHeader,&MailHeader>* CMailHeaderServices::GetByUserId( INT userId )
{
	CArray<MailHeader,&MailHeader> listMailHeader;
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType;
	BYTE RealAttach;
	INT UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail where USerId=%d ;"),userId);
	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	
	while(!dataMail->IsEOF())
	{
			
		//dataMail->GetFieldValue(L"MessageId",MessageId);
		dataMail->GetFieldValue(L"From",From);
		dataMail->GetFieldValue(L"To",To);
		dataMail->GetFieldValue(L"Subject",Subject);
		dataMail->GetFieldValue(L"Cc",Cc);
		dataMail->GetFieldValue(L"Date",Date);
		dataMail->GetFieldValue(L"ReplyTo",ReplyTo);
		dataMail->GetFieldValue(L"TextBody",TextBody);
	/*	dataMail->GetFieldValue(L"MimeVersion",MimeVersion);
		dataMail->GetFieldValue(L"ContendType",ContendType);*/

		dataMail->GetFieldValue(L"RealAttach",RealAttach);
	/*	dataMail->GetFieldValue(L"GroupId",GroupId);
		dataMail->GetFieldValue(L"UserId",UserId);
		dataMail->GetFieldValue(L"MailId",MailId);*/

		mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);

		listMailHeader.Add(mailheader);

		

	}
	return listMailHeader;


}

CArray<MailHeader,&MailHeader>* CMailHeaderServices::GetByGroupId( INT groupId )
{
	CArray<MailHeader,&MailHeader> listMailHeader;
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType;
	BYTE RealAttach;
	INT UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail where GroupId=%d;"),groupId);
	dataMail=dal->GetRecordSet(sqlCommand1);
	if(dataMail==NULL) return NULL;
	
	while(!dataMail->IsEOF())
	{
			
		//dataMail->GetFieldValue(L"MessageId",MessageId);
		dataMail->GetFieldValue(L"From",From);
		dataMail->GetFieldValue(L"To",To);
		dataMail->GetFieldValue(L"Subject",Subject);
		dataMail->GetFieldValue(L"Cc",Cc);
		dataMail->GetFieldValue(L"Date",Date);
		dataMail->GetFieldValue(L"ReplyTo",ReplyTo);
		dataMail->GetFieldValue(L"TextBody",TextBody);
	/*	dataMail->GetFieldValue(L"MimeVersion",MimeVersion);
		dataMail->GetFieldValue(L"ContendType",ContendType);*/

		dataMail->GetFieldValue(L"RealAttach",RealAttach);
	/*	dataMail->GetFieldValue(L"GroupId",GroupId);
		dataMail->GetFieldValue(L"UserId",UserId);
		dataMail->GetFieldValue(L"MailId",MailId);*/

		mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);

		listMailHeader.Add(mailheader);

		

	}
	return listMailHeader;

}




CArray<MailHeader,&MailHeader>* CMailHeaderServices::GetByUserIdGroupId( INT userId, INT groupId )
{

}
