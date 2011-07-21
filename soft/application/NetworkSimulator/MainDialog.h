#if !defined(AFX_MAINDIALOG_H__100E1A65_85D4_43A1_9A1E_AD2823368D65__INCLUDED_)
#define AFX_MAINDIALOG_H__100E1A65_85D4_43A1_9A1E_AD2823368D65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDialog.h : header file
//

#include "MainSimulator.h"

/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog

class CMainDialog : public CDialog, public CMainSimulator
{
// Construction
public:
	BOOL Bootup();
	void AllowClose(BOOL bClose);
	BOOL InitSimulator(int nXPos,int nYPos);
	CMainDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDialog)
	enum { IDD = IDD_DLG_NETWORK_SIMULATOR };
	CListDrop	m_lstOutGoingMessages;
	CListDrop	m_lstIncomingMessages;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bAllowClose;
	CDialogBar m_wndMyDialogBar;

	// Generated message map functions
	//{{AFX_MSG(CMainDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnModifyBootMsg();
	afx_msg void OnSendMessages();
	afx_msg void OnNcDestroy();
	afx_msg void OnDblclkListIncomingMessages(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
  afx_msg LRESULT OnMessageRecieve(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnMessageSend(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnSendAsyncMessage(WPARAM wParam, LPARAM lParam); // wParam : SIM1,SIM2,SIM1 | SIM2
  afx_msg void OnSendMsgFromTBR(UINT nID);
  afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDIALOG_H__100E1A65_85D4_43A1_9A1E_AD2823368D65__INCLUDED_)
