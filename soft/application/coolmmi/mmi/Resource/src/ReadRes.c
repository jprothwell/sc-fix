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
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   wgui_touch_screen.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   wgui-level functions for touch screen.
 *
 *	
 *
 * Author:
 * -------
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/*  Include: MMI header file */
#include "mmi_trace.h"
#include "string.h"
#include "globalmenuitems.h"
#include "mmi_data_types.h"
#include "custdatares.h"
#include "custmenures.h"
#include "gui_themes.h"
#include "fontdcl.h"
#include "gui.h"
#include "wgui.h"
#include "downloadprotos.h"  
#include "mmi_features.h"
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
#include "thememanager.h"
#endif
#include "phonesetupgprots.h"
#include "satdefs.h"
#ifdef  __POPULATE_ENGINE_ 
#include "exportmmifunc.h"
#endif
#include "debuginitdef.h"
#include "wgui_categories_util.h"
/***************************************************************************** 
* Define
*****************************************************************************/
#define MAX_LANGUAGE_NUM 15

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/
U8 *gp_language_struct[MAX_LANGUAGE_NUM]; 
static U8 MenuItemMask[(MAX_MENU_ITEMS_VALUE/8)+1];

/***************************************************************************** 
* Local Function
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/
extern const CUSTOM_STRING_MAP*	 gpStringMapRes;
extern const CUSTOM_STRING* gpStringRes;
extern const CUSTOM_STRMAP_SEARCH*	 	gpStrMapSearch;
extern const CUSTOM_IMAGE_MAP ImageIdMap[];
extern const CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMap[];	
 
extern const CUSTOM_IMAGE_MAP ImageIdMapEXT[]; 
extern const CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMapEXT[]; 
 
extern const CUSTOM_MENU  nCustMenus[MAX_MENU_ITEMS];

 
extern BOOL 			r2lMMIFlag;
//CSD end

extern	U16	gMaxDeployedLangs;
extern U16 gCurrLangIndex;
//extern StringResList	gStringList[MAX_LANGUAGES];
extern StringResList*	gStringList;

extern sLanguageDetails* gLanguageArray;

//decalred in the CustImageRes.c
extern CUSTOM_IMAGE* nCustImageNames;
 
extern CUSTOM_IMAGE* nCustImageNamesEXT; 
 

//decalred in the CustScreenData.c
extern CUSTOM_SCREEN nCustScreens[MAX_SCREEN_IDS];

//declared in the CustMenuDataRes.c

extern U16 CurrMaxStringId;  
extern U16 CurrMaxStringNum;  
extern U16 CurrMaxStringIdSearch;

extern U16 CurrMaxImageId;  
extern U16 CurrMaxSearchImageId;
extern U16 CurrMaxImageNum;  

 
 
extern U16 CurrMaxImageIdEXT;  
extern U16 CurrMaxSearchImageIdEXT;
extern U16 CurrMaxImageNumEXT;  
 
 

/***************************************************************************** 
* Global Function
*****************************************************************************/
extern void SATEVDLLangSel(U8 *);


/**************************************************************

	FUNCTION NAME		: GetStringLang(U16 StringId)

  	PURPOSE				: Function to get string associated with the stringId

	INPUT PARAMETERS	: (U16)StringId

	OUTPUT PARAMETERS	: (char*)Displayed String.

	RETURNS				: the String Mapped with the Id.

 
						   is set inside the WriteRes.c file.	

**************************************************************/

S8* GetStringLang(U16 StringId,U16 lang_index)
{
	S32	nFirst		=	0;
	S32 nLast = CurrMaxStringIdSearch;
	S32 nMid;
	CUSTOM_STRMAP_SEARCH* pStrMapSearch;
	CUSTOM_STRING* pStringRes;
	CUSTOM_STRING_MAP* pStringMapRes;
	
	TBM_ENTRY(0x2850);
	pStringRes = gStringList[lang_index].pStringRes;
	pStringMapRes = gStringList[lang_index].pStringMap;
	pStrMapSearch = gStringList[lang_index].pStrMapSearch;
	
	while( (nLast>=nFirst) /*&& (nLast!=0)*/ )
	{
		nMid = (nFirst+nLast)/2;;
		if((pStrMapSearch[nMid].nMinStrId<= StringId)&&(pStrMapSearch[nMid].nMaxStrId>= StringId))
		{
#ifdef __ASCII
			TBM_EXIT(0x2850);
			return (S8*) &pStringRes[(pStringMapRes[((pStrMapSearch[nMid].nMapIndex) +(StringId)-(pStrMapSearch[nMid].nMinStrId))])];
#endif
#ifdef __UCS2_ENCODING
			TBM_EXIT(0x2850);
			return (S8*) &pStringRes[(pStringMapRes[((pStrMapSearch[nMid].nMapIndex) +(StringId)-(pStrMapSearch[nMid].nMinStrId))])<<1];
#endif
		}
		if(StringId>pStrMapSearch[nMid].nMinStrId)
			nFirst = nMid+1;
		else
			nLast = nMid-1;

	}
	TBM_EXIT(0x2850);
	return (S8*)"";
}

/**************************************************************

	FUNCTION NAME		: GetString(U16 StringId)

  	PURPOSE				: Function to get string associated with the stringId

	INPUT PARAMETERS	: (U16)StringId

	OUTPUT PARAMETERS	: (char*)Displayed String.

	RETURNS				: the String Mapped with the Id.

 
						   is set inside the WriteRes.c file.	

**************************************************************/

