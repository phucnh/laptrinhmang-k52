#pragma once


// MailUser

class MailUser : public CWnd
{
	DECLARE_DYNAMIC(MailUser)

public:
	CString _username;
	CString _password;

	MailUser(CString name,CString pass);
	MailUser();
	virtual ~MailUser();
	bool InsertNewUser(MailUser *mailuser);
	bool SignIn(CString username,CString password);
	MailUser* GetUserByID(UINT userID);
	MailUser* GetUserByUsername(CString username);
	bool DeleteUser(MailUser *mailuser);
	bool DeleteUserByID(UINT id);
	bool ChangePassword(CString username,CString password);
	bool UpdateUserByID(UINT userID,CString name,CString password);


protected:
	DECLARE_MESSAGE_MAP()

};

