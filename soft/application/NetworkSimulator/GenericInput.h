#if !defined(AFX_GENERICINPUT_H__8C2FC8AB_6F80_46B8_B109_EEA71424D003__INCLUDED_)
#define AFX_GENERICINPUT_H__8C2FC8AB_6F80_46B8_B109_EEA71424D003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenericInput.h : header file
//

#include "Simulator\OutgoingMessage.h"
#include "Simulator\IncomingMsg.h"
#include "Simulator\ListDrop.h"
#include "Simulator\Format.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "AddEditDialog.h"
#include "Simulator\MessageManager.h"

/////////////////////////////////////////////////////////////////////////////
// CGenericInput dialog

class CGenericInput : public CDialog
{
// Construction
public:
	CGenericInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenericInput)
	enum { IDD = IDD_DIALOG_GENERIC_INPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenericInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenericInput)
	virtual BOOL OnInitDialog();
	afx_msg void OnListDropEditHandler(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListDatasetDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonMakebuffer();
	afx_msg void OnButtonOnclose();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  COutgoingMessage* OutGoingMsgPtr;
	UINT32 IncomingMsgID;
	CFormat* GetFormatForMessage(CMessage *pMessage);
	CData data_set_to_be_sent;
	Error_Type type_of_error;
	void InitListDrop();
	void Inittext();
	void SetLibraryIdentifier(HINSTANCE);
  CMessageManager* m_pMessagemanager;
  HINSTANCE library_identifier;
public:
	CEdit out_going_msg_name;
	CEdit out_going_msg_id;
	CEdit incoming_msg_name;
	CEdit incoming_msg_id;
	CListDrop dataset_details;
	afx_msg void OnBnClickedButtonMakebuffer();
	int DataSetValidationRoutine();
	int ConvertToNumber(LPWSTR newstring, unsigned long *sum);
	void OutPutMessage(Error_Type et,LPWSTR newstring);
	afx_msg void OnLvnItemchangedListDatasetDetails(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOnclose();
  CStatic static_inc_msg_details;
	CStatic static_inc_msg_name;
	CStatic static_inc_msg_id;
	CStatic static_outgoing_msg_details;
	CStatic static_out_msg_name;
  CStatic static_outgoing_id;
	CStatic static_ds_details;
	CString stringID_to_string;
	CButton in_dlg_Send;
	CButton in_dlg_Close;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERICINPUT_H__8C2FC8AB_6F80_46B8_B109_EEA71424D003__INCLUDED_)
