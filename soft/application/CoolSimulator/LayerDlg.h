#if !defined(AFX_LAYERDLG_H__BE80257A_C81E_4275_8077_AB9DD2CDBCA6__INCLUDED_)
#define AFX_LAYERDLG_H__BE80257A_C81E_4275_8077_AB9DD2CDBCA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerDlg.h : header file
//

#include "LayerWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg dialog

class CLayerDlg : public CDialog
{
// Construction
public:
	void UpdateScreen(int nLayer,unsigned int* pBuffer,BITMAPINFO* lpBitmapInfo);
	void SetScreenSize(CSize sz);
	CLayerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLayerDlg)
	enum { IDD = IDD_LAYER_DLG };
	CLayerWnd	m_wndLayer4;
	CLayerWnd	m_wndLayer3;
	CLayerWnd	m_wndLayer2;
	CLayerWnd	m_wndLayer1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLayerDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CSize m_ScreenSize;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERDLG_H__BE80257A_C81E_4275_8077_AB9DD2CDBCA6__INCLUDED_)
