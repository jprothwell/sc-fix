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

	FILENAME	: Inplaceselect.h

  	PURPOSE		: Declaration file
	REMARKS		: nil

	AUTHOR		: KUldeep

	DATE		: Aug 5,03

**************************************************************/


#if !defined(AFX_INPLACESELECT_H__93E3CD16_B65A_4C89_95B9_CD6602E94D64__INCLUDED_)
#define AFX_INPLACESELECT_H__93E3CD16_B65A_4C89_95B9_CD6602E94D64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InPlaceSelect.h : header file
//
#include <afxtempl.h>
#include "MMI_Data_Types.h"
/////////////////////////////////////////////////////////////////////////////
// CInPlaceSelect window
#include	"MyEdit.h"
class CInPlaceSelect : public CComboBox
{
// Construction
public:
	CInPlaceSelect();
	CInPlaceSelect(int iItem, int iSubItem, CMap<int, int, CString, CString&>  *plstItems, CString csValue,BOOL bIsEditable,UINT nMaxLength);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceSelect)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceSelect();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Generated message map functions

protected:
	//{{AFX_MSG(CInPlaceSelect)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnCloseup();
	afx_msg LRESULT OnChildLoseFocus(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int 	m_iItem;
	int 	m_iSubItem;
	CMap<int, int, CString, CString&> * m_plstItems;
	CString	 m_csDisplayValue;
	BOOL	m_bESC;		
	CMyEdit m_EditCtl;
	BOOL	m_bIsEditable;
	UINT	m_nMaxLength;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACESELECT_H__93E3CD16_B65A_4C89_95B9_CD6602E94D64__INCLUDED_)
