#pragma once

// DAL command target

class DAL : public CDatabase
{
public:
	DAL();
	virtual ~DAL();
	CDatabase *pDb;
	CRecordset *pRecordset;

	
	bool ExecuteSQL(CString sql);
	CRecordset* GetRecordSet(CString sql);
};


