#include "StdAfx.h"
#include "EntitiesServices.h"

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

	if(dataUser==NULL) return NULL;	
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
	sqlCommand2.Format(_T("Delete from User where UserId=%d"),userID);

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


		dataUser=dal->GetRecordSet(sqlCommand1);
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
	CArray<MailHeader,MailHeader&>* listMailHeader;
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttachString;
	BYTE RealAttach;
	INT UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail;"));
	try
	{
	
		dataMail=dal->GetRecordSet(sqlCommand1);
		if(dataMail==NULL) return NULL;
	
			else
				{
					int numberRecord=0;
					
					while (!dataMail->IsEOF())
					{
						numberRecord += 1;
						dataMail->MoveNext();
					}

					listMailHeader->SetSize(numberRecord);

					while(!dataMail->IsEOF())
					{
							
						//dataMail->GetFieldValue(_T("MessageId",MessageId);
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

						mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);

						listMailHeader->Add(*mailheader);


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

MailHeader* CMailHeaderServices::GetByMailId( INT mailId )
{

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,RealAttachString;
	BYTE RealAttach;
	
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
	/*	dataMail->GetFieldValue(_T("MimeVersion",MimeVersion);
		dataMail->GetFieldValue(_T("ContendType",ContendType);*/

		dataMail->GetFieldValue(_T("RealAttach"),RealAttachString);
		//convert String to BYTE
		
		RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

		mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);
		
		return mailheader;
		

		}




}

CArray<MailHeader,MailHeader&>* CMailHeaderServices::GetByUserId( INT userId )
{
	CArray<MailHeader,MailHeader&>* listMailHeader;
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttachString;
	BYTE RealAttach;
	INT UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("SELECT * FROM Mail where UserId=%d ;"),userId);
	
	try
	{
	
			dataMail=dal->GetRecordSet(sqlCommand1);
			if(dataMail==NULL) return NULL;
			else
			{
						int numberRecord=0;

						while (!dataMail->IsEOF())
						{
							numberRecord += 1;
							dataMail->MoveNext();
						}

						listMailHeader->SetSize(numberRecord);
		
						while(!dataMail->IsEOF())
						{
								
							//dataMail->GetFieldValue(_T("MessageId",MessageId);
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
							
							RealAttach=(BYTE)(LPSTR)(LPCTSTR)RealAttachString;

							mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);

							listMailHeader->Add(*mailheader);

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

CArray<MailHeader,MailHeader&>* CMailHeaderServices::GetByGroupId( INT groupId )
{
	CArray<MailHeader,MailHeader&>* listMailHeader;
	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttachString;
	BYTE RealAttach;
	INT UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("Select * From Mail where GroupId=%d;"),groupId);
	
	try
	{
				dataMail=dal->GetRecordSet(sqlCommand1);
			if(dataMail==NULL) return NULL;
			else
			{
				int numberRecord=0;

				while (!dataMail->IsEOF())
				{
					numberRecord += 1;
					dataMail->MoveNext();
				}

				listMailHeader->SetSize(numberRecord);
						
						while(!dataMail->IsEOF())
						{
								
							//dataMail->GetFieldValue(_T("MessageId",MessageId);
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

							mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);

							listMailHeader->Add(*mailheader);
							
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




CArray<MailHeader,MailHeader&>* CMailHeaderServices::GetByUserIdGroupId( INT userId, INT groupId )
{
	CArray<MailHeader,MailHeader&>* listMailHeader;

	CString  From,To, Subject,Cc, Date,ReplyTo,TextBody,MimeVersion,ContendType,RealAttachString;
	BYTE RealAttach;
	INT UserId,GroupId,MailId,MessageId;
	MailHeader* mailheader;

	sqlCommand1.Format(_T("Select * From Mail where GroupId=%d and UserId=%d ;"),groupId,userId);
	
	try
	{
				dataMail=dal->GetRecordSet(sqlCommand1);
			if(dataMail==NULL) return NULL;
			else
			{
				int numberRecord=0;

				while (!dataMail->IsEOF())
				{
					numberRecord += 1;
					dataMail->MoveNext();
				}

				listMailHeader->SetSize(numberRecord);
						
						while(!dataMail->IsEOF())
						{
								
							//dataMail->GetFieldValue(_T("MessageId",MessageId);
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

							mailheader=new MailHeader(From,To,Date,Subject,Cc,ReplyTo,TextBody,RealAttach);

							listMailHeader->Add(*mailheader);

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
	MailHeader* _returnMailHeader = new MailHeader();
	mailHeader->GroupId = 1;

	sqlCommand1.Format(_T("INSERT INTO MailHeader (MessageID, [From], To, SentDate,Subject,Cc,ReplyTo,TextBody,MimeVersion,ContentType,RealAttach,UserId,GroupId) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d);"),
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
		dal->ExecuteSQL(sqlCommand1);
	}
	catch (CDBException* e)
	{
		throw;
	}
	

	return _returnMailHeader;
}

CMailHeaderServices::CMailHeaderServices( void )
{
	this->dal = new CDAL();

}