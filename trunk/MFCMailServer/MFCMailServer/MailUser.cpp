// MailUser.cpp : implementation file
//

#include "stdafx.h"

#include "MailUser.h"
#include "DAL.h"
// MailUser


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
			return TRUE;
		else
			 return false;
	}
	catch(CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		return FALSE;
	}
}


MailUser* MailUser::GetUserByID(UINT userID )
{
	sqlMailUser.Format(_T("Select * from MailUser where UserID=%d"),userID);
	
	try
		{
			datasetMailUser = dal->GetRecordSet(sqlMailUser);
		CString name,pass;
		if(datasetMailUser==NULL) return NULL;

		if (datasetMailUser->GetRecordCount() == 0)	return NULL;

		if (datasetMailUser->GetRecordCount() == 0) return NULL;
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
		
		if (datasetMailUser->GetRecordCount() == 0)	return NULL;

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
	MailUser mailuser;
	sqlMailUser.Format("Select * from MailUser ;");
	try
				{
						
					datasetMailUser=dal->GetRecordSet(sqlMailUser);

					if ((datasetMailUser==NULL) || (datasetMailUser->GetRecordCount()==0) ) return NULL;

					else
					{

						while(!datasetMailUser->IsEOF())
						{


							
							
							datasetMailUser->GetFieldValue(_T("Username"),username);

							datasetMailUser->GetFieldValue(_T("Password"),password);

							mailuser._username=username;
							mailuser._password=password;

							listMailUser->Add(mailuser);
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

bool MailUser::CheckUsernameExist( CString username )
{

	sqlMailUser.Format(_T("Select * from MailUser where Username='%s';"),username);
	try
	{

		datasetMailUser=dal->GetRecordSet(sqlMailUser);

		if(datasetMailUser==NULL) return true;
		else
		{
			int numberRecord;
			while(!datasetMailUser->IsEOF())
			{
				numberRecord++;

				datasetMailUser->MoveNext();
			}
			 if(numberRecord>=1)
				 return true;
			 else 
				 return false;
			
		}
	}
	catch(CException* e)
	{
		throw;
		e->Delete();
		return false;
	}
	
}

INT MailUser::getIdFromUsername( CString username )
{
	
	INT UserId;
	CString UserIDString;
	sqlMailUser.Format(_T("Select * FROM MailUser where Username= '%s' ;"),username);
	try
	{

		datasetMailUser=dal->GetRecordSet(sqlMailUser);

		if(datasetMailUser==NULL)
			return NULL;
		else
		{
			datasetMailUser->GetFieldValue(_T("UserId"),UserIDString);
				UserId=atoi(UserIDString.GetString());
				return UserId;	
			

		}
	}
	catch(CException* e)
	{
		e->ReportError();
		e->Delete();
		
	}
}
