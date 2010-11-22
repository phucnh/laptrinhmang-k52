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
};

public class CAddressBookServices
{
private:
	CDAL *dal;
	CString sqlCommand;
};
