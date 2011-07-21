// SimulatorStart.cpp: implementation of the CSimulatorStart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetworkSimulator.h"
#include "SimulatorStart.h"
#include "MainDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimulatorStart::CSimulatorStart()
{

}

CSimulatorStart::~CSimulatorStart()
{

}

void CSimulatorStart::ShowSimulator()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (AfxGetApp()->m_pMainWnd)
  {
    (AfxGetApp()->m_pMainWnd)->ShowWindow(SW_SHOW);
  }
}

BOOL CSimulatorStart::InitSimulator(int nXPos,int nYPos)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  BOOL bResult = FALSE;
  if (AfxGetApp()->m_pMainWnd)
  {
    bResult = ((CMainDialog*)(AfxGetApp()->m_pMainWnd))->InitSimulator(nXPos,nYPos);
  }
  return bResult;
}

BOOL CSimulatorStart::IsInit()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  BOOL bResult = FALSE;
  if (AfxGetApp()->m_pMainWnd)
  {
    bResult = ((CMainDialog*)(AfxGetApp()->m_pMainWnd))->IsInit();
  }

  return bResult;
}

void CSimulatorStart::SetReadQueueHandle(oslMsgqid_handle handle)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (AfxGetApp()->m_pMainWnd)
  {
    ((CMainDialog*)(AfxGetApp()->m_pMainWnd))->SetReadQueueHandle((oslMsgqid)handle);
  }
}

void CSimulatorStart::SetWriteQueueHandle(oslMsgqid_handle handle)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (AfxGetApp()->m_pMainWnd)
  {
    ((CMainDialog*)(AfxGetApp()->m_pMainWnd))->SetWriteQueueHandle((oslMsgqid)handle);
  }
}

void CSimulatorStart::SetReadQueueHandler(NWRECEIVEQ pRecvHandler)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (AfxGetApp()->m_pMainWnd)
  {
    ((CMainDialog*)(AfxGetApp()->m_pMainWnd))->SetReadQueueHandler(pRecvHandler);
  }
}

void CSimulatorStart::SetWriteQueueHandler(NWWRITEQ pWriteHandler)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (AfxGetApp()->m_pMainWnd)
  {
    ((CMainDialog*)(AfxGetApp()->m_pMainWnd))->SetWriteQueueHandler(pWriteHandler);
  }
}

BOOL CSimulatorStart::Bootup()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  BOOL bResult = FALSE;
  if (AfxGetApp()->m_pMainWnd)
  {
    bResult = ((CMainDialog*)(AfxGetApp()->m_pMainWnd))->Bootup();
  }
  return bResult;
}
