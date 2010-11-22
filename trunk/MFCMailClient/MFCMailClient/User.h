#pragma once

public class CUser
{
private:
	INT userId;
	CString username;
	CString password;
	CString emailAddress;
	CString displayName;
	
public:
	CUser(void);

	INT UserId() const { return userId; }
	void UserId(INT val) { userId = val; }
	CString Username() const { return username; }
	void Username(CString val) { username = val; }
	CString Password() const { return password; }
	void Password(CString val) { password = val; }
	CString EmailAddress() const { return emailAddress; }
	void EmailAddress(CString val) { emailAddress = val; }
	CString DisplayName() const { return displayName; }
	void DisplayName(CString val) { displayName = val; }
};

public class CAddressBook
{
private:
	INT userId;
	INT friendId;
	
public:
	CAddressBook(void);

	INT UserId() const { return userId; }
	void UserId(INT val) { userId = val; }
	INT FriendId() const { return friendId; }
	void FriendId(INT val) { friendId = val; }
};