S8* GetString(U16 StringId)
{
	S32	nFirst		=	0;
	S32 nLast = CurrMaxStringIdSearch;
	S32 nMid;
	
	TBM_ENTRY(0x2851);
	while( (nLast>=nFirst) /*&& (nLast!=0)*/ )
	{
		nMid = (nFirst+nLast)/2;;
		if((gpStrMapSearch[nMid].nMinStrId<= StringId)&&(gpStrMapSearch[nMid].nMaxStrId>= StringId))
		{
#ifdef __ASCII
			TBM_EXIT(0x2851);
			return (S8*) &gpStringRes[(gpStringMapRes[((gpStrMapSearch[nMid].nMapIndex) +(StringId)-(gpStrMapSearch[nMid].nMinStrId))])];
#endif
#ifdef __UCS2_ENCODING
			TBM_EXIT(0x2851);
			return (S8*) &gpStringRes[(gpStringMapRes[((gpStrMapSearch[nMid].nMapIndex) +(StringId)-(gpStrMapSearch[nMid].nMinStrId))])<<1];
#endif
		}
		if(StringId>gpStrMapSearch[nMid].nMinStrId)
			nFirst = nMid+1;
		else
			nLast = nMid-1;

	}
	TBM_EXIT(0x2851);
	return (S8*)"";
}

/**************************************************************

	FUNCTION NAME		: GetImage(U16 ImageId)

  	PURPOSE				: Function to get filename associated with the ImageId

	INPUT PARAMETERS	: (U16)ImageId.

	OUTPUT PARAMETERS	: (char*)Displayed Image Filename. 

	RETURNS				: Returns the filename associated with the ImageId.

 
						   is set inside the WriteRes.c file.	

**************************************************************/
#if defined(MMI_ON_HARDWARE_P) && defined(__MMI_DEFAULT_THEME_3__)

#include "filesystemdef.h"
static U16 cache_image_id;
	static U32 cache_image_content[30000/4];


 
#if !defined(__BW_MMI__) && !defined(WIN32) //Target version shall use this library
extern void GIF_clear_cache(void);
#endif
// CSD end
static kal_bool fread_all(FILE_HANDLE handle,U8* dest,S32 dest_size,S32 want_size)
{
	ASSERT(dest_size>=want_size);

	pfseek(handle,0,FS_FILE_BEGIN);
	

	while(want_size>0)
	{
		S32 nRead;
		
		if(pfread(handle,dest,want_size)!= FS_NO_ERROR)
			return KAL_FALSE;
		
		want_size-=nRead;
		dest+=nRead;
	}
	return KAL_TRUE;
}
#endif

