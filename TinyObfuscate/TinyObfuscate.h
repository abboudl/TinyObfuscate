// TinyObfuscate - By Michael Haephrati (C)2017 Michael Haephrati, Secured Globe, Inc.
// 1501 Broadway STE 1200, New York 10036, NY

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTinyObfuscateApp:
// See TinyObfuscate.cpp for the implementation of this class
//

class CTinyObfuscateApp : public CWinApp
{
public:
	CTinyObfuscateApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTinyObfuscateApp theApp;