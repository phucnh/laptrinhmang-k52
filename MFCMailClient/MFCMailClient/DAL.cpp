// DAL.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "DAL.h"


// CDAL

CDAL::CDAL()
{
	_connectionString = _T("DSN=MySqlServerConnection;UID=sa;PWD=sa;APP=Microsoft\x00ae Visual Studio\x00ae 2008;WSID=PHUC-PC;DATABASE=MailClient;Network=DBMSSOCN");
}

CDAL::~CDAL()
{
}

BOOL CDAL::ExecuteSQL( CString sql )
{
	CDatabase *db = new CDatabase();
	try
	{
		//db->m_strConnect = _connectionString; db->m_hdbc
		//db->Open(_T("LTMTEST"), FALSE, FALSE, _T("ODBC;"), FALSE);
		db->OpenEx(_connectionString);
		db->ExecuteSQL(sql);
		db->Close();
		return TRUE;
	}
	catch (CException* e)
	{
		db->Close();
		delete e;
		return FALSE;
	}
}

CRecordset* CDAL::GetRecordSet( CString sql )
{
	CDatabase *db=new CDatabase();
	CRecordset *data;

	//db->Open(_T("LTMSQL"), FALSE, FALSE, _T("ODBC;"), FALSE);
	db->OpenEx(_connectionString);

	data = new CRecordset(db);

	data->Open(AFX_DB_USE_DEFAULT_TYPE,sql);
	db->Close();
	data->Close();

	return data;
}
// CDAL member functions
