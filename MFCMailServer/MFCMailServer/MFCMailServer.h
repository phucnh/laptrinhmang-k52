
// MFCMailServer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCMailServerApp:
// See MFCMailServer.cpp for the implementation of this class
//

class CMFCMailServerApp : public CWinAppEx
{
public:
	CMFCMailServerApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCMailServerApp theApp;