S8* GetImage(U16 ImageId)
{
	S32	nFirst		=	0;
	S32 nLast = CurrMaxSearchImageId;
	S32 nMid;
	
    TBM_ENTRY(0x2852);
	if(ImageId >= NV_RAM_FILE_IMAGE_ID_BASE && ImageId<=NV_RAM_FILE_IMAGE_ID_BASE_END)
	{
		//S8* image_src= GetImageFileSystem(ImageId);
		S8* image_src= 0;  
		//dyj modify 20060514
#if defined(MMI_ON_HARDWARE_P) && defined(__MMI_DEFAULT_THEME_3__)
		if (image_src[0] == IMAGE_TYPE_BMP_FILE)
			TBM_EXIT(0x2852);
			return image_src;

		if(cache_image_id != ImageId)
		{
			FILE_HANDLE file = OpenCache((U8*)(image_src+6),PFS_READ_BINARY);
			
			if(file!=(FILE_HANDLE)NULL)
			{
				S32 size = GetFSFileSize(file);
				 
				if(size<(S32)(sizeof(cache_image_content)-6)) // test the file size
				// CSD end
				if(fread_all(file,((U8*)cache_image_content)+6,(S32)(sizeof(cache_image_content)-6),size))
				{
					U8* header = (U8*) cache_image_content;
					
					switch(image_src[0])
					{
					case IMAGE_TYPE_BMP_FILE:
						header[0] = IMAGE_TYPE_BMP;break;
					case IMAGE_TYPE_GIF_FILE:
						header[0] = IMAGE_TYPE_GIF;
						 
						#if !defined(__BW_MMI__) && !defined(WIN32) //Target version shall use this library
							GIF_clear_cache();
						#endif
						// CSD end
						break;
					default:
						pfclose(file);
						TBM_EXIT(0x2852);
						return image_src;
					}

					header[1] = (U8)NULL;
					header[2] = (U8) (size & 0x000000ff);
					header[3] = (U8) ((size >> 8) & 0x000000ff);
					header[4] = (U8) ((size >> 16) & 0x000000ff);
					header[5] = (U8) ((size >> 24) & 0x000000ff);	

					cache_image_id = ImageId;
					pfclose(file);
					TBM_EXIT(0x2852);
					return (S8*)&(cache_image_content[0]);
				}
				pfclose(file);
			}
		}
		else 
			TBM_EXIT(0x2852);
			return (S8*)&(cache_image_content[0]);
#endif		
		TBM_EXIT(0x2852);
		return image_src;
	}
	else
	{
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
		U16 image_index;
		S8*	pImageData;
		U32	nImageIndex;
#endif
	while( (nLast>=nFirst) /*&& (nLast!=0)*/ )
	{
		nMid = (nFirst+nLast)/2;;
		if((ImageIdSearchMap[nMid].minImageId<= ImageId)&&(ImageIdSearchMap[nMid].maxImageId>= ImageId))
		{
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
		S32		nArrayIndex;

			image_index = ImageIdMap[(ImageIdSearchMap[nMid].index) +(ImageId)-(ImageIdSearchMap[nMid].minImageId)].nImageNum;
      		if(tm_does_theme_have_alt_image())
			{
				if(tm_is_theme_dirty_bit_set(image_index))
				{
					if(tm_get_curr_theme_type() == THEME_TYPE_SYSTEM)
						TBM_EXIT(0x2852);
						return (S8*)tm_get_system_theme_image(tm_get_theme_image_offset_or_index(ImageId,&nArrayIndex));
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh			
					else
					{
						nImageIndex = tm_get_theme_image_offset_or_index(ImageId,&nArrayIndex);
					if(nImageIndex > 0)
					{
						pImageData = (PS8)tm_get_downloaded_theme_image(nImageIndex,nArrayIndex);
						TBM_EXIT(0x2852);
						return pImageData;
					}
					else
					{
						TBM_EXIT(0x2852);
						return (S8*)(nCustImageNames[image_index].pImage);
					}

				}
#endif
				}
			}
			TBM_EXIT(0x2852);
			return (S8*)(nCustImageNames[image_index].pImage);
#else
//			TRACE_EF(g_sw_AUDIO, "````Resource```````:imageID=%d", ImageIdMap[(ImageIdSearchMap[nMid].index) +(ImageId)-(ImageIdSearchMap[nMid].minImageId)].nImageNum);
			TBM_EXIT(0x2852);
			return (S8*) nCustImageNames[ImageIdMap[(ImageIdSearchMap[nMid].index) +(ImageId)-(ImageIdSearchMap[nMid].minImageId)].nImageNum].pImage;
#endif
		}
		if(ImageId> ImageIdSearchMap[nMid].minImageId)
			nFirst = nMid+1;
		else
			nLast = nMid-1;

	}

		 
		 
		nFirst = 0;
		nLast = CurrMaxSearchImageIdEXT;
		while( (nLast>=nFirst) /*&& (nLast!=0)*/ )
		{
			nMid = (nFirst+nLast)/2;;
			if((ImageIdSearchMapEXT[nMid].minImageId<= ImageId)&&(ImageIdSearchMapEXT[nMid].maxImageId>= ImageId))
			{

#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
				S32		nArrayIndex;

				image_index = ImageIdMapEXT[(ImageIdSearchMapEXT[nMid].index) +(ImageId)-(ImageIdSearchMapEXT[nMid].minImageId)].nImageNum;
		        if(tm_does_theme_have_alt_image())
		        {
			       if(tm_is_theme_dirty_bit_set(image_index))
		     	   {
					if(tm_get_curr_theme_type() == THEME_TYPE_SYSTEM)
					TBM_EXIT(0x2852);
					return (S8*)tm_get_system_theme_image(tm_get_theme_image_offset_or_index(ImageId,&nArrayIndex));
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh			
			        else
					{
						nImageIndex = tm_get_theme_image_offset_or_index(ImageId,&nArrayIndex);
					if(nImageIndex > 0)
					{
					    pImageData = (PS8)tm_get_downloaded_theme_image(nImageIndex,nArrayIndex);
						TBM_EXIT(0x2852);
					    return pImageData;
				    }
					else
					{
						TBM_EXIT(0x2852);
					   return (S8*)(nCustImageNamesEXT[image_index].pImage);
					}

				    }
#endif
			       }
		        }
				TBM_EXIT(0x2852);
			    return (S8*)(nCustImageNamesEXT[image_index].pImage);
#else
	//			TRACE_EF(g_sw_AUDIO, "EXT   !!!!!!````Resource```````:imageID=%d", ImageIdMapEXT[(ImageIdSearchMapEXT[nMid].index) +(ImageId)-(ImageIdSearchMapEXT[nMid].minImageId)].nImageNum);
				TBM_EXIT(0x2852);
				return (S8*) nCustImageNamesEXT[ImageIdMapEXT[(ImageIdSearchMapEXT[nMid].index) +(ImageId)-(ImageIdSearchMapEXT[nMid].minImageId)].nImageNum].pImage;
#endif

			}
			if(ImageId> ImageIdSearchMapEXT[nMid].minImageId)
				nFirst = nMid+1;
			else
				nLast = nMid-1;

		}
		 
		 
	TBM_EXIT(0x2852);	
	return (S8*)(nCustImageNames[0].pImage);
}
}

#ifdef __MMI_THEMES_V2_SUPPORT__  //ritesh
U16 GetImageIndex(U16 ImageId)
{
	S32	nFirst =	0;
	S32 nLast = CurrMaxSearchImageId;
	S32 nMid;

	while( (nLast>=nFirst) /*&& (nLast!=0)*/ )
	{
		nMid = (nFirst+nLast)/2;;
		if((ImageIdSearchMap[nMid].minImageId<= ImageId)&&(ImageIdSearchMap[nMid].maxImageId>= ImageId))
			break;
		if(ImageId> ImageIdSearchMap[nMid].minImageId)
			nFirst = nMid+1;
		else
			nLast = nMid-1;
	}

	return ImageIdMap[(ImageIdSearchMap[nMid].index) +(ImageId)-(ImageIdSearchMap[nMid].minImageId)].nImageNum;
}

