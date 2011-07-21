#if !defined(AFX_LAYERWND_H__66D98864_38E8_41BF_8056_E834C0CF7EA1__INCLUDED_)
#define AFX_LAYERWND_H__66D98864_38E8_41BF_8056_E834C0CF7EA1__INCLUDED_

//#include "SIMULATORLAYER\FIRMWARE\INCLUDE\cswtype.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLayerWnd window

class CLayerWnd : public CStatic
{
// Construction
public:
	CLayerWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateScreen(unsigned int *Buffer, BITMAPINFO *lpBitmapInfo);
	virtual ~CLayerWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLayerWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  BOOL CreateMemDC(CDC* pDC);
private:
	BOOL m_bCreateMemDC;
  CDC m_MemDC;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERWND_H__66D98864_38E8_41BF_8056_E834C0CF7EA1__INCLUDED_)
