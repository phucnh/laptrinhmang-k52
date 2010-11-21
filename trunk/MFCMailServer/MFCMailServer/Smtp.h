#pragma once

#include "MFCMailServerDlg.h"
// CSmtp command target

public class CSmtp : public CAsyncSocket
{
private:
	CMFCMailServerDlg* parrentDlg;

public:
	CDialog* ParrentDlg() const { return parrentDlg; }
	void ParrentDlg(CMFCMailServerDlg* val) { parrentDlg = val; }

	CSmtp(CMFCMailServerDlg* parrent);
	CSmtp();
	virtual ~CSmtp();
	virtual void OnAccept(int nErrorCode);
};


