#include "DAL.h"

#pragma once
#include "User.h"

public class CUserEntitiesServices
{
private:
	CDAL* dal;
	CString sqlCommand;
public:
	CUserEntitiesServices(void);

	CUser* InsertNewUser(CUser* user);
	CUser* GetByUserId(INT userId);
	CUser* GetByUsername(CString username);
	CUser* DeleteUserById(INT userId);
	CUser* DeleteUserByUsername(CString username);
	BOOL UpdateUserById(INT userId);
	BOOL UpdateUserByUsername(CString username);

	BOOL Login(CString username, CString password);
	CString* ChangePassWord(INT userId);

};

public class CMailHeaderServices
{
private:
	CDAL *dal;
	CString sqlCommand;

public:
	CArray<MailHeader,&MailHeader>* GetAllMail();
	MailHeader GetByMailId(INT mailId);
	CArray<MailHeader,&MailHeader>* GetByUserId(INT userId);
	CArray<MailHeader,&MailHeader>* GetByGroupId(INT groupId);
	CArray<MailHeader,&MailHeader>* GetByUserIdGroupId(INT userId, INT groupId);
};

public class CAddressBookServices
{
private:
	CDAL *dal;
	CString sqlCommand;

public:
	CArray<CAddressBook,&CAddressBook>* GetByUserId(INT userId);
};

public class CGroupServices
{
private:
	CDAL *dal;
	CString sqlCommand;
public:
	CGroup* GetByGroupId(INT groupId);
};
