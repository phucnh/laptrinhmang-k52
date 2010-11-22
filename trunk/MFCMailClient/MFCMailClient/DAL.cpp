// DAL.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMailClient.h"
#include "DAL.h"


// CDAL

CDAL::CDAL()
{
}

CDAL::~CDAL()
{
}

BOOL CDAL::ExecuteSQL( CString sql )
{
	CDatabase *db = new CDatabase();
	try
	{
		db->Open(_T("LTMTEST"), FALSE, FALSE, _T("ODBC;"), FALSE);
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

	db->Open(_T("LTMSQL"), FALSE, FALSE, _T("ODBC;"), FALSE);

	data = new CRecordset(db);

	data->Open(AFX_DB_USE_DEFAULT_TYPE,sql);
	db->Close();
	data->Close();

	return data;
}
// CDAL member functions
