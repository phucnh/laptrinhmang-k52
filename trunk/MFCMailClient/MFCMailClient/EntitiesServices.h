#include "DAL.h"
#include "MailMessage.h"
#pragma once
#include "User.h"

public class CUserEntitiesServices
{
private:
	CDAL* dal;
	CRecordset* dataUser;
	CString sqlCommand1;
	CString sqlCommand2;
public:
	CUserEntitiesServices(void);

	CUser* InsertNewUser(CUser* user);
	CUser* GetByUserId(INT userId);
	CUser* GetByUsername(CString username);
	CUser* DeleteUserById(INT userId);
	CUser* DeleteUserByUsername(CString username);
	BOOL UpdateUserById(INT userId,CUser* user);
	BOOL UpdateUserByUsername(CString username,CUser* user);

	BOOL Login(CString username, CString password);
	CString ChangePassWord(INT userId,CString newpass);

};

public class CMailHeaderServices
{
private:
	CDAL *dal;
	CRecordset *dataMail;
	CString sqlCommand1;
	CString sqlCommand2;

public:
	CMailHeaderServices(void);

	CArray<MailHeader,MailHeader&>* GetAllMail();
	MailHeader* GetByMailId(INT mailId);
	CArray<MailHeader,MailHeader&>* GetByUserId(INT userId);
	CArray<MailHeader,MailHeader&>* GetByGroupId(INT groupId);
	CArray<MailHeader,MailHeader&>* GetByUserIdGroupId(INT userId, INT groupId);
	MailHeader* InsertNewMail(MailHeader* mailHeader);
};

public class CAddressBookServices
{
private:
	CDAL *dal;
	CString sqlCommand1;
	CString sqlCommand2;


public:
	CArray<CAddressBook,CAddressBook&>* GetByUserId(INT userId);
};

public class CGroupServices
{
private:
	CDAL *dal;
	CString sqlCommand1;
	CString sqlCommand2;
public:
	CGroup* GetByGroupId(INT groupId);
};
