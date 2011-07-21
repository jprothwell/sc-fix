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
// InPlaceSelect.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceSelect

CInPlaceSelect::CInPlaceSelect()
{
}

CInPlaceSelect::~CInPlaceSelect()
{
}

CInPlaceSelect::CInPlaceSelect(int iItem, int iSubItem, CMap<int, int, CString, CString&> *plstItems, CString csValue,BOOL bIsEditable,UINT nMaxLength):m_csDisplayValue(csValue)
{
	m_iItem = iItem;
	m_iSubItem = iSubItem;

	m_plstItems = plstItems;
	m_bESC = FALSE;
	m_bIsEditable = bIsEditable;
	m_nMaxLength = nMaxLength;
}

BEGIN_MESSAGE_MAP(CInPlaceSelect, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceSelect)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_NCDESTROY()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_MESSAGE(WM_CHILD_LOOSE_FOCUS,OnChildLoseFocus)


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceSelect message handlers

int CInPlaceSelect::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);
	CString	pValue;
	int	nIndex	=	-1;
	int	nInsertValIndex	=	0;
	if(m_plstItems)
	{
		for( int iCount = 0;iCount <  m_plstItems->GetCount();++iCount )
		{
			m_plstItems->Lookup(iCount,pValue);
			
			if(pValue.GetLength())
			{
				if(nIndex == -1 && pValue == m_csDisplayValue)
					nIndex = nInsertValIndex;
				AddString( (LPCTSTR) pValue );
				++nInsertValIndex;
			}
		}
	}

	if(nIndex == -1)
	{
		AddString( (LPCTSTR) m_csDisplayValue );
		if(	m_plstItems)
			SetCurSel( m_plstItems->GetCount() );
		else
			SetCurSel( 0);
	}
	else
		SetCurSel(nIndex);
	SetFocus();
	if(m_bIsEditable)
	{
		m_EditCtl.SubclassWindow( GetWindow(GW_CHILD)->m_hWnd);
		m_EditCtl.SetLimitText(m_nMaxLength);
	}
 	return 0;
}




BOOL CInPlaceSelect::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN 
				|| pMsg->wParam == VK_ESCAPE
				)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;				// DO NOT process further
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CInPlaceSelect::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	if(!m_bIsEditable)
	{
		CString str;
		GetWindowText(str);
		
		// Send Notification to parent of ListView ctrl
		LV_DISPINFO dispinfo;
		dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		dispinfo.hdr.idFrom = GetDlgCtrlID();
		dispinfo.hdr.code = LVN_ENDLABELEDIT;

		dispinfo.item.mask = LVIF_TEXT;
		dispinfo.item.iItem = m_iItem;
		dispinfo.item.iSubItem = m_iSubItem;
		dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
		dispinfo.item.cchTextMax = str.GetLength();

		GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
		PostMessage( WM_CLOSE );

	}
	

}

void CInPlaceSelect::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if( nChar == VK_ESCAPE )
			m_bESC = TRUE;
		GetParent()->SetFocus();
		return;
	}
	
	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CInPlaceSelect::OnNcDestroy() 
{
	CComboBox::OnNcDestroy();
	
	delete this;
}

void CInPlaceSelect::OnCloseup() 
{
	GetParent()->SetFocus();
}

LRESULT CInPlaceSelect::OnChildLoseFocus(WPARAM wParam, LPARAM lParam)
{
	
	CString str;
	GetWindowText(str);
	
	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iItem;
	dispinfo.item.iSubItem = m_iSubItem;
	dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();

	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
	PostMessage( WM_CLOSE );
	return(TRUE);
}


