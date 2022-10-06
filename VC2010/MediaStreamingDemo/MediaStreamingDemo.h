
// MediaStreamingDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMediaStreamingDemoApp:
// See MediaStreamingDemo.cpp for the implementation of this class
//

class CMediaStreamingDemoApp : public CWinApp
{
public:
	CMediaStreamingDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMediaStreamingDemoApp theApp;