S8* GetDefaultImage(void)
{
	return (S8*)(nCustImageNames[0].pImage);
}
#endif

/**************************************************************

	FUNCTION NAME		: GetParentItemId(U16 ItemId)

  	PURPOSE				: Function to get Parent MenuItemId of the Given MenuItemId.

	INPUT PARAMETERS	: (U16) MenuItemId.

	OUTPUT PARAMETERS	: (U16) Parent MenuItemId of the given MenuItemId.

	RETURNS				: the Parent MenuItemId.

 

**************************************************************/

U16 GetParentItemId(U16 ItemId)
{
		
	return nCustMenus[ItemId - 1].nParentId; 
	
}

 
/**************************************************************

	FUNCTION NAME		: GetRootParentItemId(U16 ItemId)

  	PURPOSE				: Function to get Parent MenuItemId of the Given MenuItemId in Mainmenu

	INPUT PARAMETERS	: (U16) MenuItemId.

	OUTPUT PARAMETERS	: (U16) Parent MenuItemId of the given MenuItemId in Mainmenu.

	RETURNS				: the Parent MenuItemId in Mainmenu.

 

**************************************************************/

U16 GetRootParentIndex(U16 ItemId)
{
	U16 RootId;
	U16 parent_id;
	U16 idx;

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	U16 parent_idx=0;
	U16 grand_parent_id=0;
	U16 num_of_child=0;
	U16 total_idx=0;

	parent_id = GetParentItemId(ItemId);

	/* Group menu id */
	if ( parent_id ==1 )
		return 0;
	
	grand_parent_id = GetParentItemId(parent_id);

	RootId = ItemId;

	while ( grand_parent_id !=IDLE_SCREEN_MENU_ID )
	{
		RootId = parent_id;
		parent_id = grand_parent_id;
		grand_parent_id = GetParentItemId(grand_parent_id);
	}

	num_of_child=GetNumOfChild (IDLE_SCREEN_MENU_ID);
	for ( parent_idx=0; parent_idx<num_of_child; parent_idx++)
	{
		if ( GetSeqItemId(IDLE_SCREEN_MENU_ID,parent_idx) == parent_id )
		{
			num_of_child = GetNumOfChild(parent_id);
			for ( idx=0;idx<num_of_child;idx++)
				if ( GetSeqItemId(parent_id,idx) != RootId )
					total_idx++;
				else
					return total_idx;
		}
		else
		{
			total_idx+=GetNumOfChild (parent_id);
		}
	}

	MMI_ASSERT(0);
	return 0;

#else

	RootId = ItemId;
	parent_id = GetParentItemId(ItemId);
	if ( parent_id !=IDLE_SCREEN_MENU_ID ) 
	{//ItemId is not one of the itemIds of mainmenu
		RootId = parent_id;
		parent_id = GetParentItemId(parent_id);
		while ( parent_id !=IDLE_SCREEN_MENU_ID)
		{
			RootId = parent_id;
			parent_id = GetParentItemId(parent_id);
		}
	}//RootId is the menu id in mainmenu
	for (idx=0; idx< nCustMenus[0].nNumofMenuItem; idx++)
	{
		if ( nCustMenus[0].nOrderMenuItemId[idx] == RootId )
			break;
	}

	return idx; 
#endif	
}
 
/**************************************************************

	FUNCTION NAME		: GetNumOfChild(U16 ItemId)

  	PURPOSE				: Function to get Number of Childrens of the Given MenuItemId.

	INPUT PARAMETERS	: (U16) MenuItemId

	OUTPUT PARAMETERS	: (U16) Number of Childrens of the given MenuItemId. 

	RETURNS				: Number of Childrens.

 

**************************************************************/

U16 GetNumOfChild(U16 ItemId)
{
	return nCustMenus[ItemId - 1].nNumofMenuItem; 
}

/**************************************************************

	FUNCTION NAME		: GetStringIdOfItem(U16 ItemId)

  	PURPOSE				: Function to get StringId of the Given MenuItemId.

	INPUT PARAMETERS	: (U16) MenuItemId

	OUTPUT PARAMETERS	: (U16) StringId. 

	RETURNS				: StringId of the given MenuItemId.

 

**************************************************************/

U16 GetStringIdOfItem(U16 ItemId)
{
	return nCustMenus[ItemId - 1].nStrId;  
	
}

/**************************************************************

	FUNCTION NAME		: GetImageIdOfItem(U16 ItemId)

  	PURPOSE				: Function to get ImageId of the given MenuItemId.

	INPUT PARAMETERS	: (U16) MenuItemId

	OUTPUT PARAMETERS	: (U16) ImageId. 

	RETURNS				: ImageId of the given MenuItemId.

 

**************************************************************/

U16 GetImageIdOfItem(U16 ItemId)
{
	
	if(ItemId == MAIN_MENU_CALL_HISTORY)
		ItemId = MAIN_MENU_CALL_HISTORY;
	return nCustMenus[ItemId- 1].nImageId;  
	
}

