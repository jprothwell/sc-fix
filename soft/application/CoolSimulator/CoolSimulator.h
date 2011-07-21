// CoolSimulator.h : main header file for the COOLSIMULATOR application
//

#if !defined(AFX_COOLSIMULATOR_H__9AA7135B_8E0E_4647_931C_A66997F76D0C__INCLUDED_)
#define AFX_COOLSIMULATOR_H__9AA7135B_8E0E_4647_931C_A66997F76D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCoolSimulatorApp:
// See CoolSimulator.cpp for the implementation of this class
//

class CCoolSimulatorApp : public CWinApp
{
public:
	CCoolSimulatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolSimulatorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCoolSimulatorApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLSIMULATOR_H__9AA7135B_8E0E_4647_931C_A66997F76D0C__INCLUDED_)
