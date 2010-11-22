#pragma once

#include "MFCMailServerDlg.h"
// CPop3 command target

class CPop3 : public CAsyncSocket
{
private:
	CMFCMailServerDlg* parrentDlg;
	
public:
	CDialog* ParrentDlg() const { return parrentDlg; }
	void ParrentDlg(CMFCMailServerDlg* val) { parrentDlg = val; }

	CPop3(CMFCMailServerDlg* parrentDlg);
	virtual ~CPop3();
	virtual void OnAccept(int nErrorCode);
};


