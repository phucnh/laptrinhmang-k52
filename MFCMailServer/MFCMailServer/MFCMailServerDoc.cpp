
// MFCMailServerDoc.cpp : implementation of the CMFCMailServerDoc class
//

#include "stdafx.h"
#include "MFCMailServer.h"

#include "MFCMailServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCMailServerDoc

IMPLEMENT_DYNCREATE(CMFCMailServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCMailServerDoc, CDocument)
END_MESSAGE_MAP()


// CMFCMailServerDoc construction/destruction

CMFCMailServerDoc::CMFCMailServerDoc()
{
	// TODO: add one-time construction code here

}

CMFCMailServerDoc::~CMFCMailServerDoc()
{
}

BOOL CMFCMailServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMFCMailServerDoc serialization

void CMFCMailServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMFCMailServerDoc diagnostics

#ifdef _DEBUG
void CMFCMailServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCMailServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCMailServerDoc commands
