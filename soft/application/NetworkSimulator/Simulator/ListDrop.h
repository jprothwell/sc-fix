/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/**************************************************************

	FILENAME	: ListDrop.h

  	PURPOSE		: Declaration file
	REMARKS		: nil

	AUTHOR		: KUldeep

	DATE		: Aug 5,03

**************************************************************/

#if !defined(AFX_LISTDROP_H__9A1719EF_BABF_4E05_A02F_20B72CF7B29D__INCLUDED_)
#define AFX_LISTDROP_H__9A1719EF_BABF_4E05_A02F_20B72CF7B29D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListDrop.h : header file
//
#include <afxtempl.h>
#include "MMI_Data_Types.h"
/////////////////////////////////////////////////////////////////////////////
// CListDrop window
//#include "Listdrop_decl.h"
typedef unsigned short  U16; 
enum COLUMN_TYPE{EDITABLE = 0, NON_EDITABLE,SELECTABLE};
typedef struct structValue
{
	CPtrList list;
	int		nCol;
	U16		nMaxLength;
	BOOL		bIsEditable;
}ValueListInfo;

class CListDrop : public CListCtrl
{
// Construction
public:
	CListDrop();	
	int HitTestEx(CPoint &point, int *col) const;
	CComboBox* CListDrop::ShowInPlaceList( int nItem, int nCol, 
					CMap<int, int, CString, CString&> &lstItems, CString& csValue,BOOL bIsEditable ,UINT nMaxLength);	
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,COLUMN_TYPE iType, CMap<int, int, CString, CString&>* pdisplaylist, UINT nMaxChars,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	void SetEditableColumns(int nNum);
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListDrop)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListDrop();
	int SetList(int nItem,int nCol, LPVOID pInfo);
	void ResetList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListDrop)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	byte m_nEditableCols;
	U16	m_nColsAdded;
	ValueListInfo *m_pEditableValueList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDROP_H__9A1719EF_BABF_4E05_A02F_20B72CF7B29D__INCLUDED_)
