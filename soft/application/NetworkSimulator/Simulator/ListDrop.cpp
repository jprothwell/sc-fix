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

	FILENAME	: ListDrop.cpp

  	PURPOSE		: Class for The extension of List drop control 

	REMARKS		: nil

	AUTHOR		:  Vikram

	DATE		: Oct 5,03

**************************************************************/

#include "stdafx.h"
#include "ListDrop.h"
//#include "InPlaceSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "InPlaceSelect.h"
/////////////////////////////////////////////////////////////////////////////
// CListDrop

CListDrop::CListDrop()
{
	m_nEditableCols = 0;
	m_pEditableValueList = NULL;
	m_nColsAdded = 0;

}

CListDrop::~CListDrop()
{
  // zhuoxz,2008-09-02
  if (m_pEditableValueList)
  {
    delete[] m_pEditableValueList;
  }
}


BEGIN_MESSAGE_MAP(CListDrop, CListCtrl)
	//{{AFX_MSG_MAP(CListDrop)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListDrop message handlers

int CListDrop::HitTestEx(CPoint &point, int *col) const

{

	int colnum = 0;

	int row = HitTest( point, NULL );

	

	if( col ) *col = 0;



	// Make sure that the ListView is in LVS_REPORT

	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )

		return row;



	// Get the top and bottom row visible

	row = GetTopIndex();

	int bottom = row + GetCountPerPage();

	if( bottom > GetItemCount() )

		bottom = GetItemCount();

	

	// Get the number of columns

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	int nColumnCount = pHeader->GetItemCount();



	// Loop through the visible rows

	for( ;row <= bottom;row++)

	{

		// Get bounding rect of item and check whether point falls in it.

		CRect rect;

		GetItemRect( row, &rect, LVIR_BOUNDS );

		if( rect.PtInRect(point) )

		{

			// Now find the column

			for( colnum = 0; colnum < nColumnCount; colnum++ )

			{

				int colwidth = GetColumnWidth(colnum);

				if( point.x >= rect.left 

					&& point.x <= (rect.left + colwidth ) )

				{

					if( col ) *col = colnum;

					return row;

				}

				rect.left += colwidth;

			}

		}

	}

	return -1;

}


 void CListDrop::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if( GetFocus() != this ) SetFocus();

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListDrop::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if( GetFocus() != this ) SetFocus();

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CListDrop::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		index;
	int			colnum;

	UINT	uiColIndex	=	0;
	CListCtrl::OnLButtonDown(nFlags, point);
	if(m_nEditableCols)
	{
		if( ( index = HitTestEx( point, &colnum )) != -1 )
		{
			UINT flag = LVIS_FOCUSED;
				while(((m_pEditableValueList[uiColIndex].nCol) != colnum) && (uiColIndex < m_nColsAdded))
					++uiColIndex;
				if(uiColIndex < m_nEditableCols)
				{
					CString csText = GetItemText(index,colnum);
					if(m_pEditableValueList[uiColIndex].list.GetCount() > index)
					{
						POSITION pos = m_pEditableValueList[uiColIndex].list.FindIndex(index);
						if(pos)
						{
							ShowInPlaceList( index, colnum,*(CMap<int, int, CString, CString&>*)m_pEditableValueList[uiColIndex].list.GetAt(pos), csText,
												m_pEditableValueList[uiColIndex].bIsEditable,m_pEditableValueList[uiColIndex].nMaxLength);
						}
					}
					else
						ShowInPlaceList( index, colnum, *(CMap<int, int, CString, CString&>*)m_pEditableValueList[uiColIndex].list.GetHead(), csText ,m_pEditableValueList[uiColIndex].bIsEditable,m_pEditableValueList[uiColIndex].nMaxLength);
					//if(!pos)
					//	m_pEditableValueList[uiColIndex].pList->RemoveHead();
				}
				else
					SetItemState( index, LVIS_SELECTED | LVIS_FOCUSED , 
						LVIS_SELECTED | LVIS_FOCUSED);

//			}//if( (GetItemState( index, flag ) & flag) == flag )
			
		}//if( ( index = HitTestEx( point, &colnum )) != -1 )
	}//	if(m_nEditableCols)
}


