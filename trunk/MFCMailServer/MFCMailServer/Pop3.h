#pragma once

// CPop3 command target

class CPop3
{
private:
	CString m_hostName;
	UINT m_hostPort;


	CSocket m_socket;
public:
	CPop3();
	virtual ~CPop3();
	void ListenFromClient();
};