/**************************************************************

	FUNCTION NAME		: GetDispAttributeOfItem(U16 ItemId)

  	PURPOSE				: Function to get DispAttributes of the given MenuItemId.

	INPUT PARAMETERS	: (U16) MenuItemId

	OUTPUT PARAMETERS	: (U16) DispAttribute. 

	RETURNS				: DispAttribute which is passed to the UI Element.

 

**************************************************************/

U16 GetDispAttributeOfItem(U16 ItemId)
{
	
	return nCustMenus[ItemId- 1].nDispAttribute;   
}

/**************************************************************

	FUNCTION NAME		: GetSeqItemId(U16 ParentItemId,U16 Seq)

  	PURPOSE				: Function to get MenuItemId of the given MenuItemId & Sequence.

	INPUT PARAMETERS	: (U16) MenuItemId i.e Parent & (U16) Sequence w.r.t parent. 

	OUTPUT PARAMETERS	: (U16) MenuItemId. 

	RETURNS				: MenuItemId of the given Parent MenuItemId & Sequence.

 

**************************************************************/

U16 GetSeqItemId(U16 ParentItemId,U16 Seq)
{
	
	return nCustMenus[ParentItemId- 1].nOrderMenuItemId[Seq]; 
	
}

/**************************************************************

	FUNCTION NAME		: GetSeqItemStringId(U16 ParentItemId,U16 Seq)

  	PURPOSE				: Function to get StringId of the given MenuItemId & Sequence.

	INPUT PARAMETERS	: (U16) MenuItemId i.e Parent & (U16) Sequence w.r.t parent. 

	OUTPUT PARAMETERS	: (U16) StringId of the MenuItem. 

	RETURNS				: StringId of the MenuItem of the given Parent MenuItemId & Sequence.

 

**************************************************************/

U16 GetSeqItemStringId(U16 ParentItemId,U16 Seq)
{
	
	return GetStringIdOfItem(nCustMenus[ParentItemId-1].nOrderMenuItemId[Seq]); 
	
}

/**************************************************************

	FUNCTION NAME		: GetSeqItemImageId(U16 ParentItemId,U16 Seq)

  	PURPOSE				: Function to get ImageId of the given MenuItemId & Sequence.

	INPUT PARAMETERS	: (U16) MenuItemId i.e Parent & (U16) Sequence w.r.t parent. 

	OUTPUT PARAMETERS	: (U16) ImageId of the MenuItem. 

	RETURNS				: ImageId of the MenuItem of the given Parent MenuItemId & Sequence.

 

**************************************************************/

U16 GetSeqItemImageId(U16 ParentItemId,U16 Seq)
{
	
	return GetImageIdOfItem(nCustMenus[ParentItemId-1].nOrderMenuItemId[Seq]);
	
}

/**************************************************************

	FUNCTION NAME		: GetSequenceItemIds(U16 ParentItemId,U16* ItemIds)

  	PURPOSE				: Function to get all the MenuItemIds in Sequence of the given MenuItemId i.e Parent.

	INPUT PARAMETERS	: (U16) MenuItemId i.e Parent & (U16 *) PoU16er containing the Array of MenuItemId. 

	OUTPUT PARAMETERS	:  fills the input parameter. Memory is allocated by the caller.

	RETURNS				: Void but fills the array which is taken has input.

 

**************************************************************/

void GetSequenceItemIds(U16 ParentItemId,U16* ItemIds)
{
	U16 j;
	
	for(j=0;j<nCustMenus[ParentItemId- 1].nNumofMenuItem;j++)
		ItemIds[j]=nCustMenus[ParentItemId- 1].nOrderMenuItemId[j];
	
}

/**************************************************************

	FUNCTION NAME		: GetSequenceStringIds(U16 ParentItemId,U16* StringIds)

  	PURPOSE				: Function to get all the StringIds in Sequence of the given MenuItemId i.e Parent.

	INPUT PARAMETERS	: (U16) MenuItemId i.e Parent & (U16 *) PoU16er containing the Array of StringIds. 

	OUTPUT PARAMETERS	:  fills the input parameter. Memory is allocated by the caller.

	RETURNS				: Void but fills the array which is taken has input.

 

**************************************************************/

void GetSequenceStringIds(U16 ParentItemId,U16* StringIds)
{
	U16 j;

	for(j=0;j < nCustMenus[ParentItemId- 1].nNumofMenuItem; j++)
	{
		StringIds[j] = GetStringIdOfItem(nCustMenus[ParentItemId-1].nOrderMenuItemId[j]);
	}
}

/**************************************************************

	FUNCTION NAME		: GetIndexOfStringId(U16 ParentItemId,U16 StringId)

  	PURPOSE				: Function to get the index of StringIds in the given MenuItemId i.e Parent.

	INPUT PARAMETERS	: (U16) MenuItemId i.e Parent & (U16 ) StringId

	OUTPUT PARAMETERS	:  void

	RETURNS				:   U8

 

**************************************************************/
U8 GetIndexOfStringId(U16 ParentItemId,U16 StringId)
{
	U16 j;

	for(j=0;j < nCustMenus[ParentItemId- 1].nNumofMenuItem; j++)
		if (StringId==nCustMenus[ParentItemId-1].nOrderMenuItemId[j])
			return (U8)j;
	return (U8)0;			
}

