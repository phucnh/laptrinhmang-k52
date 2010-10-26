
// MFCMailServerView.cpp : implementation of the CMFCMailServerView class
//

#include "stdafx.h"
#include "MFCMailServer.h"

#include "MFCMailServerDoc.h"
#include "MFCMailServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCMailServerView

IMPLEMENT_DYNCREATE(CMFCMailServerView, CListView)

BEGIN_MESSAGE_MAP(CMFCMailServerView, CListView)
	ON_WM_STYLECHANGED()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCMailServerView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFCMailServerView construction/destruction

CMFCMailServerView::CMFCMailServerView()
{
	// TODO: add construction code here

}

CMFCMailServerView::~CMFCMailServerView()
{
}

BOOL CMFCMailServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}


void CMFCMailServerView::OnDraw(CDC* /*pDC*/)
{
	CMFCMailServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


void CMFCMailServerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}


// CMFCMailServerView printing


void CMFCMailServerView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMFCMailServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCMailServerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCMailServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCMailServerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCMailServerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMFCMailServerView diagnostics

#ifdef _DEBUG
void CMFCMailServerView::AssertValid() const
{
	CListView::AssertValid();
}

void CMFCMailServerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMFCMailServerDoc* CMFCMailServerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCMailServerDoc)));
	return (CMFCMailServerDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCMailServerView message handlers
void CMFCMailServerView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}