CComboBox* CListDrop::ShowInPlaceList( int nItem, int nCol, 
					CMap<int, int, CString, CString&> &lstItems, CString& csValue,BOOL bIsEditable ,UINT nMaxLength)
{
	// The returned pointer should not be saved

	// Make sure that the item is visible
	if( !EnsureVisible( nItem, TRUE ) ) return NULL;

	// Make sure that nCol is valid 
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 10 ) 
		return NULL;

	// Get the column offset
	int offset = 0;
	for( int i = 0; i < nCol; i++ )
		offset += GetColumnWidth( i );

	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth( nCol ) - 3 ;
	int height = rect.bottom-rect.top;
	rect.bottom += 5*height;
	if( rect.right > rcClient.right) rect.right = rcClient.right;

	DWORD dwStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL/*|WS_HSCROLL*/
					|CBS_DISABLENOSCROLL;
	bIsEditable?dwStyle|=CBS_DROPDOWN:dwStyle|=CBS_DROPDOWNLIST;
	CComboBox *pList = new CInPlaceSelect(nItem, nCol, &lstItems, csValue,bIsEditable,nMaxLength);
	pList->Create( dwStyle, rect, this, 1009 );
	pList->SetItemHeight( -1, height);
	pList->SetHorizontalExtent( GetColumnWidth( nCol ));


	return pList;
}

int CListDrop::InsertColumn(int nCol, LPCTSTR lpszColumnHeading,COLUMN_TYPE iType, CMap<int, int, CString, CString&>* pdisplaylist, UINT nMaxChars,
		int nFormat, int nWidth, int nSubItem)
{
	int iIndex = 0;
	if(iType == EDITABLE || iType == SELECTABLE)
	{
		if(!m_pEditableValueList && m_nEditableCols>0) 
		{
      // zhuoxz,2008-09-02
			m_pEditableValueList = new ValueListInfo[m_nEditableCols];
			//memset(m_pEditableValueList,-1,sizeof(m_pEditableValueList));
		}//	if(!m_pEditableValueList)

		if(	m_nColsAdded<m_nEditableCols)
		{
			m_pEditableValueList[m_nColsAdded].nCol = nCol;
			m_pEditableValueList[m_nColsAdded].nMaxLength = nMaxChars;
			if(pdisplaylist)
				m_pEditableValueList[m_nColsAdded].list.AddTail(pdisplaylist);
			if(iType == SELECTABLE)
				m_pEditableValueList[m_nColsAdded].bIsEditable = FALSE;
			else
				m_pEditableValueList[m_nColsAdded].bIsEditable = TRUE;
				// use the list as it is
			/*
			m_pEditableValueList[m_nColsAdded].pList = new CStringList(pdisplaylist->GetCount()+1);
			for(POSITION pos = pdisplaylist->GetHeadPosition(); pos != NULL; )
				m_pEditableValueList[m_nColsAdded].pList->AddTail(pdisplaylist->GetNext(pos));
				*/

			++m_nColsAdded;
		}//if(iIndex<m_nEditableCols)

	}//if(iType == EDITABLE)

	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}


void CListDrop::SetEditableColumns(int nNum)
{
	m_nEditableCols = nNum;
}

int CListDrop::SetList(int nItem,int nCol, LPVOID pInfo)
{
	UINT uiColIndex = 0;

	while(m_pEditableValueList[uiColIndex].nCol != nCol && uiColIndex<m_nColsAdded) ++uiColIndex;
	if(uiColIndex < m_nEditableCols /*&& pInfo*/)
	{
		m_pEditableValueList[uiColIndex].list.AddTail(pInfo);
	}
	return 0;
}


void CListDrop::ResetList()
{
//	DELETE_MEM_ARR(m_pEditableValueList)
//	m_pEditableValueList = new ValueListInfo[m_nEditableCols];
	for (int iCount = 0;iCount < m_nEditableCols;++iCount)
	{
		m_pEditableValueList[iCount].list.RemoveAll();
		int n = m_pEditableValueList[iCount].list.GetCount();
	}
}