#if !defined(AFX_SELECTMESSAGEDLG_H__36921C36_3A8B_42ED_9CFB_CB596F72B3B0__INCLUDED_)
#define AFX_SELECTMESSAGEDLG_H__36921C36_3A8B_42ED_9CFB_CB596F72B3B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectMessageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectMessageDlg dialog

class CSelectMessageDlg : public CDialog
{
// Construction
public:
	CSelectMessageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectMessageDlg)
	enum { IDD = IDD_DLG_SELECT_MESSAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectMessageDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTMESSAGEDLG_H__36921C36_3A8B_42ED_9CFB_CB596F72B3B0__INCLUDED_)
