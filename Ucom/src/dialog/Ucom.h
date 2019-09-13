
// Ucom.h : PROJECT_NAME Application Header file
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include this file before include stdafx.h to generate PCH file"
#endif

#include "resource.h"		// main symbol
#include "DeConsole.h"

// CUcomApp:
// refer Ucom.cpp
//

class CUcomApp : public CWinApp
{
public:
	CUcomApp();

// overide
public:
	virtual BOOL InitInstance();

// implement

	DECLARE_MESSAGE_MAP()
};

extern CUcomApp theApp;