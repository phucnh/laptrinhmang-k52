#pragma once

// CSmtp command target

class CSmtp : CAsyncSocket
{
private:
	CDialog* parrentDlg;

public:
	CDialog* ParrentDlg() const { return parrentDlg; }
	void ParrentDlg(CDialog* val) { parrentDlg = val; }

	CSmtp();
	virtual ~CSmtp();
	virtual void OnAccept(int nErrorCode);
};


