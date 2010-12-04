// MailUser.cpp : implementation file
//

#include "stdafx.h"
//#include "ODBC6.11.h"
#include "MailUser.h"
#include "DAL.h"
// MailUser

//DAL *dal=new DAL();
CString  sqlMailUser;
CRecordset *datasetMailUser;

//
MailUser::MailUser( CString name,CString pass )
{
	this->_username=name;
	this->_password=pass;
}
MailUser::MailUser()
{
	this->_username=_T("");
	this->_password=_T("");
	dal = new DAL();
}

MailUser::~MailUser()
{
	if (dal != NULL) delete dal;
}


// MailUser message handlers



bool MailUser::InsertNewUser(MailUser *mailuser)
{
	CString name,pass;
	name=mailuser->_username;
	pass=mailuser->_password;
	sqlMailUser.Format(_T("Insert into MailUser(Username,Password) values('%s','%s') ;"),name,pass);

	try
	{
		if(dal->ExecuteSQL(sqlMailUser)) 
			return true;
		else
			 return false;
	}
	catch(CException* e)
	{
		return false;
	}
}
//bool SignIn(CString username,CString password)
//{
//	sqlMailUser.Format(_T("Select * from MailUser where Username='%s' and Password='%s' ;"),username,password);
//
//	datasetMailUser=dal->GetRecordSet(sqlMailUser);
//	int numberRecord=0;
//
//	while (!datasetMailUser->IsEOF())
//	{
//		numberRecord += 1;
//		datasetMailUser->MoveNext();
//	}
//
//	if(numberRecord==1)
//		return TRUE;
//	else
//		return FALSE;
//}

MailUser* MailUser::GetUserByID(UINT userID )
{
	sqlMailUser.Format(_T("Select * from MailUser where UserID=%d"),userID);
	
	try
		{
			datasetMailUser = dal->GetRecordSet(sqlMailUser);
		CString name,pass;
		if(datasetMailUser==NULL) return NULL;
		else
		{
			datasetMailUser->GetFieldValue(_T("Username"),name);
			datasetMailUser->GetFieldValue(_T("Password"),pass);
			return new MailUser(name,pass);
		}	
		
		}
	catch(CException* e)
	{
		throw;
		e->Delete();
	}



}

MailUser* MailUser::GetUserByUsername(CString username)
{
	sqlMailUser.Format(_T("Select * from MailUser where Username='%s'  ;"),username);
	CString name,pass;

	try
	{
		datasetMailUser  = dal->GetRecordSet(sqlMailUser);
	
		if(datasetMailUser==NULL) return NULL;
		else
		{
 			datasetMailUser->GetFieldValue(_T("Username"),name);
			datasetMailUser->GetFieldValue(_T("Password"),pass);
			return new MailUser(name,pass);
		}
	}
	catch(CException* ee)
	{
		throw;
		ee->Delete();
	}
}

bool MailUser::DeleteUser(MailUser *mailuser)
{
	CString name;
	name=mailuser->_username;
	sqlMailUser.Format(_T("Delete From MailUser where UserName='%s'"),name);
	try
	{
		if(dal->ExecuteSQL(sqlMailUser))
			return true;
		else return false;
			
	}
	catch(CException* e)
	{
		throw;
		e->Delete();
	}
}
bool MailUser::DeleteUserByID(UINT userid)
{
	sqlMailUser.Format(_T("Delete From MailUser where UserID=%d; "),userid);

		try
		{
		
		 if(dal->ExecuteSQL(sqlMailUser))
			 return true;
		 else return false;
		}
		catch(CException* e)
		{
			throw;
			e->Delete();
		}

}
//Used to change Password
//bool MailUser::ChangePassword(CString username,CString password)
//{
//	sqlMailUser.Format(_T("Update MailUser set Password='%s' where Username='%s'"),password,username);
//
//	try
//		{
//			if(dal->ExecuteSQL(sqlMailUser))
//				return true;
//			else
//				 return false;
//		}
//	catch(CException* e)
//		{
//		throw;
//		e->Delete();
//		}
//}

bool MailUser::UpdateUserByID(UINT userID,CString name,CString password)
{
	sqlMailUser.Format(_T("Update MailUser set Username='%s' , Password='%s'  where UserId=%d"),password,name,userID);

	try
		{
			if(dal->ExecuteSQL(sqlMailUser))
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

CArray<MailUser,MailUser&>* MailUser::GetAllUsers()
{
	CArray<MailUser,MailUser&>* listMailUser=new CArray<MailUser,MailUser&>();
	CString username,password;
	MailUser* mailuser;
	sqlMailUser.Format("Select * from MailUser ;");
	try
				{
						
					datasetMailUser=dal->GetRecordSet(sqlMailUser);

					if(datasetMailUser==NULL) return NULL;

					else
					{
						int numberRecord=0;

						while (!datasetMailUser->IsEOF())
						{
							numberRecord += 1;
							datasetMailUser->MoveNext();
						}

						listMailUser->SetSize(numberRecord);

						while(!datasetMailUser->IsEOF())
						{

							//datasetMailUser->GetFieldValue(_T("MessageId",MessageId);
							datasetMailUser->GetFieldValue(_T("username"),username);
							datasetMailUser->GetFieldValue(_T("Password"),password);

							mailuser= new MailUser(username,password);
							listMailUser->Add(*mailuser);
							datasetMailUser->MoveNext();
						}
						return listMailUser;
					}
				}
			catch(CException* ee)
				{
					throw;
					ee->Delete();
				}
}

bool MailUser::UpdateUserByUsername(CString oldUsername,MailUser* mailUser)
{
	sqlMailUser.Format(_T("Update MailUser set UserName='%s' , Password='%s'  where UserName='%s'"),
		mailUser->_username,
		mailUser->_password,

		oldUsername);

	try
	{
			if(dal->ExecuteSQL(sqlMailUser))
				return true;
			else return false;
	}
	catch(CException* e)
	{
		throw;
		e->Delete();

	}
}