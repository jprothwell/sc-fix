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
// TreeCtrlEnhanced.cpp : implementation file
//

#include "stdafx.h"
#include "..\NetworkSimulator.h"
#include "TreeCtrlEnhanced.h"
#include "..\Simulator\GlobalConstants.h"


// CTreeCtrlEnhanced

IMPLEMENT_DYNAMIC(CTreeCtrlEnhanced, CTreeCtrl)
CTreeCtrlEnhanced::CTreeCtrlEnhanced()
{
	m_CheckBoxOperationStatus=1;
}

CTreeCtrlEnhanced::~CTreeCtrlEnhanced()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlEnhanced, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_CHECKUNCHECK,RouterFunction)
	ON_MESSAGE(HANDLE_DOWN_ARROW_KEY,HandleDownArrowKey)
	ON_MESSAGE(HANDLE_UP_ARROW_KEY,HandleUpArrowKey)
	ON_MESSAGE(HANDLE_LEFT_ARROW_KEY,HandleLeftArrowKey)
	ON_MESSAGE(HANDLE_RIGHT_ARROW_KEY,HandleRightArrowKey)
	ON_MESSAGE(HANDLE_SPACEBAR,HandleSpacebar)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CTreeCtrlEnhanced::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnLButtonDown(nFlags, point);
    UINT flags = 0 ;

	HTREEITEM hitem = HitTest ( point, &flags ) ;

	if (m_CheckBoxOperationStatus==1)
	{
	if(hitem)
	{
		int flag = GetCheck(hitem);
		if(flag != previous_state)
		{
		SetCheckBoxState(hitem,flag);
		SetParentalCheckState(hitem,flag);
				GetParent()->SendMessage(WM_ENABLE_SAVE_BUTTON,0,0);
	}
	}
	SelectItem(hitem);
	GetParent()->SendMessage(WM_SENDVALUE,0,(LPARAM)&hitem);
	}
	else // now checkbox buttons are not functional
	{
		SetCheck(hitem,previous_state);
		SelectItem(hitem);
		GetParent()->SendMessage(WM_SENDVALUE,0,(LPARAM)&hitem);
	}
}

void CTreeCtrlEnhanced::SetCheckBoxState(HTREEITEM item,int flag)
{
	
	HTREEITEM child = GetChildItem(item);
	while(child!= NULL)
	{
		SetCheck(child,flag);
		SetCheckBoxState(child,flag);
		child = GetNextSiblingItem(child);
	}

}

/*---------------- Capture Space Bar for Tree Check Box----------------*/


BOOL CTreeCtrlEnhanced::PreTranslateMessage(MSG *pMsg)
{
	HTREEITEM SelectedItem;
	if(pMsg->message == WM_KEYDOWN)
	{

		if(pMsg->wParam == VK_SPACE)
		{
			if(m_CheckBoxOperationStatus==1)
			{
	        SelectedItem = GetSelectedItem();
	        int flag = GetCheck(SelectedItem);
	        SetCheckBoxState(SelectedItem,!(flag));
            PostMessage(WM_CHECKUNCHECK,0,0);
				GetParent()->SendMessage(WM_ENABLE_SAVE_BUTTON,0,0);
				//PostMessage(HANDLE_SPACEBAR,0,(LPARAM)&SelectedItem);
		}
			else
			{
				return 1; // To make sapce bar unoperational on Check Box
			}
		}
		
		if(pMsg->wParam == VK_DOWN)
		{
	        PostMessage(HANDLE_DOWN_ARROW_KEY,0,0);
		}
		
		if(pMsg->wParam == VK_UP)
		{
	        PostMessage(HANDLE_UP_ARROW_KEY,0,0);
		}

		if(pMsg->wParam == VK_LEFT)
		{
	        PostMessage(HANDLE_LEFT_ARROW_KEY,0,0);
		}

		if(pMsg->wParam == VK_RIGHT)
		{
	        PostMessage(HANDLE_RIGHT_ARROW_KEY,0,0);
		}
	}

	if(pMsg->message == WM_CHAR) PostMessage(HANDLE_DOWN_ARROW_KEY,0,0);

	return 0;
}

LRESULT CTreeCtrlEnhanced::HandleDownArrowKey(WPARAM wp, LPARAM lp)
{
	HTREEITEM SelectedItem = GetSelectedItem();
	GetParent()->SendMessage(WM_SENDVALUE,0,(LPARAM)&SelectedItem);
	return 0;

}

LRESULT CTreeCtrlEnhanced::HandleRightArrowKey(WPARAM wp, LPARAM lp)
{
	HTREEITEM SelectedItem = GetSelectedItem();
	GetParent()->SendMessage(WM_SENDVALUE,0,(LPARAM)&SelectedItem);
	return 0;

}

LRESULT CTreeCtrlEnhanced::HandleUpArrowKey(WPARAM wp, LPARAM lp)
{
	HTREEITEM SelectedItem = GetSelectedItem();
	GetParent()->SendMessage(WM_SENDVALUE,0,(LPARAM)&SelectedItem);
	return 0;
}

LRESULT CTreeCtrlEnhanced::HandleLeftArrowKey(WPARAM wp, LPARAM lp)
{
	HTREEITEM SelectedItem = GetSelectedItem();
	GetParent()->SendMessage(WM_SENDVALUE,0,(LPARAM)&SelectedItem);
	return 0;
}

LRESULT CTreeCtrlEnhanced::HandleSpacebar(WPARAM wp, LPARAM lp)
{
  if (m_CheckBoxOperationStatus==1)
  {
    HTREEITEM SelectedItem = GetSelectedItem();
    GetParent()->SendMessage(WM_SENDVALUE,0,(LPARAM)&SelectedItem);
  }
  return 0;
}

LRESULT CTreeCtrlEnhanced::RouterFunction(WPARAM wp, LPARAM lp)
{
	HTREEITEM item = GetSelectedItem();
	int flag = GetCheck(item);
  SetParentalCheckState(item,flag); 
	return 0;
}


void CTreeCtrlEnhanced::SetParentalCheckState(HTREEITEM item, int flag)
{
    if(flag)
	{
      HTREEITEM parent = GetParentItem(item); 
	 if(parent)
	 {
	     SetCheck(parent,flag);
	     SetParentalCheckState(parent,flag);
	 }
	}
	else
	{
        HTREEITEM parent = GetParentItem(item);
        HTREEITEM sibling = GetChildItem(parent);
		while(sibling)
		{
			if(GetCheck(sibling))
				break;
			else
				sibling = GetNextSiblingItem(sibling);
		}
		if(!sibling)
		{
			if(parent)
			{
				SetCheck(parent,flag);
				SetParentalCheckState(parent,flag);
			}
		}
	}

}
void CTreeCtrlEnhanced::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnMouseMove(nFlags, point);
	HTREEITEM item = HitTest(point,NULL);
	if(item)
	{
		previous_state = GetCheck(item);
	}
}

void CTreeCtrlEnhanced::SetCheckButtonOperatable(int CheckBoxStatus)
{
     m_CheckBoxOperationStatus=CheckBoxStatus;
}