/**************************************************************

	FUNCTION NAME		: GetSequenceImageIds(U16 ParentItemId,U16* ImageIds)

  	PURPOSE				: Function to get all the ImageIds in Sequence of the given MenuItemId i.e Parent.

	INPUT PARAMETERS	: (U16) MenuItemId i.e Parent & (U16 *) PoU16er containing the Array of ImageIds. 

	OUTPUT PARAMETERS	:  fills the input parameter. Memory is allocated by the caller.

	RETURNS				: Void but fills the array which is taken has input.

 

**************************************************************/

void GetSequenceImageIds(U16 ParentItemId,U16* ImageIds)
{
	U16 j;
	
	for(j=0;j<nCustMenus[ParentItemId- 1].nNumofMenuItem;j++)
		ImageIds[j]=GetImageIdOfItem(nCustMenus[ParentItemId-1].nOrderMenuItemId[j]);
	
}


 
#ifndef MMI_ON_HARDWARE_P
	extern S32 n_MMI_themes;
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
extern theme_details_struct *MMI_themes;
#else
	extern MMI_theme **MMI_themes;
#endif
#else
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
	extern theme_details_struct *MMI_themes;
#else
	extern CONST_HARDWARE MMI_theme **MMI_themes;
#endif
	extern s32 n_MMI_themes;
#endif
 

/**************************************************************

	FUNCTION NAME		: GetThemes

  	PURPOSE				: Retrieves the existing themes 

	INPUT PARAMETERS	: Nil

	OUTPUT PARAMETERS	:  Array of pointers

	RETURNS				: Number of themes

 

**************************************************************/
S32 GetThemesNames(U8  **dest[] )
{
	U8 noOfThemes = 0;

	if( dest == NULL)
	{
		return noOfThemes;
	}

	*dest = NULL;
	return n_MMI_themes; 
}

/**************************************************************

	FUNCTION NAME		: SetThemes

  	PURPOSE				: Set the theme

	INPUT PARAMETERS	: U8 index to  the global theme structure 

	OUTPUT PARAMETERS	: Nil

	RETURNS				: void

 

**************************************************************/
U8 g_curr_thm_index = 0;
extern MMI_theme *current_MMI_theme;

void SetThemes(U8 index)
{
	g_curr_thm_index = index;
//	set_MMI_theme((MMI_theme *)MMI_themes[index]);
//	MMI_apply_current_theme();
}


void UpdateUITheme(void)
{
//dyj del 20060523
#if 1     
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
	if(MMI_themes[g_curr_thm_index].theme != current_MMI_theme)
	{
		set_MMI_theme((MMI_theme *)MMI_themes[g_curr_thm_index].theme);
		MMI_apply_current_theme();
	}

#else
	if(MMI_themes[g_curr_thm_index] != current_MMI_theme)
	{
		set_MMI_theme((MMI_theme *)MMI_themes[g_curr_thm_index]);
		MMI_apply_current_theme();
	}
#endif
#endif
}

/**************************************************************

	FUNCTION NAME		: GetLanguages

  	PURPOSE				: Get the language list.

	INPUT PARAMETERS	: U8*** an array for getting the language list 

	OUTPUT PARAMETERS	: U16

	RETURNS				: the number of languages deployed

 

**************************************************************/
U16 GetLanguages(U8*** pppLanguages)
{
	U16		nCount;

      
	//*pppLanguages = OslMalloc(gMaxDeployedLangs*sizeof(U8*));
	*pppLanguages = &gp_language_struct[0];

	for(nCount=0; (nCount<gMaxDeployedLangs && nCount < MAX_LANGUAGE_NUM) ;++nCount)
	{
		(*pppLanguages)[nCount] = (U8*)gLanguageArray[nCount].aName;	
	//	++nCount;
	}

      
	//return gMaxDeployedLangs;
	return nCount;
}


/**************************************************************

	FUNCTION NAME		: GetLanguageIndex

  	PURPOSE				: Get the index of language specified by ssc_string.

	INPUT PARAMETERS	: U8* ssc_string of lang 

	OUTPUT PARAMETERS	: index (1 based)

	RETURNS				: void

 

**************************************************************/

U16 GetLanguageIndex(S8* ssc_str)
{
	U16 idx;
	
	if( ssc_str == NULL )
		return 0xFFFF;
	
	for (idx=0; idx<gMaxDeployedLangs;idx++)
	{
		if ( strcmp ( ssc_str, (S8*)gLanguageArray[idx].aLangSSC )==0 )
			return idx;
	}
	return 0xFFFF;
}

/**************************************************************

	FUNCTION NAME		: SetCurrentLanguage

  	PURPOSE				: Set the current language to the index.

	INPUT PARAMETERS	: U8 nIndex of lang to set

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
static pBOOL gLanguageSettingOnBootUp = TRUE;
extern void mmi_phb_callergroup_set_name_by_language(void);
#if defined(__MMI_VRSI__)
extern void mmi_vrsi_init_central_change_language_reset(void);
#endif
#if defined (__MMI_GB__)
#include "gbhmi.h"
#endif
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
#include "cutdownflashusage.h"
#endif
void SetCurrentLanguage(S8 nIndex)
{
	if(nIndex>=gMaxDeployedLangs) nIndex=0;
	gCurrLangIndex = nIndex;
	 
	//r2lMMIFlag = IsL2RMMIStyle(); //dyj del 20060523
	//CSD end
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	cdfu_do_init(CDFU_RES);
#endif
	gpStringRes = gStringList[gCurrLangIndex].pStringRes;
	gpStringMapRes = gStringList[gCurrLangIndex].pStringMap;
	gpStrMapSearch = gStringList[gCurrLangIndex].pStrMapSearch;
	CurrMaxStringNum = gStringList[gCurrLangIndex].nTotalStrRes;
	CurrMaxStringId = gStringList[gCurrLangIndex].nTotalStrMap;
	CurrMaxStringIdSearch=gStringList[gCurrLangIndex].nTotalStrMapSearch;
	if( gLanguageSettingOnBootUp == FALSE )
	{
	  // PhnsetReinitHomeCityIdx();  //dyj del 20060514
	 }
	else
		gLanguageSettingOnBootUp = FALSE;

	/*Change caller group name default vaule when change lanaguage*/
	mmi_phb_callergroup_set_name_by_language();  //add by chenq for bug 5923 

