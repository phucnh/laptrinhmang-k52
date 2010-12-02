#pragma once

// DAL command target

class DAL : public CDatabase
{
private:
	HINSTANCE hInst;

	CString sCatID, sCategory;
	CString sDriver;
	CString sDsn;
	CString sFile;
	CString sExePath;
	CString sDbPath;

	TCHAR szFullPath[_MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

protected:
	void Initialize();

public:
	DAL();
	virtual ~DAL();
	CDatabase *pDb;
	CRecordset *pRecordset;

	
	bool ExecuteSQL(CString sql);
	CRecordset* GetRecordSet(CString sql);

	
};


