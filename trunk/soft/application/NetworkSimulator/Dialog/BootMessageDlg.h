#if !defined(AFX_BOOTMESSAGEDLG_H__A28B2331_E907_461C_BB55_0C8D998DDBE8__INCLUDED_)
#define AFX_BOOTMESSAGEDLG_H__A28B2331_E907_461C_BB55_0C8D998DDBE8__INCLUDED_

#include "..\Simulator\MessageManager.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BootMessageDlg.h : header file
//

#include "TreeCtrlEnhanced.h"
#include "..\Simulator\BootUpMsg.h"

/////////////////////////////////////////////////////////////////////////////
// CBootMessageDlg dialog

class CBootMessageDlg : public CDialog
{
// Construction
public:
  HTREEITEM CBootMessageDlg::FindItem(CString strItemName, HTREEITEM hParentItem);
  LRESULT SetTreeStateChangeFlag(WPARAM a,LPARAM treeitem);
	void PrepareBootupMsgSeqList(HTREEITEM root,int node_level,CBootUpMsg* pBootUpMsg,DWORD dwSimNum);
	void SetLibraryIdentifier(HINSTANCE lib_id);
	void SaveBootupMsgSeq();
	void AddDataListInTree(CPtrList *ptrAyncEventsList,CString strRecvd,HTREEITEM htree, DWORD dwSimNum);
	CPtrList* GetAllAsyncEventsForApp(CString strRecvd, CPtrList *plstAsyncMsg, CPtrList *plstOutGoingMsg);
	void InitText();
	CBootMessageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBootMessageDlg)
	enum { IDD = IDD_DLG_BOOT_MESSAGE };
	CTreeCtrlEnhanced	m_treeControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBootMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBootMessageDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  CMessageManager m_Messagemanager;
  int m_TreeStateChangeFlag;
  HINSTANCE m_library_identifier;
//  CPtrList *m_ptrCPtrList;
//  CBootUpMsg *m_pBootUpMsg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOTMESSAGEDLG_H__A28B2331_E907_461C_BB55_0C8D998DDBE8__INCLUDED_)
