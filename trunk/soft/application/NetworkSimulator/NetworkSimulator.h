// NetworkSimulator.h : main header file for the NETWORKSIMULATOR DLL
//

#if !defined(AFX_NETWORKSIMULATOR_H__895BC184_9AF7_4F33_8111_1F5E0A63FB2C__INCLUDED_)
#define AFX_NETWORKSIMULATOR_H__895BC184_9AF7_4F33_8111_1F5E0A63FB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetworkSimulatorApp
// See NetworkSimulator.cpp for the implementation of this class
//

class CNetworkSimulatorApp : public CWinApp
{
public:
	CNetworkSimulatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkSimulatorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNetworkSimulatorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
  
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKSIMULATOR_H__895BC184_9AF7_4F33_8111_1F5E0A63FB2C__INCLUDED_)
