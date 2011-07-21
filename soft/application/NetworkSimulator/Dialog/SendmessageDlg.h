#if !defined(AFX_SENDMESSAGEDLG_H__F47F2069_2452_4731_AE8C_5D277DB76D3B__INCLUDED_)
#define AFX_SENDMESSAGEDLG_H__F47F2069_2452_4731_AE8C_5D277DB76D3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendMessageDlg.h : header file
//

#include "TreeCtrlEnhanced.h"
#include "..\Simulator\MessageManager.h"

/////////////////////////////////////////////////////////////////////////////
// CSendMessageDlg dialog

class CSendMessageDlg : public CDialog
{
// Construction
public:
	void AddDataListInTree(CPtrList *ptrAyncEventsList,CString strRecvd,HTREEITEM htree, DWORD dwSimNum);
	CPtrList* GetAllAsyncEventsForApp(CString strRecvd, CPtrList *plstAsyncMsg, CPtrList *plstOutGoingMsg);
	void InitSampleData();
	void Inittext();
	void SetLibrayIdentifier(HINSTANCE lib_id);
	CSendMessageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSendMessageDlg)
	enum { IDD = IDD_DLG_SEND_MESSAGE };
	CTreeCtrlEnhanced	m_treeAppControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  HTREEITEM FindItem(CString strItemName, HTREEITEM hParentItem);
	// Generated message map functions
	//{{AFX_MSG(CSendMessageDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  HINSTANCE m_LibraryIdentifier;
  CMessageManager m_Messagemanager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDMESSAGEDLG_H__F47F2069_2452_4731_AE8C_5D277DB76D3B__INCLUDED_)
