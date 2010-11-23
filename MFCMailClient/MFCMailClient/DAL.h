#pragma once

// CDAL command target

class CDAL : public CDatabase
{
private:
	CString _connectionString;
	
public:
	CDAL();
	virtual ~CDAL();

	CString ConnectionString() const { return _connectionString; }
	void ConnectionString(CString val) { _connectionString = val; }

	CDatabase *pDb;
	CRecordset *pRecordSet;

	BOOL ExecuteSQL(CString sql);
	CRecordset* GetRecordSet(CString sql);
};


