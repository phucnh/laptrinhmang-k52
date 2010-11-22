#pragma once

// CDAL command target

class CDAL : public CDatabase
{
public:
	CDAL();
	virtual ~CDAL();

	CDatabase *pDb;
	CRecordset *pRecordSet;

	BOOL ExecuteSQL(CString sql);
	CRecordset* GetRecordSet(CString sql);
};


