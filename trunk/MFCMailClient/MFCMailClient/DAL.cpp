// DAL.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "DAL.h"


// CDAL

CDAL::CDAL()
{
	_connectionString = _T("DSN=MySqlServerConnection;UID=sa;PWD=sa;APP=Microsoft\x00ae Visual Studio\x00ae 2008;WSID=PHUC-PC;DATABASE=MailClient;Network=DBMSSOCN");
	CDatabase::OpenEx(_connectionString);
}

CDAL::~CDAL()
{
	CDatabase::Close();
}

BOOL CDAL::ExecuteSQL( CString sql )
{
	try
	{
		CDatabase *db=new CDatabase();
		//db->m_strConnect = _connectionString; db->m_hdbc
		/*db->Open(_T("LTMTEST"), FALSE, FALSE, _T("ODBC;"), FALSE);*/
		db->OpenEx(_connectionString);
		CDatabase::ExecuteSQL(sql);
		CDatabase::Close();
		return TRUE;
	}
	catch (CException* e)
	{
		e->ReportError();
		CDatabase::Close();
//		delete e;
		return FALSE;
	}
}

CRecordset* CDAL::GetRecordSet( CString sql )
{
	CDatabase *db=new CDatabase();
	CRecordset *data;

	//db->Open(_T("LTMSQL"), FALSE, FALSE, _T("ODBC;"), FALSE);
	//db->OpenEx(_connectionString);

	data = new CRecordset(db);

	data->Open(AFX_DB_USE_DEFAULT_TYPE,sql);
	db->Close();
	data->Close();

	return data;
}
// CDAL member functions
