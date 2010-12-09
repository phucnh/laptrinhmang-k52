// dal->cpp : implementation file
//

#include "stdafx.h"
#include "DAL.h"
#include "afxdb.h"


// DAL

DAL::DAL()
{
	Initialize();
}

void DAL::Initialize()
{
	hInst = AfxGetInstanceHandle();

	GetModuleFileName(hInst, szFullPath, MAX_PATH);
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);
	sExePath.Format("%s%s", szDrive, szDir);
	sDbPath.Format("%s%s", sExePath + "\\Database\\", "MFCMailClient.mdb");

	sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";

	//sFile = "E:\\IT Documents\\Study [BK]\\20101\\Network Programming\\Project\\Lap Tring Mang 20101\\MFCMailServer\\MFCMailServer\\Database\\MFCMailServer.mdb";
	//sFile = "C:\\Documents and Settings\\LUU VAN DANG\\Desktop\\laptrinhmang-k52\\MFCMailServer\\MFCMailServer\\Database\\MFCMailServer.mdb";
	//sFile = "D:\\Hoctap\\Laptrinhmang\\BTL\\svn\\laptrinhmang-k52\\MFCMailServer\\MFCMailServer\\Database\\MFCMailServer.mdb";
	sFile = "B:\\Tai Lieu\\Hoctap\\20101\\Course Project\\Network Programming\\Source\\MFCMailServer\\MFCMailServer\\Database\\MFCMailServer.mdb";

	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);
	CDatabase::Open(NULL,false,false,sDsn);
	pRecordset = new CRecordset(this);
}

DAL::~DAL()
{
	pRecordset->Close();
	CDatabase::Close();
}


bool DAL::ExecuteSQL( CString sql)
{
	CDatabase *db=new CDatabase();
	try
	{

		//db->Open(_T("LTMTEST"), FALSE, FALSE, _T("ODBC;"), FALSE);
		////db->OpenEx("connect");
		//db->ExecuteSQL(sql);
		//db->Close();
		//return true;

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
		//delete e;
		return FALSE;
	}

}


CRecordset* DAL::GetRecordSet(CString sql )
{
	/*CDatabase *db=new CDatabase();
	CRecordset *data;
	db->Open(_T("LTMSQL"), FALSE, FALSE, _T("ODBC;"), FALSE);
	data = new CRecordset(db);
	data->Open(AFX_DB_USE_DEFAULT_TYPE,sql);

	return data;
	db->Close();
	data->Close();*/

	try
	{
		pRecordset->Open(CRecordset::forwardOnly,sql,CRecordset::readOnly);
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		pRecordset->Close();
		CDatabase::Close();
	}

	return pRecordset;
}


 //DAL member functions
