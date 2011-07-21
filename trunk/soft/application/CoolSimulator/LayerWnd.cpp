// LayerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CoolSimulator.h"
#include "LayerWnd.h"
#include "wingdi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerWnd

CLayerWnd::CLayerWnd()
{
  m_bCreateMemDC = FALSE;
}

CLayerWnd::~CLayerWnd()
{
}


BEGIN_MESSAGE_MAP(CLayerWnd, CStatic)
	//{{AFX_MSG_MAP(CLayerWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLayerWnd message handlers

BOOL CLayerWnd::CreateMemDC(CDC* pDC)
{
  ASSERT(pDC);

  BOOL bResult = FALSE;
  CBitmap bitmap;
  int nWidth = GetSystemMetrics(SM_CXSCREEN);
  int nHeight = GetSystemMetrics(SM_CYSCREEN);
  if(bitmap.CreateCompatibleBitmap(pDC,nWidth, nHeight))
  {
    if(m_MemDC.CreateCompatibleDC(pDC))
    {
      m_MemDC.SelectObject(bitmap);
      bResult = TRUE;
    }
  }

  return bResult;
}

void CLayerWnd::UpdateScreen(unsigned int *Buffer, BITMAPINFO *lpBitmapInfo)
{
  if (!m_bCreateMemDC)
  {
    CClientDC dc(this);
    m_bCreateMemDC = CreateMemDC(&dc);
  }

  CRect rect;
  GetClientRect(&rect);

  m_MemDC.Rectangle(rect);

  if (Buffer && lpBitmapInfo)
  {
    CBitmap BitMap;
    long nWidth = lpBitmapInfo->bmiHeader.biWidth;
    long nHieght = lpBitmapInfo->bmiHeader.biHeight;
    BitMap.CreateCompatibleBitmap(&m_MemDC,nWidth,nHieght);
    if(SetDIBits(NULL, (HBITMAP)BitMap, 0, nHieght, Buffer, lpBitmapInfo, DIB_RGB_COLORS))
    {
      CDC TempDC;
      TempDC.CreateCompatibleDC(&m_MemDC);
      TempDC.SelectObject(BitMap);
      m_MemDC.BitBlt( 0, 0, rect.Width(), rect.Height(), &TempDC, 0, 0, SRCCOPY);
      //dc.StretchBlt( 0, 0, rect.Width(), rect.Height(), &TempDC, 0, 0, nWidth, nHieght, SRCCOPY);
    }
  }

  Invalidate();
}

void CLayerWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
  CRect rect;
  GetClientRect(&rect);

  dc.BitBlt( 0, 0, rect.Width(), rect.Height(), &m_MemDC, 0, 0, SRCCOPY);
 	
	// Do not call CWnd::OnPaint() for painting messages
}


BOOL CLayerWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	// return CWnd::OnEraseBkgnd(pDC);
  return TRUE;
}
