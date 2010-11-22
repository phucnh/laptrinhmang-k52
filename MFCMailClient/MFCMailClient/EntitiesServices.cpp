#include "StdAfx.h"
#include "EntitiesServices.h"

CUserEntitiesServices::CUserEntitiesServices(void)
{
}

CUser* CUserEntitiesServices::InsertNewUser( CUser* user )
{
	if (user == NULL)	return NULL;

	sqlCommand.Format(_T("INSERT INTO [User] (Username, Password, EmailAddress, DisplayName) VALUES (%s,%s,%s,%s)"),
		user->Username(),
		user->Password(),
		user->EmailAddress(),
		user->DisplayName()
		);

	try
	{
		dal->ExecuteSQL(sqlCommand);
	}
	catch (CException* e)
	{
		throw;
		delete e;
	}

	return user;
}

CUser* CUserEntitiesServices::GetByUserId( INT userId )
{

}