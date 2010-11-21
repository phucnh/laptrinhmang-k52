// dal->cpp : implementation file
//

#include "stdafx.h"
#include "DAL.h"
#include "afxdb.h"


// DAL

DAL::DAL()
{
}

DAL::~DAL()
{
}


bool DAL::ExecuteSQL( CString sql)
{
	CDatabase *db=new CDatabase();
	try
	{

		db->Open(_T("LTMTEST"), FALSE, FALSE, _T("ODBC;"), FALSE);
		db->ExecuteSQL(sql);
		db->Close();
		return true;
	}
	catch (CException* e)
	{
		db->Close();
		delete e;
		return false;
	}

}


CRecordset* DAL::GetRecordSet(CString sql )
{
	CDatabase *db=new CDatabase();
	CRecordset *data;
	db->Open(_T("LTMSQL"), FALSE, FALSE, _T("ODBC;"), FALSE);
	data = new CRecordset(db);
	data->Open(AFX_DB_USE_DEFAULT_TYPE,sql);

	return data;
	db->Close();
	data->Close();

}


 //DAL member functions
