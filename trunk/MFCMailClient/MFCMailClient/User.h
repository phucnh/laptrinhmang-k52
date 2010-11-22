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

public class CGroup
{
private:
	INT groupId;
	CString groupName;
	CString description;
	
	INT GroupId() const { return groupId; }
	void GroupId(INT val) { groupId = val; }
	CString GroupName() const { return groupName; }
	void GroupName(CString val) { groupName = val; }
	CString Description() const { return description; }
	void Description(CString val) { description = val; }
};