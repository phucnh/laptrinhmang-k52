#pragma once

// CSmtp command target

public class CSmtp : public CAsyncSocket
{
private:
	CDialog* parrentDlg;

public:
	CDialog* ParrentDlg() const { return parrentDlg; }
	void ParrentDlg(CDialog* val) { parrentDlg = val; }

	CSmtp(CDialog* parrent);
	CSmtp();
	virtual ~CSmtp();
	virtual void OnAccept(int nErrorCode);
};


