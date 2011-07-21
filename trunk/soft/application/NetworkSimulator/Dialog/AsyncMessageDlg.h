#if !defined(AFX_ASYNCMESSAGEDLG_H__7FCA4732_DF7D_4FF4_82D7_09C23AAA3A91__INCLUDED_)
#define AFX_ASYNCMESSAGEDLG_H__7FCA4732_DF7D_4FF4_82D7_09C23AAA3A91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AsyncMessageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAsyncMessageDlg dialog

class CAsyncMessageDlg : public CDialog
{
// Construction
public:
	CAsyncMessageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAsyncMessageDlg)
	enum { IDD = IDD_DLG_ASYNC_MESSAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsyncMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAsyncMessageDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNCMESSAGEDLG_H__7FCA4732_DF7D_4FF4_82D7_09C23AAA3A91__INCLUDED_)
