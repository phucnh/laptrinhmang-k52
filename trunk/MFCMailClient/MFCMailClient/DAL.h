#pragma once

// CDAL command target

class CDAL : public CDatabase
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
	
public:
	CDAL();
	virtual ~CDAL();

	//CString ConnectionString() const { return _connectionString; }
	//void ConnectionString(CString val) { _connectionString = val; }

	CDatabase *pDb;
	CRecordset *pRecordSet;

	void Initialize();

	BOOL ExecuteSQL(CString sql);
	CRecordset* GetRecordSet(CString sql);
};


