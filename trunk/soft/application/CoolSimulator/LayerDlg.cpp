// LayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolSimulator.h"
#include "LayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg dialog


CLayerDlg::CLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayerDlg)
	DDX_Control(pDX, IDC_STATIC_LAYER4, m_wndLayer4);
	DDX_Control(pDX, IDC_STATIC_LAYER3, m_wndLayer3);
	DDX_Control(pDX, IDC_STATIC_LAYER2, m_wndLayer2);
	DDX_Control(pDX, IDC_STATIC_LAYER1, m_wndLayer1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayerDlg, CDialog)
	//{{AFX_MSG_MAP(CLayerDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg message handlers

void CLayerDlg::SetScreenSize(CSize sz)
{
  CRect ClientRect,WindowRect;
  GetClientRect(&ClientRect);
  GetWindowRect(&WindowRect);

  this->SetWindowPos(NULL,0,0,sz.cx*2+WindowRect.Width() - ClientRect.Width()+1,sz.cy*2+WindowRect.Height() - ClientRect.Height()+1,SWP_DRAWFRAME);
  m_wndLayer1.SetWindowPos(NULL,0,0,sz.cx,sz.cy,SWP_DRAWFRAME);
  m_wndLayer2.SetWindowPos(NULL,sz.cx+1,0,sz.cx,sz.cy,SWP_DRAWFRAME);
  m_wndLayer3.SetWindowPos(NULL,0,sz.cy+1,sz.cx,sz.cy,SWP_DRAWFRAME);
  m_wndLayer4.SetWindowPos(NULL,sz.cx+1,sz.cy+1,sz.cx,sz.cy,SWP_DRAWFRAME);

  m_wndLayer1.UpdateScreen(NULL,NULL);
  m_wndLayer2.UpdateScreen(NULL,NULL);
  m_wndLayer3.UpdateScreen(NULL,NULL);
  m_wndLayer4.UpdateScreen(NULL,NULL);
}

void CLayerDlg::UpdateScreen(int nLayer, unsigned int *pBuffer, BITMAPINFO *lpBitmapInfo)
{
  switch(nLayer)
  {
  case 0:
    m_wndLayer1.UpdateScreen(pBuffer,lpBitmapInfo);
  	break;
  case 1:
    m_wndLayer2.UpdateScreen(pBuffer,lpBitmapInfo);
  	break;
  case 2:
    m_wndLayer3.UpdateScreen(pBuffer,lpBitmapInfo);
    break;
  case 3:
    m_wndLayer4.UpdateScreen(pBuffer,lpBitmapInfo);
    break;
  default:
      break;
  }

}