#if defined(__MMI_VRSI__)
	/*Reset voice command central command according to the language.*/
	mmi_vrsi_init_central_change_language_reset();
#endif	
//micha1016	
	switch(nIndex) {
		case 0:
			//SATEVDLLangSel((PU8)SAT_LANG_ENGLISH);  //dyj del 20060514  
			return;
		case 1:
		case 2:			
			//SATEVDLLangSel((PU8)SAT_LANG_CHINESE);   //dyj del 20060514 
			return; 
   }
}

 
void ResetLanguageSettingFlag(void)
{
	gLanguageSettingOnBootUp = TRUE;
}
// CSD end

 
/**************************************************************

	FUNCTION NAME		: GetSeqItems

  	PURPOSE				: Get child menuitems.

	INPUT PARAMETERS	: U16 id of parent

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
U16* GetSeqItems(U16 ParentItemId)
{
	return nCustMenus[ParentItemId- 1].nOrderMenuItemId;
}
 

/* APIs for hide some menuitems at run time */

/*****************************************************************************
* FUNCTION
*  mmi_frm_hide_menu_item
* DESCRIPTION
*  Set the bit represented the menuitem on to hide it
*
* PARAMETERS
*  menu_item_id	IN	id of the menuitem
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
void mmi_frm_hide_menu_item(U16 menu_item_id)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 idx=0,reminding=0;
	U16 item_id=menu_item_id-1;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if ( menu_item_id < MAX_MENU_ITEMS_VALUE )
	{
		idx = item_id / 8;
		reminding = item_id - (idx * 8 );
		MenuItemMask[idx] |= (1<<(7-reminding));
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_unhide_menu_item
* DESCRIPTION
*  Reset the bit represented the menuitem on to unhide it
*
* PARAMETERS
*  menu_item_id	IN	id of the menuitem
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
void mmi_frm_unhide_menu_item(U16 menu_item_id)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 idx=0,reminding=0;
	U16 item_id=menu_item_id-1;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if ( menu_item_id < MAX_MENU_ITEMS_VALUE )
	{
		idx = item_id / 8;
		reminding = item_id - (idx * 8 );
		MenuItemMask[idx] &= ~(1<<(7-reminding));
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_test_menu_item_show_status
* DESCRIPTION
*  test if the menuitem is hidden or not
*
* PARAMETERS
*  menu_item_id	IN	id of the menuitem
*
* RETURNS
*  MMI_BOOL	MMI_TRUE if the menuitem is hidden
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
MMI_BOOL mmi_frm_test_menu_item_hide(U16 menu_item_id)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 idx=0,reminding=0;
	U16 item_id=menu_item_id-1;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if ( menu_item_id < MAX_MENU_ITEMS_VALUE )
	{
		idx = item_id / 8;
		reminding = item_id - (idx * 8 );
		if ( (MenuItemMask[idx]>> (7-reminding) ) & 1)
		return MMI_TRUE;
	}
	return MMI_FALSE;
}

/*****************************************************************************
* FUNCTION
*  GetNumOfChild_Ext
*
* DESCRIPTION
*  Function to get Number of Childrens of the Given MenuItemId considering MenuItemMask.
*
* PARAMETERS
*  menu_item_id	IN	id of the menuitem
*
* RETURNS
*  U16	Number of Childrens.
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U16 GetNumOfChild_Ext(U16 menu_item_id)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count=0, num_child=nCustMenus[menu_item_id-1].nNumofMenuItem;
	U8 i=0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	for ( i=0; i<num_child; i++ )
	{
		if ( !mmi_frm_test_menu_item_hide(nCustMenus[menu_item_id-1].nOrderMenuItemId[i]) )
			count++;
	}

	return count;
}

/*****************************************************************************
* FUNCTION
*  GetSeqItemId_Ext
*
* DESCRIPTION
*  Function to get MenuItemId of the given parent_item_id & index considering MenuItemMask.
*
* PARAMETERS
*  parent_item_id		IN	id of the parent menuitem
*  index			IN	index of the child menuitem
*
* RETURNS
*  U16	item id
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U16 GetSeqItemId_Ext(U16 parent_item_id,U16 index)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 i=0,idx = 0;
	U16 item_id = 0;
	U8 child_count = (U8)nCustMenus[parent_item_id- 1].nNumofMenuItem;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for ( i=0; i<child_count; i++ )
	{
		item_id = nCustMenus[parent_item_id- 1].nOrderMenuItemId[i];
		if ( !mmi_frm_test_menu_item_hide(item_id) ) // the item is not hidden
		{
			if ( idx==index )
				break;
			else
				idx++;
		}
	}
	MMI_ASSERT( i<child_count);//all children are hidden
	return item_id ;
}

/*****************************************************************************
* FUNCTION
*  GetSeqItemStringId_Ext
*
* DESCRIPTION
*  Function to get StringId of the given parent_item_id and index considering MenuItemMask.
*
* PARAMETERS
*  parent_item_id		IN	id of the parent menuitem
*  index			IN	index of the child menuitem
*
* RETURNS
*  U16	string id
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U16 GetSeqItemStringId_Ext(U16 parent_item_id,U16 index)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 idx = (U8)GetSeqItemId_Ext(parent_item_id,index);

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	return GetStringIdOfItem(idx);
}

/*****************************************************************************
* FUNCTION
*  GetSeqItemImageId_Ext
*
* DESCRIPTION
*  Function to get Image Id of the given parent_item_id and index considering MenuItemMask.
*
* PARAMETERS
*  parent_item_id		IN	id of the parent menuitem
*  index			IN	index of the child menuitem
*
* RETURNS
*  U16	image id
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U16 GetSeqItemImageId_Ext(U16 parent_item_id,U16 index)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 idx = (U8)GetSeqItemId_Ext(parent_item_id,index);

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	return GetImageIdOfItem(idx);
}

/*****************************************************************************
* FUNCTION
*  GetSequenceItemIds_Ext
*
* DESCRIPTION
*  Function to get MenuItemId of the given parent_item_id considering MenuItemMask.
*
* PARAMETERS
*  parent_item_id		IN	id of the parent menuitem
*  ItemIds			IN/OUT	array of the child menuitem id
*
* RETURNS
*  U16 number of child menuitem
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U16 GetSequenceItemIds_Ext(U16 parent_item_id,U16* ItemIds)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 j,ret_idx=0;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(j=0;j<nCustMenus[parent_item_id- 1].nNumofMenuItem;j++)
	{
		U16 item_id = nCustMenus[parent_item_id- 1].nOrderMenuItemId[j];

		if ( !mmi_frm_test_menu_item_hide( item_id ) )//menuitem id hidden
			ItemIds[ret_idx++]=nCustMenus[parent_item_id- 1].nOrderMenuItemId[j];
	}
	return ret_idx;
}

/*****************************************************************************
* FUNCTION
*  GetSequenceStringIds_Ext
*
* DESCRIPTION
*  Function to get StringId of the given parent_item_id considering MenuItemMask.
*
* PARAMETERS
*  parent_item_id		IN	id of the parent menuitem
*  StringIds			IN/OUT	array of string ids of the child menuitem
*
* RETURNS
*  U16 number of child menuitem
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U16 GetSequenceStringIds_Ext(U16 parent_item_id,U16* StringIds)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 j,ret_idx=0;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(j=0;j<nCustMenus[parent_item_id- 1].nNumofMenuItem;j++)
	{
		U16 item_id = nCustMenus[parent_item_id- 1].nOrderMenuItemId[j];

		if ( !mmi_frm_test_menu_item_hide( item_id ) )//menuitem id hidden
			StringIds[ret_idx++]=GetStringIdOfItem(nCustMenus[parent_item_id- 1].nOrderMenuItemId[j]);
	}
	return ret_idx;
}

/*****************************************************************************
* FUNCTION
*  GetSequenceImageIds_Ext
*
* DESCRIPTION
*  Function to get ImageId of the given parent_item_id considering MenuItemMask.
*
* PARAMETERS
*  parent_item_id		IN	id of the parent menuitem
*  ImageIds			IN/OUT	array of image ids of the child menuitem
*
* RETURNS
*  U16 number of child menuitem
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U16 GetSequenceImageIds_Ext(U16 parent_item_id,U16* ImageIds)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 j,ret_idx=0;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(j=0;j<nCustMenus[parent_item_id- 1].nNumofMenuItem;j++)
	{
		U16 item_id = nCustMenus[parent_item_id- 1].nOrderMenuItemId[j];

		if ( !mmi_frm_test_menu_item_hide( item_id ) )//menuitem id hidden
			ImageIds[ret_idx++]=GetImageIdOfItem(nCustMenus[parent_item_id- 1].nOrderMenuItemId[j]);
	}
	return ret_idx;
}

/*****************************************************************************
* FUNCTION
*  GetIndexOfStringId_Ext
*
* DESCRIPTION
*  Function to get ImageId of the given parent_item_id considering MenuItemMask.
*
* PARAMETERS
*  parent_item_id		IN			id of the parent menuitem
*  StringId			IN			id of the string
*
* RETURNS
*  U8 index
*
* GLOBALS AFFECTED
*  void
*****************************************************************************/
U8 GetIndexOfStringId_Ext(U16 parent_item_id,U16 StringId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 j,ret_idx=0;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(j=0;j<nCustMenus[parent_item_id- 1].nNumofMenuItem;j++)
	{
		U16 item_id = nCustMenus[parent_item_id- 1].nOrderMenuItemId[j];

		if ( !mmi_frm_test_menu_item_hide( item_id ) )//menuitem id hidden
		{
			ret_idx++;
			if (StringId==nCustMenus[parent_item_id-1].nOrderMenuItemId[j])
				return (U8)ret_idx;
		}
	}

	return (U8)0;
}



