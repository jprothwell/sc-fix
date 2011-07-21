// NetworkSimulator.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "NetworkSimulator.h"
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CNetworkSimulatorApp

BEGIN_MESSAGE_MAP(CNetworkSimulatorApp, CWinApp)
	//{{AFX_MSG_MAP(CNetworkSimulatorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkSimulatorApp construction

CNetworkSimulatorApp::CNetworkSimulatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNetworkSimulatorApp object

CNetworkSimulatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNetworkSimulatorApp initialization

BOOL CNetworkSimulatorApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

  CMainDialog* pMainFrame = new CMainDialog;
  pMainFrame->Create(IDD_DLG_NETWORK_SIMULATOR);
	m_pMainWnd = pMainFrame;
  //m_pMainWnd->ShowWindow(SW_HIDE);

	return TRUE;
}

int CNetworkSimulatorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pMainWnd)
  {
    (( CMainDialog*)m_pMainWnd)->AllowClose(TRUE);
    m_pMainWnd->SendMessage(WM_CLOSE,0,0);
    m_pMainWnd->DestroyWindow();
  }
  
	return CWinApp::ExitInstance();
}
