#pragma once

// CPop3 command target

public class CPop3 : public CAsyncSocket
{
private:
	CDialog* parrentDlg;
	
public:
	CDialog* ParrentDlg() const { return parrentDlg; }
	void ParrentDlg(CDialog* val) { parrentDlg = val; }

	CPop3(CDialog* parrentDlg);
	virtual ~CPop3();
	virtual void OnAccept(int nErrorCode);
};


