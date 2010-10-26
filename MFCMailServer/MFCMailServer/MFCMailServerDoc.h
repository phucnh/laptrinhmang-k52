
// MFCMailServerDoc.h : interface of the CMFCMailServerDoc class
//


#pragma once


class CMFCMailServerDoc : public CDocument
{
protected: // create from serialization only
	CMFCMailServerDoc();
	DECLARE_DYNCREATE(CMFCMailServerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMFCMailServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


