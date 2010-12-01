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
	dal=new DAL();
}

MailUser::~MailUser()
{
	dal=new DAL();
}


// MailUser message handlers



bool MailUser::InsertNewUser(MailUser *mailuser)
{
	CString name,pass;
	name=mailuser->_username;
	pass=mailuser->_password;
	sqlMailUser.Format(_T("Insert into MailUser values ('%s','%s')"),name,pass);

	return dal->ExecuteSQL(sqlMailUser);
}
bool SignIn(CString username,CString password)
{
	sqlMailUser.Format(_T("Select * from MailUser where Username='%s' and Password='%s'"),username,password);

	datasetMailUser=dal->GetRecordSet(sqlMailUser);
	LONG a=datasetMailUser->GetRecordCount();
	if(a==1)
		return true;
	else 
		return false;
}

MailUser* MailUser::GetUserByID(UINT userID )
{
	sqlMailUser.Format(_T("Select * from MailUser where UserID=%d"),userID);
	datasetMailUser = dal->GetRecordSet(sqlMailUser);
	CString name,pass;
	datasetMailUser->GetFieldValue(_T("Name"),name);
	datasetMailUser->GetFieldValue(_T("Password"),pass);
	return new MailUser(name,pass);



}

MailUser* MailUser::GetUserByUsername(CString username)
{
	sqlMailUser.Format(_T("Select * from MailUser where UserName='%s'"),username);
	datasetMailUser  = dal->GetRecordSet(sqlMailUser);
	CString name,pass;
	datasetMailUser->GetFieldValue(_T("Name"),name);
	datasetMailUser->GetFieldValue(_T("Password"),pass);
	return new MailUser(name,pass);
}

bool MailUser::DeleteUser(MailUser *mailuser)
{
	CString name;
	name=mailuser->_username;
	sqlMailUser.Format(_T("Delete From MailUser where UserName='%s'"),name);
	return dal->ExecuteSQL(sqlMailUser);
}
bool MailUser::DeleteUserByID(UINT userid)
{
	sqlMailUser.Format(_T("Delete From MailUser where UserID=%d; "),userid);

	return dal->ExecuteSQL(sqlMailUser);

}
//Used to change Password
bool MailUser::ChangePassword(CString username,CString password)
{
	sqlMailUser.Format(_T("Update MailUser set Password='%s' where UserName='%s'"),password,username);

	return dal->ExecuteSQL(sqlMailUser);
}

bool MailUser::UpdateUserByID(UINT userID,CString name,CString password)
{
	sqlMailUser.Format(_T("Update MailUser set UserName='%s' , Password='%s'  where UserID=%d"),password,name,userID);

	return dal->ExecuteSQL(sqlMailUser);
}

CArray<MailUser,MailUser>* MailUser::GetAllUsers()
{
	CArray<MailUser,MailUser>* mailUsers;

	// TODO : Implement GetAllUser in here

	return mailUsers;
}

bool MailUser::UpdateUserByUsername( CString oldUsername, CString newUsername, CString newPassword )
{
	sqlMailUser.Format(_T("Update MailUser set UserName='%s' , Password='%s'  where UserName='%s'"),
		newUsername,
		newPassword,
		oldUsername);

	return dal->ExecuteSQL(sqlMailUser);
}