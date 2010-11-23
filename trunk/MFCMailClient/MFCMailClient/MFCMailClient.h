
// MFCMailClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Pop3.h"
#include "SMTP.h"
#include "MailMessage.h"

// CMFCMailClientApp:
// See MFCMailClient.cpp for the implementation of this class
//

class CMFCMailClientApp : public CWinApp
{
public:
	
public:
	CMFCMailClientApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCMailClientApp theApp;