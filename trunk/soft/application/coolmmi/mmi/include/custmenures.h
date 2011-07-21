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

/*******************************************************************************
 * Filename:
 * ---------
 *  CustMenuRes.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
	FILENAME	: CustMenuRes.h

  	PURPOSE		: To define Common Structures used by the UI Elements and Cutomization Tool 
				  for Customizing Menus(MenuItem).

 

	AUTHOR		: Subramanya BG 

	DATE		: 20thSept 2002.
**********************************************************************************/

#ifndef _PIXCOM_CUSTMENURES_H
#define _PIXCOM_CUSTMENURES_H

#include "mmi_data_types.h"
#include "globalmenuitems.h"
/* neerajm5 */
#ifdef __MOD_SMSAL__
//#define MAX_MENU_ITEMS	500
#else
//#define MAX_MENU_ITEMS	200
#endif
#define MENU_ITEM_SIZE  50
#define MAX_HILITE_HANDLER		MAX_MENU_ITEMS

typedef enum
{
	LIST_MENU=1,
	MATRIX_MENU,
	CIRCULAR_3D_MENU,
	PAGE_MENU,
	FIXED_GRID_MENU,
	ANI_BG_LIST_MENU,
	MATRIX_MENU_EMS,
	ROTATE_MENU,
	LIST_MATRIX_MENU
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#ifdef __MMI_SYMBIAN_STYLE_IDLE__
	,SYMBIAN_IDLE
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
}MMI_MENU_TYPES;

//The Below is the structure of the MenuItem which can be transfered to any screen.
typedef struct Custom_MenuItem 
{
	U16 nMenuItemId; //Unique Id for each MenuItem.
	U16 nParentId;  //ItemId of the parent if 0(zero) then no parent.
	U16 nNumofMenuItem; //Number of Children it has.
	U16 nHideFlag; //This is a flag to mask/hide.(not sure currently)
	U16 nMovable; //This is a flag for moving like 0-NotMovable Anywhere, 
								//1-Movable within the same parent..etc

	U16 nDispAttribute; //This is a display attribute flag like 1-list,2-Circular,3-Tab..etc.
	U16 nStrId; //Unique StringId of the MenuItem. 
	U16 nImageId; //Unique Image of the MenuItem.
	U16 *nOrderMenuItemId; //ItemId of the children in Sequence
}CUSTOM_MENU;

//The below function gets the ParentId of the ItemId.
U16 GetParentItemId(U16 ItemId);

//The below function gets the Number of Child of the ItemId.
U16 GetNumOfChild(U16 ItemId);

//The below function gets the StringId of the ItemId.
U16 GetStringIdOfItem(U16 ItemId);

//The below function gets the ImageId of the ItemId.
U16 GetImageIdOfItem(U16 ItemId);

//The below function gets the Display Attribute of the ItemId.
U16 GetDispAttributeOfItem(U16 ItemId);

//The below function gets the ItemId,StringId,ImageId of the given SeqId & ItemId(Parent).
U16 GetSeqItemId(U16 ParentItemId,U16 Seq);
U16 GetSeqItemStringId(U16 ParentItemId,U16 Seq);
U16 GetSeqItemImageId(U16 ParentItemId,U16 Seq);

//The below functions gets the ItemIds,StringIds,ImageIds in sequence of the ItemId(Parent).
void GetSequenceItemIds(U16 ParentItemId,U16* ItemIds);
void GetSequenceStringIds(U16 ParentItemId,U16* StringIds);
void GetSequenceImageIds(U16 ParentItemId,U16* ImageIds);
U8 GetIndexOfStringId(U16 ParentItemId,U16 StringId);

S16 GetChildIndex(U16 nChildId, U16 nParentId);

/* The below functions conders the hidden menuitems */
U16 GetNumOfChild_Ext(U16 menu_item_id);
U16 GetSeqItemId_Ext(U16 parent_item_id,U16 index);
U16 GetSeqItemStringId_Ext(U16 parent_item_id,U16 index);
U16 GetSeqItemImageId_Ext(U16 parent_item_id,U16 index);
U16 GetSequenceItemIds_Ext(U16 parent_item_id,U16* ItemIds);
U16 GetSequenceStringIds_Ext(U16 parent_item_id,U16* StringIds);
U16 GetSequenceImageIds_Ext(U16 parent_item_id,U16* ImageIds);
U8  GetIndexOfStringId_Ext(U16 parent_item_id,U16 StringId);

void mmi_frm_hide_menu_item(U16 menu_item_id);
void mmi_frm_unhide_menu_item(U16 menu_item_id);

#endif //_PIXCOM_CUSTMENURES_H

