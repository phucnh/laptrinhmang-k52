#pragma once

// CPop3 command target

class CPop3 : CAsyncSocket
{
private:
	CDialog* parrentDlg;
	
public:
	CDialog* ParrentDlg() const { return parrentDlg; }
	void ParrentDlg(CDialog* val) { parrentDlg = val; }

	CPop3();
	virtual ~CPop3();
};


