// DAL.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "DAL.h"


// CDAL

CDAL::CDAL()
{
	//_connectionString = _T("DSN=MySqlServerConnection;UID=sa;PWD=sa;APP=Microsoft\x00ae Visual Studio\x00ae 2008;WSID=PHUC-PC;DATABASE=MailClient;Network=DBMSSOCN");
	//CDatabase::OpenEx(_connectionString);
	Initialize();
}

CDAL::~CDAL()
{
	pRecordSet->Close();
	CDatabase::Close();
}

BOOL CDAL::ExecuteSQL( CString sql )
{
	try
	{
		//CDatabase *db=new CDatabase();
		//db->m_strConnect = _connectionString; db->m_hdbc
		/*db->Open(_T("LTMTEST"), FALSE, FALSE, _T("ODBC;"), FALSE);*/
		//db->OpenEx(_connectionString);
		//Initialize();
		if (!this->IsOpen())
			CDatabase::Open(NULL,false,false,sDsn);
		CDatabase::ExecuteSQL(sql);
		CDatabase::Close();
		return TRUE;
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		CDatabase::Close();
//		delete e;
		return FALSE;
	}
}

CRecordset* CDAL::GetRecordSet( CString sql )
{
	//CDatabase *db=new CDatabase();
	//CRecordset *data;

	//db->Open(_T("LTMSQL"), FALSE, FALSE, _T("ODBC;"), FALSE);
	//db->OpenEx(_connectionString);
	try
	{
		//data = new CRecordset(this);

		//data->Open(AFX_DB_USE_DEFAULT_TYPE,sql);
		//data->Close();
		pRecordSet->Open(CRecordset::forwardOnly,sql,CRecordset::readOnly);
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		pRecordSet->Close();
		CDatabase::Close();
	}
	
	//db->Close();

	return pRecordSet;
}

void CDAL::Initialize()
{
	hInst = AfxGetInstanceHandle();

	GetModuleFileName(hInst, szFullPath, MAX_PATH);
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);
	sExePath.Format("%s%s", szDrive, szDir);
	sDbPath.Format("%s%s", sExePath + "\\Database\\", "MFCMailClient.mdb");

	sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";
	sFile = "C:\\Documents and Settings\\LUU VAN DANG\\Desktop\\laptrinhmang-k52\\MFCMailClient\\MFCMailClient\\Database\\MFCMailClient.mdb";
	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);
	CDatabase::Open(NULL,false,false,sDsn);
	pRecordSet = new CRecordset(this);
}
// CDAL member functions
