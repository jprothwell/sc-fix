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
 * PhoneBookSearch.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for sorting, search, quick search function in Phonebook application
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 #ifndef _PHONEBOOKSEARCH_C
#define _PHONEBOOKSEARCH_C

#define __NEWSIMULATOR
#include "mmi_features.h"

#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
#include "t9ccapi.h"
#include "t9main.h"
#else
#include "t9api.h"
#include "t9main.h"
#endif
#endif

#if defined(__MMI_ZI__)
#include "zi8api.h"

#ifndef ZI8_GETMODE_BOPOMOFO_NAME
#define ZI8_GETMODE_BOPOMOFO_NAME ZI8_GETMODE_BOPOMOFO
#endif

#ifndef ZI8_GETMODE_PINYIN_NAME
#define ZI8_GETMODE_PINYIN_NAME ZI8_GETMODE_PINYIN
#endif

#endif

#if defined(__MMI_ITAP__)
#include "lxs_itap.h"
#include "lxs_defs.h"
#include "itaphmidef.h"
#endif

 
#if defined(__MMI_GB__)
#include "eb.h"
#endif

#include "stdc.h"
#include "mmi_trace.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"
#include "historygprot.h"
#include "queuegprot.h"
#include "mainmenudef.h"
#include "unicodexdcl.h"
#include "globaldefs.h"
#include "commonscreens.h"
#include "timerevents.h"

#include "nvramprot.h"
#include "nvramtype.h"
#include "nvramenum.h"
#include "nvram_data_items.h"

#include "gbhmi.h"//added by zhoumn for the bug of editor full @2007/06/16

#include "settinggenum.h"
#include "securitysetup.h"

#if defined(__MMI_PHB_EMAIL_SORT__)
#include "nvram_user_defs.h"	/*for email sorting. need accessing nvram data*/
#include "nvramprot.h"
#endif

#if defined(__MMI_INTELLIGENT_CALL_ALERT__)
#include "mdi_datatype.h"
#include "mdi_audio.h"
#endif

#ifdef __FLIGHT_MODE_SUPPORT__
#include "simdetectiongexdcl.h"
#endif
#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"
#endif
#include "simdetectiongprot.h"

#undef __NEWSIMULATOR

/*
** Typedef
*/
typedef void (*phb_comp_func)(U16 first, U16 second,  U8 *same_count, S32 *distance);

/*
** Define
*/
#define MMI_PHB_COMPARE_LENGTH 10		/*Compare 10 characters*/
#define MMI_PHB_CONVERT_BUFF_ZI 32 	/*ZI convert function need 32 bytes for buffer.*/
#define MMI_PHB_QUICK_SEARCH_INPUT_LENGTH 6


#define MMI_PHB_SORT_LENGTH 10		/*Compare 10 characters*/
#define MAX_PHB_NUMBER_COMP	20	/*Compare 20 entries for number filed*/

#define BYTE_SIZE                      8 /* for g_phb_qsearch_bitmask */
#define BYTE_SIZE_SHIFT_ARITHMETIC     3


#ifdef __FT_IDLE_SMART_DIAL__ 
extern  S32 SmartDial_number_of_items ;
SmartDialMatch_STRUCT m_smartDialSearchTable[MAX_PB_ENTRIES];
SmartDialMatch_STRUCT m_smartDialListTable[MAX_PB_ENTRIES];
int dialLen = 0;
int preDialLen =  0;
U16    list_count =  0;
int     nowHeap    = 0;        /*目前筛选出的记录数*/
#endif

/*
** Local Variable
*/
#if defined(__MMI_PHB_QUICK_SEARCH__)
static U16	g_phb_qsearch_cache[MAX_PB_ENTRIES];
//static U8	g_phb_qsearch[MAX_PB_ENTRIES];
static U8	g_phb_qsearch_bitmask[(MAX_PB_ENTRIES + 7) / 8];
static U8 g_phb_qsearch_input[(MMI_PHB_QUICK_SEARCH_INPUT_LENGTH + 1) * ENCODING_LENGTH];
static U8 g_phb_qsearch_is_cache = 0;	/*Check if cache already build*/
#endif

#if defined(__MMI_PHB_PINYIN_SORT__)
static U8 g_phb_name_pinyin_index[MAX_PB_ENTRIES][(MMI_PHB_SORT_LENGTH+1)* ENCODING_LENGTH];
#endif

static	U8	g_phb_compare_length = 0xff;	/*number compare length*/

/*
** Global Variable
*/
extern MMI_PHB_LIST_VIEW phbListView;
extern S8 pbName[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];

extern MMI_PHB_ENTRY_BCD_STRUCT	PhoneBook[];
extern U16 g_phb_name_index[];
extern U16	PhoneBookEntryCount;
extern U8 MMI_current_input_mode;

extern MMI_PHB_LOOKUP_NODE_STRUCT LookUpTable[];

#if defined(__MMI_INTELLIGENT_CALL_ALERT__)
extern U8 g_phb_is_speaking_name;
#endif

#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
extern T9CCAWFieldInfo FieldInfo;
#else
extern T9FieldInfo FieldInfo;
#endif
#endif

#if defined(__MMI_ITAP__)
extern LxS_iTAP_IO iTAP_IO;
extern LxS_iTAP *piTAP;
extern ITAPWCHAR   itapRetCodes;
extern U16   StrokeInternalToUnicode(U8 bChar);
#endif

#if defined(__MMI_GB_SM_CHINESE__)
extern void InitGbSMInputMethod(void);
#endif
extern void GbChangeInputMethod(U16 inputMode);
 
#if defined(__MMI_GB__)
#ifdef __MMI_GB_V5__
#include "gbim.h"
extern GBInputStruct g_gbis;
#else
extern EBInputStruct ebis;
#endif
static char buffer[(MMI_PHB_SORT_LENGTH + 1) * ENCODING_LENGTH];// = { 0 };   //guojian modified for GreenStone 10221  
#endif

extern PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;
extern S8 pbEmailAddress[];
U8 gSearchnamecode = 0;
/*
** Local Function
*/
static void mmi_phb_sort_name_fast_quicksort(U16 left, U16 right, phb_comp_func compare);
static void mmi_phb_sort_name_quicksort(U16 left, U16 right, phb_comp_func compare);
static void mmi_phb_sort_name_insertionsort(U16 low, U16 high, phb_comp_func compare);
static void mmi_phb_sort_name_swap(U16 a, U16 b);
/*
** Global Function
*/

#define MMI_PHB_SEARCH_INIT
/*****************************************************************************
* FUNCTION
*  mmi_phb_search_init()
* DESCRIPTION
*   Initial function for phonebook search and sort.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_search_init(void)
{
	SetHiliteHandler(MITEM101_PBOOK_SEARCH_ENTRY	,mmi_phb_highlight_search_name);
	SetHiliteHandler(MITEM1010_SEARCH			,mmi_phb_highlight_search_name_option_search);
	SetHiliteHandler(MITEM1011_INPUT_MEHTHOD	,mmi_phb_highlight_search_name_option_input_method);

#if defined(__MMI_PHB_QUICK_SEARCH__)
	SetHiliteHandler(MENU_ID_PHB_QUICK_SEARCH		,mmi_phb_highlight_quick_search_list);

#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
	SetHiliteHandler(MENU_ID_PHB_QUICK_SEARCH_INPUT_METHOD	,mmi_phb_highlight_quick_search_input_method);
	SetHiliteHandler(MENU_ID_PHB_QUICK_SEARCH_OP_OPTION		,mmi_phb_highlight_quick_search_op_option);
#endif

#endif

}

#define MMI_PHB_SORT
/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_sort_build_name_index(void)
{
	if(PhoneBookEntryCount > 1)
	{
#if defined(__MMI_PHB_PINYIN_SORT__)
		if(g_phb_cntx.sort_type == MMI_PHB_SORT_PINYIN)
			mmi_phb_sort_name_fast_quicksort(0, (U16)(PhoneBookEntryCount - 1), mmi_phb_sort_compare_name_index_by_pinyin_cache);
		else
#endif
		mmi_phb_sort_name_fast_quicksort(0, (U16)(PhoneBookEntryCount - 1), mmi_phb_sort_compare_name_index_by_encoding);
}
}

/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
U16 mmi_phb_sort_insert_name_index(U16 store_index)
{
	U16 pos;
	U16 i;

	if(PhoneBookEntryCount == 0)
	{
		g_phb_name_index[PhoneBookEntryCount] = store_index;
		PhoneBookEntryCount++;
		return 0;
	}
	else
	{
		pos = mmi_phb_sort_insert_name_index_find_pos(0, (PhoneBookEntryCount-1), store_index);

		for(i = PhoneBookEntryCount ; i > pos ; i--)
		{
			g_phb_name_index[i] = g_phb_name_index[i-1];
		}
		g_phb_name_index[pos] = store_index;
		PhoneBookEntryCount++;

		return pos;
	}
}

/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
U16 mmi_phb_sort_insert_name_index_find_pos(S32 low, S32 high, U16 store_index)
{
	S32 mid = 0;
	S16 flag = 0;
	U8 same_count;
	S32 distance;

	while(low <= high)
	{
		mid = (low + high)/2;

#if defined(__MMI_PHB_PINYIN_SORT__)
		if(g_phb_cntx.sort_type == MMI_PHB_SORT_PINYIN)
			mmi_phb_sort_compare_name_index_by_pinyin(store_index,g_phb_name_index[mid], &same_count, &distance);
		else
#endif
		mmi_phb_sort_compare_name_index_by_encoding(store_index,g_phb_name_index[mid], &same_count, &distance);

		if( distance > 0)
		{
			low = mid + 1;
			flag = -1;
		}
		else /*Either > 0 or < 0, not possible ==0*/
		{
			high = mid - 1;
			flag = 1;
		}
	}

	if(flag == -1)
		return (U16)(mid + 1);
	else
		return (U16)mid;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_compare_name_index()
* DESCRIPTION
*   Compare two phonebook by name string. If name is empty use number instead.
*
* PARAMETERS
*  first				IN			store_index of first compare element
*  second			IN			store_index of second compare element
*  string1		IN	First name string
*  string2		IN	Second name string
*  same_count	IN/OUT	same count of two element
*  distance			IN/OUT	difference of two element
*
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sort_compare_name_index(U16 first, U16 second, S8* string1, S8* string2,U8 *same_count, S32 *distance)
{
	S8 number1[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
	S8 number2[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];

	/*Use number to sort if name empty*/
	if(pfnUnicodeStrlen(string1) == 0)
	{
		mmi_phb_convert_get_ucs2_number(number1, first);
		string1 = (S8*)number1;
	}

	/*Use number to sort if name empty*/
	if(pfnUnicodeStrlen(string2) == 0)
	{
		mmi_phb_convert_get_ucs2_number(number2, second);
		string2 = (S8*)number2;
	}

	/*Compare two strings*/
	mmi_phb_compare_ucs2_strings(string1, string2, same_count, distance);

	/*sort according storage location, let entry in NVRAM put before in SIM. (store_index can decide store location)*/
	if(*distance == 0)
	{
		if(first < second)
			*distance = -1;
		else if(first > second)
			*distance = 1;
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_compare_name_index_by_encoding()
* DESCRIPTION
*   Compare two phonebook entry element by encoding.
*
* PARAMETERS
*  first			IN		store_index of first compare element
*  second			IN		store_index of second compare element
*  same_count	IN/OUT	same count of two element
*  distance		IN/OUT	difference of two element
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sort_compare_name_index_by_encoding(U16 first, U16 second,  U8 *same_count, S32 *distance)
{
	S8* string1, * string2;

	string1 = (S8*)PhoneBook[first].alpha_id.name;
	string2 = (S8*)PhoneBook[second].alpha_id.name;

	mmi_phb_sort_compare_name_index(first, second,string1,string2,same_count,distance);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_compare_name_index_by_pinyin_cache()
* DESCRIPTION
*   Compare two phonebook entry element by pinyin by using prior convert result.
*
* PARAMETERS
*  first			IN		store_index of first compare element
*  second			IN		store_index of second compare element
*  same_count	IN/OUT	same count of two element
*  distance		IN/OUT	difference of two element
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__MMI_PHB_PINYIN_SORT__)
void mmi_phb_sort_compare_name_index_by_pinyin_cache(U16 first, U16 second,  U8 *same_count, S32 *distance)
{
	S8* string1, * string2;

	if(PhoneBook[first].alpha_id.name_dcs == MMI_PHB_ASCII)
		string1 = (S8*)PhoneBook[first].alpha_id.name;
	else
		string1 = (S8*)g_phb_name_pinyin_index[first];

	if(PhoneBook[second].alpha_id.name_dcs == MMI_PHB_ASCII)
		string2 = (S8*)PhoneBook[second].alpha_id.name;
	else
		string2 = (S8*)g_phb_name_pinyin_index[second];

	mmi_phb_sort_compare_name_index(first, second,string1,string2,same_count,distance);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_compare_name_index_by_pinyin()
* DESCRIPTION
*   Compare two phonebook entry element by pinyin.
*
* PARAMETERS
*  first			IN		store_index of first compare element
*  second			IN		store_index of second compare element
*  same_count	IN/OUT	same count of two element
*  distance		IN/OUT	difference of two element
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sort_compare_name_index_by_pinyin(U16 first, U16 second,  U8 *same_count, S32 *distance)
{
	S8* string1, * string2;

	S8 buffer1[(MMI_PHB_SORT_LENGTH+1)*ENCODING_LENGTH];
	S8 buffer2[(MMI_PHB_SORT_LENGTH+1)*ENCODING_LENGTH];

	U16 count;

	if(PhoneBook[first].alpha_id.name_dcs == MMI_PHB_ASCII)
		string1 = (S8*)PhoneBook[first].alpha_id.name;
	else
	{
		count = mmi_phb_util_convert_to_pinyin(PhoneBook[first].alpha_id.name, (U8*)buffer1, (MMI_PHB_SORT_LENGTH*ENCODING_LENGTH));
		buffer1[count++] = 0x00;
		buffer1[count] = 0x00;
		string1 = (S8*)buffer1;
	}

	if(PhoneBook[second].alpha_id.name_dcs == MMI_PHB_ASCII)
		string2 = (S8*)PhoneBook[second].alpha_id.name;
	else
	{
		count = mmi_phb_util_convert_to_pinyin(PhoneBook[second].alpha_id.name, (U8*)buffer2, (MMI_PHB_SORT_LENGTH*ENCODING_LENGTH));
		buffer2[count++] = 0x00;
		buffer2[count] = 0x00;
		string2 = (S8*)buffer2;
	}

	mmi_phb_sort_compare_name_index(first, second,string1,string2,same_count,distance);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_name_fast_quicksort()
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_sort_name_fast_quicksort(U16 left, U16 right, phb_comp_func compare)
{
   mmi_phb_sort_name_quicksort(left, right, compare);
   mmi_phb_sort_name_insertionsort(left, right, compare);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_name_quicksort()
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_sort_name_quicksort(U16 left, U16 right, phb_comp_func compare)
{
	U16 const Min = 4;	/*Lower bound to sort*/
	U16 i;
	U16 j;
	U16 pivot;

	S32 distance;
	U8 same_count;

#ifndef MMI_ON_WIN32	
	DBG_ASSERT((compare != NULL), "compare is NULL!");
#endif

	if ((right - left) > Min)
	{
      	i = (right + left) / 2;

		/* left > mid */
		(*compare)(g_phb_name_index[left], g_phb_name_index[i], &same_count, &distance);
		if (distance > 0)
			mmi_phb_sort_name_swap(left, i);

		/* left > right */
		(*compare)(g_phb_name_index[left], g_phb_name_index[right], &same_count, &distance);
		if (distance > 0)
			mmi_phb_sort_name_swap(left, right);

		/* mid > right */
		(*compare)(g_phb_name_index[i], g_phb_name_index[right], &same_count, &distance);
		if (distance > 0)
			mmi_phb_sort_name_swap(i, right);

		j = right - 1;
		mmi_phb_sort_name_swap(i , j);

		i = left;

		pivot = g_phb_name_index[j];/*kal_mem_cpy(&v, (void const*)&a[j], sizeof(pindex_struct));*/

		for(;;)
		{
			do /* ++i < pivot */
			{
				(*compare)(g_phb_name_index[++i], pivot, &same_count, &distance);
			}
			while(distance < 0);


			do /* --j > pivot */
			{
				(*compare)(g_phb_name_index[--j], pivot, &same_count, &distance);
			}
			while(distance > 0);

			if (j < i)
				break;

			mmi_phb_sort_name_swap (i, j);
		}

		mmi_phb_sort_name_swap(i, (U16)(right - 1));
		mmi_phb_sort_name_quicksort(left, j, compare);
		mmi_phb_sort_name_quicksort((U16)(i + 1), right, compare);
	}
}  /* end of phb_quicksort */

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_name_insertionsort()
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_sort_name_insertionsort(U16 low, U16 high, phb_comp_func compare)
{
	U16 i;
	U16 j;
	U16 pivot;

	S32 distance;
	U8 same_count;

	ASSERT(compare != NULL);

	for (i = low + 1; i <= high; ++i)
	{
		pivot = g_phb_name_index[i];

		j = i;
		while (j > low)
		{
			(*compare)(g_phb_name_index[j - 1], pivot, &same_count, &distance);
			if (distance <= 0)
				break;

			g_phb_name_index[j] = g_phb_name_index[j-1];

			j--;
		}

		g_phb_name_index[j] = pivot;
	}
}  /* end of phb_insertion_sort */

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_name_swap()
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_sort_name_swap(U16 a, U16 b)
{
	U16 tmp;

	tmp = g_phb_name_index[a];
	g_phb_name_index[a] = g_phb_name_index[b];
	g_phb_name_index[b] = tmp;
}



#define MMI_PHB_SEARCH_NAME

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_search_name()
* DESCRIPTION
*   Highlight handler for "Search Entry" in PHB main menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_search_name(void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_highlight_search_name.>\n",__FILE__,__LINE__));

	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(mmi_phb_search_name_pre_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_search_name_pre_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	phbListView = MMI_PHB_LIST_FOR_PHB;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_search_name_option_search()
* DESCRIPTION
*   Highlight handler for "Search" in Search Option menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_search_name_option_search(void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_highlight_search_name_option_search.>\n",__FILE__,__LINE__));

	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction (mmi_phb_search_name, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_search_name, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_search_name_option_input_method()
* DESCRIPTION
*   Highlight handler for "Input Method" in Search Option menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_search_name_option_input_method(void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_highlight_search_name_option_input_method.>\n",__FILE__,__LINE__));

	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
 
	SetInputMethodAndDoneCaptionIcon(IMG_SCR_PBOOK_CAPTION); // zrx del 20060612
	SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP);  // zrx del 20060612
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EntryInputMethodScreen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);  // zrx del 20060612 
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_search_name_pre_entry()
* DESCRIPTION
*   Calls mmi_phb_entry_search_name() after few checks
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_search_name_pre_entry(void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_search_name_pre_entry.>\n",__FILE__,__LINE__));

	if (g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		memset(pbName, 0, ENCODING_LENGTH);
		if(PhoneBookEntryCount)
			mmi_phb_entry_search_name();
		else
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_search_name()
* DESCRIPTION
*   Gets name input from user
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_search_name(void)
{
	U8* guiBuffer;
	S8	state;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_search_name.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_SEARCH_NAME_ENTRY, mmi_phb_exit_search_name, NULL,NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_SEARCH_NAME_ENTRY);

	if(guiBuffer != NULL)
	{
		state = get_multiline_inputbox_category_history_state(SCR_SEARCH_NAME_ENTRY,guiBuffer);
		if(state != -1)
			phbListView = (U8)state;
	}

	ShowCategory5Screen_ext(STR_SCR_SEARCH_NAME_CAPTION, IMG_SCR_PBOOK_CAPTION,
							  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  (INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_USE_ENCODING_BASED_LENGTH|INPUT_TYPE_ONE_LESS_CHARACTER),
							  (PU8)pbName, MAX_SEARCH_NAME_LENGTH + 1, guiBuffer,INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL,
							  /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 14:23 */
							  NULL,
							  NULL);
							  /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 14:23 */
#if defined(__MMI_GB__)
	SetLeftSoftkeyFunction(mmi_phb_entry_search_name_option,KEY_EVENT_UP);
	GBSetInputboxLSKFunction(mmi_phb_entry_search_name_option);                                 //add by yaosq 20061108
#endif
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_search_name, KEY_SEND, KEY_EVENT_DOWN);
	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_search_name()
* DESCRIPTION
*   Exit function for mmi_phb_entry_search_name()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_search_name(void)
{
	history_t   h;
	S8 state = (S8)phbListView;

	if (g_phb_cntx.start_scr_id == 0) /* start from Shortcut */
	{
		g_phb_cntx.start_scr_id = GetCurrScrnId();
	}
	g_phb_cntx.end_scr_id = SCR_SEARCH_NAME_ENTRY;

	h.scrnID = SCR_SEARCH_NAME_ENTRY;
	h.entryFuncPtr = mmi_phb_entry_search_name;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, ( S8*)pbName);
	GetCategoryHistory(h.guiBuffer);
	set_multiline_inputbox_category_history_state(SCR_SEARCH_NAME_ENTRY,h.guiBuffer,state);
	AddHistory(h);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_search_name_option()
* DESCRIPTION
*   Draws the Search Options menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_search_name_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[2];
	U16 nNumofItem;
	S8 state;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_search_name_option.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_SEARCH_OPTIONS, mmi_phb_exit_search_name_option, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_SEARCH_OPTIONS);

	if(guiBuffer != NULL)
	{
		state = get_list_menu_category_history_state(SCR_PBOOK_LIST,guiBuffer);
		if (state != -1)
			phbListView = (U8)state;	 /*Restore State in history.*/
	}

	nNumofItem = GetNumOfChild(MITEM101_SEARCH_OPTIONS);
	GetSequenceStringIds(MITEM101_SEARCH_OPTIONS, nStrItemList);
	SetParentHandler(MITEM101_SEARCH_OPTIONS);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ShowCategory15Screen(STR_GLOBAL_OPTIONS, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
 
	RegisterInputMethodScreenCloseFunction(mmi_phb_go_back_2_history);  // zrx del 20060612
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_search_name_option()
* DESCRIPTION
*   Exit function for mmi_phb_entry_search_name_option()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_search_name_option(void)
{
	history_t   h;
	S16		nHistory	=	0;
	S8 state = (S8)phbListView;

	g_phb_cntx.end_scr_id = SCR_SEARCH_OPTIONS;

	h.scrnID = SCR_SEARCH_OPTIONS;
	h.entryFuncPtr = mmi_phb_entry_search_name_option;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);
	set_list_menu_category_history_state(SCR_PBOOK_LIST,h.guiBuffer,state); /*Keep State in history.*/
	AddHistory(h);
}

/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_search_name(void)
{
	U16 highlight_entry =0;
	
	highlight_entry = mmi_phb_search_name_find_closet();

	/*zhangm Add Start For 10825 Ver: GreenStone  on 2008-12-18 11:21 */
	if(gSearchnamecode == 0)
	{
		DisplayPopup((PU8)GetString(STR_NO_THIS_ENTRIE_MESSAGE), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		DeleteScreenIfPresent(SCR_SEARCH_OPTIONS); 
		return;
	}
	/*zhangm Add End  For 10825 Ver: GreenStone  on 2008-12-18 11:21 */
	
	g_phb_cntx.highlight_entry = highlight_entry;
	if(phbListView == MMI_PHB_LIST_FOR_PHB)
		mmi_phb_list_pre_entry();	/*Dynamic List*/
	else if((phbListView == MMI_PHB_LIST_FOR_SMS) || (phbListView == MMI_PHB_LIST_FOR_SMS_SEARCH_NAME))
		mmi_phb_list_pre_entry_second_level();

	DeleteNScrId(SCR_SEARCH_NAME_ENTRY);

	/*Not Found, Not Possible, will return closet entry*/
	//	DisplayPopup((PU8)GetString(STR_NOENTRY_FOUND_MSG), IMG_GLOBAL_WARNING, FALSE, PHB_NOTIFY_TIMEOUT, WARNING_TONE);
	//	DeleteNScrId(SCR_SEARCH_NAME_ENTRY);

}

/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
U16 mmi_phb_search_name_find_closet(void)
{
	U16 result_index;

	/*zhangm Add Start For 10953 Ver: GreenStone  on 2008-12-22 11:26 */
	#if 0
	if(pfnUnicodeStrlen((S8*)pbName) == 0)
		return 0;
	#endif
	/*zhangm Add End  For 10953 Ver: GreenStone  on 2008-12-22 11:26 */

	//result_index = mmi_phb_search_name_binary_search(0, (PhoneBookEntryCount - 1), (S8*)pbName);

	result_index = mmi_phb_search_name_code_search((S8*)pbName);
	
	/*Check boundary*/
	if(result_index >= PhoneBookEntryCount)
	{
		result_index = PhoneBookEntryCount - 1;
	}

	return result_index;
}

U16 mmi_phb_search_name_code_search(S8* ucs2_name)
{
	S32 index = 0;
	S32 Phonebookindex = 0;
	S8 *pName = NULL;
	S8 *pPhoneBookName = NULL;

	for(index = 0; index < PhoneBookEntryCount; index++)
	{
		Phonebookindex = g_phb_name_index[index];
		pPhoneBookName = PhoneBook[Phonebookindex].alpha_id.name;
		pName = ucs2_name;
	
		while(!(*pName == 0x00 && *(pName + 1) == 0x00) && !(*pPhoneBookName == 0x00 && *(pPhoneBookName + 1) == 0x00))
		{
			if(*pName == *pPhoneBookName)
			{
				pName++;
				pPhoneBookName++;					
			}
			else
			{
				break;
			}
		}

		if(*pName == 0x00 && *(pName + 1) == 0x00)
		{
			gSearchnamecode = 1;
			return index;
		}
	}

	/*zhangm Add Start For 10825 Ver: GreenStone  on 2008-12-18 11:21 */
	if(index >= PhoneBookEntryCount)
	{
		gSearchnamecode = 0;
	}
	else
	{
		gSearchnamecode = 1;	
	}
	/*zhangm Add End For 10825 Ver: GreenStone  on 2008-12-18 11:21 */

	return index;
}

/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
U16 mmi_phb_search_name_binary_search(S32 low, S32 high, S8* ucs2_name)
{
	S32 mid = 0;
	S16 flag = 0;
	U8 same_count;
	S32 distance;
	U16 index;
	S8* string;
	S8 number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];


#if defined(__MMI_PHB_PINYIN_SORT__)
	S8 buffer1[(MMI_PHB_SORT_LENGTH+1)*ENCODING_LENGTH];
	S8 buffer2[(MMI_PHB_SORT_LENGTH+1)*ENCODING_LENGTH];
	U16 count;


	if(GetUCS2Flag(ucs2_name))
	{
		count = mmi_phb_util_convert_to_pinyin((U8*)ucs2_name, (U8*)buffer1, (MMI_PHB_SORT_LENGTH*ENCODING_LENGTH));
		buffer1[count++] = 0x00;
		buffer1[count] = 0x00;
		ucs2_name = (S8*)buffer1;
	}
#endif

	/*
	* Use binary search to find nearest name.
	*/
	while(low <= high)
	{
		mid = (low + high)/2;

		/*
		* Prepare entry data to compare
		*/
		index = g_phb_name_index[mid];

#if defined(__MMI_PHB_PINYIN_SORT__)
		if(PhoneBook[index].alpha_id.name_dcs == MMI_PHB_ASCII)
			string = (S8*)PhoneBook[index].alpha_id.name;
		else
		{
			count = mmi_phb_util_convert_to_pinyin(PhoneBook[index].alpha_id.name, (U8*)buffer2, (MMI_PHB_SORT_LENGTH*ENCODING_LENGTH));
			buffer2[count++] = 0x00;
			buffer2[count] = 0x00;
			string = (S8*)buffer2;
		}
#else
		string = (S8*)PhoneBook[index].alpha_id.name;
#endif

		/*
		* Use number to compare if name is empty
		*/
		if(pfnUnicodeStrlen(string) == 0)
		{
			mmi_phb_convert_get_ucs2_number(number, index);
			string = (S8*)number;
		}

		/*
		* Compare search pattern with entry
		*/
		mmi_phb_compare_ucs2_strings(ucs2_name, string, &same_count, &distance);

		if( distance > 0)
		{
			low = mid + 1;
			flag = -1;
		}
		/*
		* Distance == 0 or < 0 here
		* If there are duplicate entry, this will help to move up until first entry.
		*/
		else
		{
			high = mid - 1;
			flag = 1;
		}
	}
	
	if(flag == -1)
		return (U16)(mid + 1);
	else
		return (U16)mid;
}

#define MMI_PHB_QUICKSEARCH
#if defined(__MMI_PHB_QUICK_SEARCH__)
#if defined(__MMI_T9__) && (defined(__MMI_LANG_TR_CHINESE__) || defined(__MMI_LANG_SM_CHINESE__))
#if defined(__MMI_T9_V7__)
/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL  mmi_phb_quick_search_convert_to_spelling(U8* input_string, U8* output_string, U8* out_len)
{
	S8	out = 0;
	U8	i = 0;
	T9STATUS	  	    t9ReturnCodes;

	/*Should already switch to the proper input language in "mmi_phb_quick_search_convert_cache()".*/

	if(input_string[0] != 0 || input_string[1] != 0)
	{
		if(MMI_current_input_mode == INPUT_MODE_MMI_SM_STROKE || MMI_current_input_mode == INPUT_MODE_MMI_TR_STROKE)
			t9ReturnCodes = T9CCGetCharStrokes(&FieldInfo.CCFieldInfo,(U16*)input_string,0);
		else
			t9ReturnCodes = T9CCGetCharSpell(&FieldInfo.CCFieldInfo,(U16*)input_string,0);

		if(t9ReturnCodes == 0)
		{
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MMI_MULTITAP_BPMF:
					while ((FieldInfo.CCFieldInfo.pbSpellBuf[i] != 0x00) && (i <= 7))
					{
						*((U16*)(output_string + out)) = BPMFInternalToUnicode(FieldInfo.CCFieldInfo.pbSpellBuf[i]);
						out = out + 2;
						i = i + 1;
					}
					break;

				case INPUT_MODE_MMI_MULTITAP_PINYIN:
					while ((FieldInfo.CCFieldInfo.pbSpellBuf[i] != 0x00) && (i <= 7))
					{
						output_string[out] = FieldInfo.CCFieldInfo.pbSpellBuf[i];
						output_string[out+1] = 0;
						out = out + 2;
						i = i + 1;
					}
					break;

		        	case INPUT_MODE_MMI_SM_STROKE:
		        	case INPUT_MODE_MMI_TR_STROKE:
					while ((FieldInfo.CCFieldInfo.pbKeyBuf[i] != 0x00) && \
							(i < FieldInfo.CCFieldInfo.nKeyBufLen) && \
							 (i <= 7))
					{
			        		*((U16*)(output_string+out)) = StrokeInternalToUnicode(FieldInfo.CCFieldInfo.pbKeyBuf[i]);
						out = out + 2;
						i = i + 1;
					}
					break;

				default:
					break;
			}

			*out_len = i;
			return TRUE;
		}

		/*Input String Does not Converted.*/
		memcpy(output_string,input_string,2);
		*out_len = 1;
		return TRUE;
	}

	*out_len = 0;
	return FALSE;

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_cache
* DESCRIPTION
*   This function convert first character of name field into cache.
*
* PARAMETERS
*  index	IN	phonebook entry index
* RETURNS
*  void
* GLOBALS AFFECTED
*   g_phb_qsearch_cache
*****************************************************************************/
void mmi_phb_quick_search_convert_cache(U16 index)
{
	T9STATUS	  	    t9ReturnCodes;

	/*move change language function out from loop to speed up*/

	if(MMI_current_input_mode == INPUT_MODE_MMI_SM_STROKE || MMI_current_input_mode == INPUT_MODE_MMI_TR_STROKE)
		t9ReturnCodes = T9CCGetCharStrokes(&FieldInfo.CCFieldInfo,(U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name,0);
	else
		t9ReturnCodes = T9CCGetCharSpell(&FieldInfo.CCFieldInfo,(U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name,0);

	if(t9ReturnCodes == 0)
	{
		switch(MMI_current_input_mode)
		{
			case INPUT_MODE_MMI_MULTITAP_BPMF:
				g_phb_qsearch_cache[index] = BPMFInternalToUnicode(FieldInfo.CCFieldInfo.pbSpellBuf[0]);
				break;

			case INPUT_MODE_MMI_MULTITAP_PINYIN:
				g_phb_qsearch_cache[index] = FieldInfo.CCFieldInfo.pbSpellBuf[0];
				break;

			case INPUT_MODE_MMI_SM_STROKE:
			case INPUT_MODE_MMI_TR_STROKE:
	        		g_phb_qsearch_cache[index] = StrokeInternalToUnicode(FieldInfo.CCFieldInfo.pbKeyBuf[0]);
				break;

			default:
				g_phb_qsearch_cache[index] = *((U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name);
				break;
		}
	}
	else
	{
		g_phb_qsearch_cache[index] = *((U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name);
	}

}
#else
/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL  mmi_phb_quick_search_convert_to_spelling(U8* input_string, U8* output_string, U8* out_len)
{
	S8	out = 0;
	U8	i = 0;
	T9Event             t9EventInput;
	T9STATUS	  	    t9ReturnCodes;

	/*Should already switch to the proper input language in "mmi_phb_quick_search_convert_cache()".*/

	if(input_string[0] != 0 || input_string[1] != 0)
	{
		if(MMI_current_input_mode == INPUT_MODE_MMI_SM_STROKE || MMI_current_input_mode == INPUT_MODE_MMI_TR_STROKE)
			t9EventInput.mType = T9EVTSTROKES;
		else
			t9EventInput.mType = T9EVTSPELLING;
		t9EventInput.data.sLDBData.psBuf = (U16*)input_string;
		t9EventInput.data.sLDBData.mCtrlID = 0;

		t9ReturnCodes = T9HandleEvent(&FieldInfo, &t9EventInput);

		if(t9ReturnCodes == 0)
		{
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MMI_MULTITAP_BPMF:
					while ((FieldInfo.uLangData.pAuxChinese->pbSpellBuf[i] != 0x00) && (i <= 7))
					{
						*((U16*)(output_string + out)) = BPMFInternalToUnicode(FieldInfo.uLangData.pAuxChinese->pbSpellBuf[i]);
						out = out + 2;
						i = i + 1;
					}
					break;

				case INPUT_MODE_MMI_MULTITAP_PINYIN:
					while ((FieldInfo.uLangData.pAuxChinese->pbSpellBuf[i] != 0x00) && (i <= 7))
					{
						output_string[out] = FieldInfo.uLangData.pAuxChinese->pbSpellBuf[i];
						output_string[out+1] = 0;
						out = out + 2;
						i = i + 1;
					}
					break;

		        	case INPUT_MODE_MMI_SM_STROKE:
		        	case INPUT_MODE_MMI_TR_STROKE:
					while ((FieldInfo.uLangData.pAuxChinese->pbKeyBuf[i] != 0x00) && \
							 (i < FieldInfo.uLangData.pAuxChinese->nKeyBufLen) && \
							 (i <= 7))
					{
			        		*((U16*)(output_string+out)) = StrokeInternalToUnicode(FieldInfo.uLangData.pAuxChinese->pbKeyBuf[i]);
						out = out + 2;
						i = i + 1;
					}
					break;

				default:
					break;
			}

			*out_len = i;
			return TRUE;
		}

		/*Input String Does not Converted.*/
		memcpy(output_string,input_string,2);
		*out_len = 1;
		return TRUE;
	}

	*out_len = 0;
	return FALSE;

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_cache
* DESCRIPTION
*   This function convert first character of name field into cache.
*
* PARAMETERS
*  index	IN	phonebook entry index
* RETURNS
*  void
* GLOBALS AFFECTED
*   g_phb_qsearch_cache
*****************************************************************************/
void mmi_phb_quick_search_convert_cache(U16 index)
{
	T9Event             t9EventInput;
	T9STATUS	  	    t9ReturnCodes;

	/*move change language function out from loop to speed up*/

	if(MMI_current_input_mode == INPUT_MODE_MMI_SM_STROKE || MMI_current_input_mode == INPUT_MODE_MMI_TR_STROKE)
		t9EventInput.mType = T9EVTSTROKES;
	else
		t9EventInput.mType = T9EVTSPELLING;
	t9EventInput.data.sLDBData.psBuf = (U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name;
	t9EventInput.data.sLDBData.mCtrlID = 0;

	t9ReturnCodes = T9HandleEvent(&FieldInfo, &t9EventInput);

	if(t9ReturnCodes == 0)
	{
		switch(MMI_current_input_mode)
		{
			case INPUT_MODE_MMI_MULTITAP_BPMF:
				g_phb_qsearch_cache[index] = BPMFInternalToUnicode(FieldInfo.uLangData.pAuxChinese->pbSpellBuf[0]);
				break;

			case INPUT_MODE_MMI_MULTITAP_PINYIN:
				g_phb_qsearch_cache[index] = FieldInfo.uLangData.pAuxChinese->pbSpellBuf[0];
				break;

			case INPUT_MODE_MMI_SM_STROKE:
			case INPUT_MODE_MMI_TR_STROKE:
	        		g_phb_qsearch_cache[index] = StrokeInternalToUnicode(FieldInfo.uLangData.pAuxChinese->pbKeyBuf[0]);
				break;

			default:
				g_phb_qsearch_cache[index] = *((U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name);
				break;
		}
	}
	else
	{
		g_phb_qsearch_cache[index] = *((U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name);
	}

}
#endif
#elif defined(__MMI_ZI__)
/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL  mmi_phb_quick_search_convert_to_spelling(U8* input_string, U8* output_string, U8* out_len)
{
	S8	out = 0;
	U8	i = 0;
	U16 code;
	U16 mode;
	U16 pCharInfoBuffer[MMI_PHB_CONVERT_BUFF_ZI];
	U16 CharInfo;

	/*Change convert mode according to input method.*/
	switch(MMI_current_input_mode)
	{
		case INPUT_MODE_MMI_MULTITAP_BPMF:
		      //mode = ZI8_GETMODE_BOPOMOFO;
		      mode = ZI8_GETMODE_BOPOMOFO_NAME;
			break;

		case INPUT_MODE_MMI_MULTITAP_PINYIN:
			//mode = ZI8_GETMODE_PINYIN;
			mode = ZI8_GETMODE_PINYIN_NAME;
			break;

		case INPUT_MODE_MMI_SM_STROKE:
		case INPUT_MODE_MMI_TR_STROKE:
	       	mode = ZI8_GETMODE_STROKES;
			break;

		default:
			return FALSE;
			break;
	}

	if(input_string[0] != 0 || input_string[1] != 0)
	{
		memcpy(&code ,input_string,2);

		if ((code >= 0x4e00) && (code <= 0x9fa5) &&
			Zi8GetCharInfo(code, (U16*)pCharInfoBuffer, MMI_PHB_CONVERT_BUFF_ZI, mode))
		{
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MMI_MULTITAP_BPMF:
					/*Do not convert tone*/
					while ((pCharInfoBuffer[i] != 0x0000) && ((pCharInfoBuffer[i] < 0xf431) || (pCharInfoBuffer[i] > 0xf435)))
					{
						CharInfo = pCharInfoBuffer[i] - ZI8_BASE_BPMF + 0x3105;
						memcpy(output_string+out, &CharInfo, 2);
						out = out + 2;
						i = i + 1;
					}
				break;

				case INPUT_MODE_MMI_MULTITAP_PINYIN:
					/*Do not convert tone*/
					while ((pCharInfoBuffer[i] != 0x0000) && ((pCharInfoBuffer[i] < 0xf431) || (pCharInfoBuffer[i] > 0xf435)))
					{
						CharInfo = pCharInfoBuffer[i] - ZI8_BASE_PINYIN + 0x0061;
						memcpy(output_string+out, &CharInfo, 2);
						out = out + 2;
						i = i + 1;
					}
					break;

				case INPUT_MODE_MMI_SM_STROKE:
				case INPUT_MODE_MMI_TR_STROKE:
					while ((pCharInfoBuffer[i] != 0x0000) && (i <= 7))
					{
						memcpy(output_string+out, &pCharInfoBuffer[i], 2);
						out = out + 2;
						i = i + 1;
					}
				break;

				default:
				break;
			}

			*out_len = i;
			return TRUE;

		}

		memcpy(output_string,input_string,2);
		*out_len = 1;
		return TRUE;
	}

	*out_len = 0;
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_cache
* DESCRIPTION
*   This function convert first character of name field into cache.
*
* PARAMETERS
*  index	IN	phonebook entry index
* RETURNS
*  void
* GLOBALS AFFECTED
*   g_phb_qsearch_cache
*****************************************************************************/
void mmi_phb_quick_search_convert_cache(U16 index)
{
	U16 code;
	U16 mode;
	U16 pCharInfoBuffer[MMI_PHB_CONVERT_BUFF_ZI];

	memcpy(&code, PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);

	/*Change to proper input method.*/
	switch(MMI_current_input_mode)
	{
		case INPUT_MODE_MMI_MULTITAP_BPMF:
			mode = ZI8_GETMODE_BOPOMOFO;
			break;

		case INPUT_MODE_MMI_MULTITAP_PINYIN:
			mode = ZI8_GETMODE_PINYIN;
			break;

		case INPUT_MODE_MMI_SM_STROKE:
		case INPUT_MODE_MMI_TR_STROKE:
        		mode = ZI8_GETMODE_STROKES;
			break;

		default:
			memcpy(&g_phb_qsearch_cache[index], PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);
			return;
	}

	/*Begin to convert and store the result according to the input method.*/
	if ((code >= 0x4e00) && (code <= 0x9fa5) && Zi8GetCharInfo(code, (U16*)pCharInfoBuffer, MMI_PHB_CONVERT_BUFF_ZI, mode))
	{
		switch(MMI_current_input_mode)
		{
			case INPUT_MODE_MMI_MULTITAP_BPMF:
				g_phb_qsearch_cache[index] = pCharInfoBuffer[0] -ZI8_BASE_BPMF + 0x3105;
				break;

			case INPUT_MODE_MMI_MULTITAP_PINYIN:
				g_phb_qsearch_cache[index] = pCharInfoBuffer[0] - ZI8_BASE_PINYIN + 0x0061;
				break;

			case INPUT_MODE_MMI_SM_STROKE:
			case INPUT_MODE_MMI_TR_STROKE:
	        		g_phb_qsearch_cache[index] = pCharInfoBuffer[0];
				break;

			default:
				memcpy(&g_phb_qsearch_cache[index], PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);
				break;
		}
	}
	else
		memcpy(&g_phb_qsearch_cache[index], PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);
}

#elif defined(__MMI_ITAP__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_to_spelling
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL  mmi_phb_quick_search_convert_to_spelling(U8* input_string, U8* output_string, U8* out_len)
{

	S8	out = 0;
	U8	i = 0;
	U16 ResultArray[MMI_PHB_CONVERT_BUFF_ZI] ;
	U16 buffer;
	U16 CharInfo;

	/*Change to proper input method.*/
	iTAP_IO.mKeyEntryState = kKeyEntryState_Back_Conversion;

	switch(MMI_current_input_mode)
	{
		case INPUT_MODE_MMI_MULTITAP_BPMF:
			iTAP_IO.mLanguageID = kLang_Chinese_Traditional;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_BoPoMoFo;
			break;

		case INPUT_MODE_MMI_MULTITAP_PINYIN:
			iTAP_IO.mLanguageID = kLang_Chinese_Simplified;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_PinYin;
			break;

		case INPUT_MODE_MMI_TR_STROKE:
        		iTAP_IO.mLanguageID = kLang_Chinese_Traditional;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_Stroke;
			break;

		case INPUT_MODE_MMI_SM_STROKE:
        		iTAP_IO.mLanguageID = kLang_Chinese_Simplified;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_Stroke;
			break;

		default:
			return FALSE;
			break;
	}

	if(input_string[0] != 0 || input_string[1] != 0)
	{
		memcpy(&buffer ,input_string,2);

		iTAP_IO.mpInputBuffer = &buffer;
		iTAP_IO.mBufferLength = 1;
		iTAP_IO.mpOutputBuffer =ResultArray;
		iTAP_IO.mOutputBufferLength = sizeof(ResultArray);

	       itapRetCodes = LxS_iTAP_GetResults( piTAP, &iTAP_IO );


		if( !LXS_FAILED(itapRetCodes) )
		{
			i = 1; /*iTap buffer begin from 1*/
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MMI_MULTITAP_BPMF:
					/*Do not convert tone*/
					while ((iTAP_IO.mpOutputBuffer[i] != 0x0000) && ((iTAP_IO.mpOutputBuffer[i] < 0x0031) || (iTAP_IO.mpOutputBuffer[i] > 0x0035)))
					{
						//CharInfo = iTAP_IO.mpOutputBuffer[i];
						memcpy(output_string+out, &iTAP_IO.mpOutputBuffer[i], 2);
						out = out + 2;
						i = i + 1;
					}
				break;

				case INPUT_MODE_MMI_MULTITAP_PINYIN:
					/*Do not convert tone*/
					while ((iTAP_IO.mpOutputBuffer[i] != 0x0000) && ((iTAP_IO.mpOutputBuffer[i] < 0x0031) || (iTAP_IO.mpOutputBuffer[i] > 0x0035)))
					{
						//CharInfo = iTAP_IO.mpOutputBuffer[i];
						memcpy(output_string+out, &iTAP_IO.mpOutputBuffer[i], 2);
						out = out + 2;
						i = i + 1;
					}
					break;

				case INPUT_MODE_MMI_SM_STROKE:
				case INPUT_MODE_MMI_TR_STROKE:
					while ((iTAP_IO.mpOutputBuffer[i] != 0x0000) && (i <= 7))
					{
						CharInfo = StrokeInternalToUnicode((U8)iTAP_IO.mpOutputBuffer[i]);
						memcpy(output_string+out, &CharInfo, 2);
						out = out + 2;
						i = i + 1;
					}
				break;

				default:
				break;
			}

			*out_len = i - 1;
			return TRUE;

		}

		memcpy(output_string,input_string,2);
		*out_len = 1;
		return TRUE;
	}

	*out_len = 0;
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_cache
* DESCRIPTION
*   This function convert first character of name field into cache.
*
* PARAMETERS
*  index	IN	phonebook entry index
* RETURNS
*  void
* GLOBALS AFFECTED
*   g_phb_qsearch_cache
*****************************************************************************/
void mmi_phb_quick_search_convert_cache(U16 index)
{
	U16 ResultArray[MMI_PHB_CONVERT_BUFF_ZI] ;
	U16 buffer;


	memcpy(&buffer, PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);

	/*Change to proper input method.*/
	iTAP_IO.mKeyEntryState = kKeyEntryState_Back_Conversion;

	switch(MMI_current_input_mode)
	{
		case INPUT_MODE_MMI_MULTITAP_BPMF:
			iTAP_IO.mLanguageID = kLang_Chinese_Traditional;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_BoPoMoFo;
			break;

		case INPUT_MODE_MMI_MULTITAP_PINYIN:
			iTAP_IO.mLanguageID = kLang_Chinese_Simplified;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_PinYin;
			break;

		case INPUT_MODE_MMI_TR_STROKE:
        		iTAP_IO.mLanguageID = kLang_Chinese_Traditional;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_Stroke;
			break;

		case INPUT_MODE_MMI_SM_STROKE:
        		iTAP_IO.mLanguageID = kLang_Chinese_Simplified;
			iTAP_IO.mWordEntryState = kWordEntryState_Chinese_Stroke;
			break;

		default:
			memcpy(&g_phb_qsearch_cache[index], PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);
			return;
	}


	iTAP_IO.mpInputBuffer = &buffer;
	iTAP_IO.mBufferLength = 1;
	iTAP_IO.mpOutputBuffer =ResultArray;
	iTAP_IO.mOutputBufferLength = sizeof(ResultArray);

       itapRetCodes = LxS_iTAP_GetResults( piTAP, &iTAP_IO );

	if( !LXS_FAILED(itapRetCodes) )
	{
		switch(MMI_current_input_mode)
		{
			case INPUT_MODE_MMI_MULTITAP_BPMF:
				g_phb_qsearch_cache[index] = iTAP_IO.mpOutputBuffer[1];
				break;

			case INPUT_MODE_MMI_MULTITAP_PINYIN:
				g_phb_qsearch_cache[index] = iTAP_IO.mpOutputBuffer[1];
				break;

			case INPUT_MODE_MMI_SM_STROKE:
			case INPUT_MODE_MMI_TR_STROKE:
	        		g_phb_qsearch_cache[index] = StrokeInternalToUnicode((U8)iTAP_IO.mpOutputBuffer[1]);
				break;

			default:
				memcpy(&g_phb_qsearch_cache[index], PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);
				break;
		}
	}
	else
		memcpy(&g_phb_qsearch_cache[index], PhoneBook[g_phb_name_index[index]].alpha_id.name, 2);

}
 
#elif defined(__MMI_GB__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_to_spelling
* DESCRIPTION
*   This function is just for chinese quick searching.
*
* PARAMETERS
*  
*  
* RETURNS
*****************************************************************************/
BOOL  mmi_phb_quick_search_convert_to_spelling(U16 store_index, U8 *input_string, U8 *output_string, U8 *out_len)
{
	U8	out = 0;
	U8	i = 0;
	UINT16  swap;
	int ret;
	//char buffer_temp[MAX_PB_ENTRIES][(MMI_PHB_SORT_LENGTH + 1)* ENCODING_LENGTH] = { 0 };
	
	memset(&buffer[0],0x00,(MMI_PHB_SORT_LENGTH + 1) * ENCODING_LENGTH);
	swap = *(UINT16 *)(input_string);
	#ifdef __MMI_GB_V5__
	GbIMESetEngineCurrentLang(GBL_Chinese, GBIM_Pinyin, 0);
	ret = GBWord2CodesByInputmod(&g_gbis, GBIM_Pinyin,swap, 0, &buffer[0]);
	#else/*__MMI_GB_V5__*/
	ebis.inputMethod = EB_INPUT_METHOD_PINYIN;
	EBSwitchInputMethod(&ebis, ebis.inputMethod);
	ret = EBWord2Codes(&ebis, swap, &buffer[0]) ;
	#endif/*__MMI_GB_V5__*/
	
	switch (MMI_current_input_mode)
	{
		case INPUT_MODE_MMI_LOWERCASE_ABC:
		case INPUT_MODE_MMI_MULTITAP_PINYIN:
			while ((buffer[i] != 0x00) && (i <= 7))
			{
				output_string[out] = buffer[i];
				output_string[out+1] = 0;
				out = out + 2;
				i = i + 1;
			}
			*out_len = i;
			return TRUE;
			
			break;

		default:					
		/*Input String Does not Converted.*/				
		memcpy(output_string,input_string,2);
		*out_len = 1;
		return FALSE;
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_cache
* DESCRIPTION
*   This function convert first character of name field into cache.
*
* PARAMETERS
*  index	IN	phonebook entry index
* RETURNS
*  void
* GLOBALS AFFECTED
*   g_phb_qsearch_cache
*****************************************************************************/
void mmi_phb_quick_search_convert_cache(U16 index)
{		
	U16 store_index;		
	store_index = g_phb_name_index[index];

	switch (MMI_current_input_mode)
	{
		case INPUT_MODE_MMI_LOWERCASE_ABC:
#if defined(__MMI_PHB_PINYIN_SORT__)
		case INPUT_MODE_MMI_MULTITAP_PINYIN:
			 g_phb_qsearch_cache[index] = g_phb_name_pinyin_index[store_index][0];
			 break;
#endif


		default:
			g_phb_qsearch_cache[index] = *((U16*)PhoneBook[g_phb_name_index[index]].alpha_id.name);
			break;
	}
}

#else /*No default input method library*/
/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_to_spelling
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL  mmi_phb_quick_search_convert_to_spelling(U8* input_string, U8* output_string, U8* out_len)
{
	memcpy(output_string,input_string,2);
	*out_len = 1;
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_convert_cache
* DESCRIPTION
*   This function convert first character of name field into cache.
*
* PARAMETERS
*  index	IN	phonebook entry index
* RETURNS
*  void
* GLOBALS AFFECTED
*   g_phb_qsearch_cache
*****************************************************************************/
void mmi_phb_quick_search_convert_cache(U16 index)
{
	g_phb_qsearch_cache[index] = (U16)PhoneBook[g_phb_name_index[index]].alpha_id.name[0];
}

#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_quick_search_make_cache()
* DESCRIPTION
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_phb_quick_search_make_cache(void)
{
	U16 index;
	U16 pre_store_index;
	U16 store_index;

#if defined(__MMI_T9__)
	/*Switch to the proper input language, only for T9*/
	switch(MMI_current_input_mode)
	{
#if defined(__MMI_T9_TR_CHINESE__)
		case INPUT_MODE_MMI_MULTITAP_BPMF:
			T9ChangeLanguageToTRChineseBoPoMo();
			break;
		case INPUT_MODE_MMI_TR_STROKE:
			T9ChangeLanguageToTRChineseStroke();
			break;
#endif
#if defined(__MMI_T9_SM_CHINESE__)
		case INPUT_MODE_MMI_MULTITAP_PINYIN:
			T9ChangeLanguageToSMChinesePinYin();
			break;
		case INPUT_MODE_MMI_SM_STROKE:
			T9ChangeLanguageToSMChineseStroke();
			break;
#endif

		default:
			break;
	}
#endif

	/*Begin to convert first character of name field to associated input method string.*/
	for(index = 0 ; index < PhoneBookEntryCount ; index++)
	{
		store_index = g_phb_name_index[index];

		if(pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name) > 0)
		{
			/*The Entry Name is an UCS2 string*/
			if(PhoneBook[store_index].alpha_id.name_dcs == PHB_UCS2 )
			{
				if(index != 0)
				{
					pre_store_index = g_phb_name_index[index-1];

					/*If character is the same, do not convert again.*/
					if((PhoneBook[pre_store_index].alpha_id.name[0] == PhoneBook[store_index].alpha_id.name[0]) && \
						(PhoneBook[pre_store_index].alpha_id.name[1] == PhoneBook[store_index].alpha_id.name[1]))
						{
							g_phb_qsearch_cache[index] = g_phb_qsearch_cache[index-1];
							continue;
						}
				}

				mmi_phb_quick_search_convert_cache(index);
			}
			else
			/*This is an ASCII string*/
				g_phb_qsearch_cache[index] = (U16)PhoneBook[store_index].alpha_id.name[0];
			}
		/*The name field is Empty*/
		else
		{
			g_phb_qsearch_cache[index] = 0x0000;
		}
	}

	g_phb_qsearch_is_cache = 1;	/*Convert cache done*/
}


/*****************************************************************************
* FUNCTION
*   mmi_phb_quick_search_find_entry
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
S32  mmi_phb_quick_search_find_entry(U8* keyword)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 key_length;
	U8 flag;
	U16 i;
	U16 code1, code2;
	U16 result_count;

	U8 same_count;
	U8 convert_result;
	S32 distance;
	S8 *string1, *string2 = NULL;
	S8 convert_string[22 * ENCODING_LENGTH]; /* 7*3 + 1 => (char1 + char2 + char3 + Null Terminator)*/
	U8 char1[7 * ENCODING_LENGTH]; /*Cache 1st character convert result*/
	U8 char2[7 * ENCODING_LENGTH]; /*Cache 2nd character convert result*/
	U8 char3[7 * ENCODING_LENGTH]; /*Cache 3rd character convert result*/
	U8 len1, len2, len3;
	U16 store_index;
	U8 prev_ucs2 = 0; /*Is previous record ucs2 encoding*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	/*Check if cache already build up before using*/
	if(!g_phb_qsearch_is_cache)
	{
		StopTimer(PHB_QUICK_SEARCH_TIMER);
		mmi_phb_quick_search_make_cache();
	}

	/*Count the input string length to decide behavior*/
	key_length = (U8)pfnUnicodeStrlen((S8*)keyword);
	result_count = 0;

	/*For each entry, check if it is in the quick search sub-list*/
	for(i = 0 ; i < PhoneBookEntryCount ; i++)
	{
		flag = FALSE;

		/*
		* search length == 1, compare whole list with first character only.
		*/
		if(key_length == 1)
		{
			code1 = (U16)g_phb_qsearch_cache[i];
			memcpy(&code2, keyword, 2);

			if(mmi_phb_compare_chars(code1,code2) == 0)
				flag = TRUE;
		}
		/*
		* search length > 1, compare subset list with maximum search character.
		*/
		else if(key_length > 1)
		{
			code1 = (U16)g_phb_qsearch_cache[i];
			memcpy(&code2, keyword, 2);

			/*Compare character in cache first. If match, compare the rest part.*/
			if(mmi_phb_compare_chars(code1,code2) == 0)
			{
				store_index = g_phb_name_index[i];
				flag = FALSE;

				if(PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2)
				{
					/*Use previous record convert result as cache*/
					if(i != 0)
						string2 = (S8*)PhoneBook[g_phb_name_index[i-1]].alpha_id.name;

					/*Current record*/
					string1 = (S8*)PhoneBook[store_index].alpha_id.name;

					/*
					*  Convert 1st character
					*/
					if(	(i != 0) && (string2[0] == string1[0]) && (string2[1] == string1[1]) && prev_ucs2)
					{
						convert_result = 10;	/* Use 10 to represent that first character is the same as previous one*/
					}
					else
					{
						U16 code;
						memcpy(&code , &PhoneBook[store_index].alpha_id.name[0], 2);
						if ((code >= 0x4e00) && (code <= 0x9fa5))
						{                  
					#if defined(__MMI_GB__)
							convert_result = mmi_phb_quick_search_convert_to_spelling(store_index, &PhoneBook[store_index].alpha_id.name[0], char1, &len1);
					#else
							convert_result = mmi_phb_quick_search_convert_to_spelling(&PhoneBook[store_index].alpha_id.name[0], char1, &len1);
					#endif
						}
						else
							convert_result = FALSE;
					}

					if(convert_result)
					{
						memset(convert_string,0, 22*ENCODING_LENGTH); /*Clear String in advance, to make sure there will be terminator*/

						len1 = (len1 > 7) ? 7 : len1;
						memcpy(convert_string, char1, (len1* ENCODING_LENGTH));

						/*
						*  Convert 2nd Character
						*/
						if(len1 < key_length)
						{
							if((convert_result == 10 ) && (string2[2] == string1[2]) && (string2[3] == string1[3]) && prev_ucs2)
								convert_result = 20; /* Use 20 to represent that second character is the same as previous one*/
							else
							{                  
							#if defined(__MMI_GB__)
								convert_result = mmi_phb_quick_search_convert_to_spelling(store_index, &PhoneBook[store_index].alpha_id.name[2], char2, &len2);
							#else
								convert_result = mmi_phb_quick_search_convert_to_spelling(&PhoneBook[store_index].alpha_id.name[2], char2, &len2);
							#endif
							}

							if(convert_result)
							{
								len2 = (len2 > 7) ? 7 : len2;
								memcpy((convert_string+len1*ENCODING_LENGTH), char2, (len2* ENCODING_LENGTH));

								/*
								*  Convert 3rd Character
								*/
								if((len1+len2) < key_length)
								{
									if((convert_result == 20 ) && (string2[4] == string1[4]) && (string2[5] == string1[5]) && prev_ucs2)
										convert_result = 30; /* Use 30 to represent that third character is the same as previous one*/
									else
									{                  
									#if defined(__MMI_GB__)
										convert_result = mmi_phb_quick_search_convert_to_spelling(store_index, &PhoneBook[store_index].alpha_id.name[4], char3, &len3);
									#else
										convert_result = mmi_phb_quick_search_convert_to_spelling(&PhoneBook[store_index].alpha_id.name[4], char3, &len3);
									#endif
									}                                        

									if(convert_result)
									{
										len3 = (len3 > 7) ? 7 : len3;
										memcpy( (convert_string+(len1+len2)*ENCODING_LENGTH), char3, (len3* ENCODING_LENGTH));
									}
									else
									{
										memcpy((convert_string+len1*ENCODING_LENGTH), &PhoneBook[store_index].alpha_id.name[4], 2);
									}
								}
							}
							else
							{
								memcpy((convert_string+len1*ENCODING_LENGTH), &PhoneBook[store_index].alpha_id.name[2], 2);
							}
						}

						/*Chinese convert successfully, use convert result to compare*/
						string1 = convert_string;
						prev_ucs2 = 1;
					}
					else	/*Not Chinese*/
					{
						string1 = (S8*)PhoneBook[store_index].alpha_id.name;
						prev_ucs2 = 0;
					}
				}
				else	/*ASCII*/
				{
					string1 = (S8*)PhoneBook[store_index].alpha_id.name;
					prev_ucs2 = 0;
				}

				mmi_phb_compare_ucs2_strings((S8*)string1, (S8*)keyword, &same_count, &distance);

				if(same_count == key_length)
					flag = TRUE;
			}
		}
		/*
		*search length == 0, display whole list.
		*/
		else
		{
			flag = TRUE;
		}

		/*Add to list if entry is a subset of a quick search result*/
		if(flag)
		{
			//g_phb_qsearch[i] = 1;
			g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] |= 1 << (i % BYTE_SIZE);
			result_count++;
		}
		else
		{
			//g_phb_qsearch[i] = 0;
			g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] &= (kal_uint8)(~(1 << (i % BYTE_SIZE)));
		}
	}

	if(result_count == 0) /*Clear send key when no entry found*/
	{
		ClearKeyHandler(KEY_SEND, KEY_EVENT_UP);
		ClearKeyHandler(KEY_SEND, KEY_EVENT_DOWN);
        	SetCategory200LeftSoftkeyFunction(NULL,KEY_EVENT_UP);
		ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
		ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
#if defined(__MMI_INTELLIGENT_CALL_ALERT__) && !defined(__PROJECT_GALLITE_C01__)
		StopTimer(PHB_LIST_SPEAK_NAME_TIMER);
		if( g_phb_is_speaking_name )
			mdi_audio_stop_string();
#endif
	}
	else if (HasValidSIM()
#ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() !=1
#endif
	)
	{
#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
	SetCategory200LeftSoftkeyFunction(mmi_phb_entry_quick_search_option,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phb_entry_quick_search_option,KEY_EVENT_UP);
#else
	SetCategory200LeftSoftkeyFunction(mmi_phb_entry_op_option,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phb_entry_op_option,KEY_EVENT_UP);
#endif
#ifdef __MMI_MULTI_SIM__
	if (MTPNP_AD_Get_UsableSide_Number() > 0)
	{
        	MTPNP_PFAL_CC_HandleSendKeys(mmi_phb_list_pre_choose_number,KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
		SetKeyHandler(mmi_phb_entry_quick_search_option,KEY_ENTER,KEY_EVENT_UP );
#else
		SetKeyHandler(mmi_phb_entry_op_option,KEY_ENTER,KEY_EVENT_UP );
#endif
#endif
	}
#else
        SetKeyHandler(mmi_phb_list_pre_choose_number, KEY_SEND, KEY_EVENT_DOWN);
#endif     

	}


	return result_count;

}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_quick_search_get_index(S32 nIndex)
{
	U16 i, count;

	S32 m_nIndex = nIndex;

	#ifdef __MMI_VIP_FUNCTION__
	m_nIndex = (S32)g_vip_context.PHBIndexMap[nIndex];
	#endif

	count = 0;
	for(i = 0; i < PhoneBookEntryCount ; i++)
	{
		//if(g_phb_qsearch[i] == 1)
		if (g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] & (1 << (i % BYTE_SIZE)))
		{
			if(count == m_nIndex)
				break;
			count++;
		}
	}

	g_phb_cntx.active_index = i;

#if defined(__MMI_INTELLIGENT_CALL_ALERT__) && !defined(__PROJECT_GALLITE_C01__)
	StopTimer(PHB_LIST_SPEAK_NAME_TIMER);
	if( g_phb_is_speaking_name )
		mdi_audio_stop_string();

	if( g_phb_cntx.speak_name )
		StartTimer( PHB_LIST_SPEAK_NAME_TIMER, UI_POPUP_NOTIFYDURATION_TIME, mmi_phb_list_speak_name );
#endif

}

/*****************************************************************************
* FUNCTION
*   mmi_phb_highlight_quick_search_list()
* DESCRIPTION
*
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nonw
*****************************************************************************/
void mmi_phb_highlight_quick_search_list(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_quick_search_list_pre_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_quick_search_list_pre_entry,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_quick_search_list_pre_entry(void)
{
	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		if(PhoneBookEntryCount)
			mmi_phb_entry_quick_search_list();
		else
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
#ifdef __MMI_VIP_FUNCTION__
void mmi_VIP_get_phb_suited_number_qsearch(U16 qCount)
{
	U16 i;
	U16 display_index;
	U16 count;
	U16 store_index;
	S8 num_buffer[(MAX_PB_NUMBER_LENGTH+1)*ENCODING_LENGTH];
	U16 nSuited = 0;
	U16 j = 0;
	
	for(display_index = 0; display_index < qCount; display_index++)
	{
		count = 0;
		for(i = 0; i < PhoneBookEntryCount ; i++)
		{
			if (g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] & (1 << (i % BYTE_SIZE)))
			{
				if(count == display_index)
					break;
				count++;
			}
		}
		store_index = g_phb_name_index[i];
		
		memset(num_buffer, 0, sizeof(num_buffer));
		mmi_phb_convert_get_ucs2_number(num_buffer, store_index);

		if(mmi_VIP_is_vip_number(num_buffer))
		{
			nSuited++;
		}
		else
		{
			g_vip_context.PHBIndexMap[j++] = display_index;
		}
	}
	g_vip_context.nPHBSuited = nSuited;
	
}
#endif
void mmi_phb_entry_quick_search_list(void)
{
	U8* guiBuffer;
	U16 entryCount;
	U16 nVIPSuited = 0;

	EntryNewScreen(SCR_ID_PHB_QUICK_SEARCH_LIST, mmi_phb_exit_quick_search_list, mmi_phb_quick_search_list_pre_entry, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_QUICK_SEARCH_LIST);
	RegisterHighlightHandler(mmi_phb_quick_search_get_index);

	RegisterCat200SearchFunction(mmi_phb_quick_search_find_entry);

	/*First Time enter or any entry has been modified.. */
	if((guiBuffer == NULL) || (g_phb_cntx.refresh_list == MMI_PHB_ENTRY_REFRESH))
	{
		g_phb_cntx.highlight_entry = 0;
		memset(g_phb_qsearch_input, 0, ENCODING_LENGTH);
		//memset(g_phb_qsearch,1,MAX_PB_ENTRIES);	/*All records are in the subset at the beginning*/
		memset(g_phb_qsearch_bitmask, 0xff, (MAX_PB_ENTRIES+7)/8);

		entryCount = PhoneBookEntryCount;
		guiBuffer = NULL;
		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_QSEARCH; /* '2' is used only for this screen.*/
	}
	else
	{
		entryCount = (U16)mmi_phb_quick_search_find_entry(g_phb_qsearch_input);
	}
	#ifdef __MMI_VIP_FUNCTION__
	mmi_VIP_get_phb_suited_number_qsearch(entryCount);
	nVIPSuited = g_vip_context.nPHBSuited;
	 if(PhoneBookEntryCount - nVIPSuited == 0 && entryCount == PhoneBookEntryCount)
	{
		DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		DeleteScreenIfPresent(SCR_ID_PHB_QUICK_SEARCH_LIST);
		return;
	}
	#endif
	ShowCategory200Screen(STR_SCR_PBOOK_VIEW_CAPTION,
									IMG_SCR_PBOOK_CAPTION,
									STR_GLOBAL_OPTIONS,
									IMG_GLOBAL_OPTIONS,
									STR_GLOBAL_BACK,
									IMG_GLOBAL_BACK,
									entryCount - nVIPSuited,
									mmi_phb_quick_search_list_get_item,
									mmi_phb_quick_search_list_get_hint,
									g_phb_cntx.highlight_entry,
									IMG_ID_PHB_QUICK_SEARCH_FIND,
									(U8*) g_phb_qsearch_input ,
									MMI_PHB_QUICK_SEARCH_INPUT_LENGTH,
									guiBuffer);

	SetCategory200RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
	SetCategory200LeftSoftkeyFunction(mmi_phb_entry_quick_search_option,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phb_entry_quick_search_option,KEY_EVENT_UP);
#else
	SetCategory200LeftSoftkeyFunction(mmi_phb_entry_op_option,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phb_entry_op_option,KEY_EVENT_UP);
#if defined(__MMI_MAINLCD_220X176__)
	SetKeyHandler(mmi_phb_entry_op_view_entry,KEY_ENTER,KEY_EVENT_UP);	
#endif
#endif
	if (HasValidSIM()
#ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() !=1
#endif
	)
	{
    #ifdef __MMI_MULTI_SIM__
	if (MTPNP_AD_Get_UsableSide_Number() > 0)
	{
       	 MTPNP_PFAL_CC_HandleSendKeys(mmi_phb_list_pre_choose_number,KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		 ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
		SetKeyHandler(mmi_phb_entry_quick_search_option,KEY_ENTER,KEY_EVENT_UP );
#else
		SetKeyHandler(mmi_phb_entry_op_option,KEY_ENTER,KEY_EVENT_UP );
#endif
#endif
	}
    #else
        SetKeyHandler(mmi_phb_list_pre_choose_number, KEY_SEND, KEY_EVENT_DOWN);
    #endif
	}
	 
	//SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/*Make cache according to the current input method, after input method initialized in ShowCategoryScreen.*/
	if(guiBuffer == NULL)
	{
		g_phb_qsearch_is_cache = 0;
		mmi_phb_quick_search_make_cache();
	}
}


void mmi_phb_qsearch_get_index_by_store_location_second(S32 nIndex)
{
	U16 i, count;

	S32 m_nIndex = nIndex;

	#ifdef __MMI_VIP_FUNCTION__
	m_nIndex = (S32)g_vip_context.PHBIndexMap[nIndex];
	#endif

	count = 0;
	for(i = 0; i < PhoneBookEntryCount ; i++)
	{
		//if(g_phb_qsearch[i] == 1)
		if (g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] & (1 << (i % BYTE_SIZE)))
		{
			if(count == m_nIndex)
				break;
			count++;
		}
	}
	if(MMI_PHB_LIST_FOR_SMS == phbListView)
		g_phb_cntx.active_index_second = i;
	else
		g_phb_cntx.active_index_second = g_phb_name_index[i];

#if defined(__MMI_INTELLIGENT_CALL_ALERT__) && !defined(__PROJECT_GALLITE_C01__)
	StopTimer(PHB_LIST_SPEAK_NAME_TIMER);
	if( g_phb_is_speaking_name )
		mdi_audio_stop_string();

	if( g_phb_cntx.speak_name )
		StartTimer( PHB_LIST_SPEAK_NAME_TIMER, UI_POPUP_NOTIFYDURATION_TIME, mmi_phb_list_speak_name );
#endif

}

/*****************************************************************************
* FUNCTION
*   mmi_phb_quick_search_find_entry
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/

FunctionPtr quick_search_lsk_fun;
FunctionPtr quick_search_SendKey_fun;

S32  mmi_phb_quick_search_find_entry_for_app(U8* keyword)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 key_length;
	U8 flag;
	U16 i;
	U16 code1, code2;
	U16 result_count;

	U8 same_count;
	U8 convert_result;
	S32 distance;
	S8 *string1, *string2 = NULL;
	S8 convert_string[22 * ENCODING_LENGTH]; /* 7*3 + 1 => (char1 + char2 + char3 + Null Terminator)*/
	U8 char1[7 * ENCODING_LENGTH]; /*Cache 1st character convert result*/
	U8 char2[7 * ENCODING_LENGTH]; /*Cache 2nd character convert result*/
	U8 char3[7 * ENCODING_LENGTH]; /*Cache 3rd character convert result*/
	U8 len1, len2, len3;
	U16 store_index;
	U8 prev_ucs2 = 0; /*Is previous record ucs2 encoding*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	/*Check if cache already build up before using*/
	if(!g_phb_qsearch_is_cache)
	{
		StopTimer(PHB_QUICK_SEARCH_TIMER);
		mmi_phb_quick_search_make_cache();
	}

	/*Count the input string length to decide behavior*/
	key_length = (U8)pfnUnicodeStrlen((S8*)keyword);
	result_count = 0;

	/*For each entry, check if it is in the quick search sub-list*/
	for(i = 0 ; i < PhoneBookEntryCount ; i++)
	{
		flag = FALSE;

		/*
		* search length == 1, compare whole list with first character only.
		*/
		if(key_length == 1)
		{
			code1 = (U16)g_phb_qsearch_cache[i];
			memcpy(&code2, keyword, 2);

			if(mmi_phb_compare_chars(code1,code2) == 0)
				flag = TRUE;
		}
		/*
		* search length > 1, compare subset list with maximum search character.
		*/
		else if(key_length > 1)
		{
			code1 = (U16)g_phb_qsearch_cache[i];
			memcpy(&code2, keyword, 2);

			/*Compare character in cache first. If match, compare the rest part.*/
			if(mmi_phb_compare_chars(code1,code2) == 0)
			{
				store_index = g_phb_name_index[i];
				flag = FALSE;

				if(PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2)
				{
					/*Use previous record convert result as cache*/
					if(i != 0)
						string2 = (S8*)PhoneBook[g_phb_name_index[i-1]].alpha_id.name;

					/*Current record*/
					string1 = (S8*)PhoneBook[store_index].alpha_id.name;

					/*
					*  Convert 1st character
					*/
					if(	(i != 0) && (string2[0] == string1[0]) && (string2[1] == string1[1]) && prev_ucs2)
					{
						convert_result = 10;	/* Use 10 to represent that first character is the same as previous one*/
					}
					else
					{
						U16 code;
						memcpy(&code , &PhoneBook[store_index].alpha_id.name[0], 2);
						if ((code >= 0x4e00) && (code <= 0x9fa5))
						{                  
							#if defined(__MMI_GB__)
							convert_result = mmi_phb_quick_search_convert_to_spelling(store_index, &PhoneBook[store_index].alpha_id.name[0], char1, &len1);
							#else
							convert_result = mmi_phb_quick_search_convert_to_spelling(&PhoneBook[store_index].alpha_id.name[0], char1, &len1);
							#endif
						}
						else
							convert_result = FALSE;
					}

					if(convert_result)
					{
						memset(convert_string,0, 22*ENCODING_LENGTH); /*Clear String in advance, to make sure there will be terminator*/

						len1 = (len1 > 7) ? 7 : len1;
						memcpy(convert_string, char1, (len1* ENCODING_LENGTH));

						/*
						*  Convert 2nd Character
						*/
						if(len1 < key_length)
						{
							if((convert_result == 10 ) && (string2[2] == string1[2]) && (string2[3] == string1[3]) && prev_ucs2)
								convert_result = 20; /* Use 20 to represent that second character is the same as previous one*/
							else
							{                  
							#if defined(__MMI_GB__)
								convert_result = mmi_phb_quick_search_convert_to_spelling(store_index, &PhoneBook[store_index].alpha_id.name[2], char2, &len2);
							#else
								convert_result = mmi_phb_quick_search_convert_to_spelling(&PhoneBook[store_index].alpha_id.name[2], char2, &len2);
							#endif
							}

							if(convert_result)
							{
								len2 = (len2 > 7) ? 7 : len2;
								memcpy((convert_string+len1*ENCODING_LENGTH), char2, (len2* ENCODING_LENGTH));

								/*
								*  Convert 3rd Character
								*/
								if((len1+len2) < key_length)
								{
									if((convert_result == 20 ) && (string2[4] == string1[4]) && (string2[5] == string1[5]) && prev_ucs2)
										convert_result = 30; /* Use 30 to represent that third character is the same as previous one*/
									else
									{                  
									#if defined(__MMI_GB__)
										convert_result = mmi_phb_quick_search_convert_to_spelling(store_index, &PhoneBook[store_index].alpha_id.name[4], char3, &len3);
									#else
										convert_result = mmi_phb_quick_search_convert_to_spelling(&PhoneBook[store_index].alpha_id.name[4], char3, &len3);
									#endif
									}                                        

									if(convert_result)
									{
										len3 = (len3 > 7) ? 7 : len3;
										memcpy( (convert_string+(len1+len2)*ENCODING_LENGTH), char3, (len3* ENCODING_LENGTH));
									}
									else
									{
										memcpy((convert_string+len1*ENCODING_LENGTH), &PhoneBook[store_index].alpha_id.name[4], 2);
									}
								}
							}
							else
							{
							memcpy((convert_string+len1*ENCODING_LENGTH), &PhoneBook[store_index].alpha_id.name[2], 2);
							}
						}

						/*Chinese convert successfully, use convert result to compare*/
						string1 = convert_string;
						prev_ucs2 = 1;
					}
					else	/*Not Chinese*/
					{
						string1 = (S8*)PhoneBook[store_index].alpha_id.name;
						prev_ucs2 = 0;
					}
				}
				else	/*ASCII*/
				{
					string1 = (S8*)PhoneBook[store_index].alpha_id.name;
					prev_ucs2 = 0;
				}

				mmi_phb_compare_ucs2_strings((S8*)string1, (S8*)keyword, &same_count, &distance);

				if(same_count == key_length)
					flag = TRUE;
			}
		}
		/*
		*search length == 0, display whole list.
		*/
		else
		{
			flag = TRUE;
		}

		/*Add to list if entry is a subset of a quick search result*/
		if(flag)
		{
			//g_phb_qsearch[i] = 1;
			g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] |= 1 << (i % BYTE_SIZE);
			result_count++;
		}
		else
		{
			//g_phb_qsearch[i] = 0;
			g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] &= (kal_uint8)(~(1 << (i % BYTE_SIZE)));
		}
	}

	if(result_count == 0) /*Clear send key when no entry found*/
	{
		ClearKeyHandler(KEY_SEND, KEY_EVENT_UP);
		ClearKeyHandler(KEY_SEND, KEY_EVENT_DOWN);
		SetCategory200LeftSoftkeyFunction(NULL,KEY_EVENT_UP);
		ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
		ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
	#if defined(__MMI_INTELLIGENT_CALL_ALERT__) && !defined(__PROJECT_GALLITE_C01__)
		StopTimer(PHB_LIST_SPEAK_NAME_TIMER);
		if( g_phb_is_speaking_name )
			mdi_audio_stop_string();
	#endif
	}
	else if (HasValidSIM()
	#ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() !=1
	#endif
		)
	{
		SetCategory200LeftSoftkeyFunction(quick_search_lsk_fun,KEY_EVENT_UP);
		SetLeftSoftkeyFunction(quick_search_lsk_fun,KEY_EVENT_UP);
	#ifdef __MMI_MULTI_SIM__
		if (MTPNP_AD_Get_UsableSide_Number() > 0)
		{
			MTPNP_PFAL_CC_HandleSendKeys(quick_search_SendKey_fun,KEY_EVENT_DOWN);
		#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
			ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
			SetKeyHandler(quick_search_SendKey_fun,KEY_ENTER,KEY_EVENT_UP );
		#endif
		}
	#else
		SetKeyHandler(quick_search_SendKey_fun, KEY_SEND, KEY_EVENT_DOWN);
	#endif     

	}

	return result_count;

}

void mmi_phb_entry_quick_search_list_for_app(U16 STR_TITLE, U16 IMG_TITLE,U16 STR_LSK, U16 IMG_LSK, void (*HighlightPtr)(S32), FunctionPtr LSKPtr, FunctionPtr SendKeyPtr, FunctionPtr EntryPtr)
{
	U8* guiBuffer;
	U16 entryCount;
	U16 nVIPSuited = 0;

	EntryNewScreen(SCR_ID_PHB_QUICK_SEARCH_LIST, mmi_phb_exit_quick_search_list, EntryPtr, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_QUICK_SEARCH_LIST);
	
	RegisterHighlightHandler(HighlightPtr);

	RegisterCat200SearchFunction(mmi_phb_quick_search_find_entry_for_app);

	/*First Time enter or any entry has been modified.. */
	if((guiBuffer == NULL) || (g_phb_cntx.refresh_list == MMI_PHB_ENTRY_REFRESH))
	{
		g_phb_cntx.highlight_entry = 0;
		memset(g_phb_qsearch_input, 0, ENCODING_LENGTH);
		//memset(g_phb_qsearch,1,MAX_PB_ENTRIES);	/*All records are in the subset at the beginning*/
		memset(g_phb_qsearch_bitmask, 0xff, (MAX_PB_ENTRIES+7)/8);

		entryCount = PhoneBookEntryCount;
		guiBuffer = NULL;
		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_QSEARCH; /* '2' is used only for this screen.*/
	}
	else
	{
		entryCount = (U16)mmi_phb_quick_search_find_entry(g_phb_qsearch_input);
	}
	
#ifdef __MMI_VIP_FUNCTION__
	mmi_VIP_get_phb_suited_number_qsearch(entryCount);
	nVIPSuited = g_vip_context.nPHBSuited;
	if(PhoneBookEntryCount - nVIPSuited == 0 && entryCount == PhoneBookEntryCount)
	{
		DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		DeleteScreenIfPresent(SCR_ID_PHB_QUICK_SEARCH_LIST);
		return;
	}
#endif

	ShowCategory200Screen(STR_TITLE,
						IMG_TITLE,
						STR_LSK,
						IMG_LSK,
						STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,
						entryCount - nVIPSuited,
						mmi_phb_quick_search_list_get_item,
						mmi_phb_quick_search_list_get_hint,
						g_phb_cntx.highlight_entry,
						IMG_ID_PHB_QUICK_SEARCH_FIND,
						(U8*) g_phb_qsearch_input ,
						MMI_PHB_QUICK_SEARCH_INPUT_LENGTH,
						guiBuffer);

	SetCategory200RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetCategory200LeftSoftkeyFunction(LSKPtr,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(LSKPtr,KEY_EVENT_UP);
	if (HasValidSIM()
#ifdef __FLIGHT_MODE_SUPPORT__
	&& mmi_bootup_get_active_flight_mode() != 1
#endif
	)
	{
	#ifdef __MMI_MULTI_SIM__
		if(SendKeyPtr != NULL)
		{
			MTPNP_PFAL_CC_HandleSendKeys(SendKeyPtr,KEY_EVENT_DOWN);
		#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
			ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
			SetKeyHandler(LSKPtr,KEY_ENTER,KEY_EVENT_UP );
		#endif
		}
	#else
		SetKeyHandler(SendKeyPtr, KEY_SEND, KEY_EVENT_DOWN);
	#endif

	#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
		if (phbListView == MMI_PHB_LIST_FOR_PHB)
		{
		#ifdef __MMI_MULTI_SIM__
			if(SendKeyPtr != MTPNP_NULL)
			{
				MTPNP_PFAL_CC_SetCenterFunc(SendKeyPtr);
				SetCenterSoftkeyFunction(MTPNP_PFAL_CC_CSKHandler, KEY_EVENT_UP);
			}
			else
			{
				SetCenterSoftkeyFunction(SendKeyPtr, KEY_EVENT_UP);
			}
		#else  /* __MMI_MULTI_SIM__ */
			SetCenterSoftkeyFunction(SendKeyPtr, KEY_EVENT_UP);
		#endif /* __MMI_MULTI_SIM__ */
		}    
	#else
		if (phbListView == MMI_PHB_LIST_FOR_PHB)
		{
		#ifdef __MMI_MULTI_SIM__
			if(SendKeyPtr != MTPNP_NULL)
			{
				MTPNP_PFAL_CC_SetCenterFunc(SendKeyPtr);
			}
		#endif /* __MMI_MULTI_SIM__ */
			}  
	#endif /* (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__) */

#if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
		if (phbListView == MMI_PHB_LIST_FOR_PHB)
		{
			SimulateCenterKeyDailFunction(SendKeyPtr, KEY_EVENT_UP);
			SetNeedCenterKeyFlag(TRUE);
		}
#endif

	}
	

	/*Make cache according to the current input method, after input method initialized in ShowCategoryScreen.*/
	if(guiBuffer == NULL)
	{
		g_phb_qsearch_is_cache = 0;
		mmi_phb_quick_search_make_cache();
	}
	quick_search_lsk_fun = LSKPtr;
	quick_search_SendKey_fun = SendKeyPtr;
	
}
/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_exit_quick_search_list(void)
{
#if defined(__MMI_PHB_RSK_QUICK_SEARCH__)

	if(g_idle_context.ToNameScrFromIdleApp)
		g_phb_cntx.start_scr_id = SCR_ID_PHB_QUICK_SEARCH_LIST;
	g_phb_cntx.end_scr_id = SCR_ID_PHB_QUICK_SEARCH_LIST;

	g_idle_context.ToNameScrFromIdleApp=0;
	g_idle_context.RskPressedFromIdleApp=0;

	StopTimer(KEYPAD_LOCK_TIMER);

#if defined(__MMI_VRSD_DIAL__)
	StopTimer(VRSD_DIAL_PLAYBACK_TIMER);
#endif

#endif

#if defined(__MMI_INTELLIGENT_CALL_ALERT__) && !defined(__PROJECT_GALLITE_C01__)
	StopTimer(PHB_LIST_SPEAK_NAME_TIMER);
	if( g_phb_is_speaking_name )
		mdi_audio_stop_string();
#endif
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
pBOOL mmi_phb_quick_search_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	U16 i, count, store_index;
	S32 m_item_index = item_index;

	#ifdef __MMI_VIP_FUNCTION__
	m_item_index = (S32)g_vip_context.PHBIndexMap[item_index];
	#endif

	count = 0;
	for(i = 0; i < PhoneBookEntryCount ; i++)
	{
		//if(g_phb_qsearch[i] == 1)
		if (g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] & (1 << (i % BYTE_SIZE)))
		{
			if(count == m_item_index)
				break;
			count++;
		}
	}

	store_index = g_phb_name_index[i];
	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name))
		pfnUnicodeStrcpy((S8*) str_buff, (S8*)PhoneBook[store_index].alpha_id.name);
	else
		mmi_phb_convert_get_ucs2_number((S8*)str_buff, store_index);

#ifdef __MMI_MULTI_SIM__
	if (store_index < MAX_PB_PHONE_ENTRIES)
	{
		*img_buff_p = get_image(IMG_STORAGE_HANDSET);
	}
	else
	{
		U8 nSimIndex = (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;
		*img_buff_p = get_image(IMG_STORAGE_SIM_1+ nSimIndex);
	}
#else
	if (store_index < MAX_PB_PHONE_ENTRIES)
		*img_buff_p = get_image(IMG_STORAGE_HANDSET);
	else
		*img_buff_p = get_image(IMG_STORAGE_SIM);
#endif

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
S32 mmi_phb_quick_search_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	U16 i, count,store_index;
	S8 temp_number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
	S32 m_item_index = item_index;

	#ifdef __MMI_VIP_FUNCTION__
	m_item_index = (S32)g_vip_context.PHBIndexMap[item_index];
	#endif
	
	count = 0;
	for(i = 0; i < PhoneBookEntryCount ; i++)
	{
		//if(g_phb_qsearch[i] == 1)
		if (g_phb_qsearch_bitmask[i >> BYTE_SIZE_SHIFT_ARITHMETIC] & (1 << (i % BYTE_SIZE)))
		{
			if(count == m_item_index)
				break;
			count++;
		}
	}

	store_index = g_phb_name_index[i];
	mmi_phb_convert_get_ucs2_number((S8*)temp_number, store_index);

	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name) && pfnUnicodeStrlen((S8*)temp_number))
		pfnUnicodeStrcpy((S8*)hint_array[0], (S8*)temp_number);
	else
		return 0;

	return 1;  /*One hint data only, can be more hints.*/
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_quick_search_change_input_mode(void)
{
	//memset(g_phb_qsearch,1,MAX_PB_ENTRIES);	/*All records are in the subset at the beginning*/
	memset(g_phb_qsearch_bitmask, 0xff, (MAX_PB_ENTRIES+7)/8);
	g_phb_qsearch_is_cache = 0;

	/*
	* Make cache according to the current input method.
	* Start a timer to avoid convert cache each time when user change input method quickly.
	*/
	StopTimer(PHB_QUICK_SEARCH_TIMER);
	StartTimer(PHB_QUICK_SEARCH_TIMER,500,mmi_phb_quick_search_make_cache);
}

#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
extern void EntryQuickSearchInputMethodScreen(void);
/*****************************************************************************
* FUNCTION
*   mmi_phb_quick_search_go_back_editor
* DESCRIPTION
*   Go back screen after change input method.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_phb_quick_search_go_back_editor(void)
{
	GoBackToHistory(SCR_ID_PHB_QUICK_SEARCH_LIST);
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_highlight_quick_search_input_method
* DESCRIPTION
*   Highlight function to enter change input method
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_phb_highlight_quick_search_input_method(void)
{
	SetLeftSoftkeyFunction(EntryQuickSearchInputMethodScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryQuickSearchInputMethodScreen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

	RegisterInputMethodScreenCloseFunction(mmi_phb_quick_search_go_back_editor);
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_highlight_quick_search_op_option
* DESCRIPTION
*   Highlight function to enter entry option.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_phb_highlight_quick_search_op_option(void)
{
	SetLeftSoftkeyFunction(mmi_phb_entry_op_option,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_entry_quick_search_option
* DESCRIPTION
*   Entry function of phonebook quick search option.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_phb_entry_quick_search_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[10];	/*Need Expand when has more options.*/
	U16 nNumofItem;

	EntryNewScreen(SCR_ID_PHB_QUICK_SEARCH_OPTION, mmi_phb_exit_quick_search_option, mmi_phb_entry_quick_search_option, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_QUICK_SEARCH_OPTION);
	nNumofItem = GetNumOfChild(MENU_ID_PHB_QUICK_SEARCH_OPTION);
	GetSequenceStringIds(MENU_ID_PHB_QUICK_SEARCH_OPTION, nStrItemList);
	SetParentHandler(MENU_ID_PHB_QUICK_SEARCH_OPTION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_GLOBAL_OPTIONS,
						  IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK,
						  IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK,
						  IMG_GLOBAL_BACK,
						  nNumofItem,
						  nStrItemList,
						  (U16*)gIndexIconsImageList,
						  LIST_MENU,
						  0,
						  guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*    mmi_phb_exit_quick_search_option
* DESCRIPTION
*    Exit function of phonebook quick search option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_phb_exit_quick_search_option(void)
{
	g_phb_cntx.end_scr_id = SCR_ID_PHB_QUICK_SEARCH_OPTION;
}

#endif


#endif 	/*End of #if defined(__MMI_PHB_QUICK_SEARCH__)*/



#define MMI_PHB_UTIL_SEARCH
/*---------------------------------------------------- General Compare String Functions -------------------------------------------------*/
/*****************************************************************************
* FUNCTION		:	mmi_phb_compare_chars
*
* DESCRIPTION	: compare two characters case insensitively.
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
S32 mmi_phb_compare_chars(U16 char1, U16 char2) {
   /* try UCS2 matching */
   if (char1 == char2)
      return 0;

   /* if not, try ASCII case-insensitive matching */
   else if ((char1 < 0x007E) && (char2 < 0x007E)) {
      if ((char1 <= 0x007b) && (char1 >= 0x0061))
         char1 -= 0x20;
      if ((char2 <= 0x007b) && (char2 >= 0x0061))
         char2 -= 0x20;

      return char1 - char2;
   }
   /*Wangzl Add Start Ver: Multi_Language  on 2008-2-26 16:27 */
   //only allow input lowercase char,this is the mapping of lowercase and uppercase char
   #ifdef __MMI_LANG_FRENCH__
   if(char2>=0x00E0&&char2<=0x00FC)
   {
   	char2-=0x20;
	return char1 - char2;
   }
   else if(char2==0x0153)
   {
   	return (char1 == 0x0152);
   }
   else if(char2 == 0x00FF)
   {
   	return (char1 == 0x0178);
   }
   #endif
   #ifdef __MMI_LANG_RUSSIAN__
   if(char2 == 0x0451)
   {
   	return (char1==0x0401);
   }
   if(char2>=0x0430&&char2<=0x044F)
   {
   	char2 -= 0x20;
	return char1 - char2;
   }
   #endif
   /*Wangzl Add End Ver: Multi_Language  on 2008-2-26 16:27 */
   /* no match */
   return char1 - char2;
}

/*****************************************************************************
* FUNCTION		:	mmi_phb_compare_ucs2_strings
*
* DESCRIPTION	:    string1 and string2 are all UCS2 strings, compare them case insensitively.
*						will compare only length of "MMI_PHB_COMPARE_LENGTH".
*						For Quick Search Use only currently.
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_compare_ucs2_strings(S8* string1, S8* string2, U8* same_count, S32* distance) {
	U8 len1, len2;
	U8 max_len, min_len;
	U8 i;
	U16 code1, code2;

	*same_count = 0;
	*distance = 0;

	len1 = (U8)pfnUnicodeStrlen(string1);
	len2 = (U8)pfnUnicodeStrlen(string2);

	if (len1 > len2) {
		max_len = len1;
		min_len = len2;
	}
	else {
		max_len = len2;
		min_len = len1;
	}

	for (i = 0; i < max_len && i < MAX_PB_EMAIL_LENGTH; i++)  
	//for (i = 0; i < max_len; i++)
	{
		/* If all characters are identical from 0 to min_len. */
		if (i >= min_len)
		{
			/* First is greater than second */
			if (len1 > len2)
				*distance = (U16)string1[i*ENCODING_LENGTH];
			/* First and second is congruence */
			else if (len1 == len2)
				*distance = 0;
			/* len1 < len2: First is less than second */
			else
				*distance = - (U16)string2[i*ENCODING_LENGTH];

			//if (min_len == 0)
			//	*distance = - *distance;

			break;
		}

		/* First, compare from 0 to min_len */
		else
		{
			memcpy(&code1, (string1 + i*ENCODING_LENGTH), 2);
			memcpy(&code2, (string2 + i*ENCODING_LENGTH), 2);

			if ( (*distance = mmi_phb_compare_chars(code1,code2) ) == 0)
				*same_count = *same_count + 1;
			/* First and second is not equal, return distance */
			else
				break;
		}
	}

}

/*****************************************************************************
* FUNCTION		:	mmi_phb_compare_ASCII_strings
*
* DESCRIPTION	:    string1 and string2 are all ascii strings, compare them case insensitively.
*
* PARAMETERS
*
* RETURNS		:	0 if equal; >0 if string1 > string2 ; <0 if sting1 < string2
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_compare_ascii_strings(S8* string1, S8* string2, U16* same_count, S32* distance) {
	U16 len1, len2;
	U16 max_len, min_len;
	U16 i;
	U16 code1, code2;

	*same_count = 0;
	*distance = 0;

	len1 = strlen(string1);
	len2 = strlen(string2);

	if (len1 > len2) {
		max_len = len1;
		min_len = len2;
	}
	else {
		max_len = len2;
		min_len = len1;
	}

	for (i = 0; i < max_len; i++)  {
		if (i >= min_len) /* If all characters are identical from 0 to min_len. */
		{
			if (len1 > len2) /* First is greater than second */
				*distance = (U16)string1[i];
			else if (len1 == len2)	/* First and second is congruence */
				*distance = 0;
			else 	/* len1 < len2: First is less than second */
				*distance = - (U16)string2[i];

			if (min_len == 0)
				*distance = -*distance;
			break;
		}
      	else /* First, compare from 0 to min_len */
		{
			code1 = (U16)string1[i];
			code2 = (U16)string2[i];

			if ((*distance = mmi_phb_compare_chars(code1,code2)) == 0)
				*same_count = *same_count + 1;
			else /* First and second is not equal, return distance */
				break;
		}
	}
}

#if defined(__MMI_PHB_PINYIN_SORT__)
/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_util_make_pinyin_cache(U16 store_index)
{
	U16 count;

	if((PhoneBook[store_index].alpha_id.name_length != 0) && (PhoneBook[store_index].alpha_id.name_dcs != MMI_PHB_ASCII))
	{
			
		count = mmi_phb_util_convert_to_pinyin(PhoneBook[store_index].alpha_id.name, &g_phb_name_pinyin_index[store_index][0], MMI_PHB_SORT_LENGTH*ENCODING_LENGTH);
		g_phb_name_pinyin_index[store_index][count++] = 0x00;
		g_phb_name_pinyin_index[store_index][count++] = 0x00;
	}
	else
		memset(&g_phb_name_pinyin_index[store_index][0],0,2);
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/

void mmi_phb_util_get_pinyin_cache(U16 store_index, U8** pinyin_ptr)
{
	*pinyin_ptr = &g_phb_name_pinyin_index[store_index][0];
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_util_convert_to_pinyin()
* DESCRIPTION
*  This function convert the input string into Chinese Pinyin string according to the library
*  Between each pinyin string result, it will append the original encoding to seperate two
*  Chinese pinyin result.
*
* PARAMETERS
*  inputString   :   The input string buffer in ucs2 format.
*  outputString   :   The output string buffer, it will be ucs2 format
*  out_length   : The output string buffer size, in bytes.
*
* RETURNS
*  void
* GLOBALS AFFECTED
*
*****************************************************************************/
#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
U16 mmi_phb_util_convert_to_pinyin(U8* inputString, U8* outputString, U16 out_length)
{
	T9STATUS	t9ReturnCodes;
	U8  i, out = 0;

	T9ChangeLanguageToSMChinesePinYin();

	while ( !((*inputString == 0) && (*(inputString + 1 ) == 0)) && ((out +1 ) < out_length))
	{
		t9ReturnCodes = T9CCGetCharSpell(&FieldInfo.CCFieldInfo,(U16*)inputString,0);

		if(t9ReturnCodes == 0) /*(code >= 0x4e00) && (code <= 0x9fa5) */
		{
			i = 0;
			while ((FieldInfo.CCFieldInfo.pbSpellBuf[i] != 0x00) && ( (out +1 ) < out_length) )
			{
				outputString[out++] = FieldInfo.CCFieldInfo.pbSpellBuf[i++];
				outputString[out++] = 0x00;
			}
		}

			/*Add character encoding beetween pinyin strings*/
		if((out + 1) < out_length)
		{
				outputString[out++] = inputString[0];
				outputString[out++] = inputString[1];
		}

		inputString += 2;
	}

	return out;
}
#else
U16 mmi_phb_util_convert_to_pinyin(U8* inputString, U8* outputString, U16 out_length)
{
	T9Event		t9EventInput;
	T9STATUS	t9ReturnCodes;
	U8  i, out = 0;

	T9ChangeLanguageToSMChinesePinYin();

	while ( !((*inputString == 0) && (*(inputString + 1 ) == 0)) && ((out +1 ) < out_length))
	{
		t9EventInput.mType = T9EVTSPELLING;
		t9EventInput.data.sLDBData.psBuf = (U16*)inputString;
		t9EventInput.data.sLDBData.mCtrlID = 0;
		t9ReturnCodes = T9HandleEvent(&FieldInfo, &t9EventInput);

		if(t9ReturnCodes == 0) /*(code >= 0x4e00) && (code <= 0x9fa5) */
		{
			i = 0;
			while ((FieldInfo.uLangData.pAuxChinese->pbSpellBuf[i] != 0x00) && ( (out +1 ) < out_length) )
			{
				outputString[out++] = FieldInfo.uLangData.pAuxChinese->pbSpellBuf[i++];
				outputString[out++] = 0x00;
			}
		}

			/*Add character encoding beetween pinyin strings*/
		if((out + 1) < out_length)
		{
				outputString[out++] = inputString[0];
				outputString[out++] = inputString[1];
		}

		inputString += 2;
	}

	return out;
}
#endif
#elif defined(__MMI_ZI__)
U16 mmi_phb_util_convert_to_pinyin(U8* inputString, U8* outputString, U16 out_length)
{
	U8  i, out = 0;
	U16 code;
	U16 pCharInfoBuffer[MMI_PHB_CONVERT_BUFF_ZI];
	U16 PinYin_Base = ZI8_BASE_PINYIN - 0x0061;

	while ( !((*inputString == 0) && (*(inputString + 1 ) == 0)) && ((out +1 ) < out_length))
	{
		memcpy(&code ,inputString,2);

		if ((code >= 0x4e00) && (code <= 0x9fa5) &&
			Zi8GetCharInfo(code, (U16*)pCharInfoBuffer, MMI_PHB_CONVERT_BUFF_ZI, ZI8_GETMODE_PINYIN))
		{
			i = 0;
			while ((pCharInfoBuffer[i] != 0x0000) && (out < out_length) &&
				    ( (pCharInfoBuffer[i] < 0xf431) || (pCharInfoBuffer[i] > 0xf435))) /*No tone needed*/
				{
						outputString[out++] =(U8) (pCharInfoBuffer[i++] - PinYin_Base);
						outputString[out++] = 0x00;
				}
		}

			/*Add character encoding beetween pinyin strings*/
		if((out + 1) < out_length)
		{
				outputString[out++] = inputString[0];
				outputString[out++] = inputString[1];
		}

		inputString += 2;
	}

	return out;
}

#elif defined(__MMI_ITAP__)
U16 mmi_phb_util_convert_to_pinyin(U8* inputString, U8* outputString, U16 out_length)
{
	U8  i, out = 0;
	U16 ResultArray[MMI_PHB_CONVERT_BUFF_ZI] ;
	U16 buffer;

	iTAP_IO.mKeyEntryState = kKeyEntryState_Back_Conversion;

	iTAP_IO.mLanguageID = kLang_Chinese_Simplified;
	iTAP_IO.mWordEntryState = kWordEntryState_Chinese_PinYin;

	iTAP_IO.mpOutputBuffer =ResultArray;
	iTAP_IO.mOutputBufferLength = sizeof(ResultArray);

	iTAP_IO.mBufferLength = 1;

	while ( !((*inputString == 0) && (*(inputString + 1 ) == 0)) && ((out +1 ) < out_length))
	{
		memcpy(&buffer ,inputString,2);

		iTAP_IO.mpInputBuffer = &buffer;

      		itapRetCodes = LxS_iTAP_GetResults( piTAP, &iTAP_IO );

		if( !LXS_FAILED(itapRetCodes) )
		{
			i = 1;
			while ((iTAP_IO.mpOutputBuffer[i] != 0x0000) && (out < out_length) &&
				    ( (iTAP_IO.mpOutputBuffer[i] < 0x0031) || (iTAP_IO.mpOutputBuffer[i] > 0x0035))) /*No tone needed*/
				{
						outputString[out++] =(U8) (iTAP_IO.mpOutputBuffer[i++]);
						outputString[out++] = 0x00;
				}
		}

			/*Add character encoding beetween pinyin strings*/
		if((out + 1) < out_length)
		{
				outputString[out++] = inputString[0];
				outputString[out++] = inputString[1];
		}

		inputString += 2;
	}

	return out;
}

 
#elif defined(__MMI_GB__)

/**********************************************************************************************
Name:   mmi_phb_util_convert_to_pinyin
Desc:    provide buffer to pinyin sort
Params:  
		
Returns: 
 
***********************************************************************************************/
extern s16 GBIMEmethod;
U16 mmi_phb_util_convert_to_pinyin(U8 *inputString, U8 *outputString, U16 out_length)
{
#ifdef __MMI_GB_V5__
	U8  i; 
	U16 out = 0;
	int ret;
	UINT16  swap;

	memset(&buffer[0],0x00,(MMI_PHB_SORT_LENGTH + 1) * ENCODING_LENGTH);

	GbChangeInputMethod(INPUT_MODE_SM_PINYIN);

	while (!((*inputString == 0) && (*(inputString + 1 ) == 0)) && ((out +1 ) < out_length))
	{	
		swap = *(UINT16 *)(inputString);

		ret = GBWord2CodesByInputmod(&g_gbis, GBIM_Pinyin,swap, 0, &buffer[0]);
		
		if (ret == EB_OK) 
		{
			i = 0;
			while ((buffer[i] != 0x00) && ( (out +1 ) < out_length) )
			{	

				outputString[out++] = buffer[i++];
				outputString[out++] = 0x00;
			}
			
		}
                                                                                       
			/*Add character encoding beetween pinyin strings*/

		if ((out + 1) < out_length)
		{
			outputString[out++] = inputString[0];
			outputString[out++] = inputString[1];
		}

		inputString += 2;
	}
	if(GBIMEmethod != 0)
		GbChangeInputMethod(GBIMEmethod);
	return out;
#else
	U8  i; 
	U16 out = 0;
	int ret;
	UINT16  swap;


#ifdef NEVER
	//unicode = HIBYTE(swap) | LOBYTE(swap) << 8;
	InitGbSMInputMethod();						//Renwy add for bug 9522 2008/08/29
	GbChangeInputMethod(INPUT_MODE_SM_PINYIN);//Renwy add for bug 9522 2008/08/29
#endif /* NEVER */
	

	/*Wangzl Add Start For 6452 Ver: TBM780  on 2007-11-15 16:32 */
	EBInputStruct old_ebis;
	memcpy(&old_ebis, &ebis, sizeof(EBInputStruct));
	/*Wangzl Add End  For 6452 Ver: TBM780  on 2007-11-15 16:32 */
	//unicode = HIBYTE(swap) | LOBYTE(swap) << 8;

	memset(&buffer[0],0x00,(MMI_PHB_SORT_LENGTH + 1) * ENCODING_LENGTH);
	ebis.inputMethod = EB_INPUT_METHOD_PINYIN;
	EBSwitchInputMethod(&ebis, ebis.inputMethod);
	while (!((*inputString == 0) && (*(inputString + 1 ) == 0)) && ((out +1 ) < out_length))
	{	
		swap = *(UINT16 *)(inputString);
		//Be careful,swap is unicode ,not gb,GB input method's document writes wrong.
		//and swap's hibyte and lowbyte doesn't reverse
		ret = EBWord2Codes(&ebis, swap, &buffer[0]) ;
		 
		
		if (ret == EB_OK) 
		{
			i = 0;
			while ((buffer[i] != 0x00) && ( (out +1 ) < out_length) )
			{	

				outputString[out++] = buffer[i++];
				outputString[out++] = 0x00;
			}
			
		}
                                                                                       
			/*Add character encoding beetween pinyin strings*/

		if ((out + 1) < out_length)
		{
			outputString[out++] = inputString[0];
			outputString[out++] = inputString[1];
		}

		inputString += 2;
	}
	/*Wangzl Add Start For 6452 Ver: TBM780  on 2007-11-15 16:33 */
	EBSwitchInputMethod(&ebis, old_ebis.inputMethod);
	memcpy(&ebis, &old_ebis, sizeof(EBInputStruct));
	/*Wangzl Add End  For 6452 Ver: TBM780  on 2007-11-15 16:33 */

	return out;
#endif
	
}


#else
U16 mmi_phb_util_convert_to_pinyin(U8* inputString, U8* outputString, U16 out_length)
{
	pfnUnicodeStrncpy((S8*)outputString, (S8*)inputString, out_length);
	return out_length;
}
#endif
#endif


#define MMI_PHB_LOOKUP_TABLE
/**************************************************************

	FUNCTION NAME		: mmi_phb_lookup_table_sort()

  	PURPOSE				: Sorts the look-up table

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  Pluto. It will perform insertion sort for
						  array chunks of size less than 4 and quick
						  sort for size greater than 4.

**************************************************************/
void mmi_phb_lookup_table_sort(void)
{
	if(g_phb_cntx.lookup_table_count)
	{
		/*Set to zero beore sorting, check if this flag larger than phonebook entries to see if finish sorting.*/
		g_phb_cntx.populate_count = 0;

		/* Begin to sort.*/
		mmi_phb_lookup_table_quicksort(0, (U16)(g_phb_cntx.lookup_table_count-1));
		mmi_phb_lookup_table_insertionsort(0, (U16)(g_phb_cntx.lookup_table_count-1));

		/*After sorting, set it to total phoneboo entries.*/
		g_phb_cntx.populate_count = 0xffff;
	}
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_lookup_table_quicksort()

  	PURPOSE				: Sorts the lookup table using quick sort algorithm

	INPUT PARAMETERS	: U16 l, U16 r

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_lookup_table_quicksort(U16 l, U16 r)
{
	U16 i, j;
	U32 pivot;

	if ((r - l) > 4)
	{
		i = (r + l)/2;
		if(LookUpTable[l].number > LookUpTable[i].number)
			mmi_phb_lookup_table_swap_node(l, i);
		if(LookUpTable[l].number > LookUpTable[r].number)
			mmi_phb_lookup_table_swap_node(l, r);
		if(LookUpTable[i].number > LookUpTable[r].number)
			mmi_phb_lookup_table_swap_node(i, r);

		j = r - 1;
		mmi_phb_lookup_table_swap_node(i, j);
		i = l;
		pivot = LookUpTable[j].number;

		for(;;)
		{
			do{} while(LookUpTable[++i].number < pivot);
			do{} while(LookUpTable[--j].number > pivot);

			if(j < i)
				break;

			mmi_phb_lookup_table_swap_node(i, j);
		}

		mmi_phb_lookup_table_swap_node(i, (U16)(r - 1));
		mmi_phb_lookup_table_quicksort(l, j);
		mmi_phb_lookup_table_quicksort((U16)(i + 1), r);
	}
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_lookup_table_insertionsort()

  	PURPOSE				: Sorts the lookup table using insertion sort algorithm

	INPUT PARAMETERS	: U16 lo, U16 hi

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_lookup_table_insertionsort(U16 lo, U16 hi)
{
	U16 i, j;
	MMI_PHB_LOOKUP_NODE_STRUCT elem;

	for(i = lo + 1; i <= hi; ++i)
	{
		memcpy(&elem, &LookUpTable[i], sizeof(MMI_PHB_LOOKUP_NODE_STRUCT));

		j = i;
		while(j > lo)
		{
			if(LookUpTable[j-1].number <= elem.number)
				break;
			memcpy(&LookUpTable[j], &LookUpTable[j-1], sizeof(MMI_PHB_LOOKUP_NODE_STRUCT));
			j--;
		}
		memcpy(&LookUpTable[j], &elem, sizeof(MMI_PHB_LOOKUP_NODE_STRUCT));
	}
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_lookup_table_swap_node()

  	PURPOSE				: Swaps the look-up table nodes

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_lookup_table_swap_node(U16 i, U16 j)
{
	MMI_PHB_LOOKUP_NODE_STRUCT temp;

	memcpy(&temp, &LookUpTable[i], sizeof(MMI_PHB_LOOKUP_NODE_STRUCT));
	memcpy(&LookUpTable[i], &LookUpTable[j], sizeof(MMI_PHB_LOOKUP_NODE_STRUCT));
	memcpy(&LookUpTable[j], &temp, sizeof(MMI_PHB_LOOKUP_NODE_STRUCT));
}

/*Advance Search Phone Numbe. For Mobile Field Only!!!*/
#if defined(__MMI_PHB_ADV_NUM_MATCH_MOBILE__)
/**************************************************************

	FUNCTION NAME		: mmi_phb_number_has_extension()

  	PURPOSE				:  Find out if a phone number has extension field.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				:  The position of the first extension character.
	                                   Ex. *31#+81234567890p123 => 16

 

**************************************************************/
U16 mmi_phb_number_has_extension(S8* number)
{
#if 1
	U16 temp;
	U16 pos = 0;

	/*Ignore *31# and #31#*/
	while( ((number[0] == '*') && (number[1] == '3') && (number[2] == '1') && (number[3] == '#')) ||
	       ((number[0] == '#') && (number[1] == '3') && (number[2] == '1') && (number[3] == '#')) )
	{
		number += 4;
		pos += 4;
	}

	/* Ignore '+' */
	if(number[0] == '+')
	{
		number++;
		pos++;
	}

	/*Find position of first extension character*/
	temp = strcspn((PS8)number, "+pw*#");
	if(temp)
	{
		number = number + temp;
		pos = pos + temp;
	}

	/*Check if extension character exists.*/
	if(number[0] == '+' || number[0] == 'p' || number[0] == 'w' || number[0] == '*' || number[0] == '#')
	{
		return pos;
	}
	else
	{
		return 0;
	}
#endif

}

/**************************************************************

	FUNCTION NAME		: mmi_phb_number_compare_extension()

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				:

 

**************************************************************/
BOOL mmi_phb_number_compare_extension(S8* number1, S8* number2)
{
	U8 temp;

	/*Ignore *31# and #31# when compare.*/
	while( ((number1[0] == '*') && (number1[1] == '3') && (number1[2] == '1') && (number1[3] == '#')) ||
	       ((number1[0] == '#') && (number1[1] == '3') && (number1[2] == '1') && (number1[3] == '#')) )
	number1 += 4;

	while( ((number2[0] == '*') && (number2[1] == '3') && (number2[2] == '1') && (number2[3] == '#')) ||
	       ((number2[0] == '#') && (number2[1] == '3') && (number2[2] == '1') && (number2[3] == '#')) )
	number2 += 4;

	/*Ignore '+' when compare.*/
	if(number1[0] == '+') number1++;
	if(number2[0] == '+') number2++;

	/*Only Compare Extension part.*/
	temp = strcspn((PS8)number1, "+pw*#");
	if(temp)
		number1 = number1 + temp;

	temp = strcspn((PS8)number2, "+pw*#");
	if(temp)
		number2 = number2 + temp;

	/*Compare if the number is full match.*/
	if(!strcmp((PS8)number1, (PS8)number2))
		return TRUE;
	else
		return FALSE;
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_number_compare_before_extension()

  	PURPOSE				:  Compare number before extension part
  							   If two numbers are less than 9 digts, do not need to
  							   do extra handling

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				:

 

**************************************************************/
BOOL mmi_phb_number_compare_before_extension(S8 * number1, S8 * number2)
{
	U16 i;
	U16 len1, len2;
	U16 ext_pos1, ext_pos2;

	/*compare length shorter than 9
	 *if(g_phb_compare_length <= 9)
	 *	return TRUE;
	 *although already compared when enter this function, still need to confirm the number type
	 *and need to do more comparation*/
	ext_pos1 = mmi_phb_number_has_extension(number1);
	ext_pos2 = mmi_phb_number_has_extension(number2);

	if ((ext_pos1 == 0 && ext_pos2 != 0 && (number2[ext_pos2] =='#' || number2[ext_pos2] =='*')) 
		|| (ext_pos2 == 0 && ext_pos1 != 0 && (number2[ext_pos1] =='#' || number2[ext_pos1] =='*')))
	{  /* only one with extension '#' or '*'. Not match! */
		return FALSE;
	}
	len1 = (ext_pos1 == 0) ? strlen(number1) : ext_pos1;
	len2 = (ext_pos2 == 0) ? strlen(number2) : ext_pos2;

	if((len1 < g_phb_compare_length) || (len2 < g_phb_compare_length))
	{
		U16 num_len1 = (number1[0] == '+') ? (len1-1): len1; 
		U16 num_len2 = (number2[0] == '+') ? (len2-1): len2;
		if(num_len1 != num_len2)
		{
			return FALSE;
		}
		else	/* the same length except "+" sign */
		{
			for(i = 1 ; i <= len1 && i <= len2 ; i++)
			{
				if(number1[len1 - i] != number2[len2 - i])
				{
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	/*(len1 >= g_phb_compare_length) && (len2 >= g_phb_compare_length)*/
	for(i = 1 ; i <= g_phb_compare_length ; i++)
	{
		if(number1[len1 - i] != number2[len2 - i])
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_number_check_full_match_with_extension()

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				:

 

**************************************************************/
BOOL mmi_phb_number_check_full_match_with_extension(U16 lookup_index, S8* number_in)
{
	U8 number_buff[MAX_PB_NUMBER_LENGTH+1+1];
	U16 store_index = LookUpTable[lookup_index].store_index;

	/*Check if mobile field is match*/
	if(PhoneBook[store_index].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
	{
		number_buff[0] = '+';
		mmi_phb_convert_to_digit((U8*)(number_buff+1), PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);
	}
	else
	{
		mmi_phb_convert_to_digit(number_buff, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);
	}

	if(!mmi_phb_number_has_extension((S8*)number_buff) &&
		mmi_phb_number_compare_before_extension(number_in,(S8*)number_buff))
	{
		return TRUE;
	}

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	/*Check number in the optional field*/
	if((store_index < MAX_PB_PHONE_ENTRIES) && \
		((PhoneBook[store_index].field & MMI_PHB_ENTRY_FIELD_HOME) || \
		 (PhoneBook[store_index].field & MMI_PHB_ENTRY_FIELD_OFFICE) || \
		 (PhoneBook[store_index].field & MMI_PHB_ENTRY_FIELD_FAX)))
	{
		S16 pError;
#if 0
		ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
		if(PhoneBook[store_index].field |MMI_PHB_ENTRY_FIELD_HOME)
		{
			if(!mmi_phb_number_has_extension((S8*)PhoneBookOptionalFields.homeNumber) &&
				mmi_phb_number_compare_before_extension(number_in,(S8*)PhoneBookOptionalFields.homeNumber))
			{
				g_phb_cntx.searched_number_type = MMI_PHB_ENTRY_FIELD_HOME;
				return TRUE;
			}
		}
		if(PhoneBook[store_index].field |MMI_PHB_ENTRY_FIELD_OFFICE)
		{
			if(!mmi_phb_number_has_extension((S8*)PhoneBookOptionalFields.officeNumber) &&
				mmi_phb_number_compare_before_extension(number_in,(S8*)PhoneBookOptionalFields.officeNumber))
			{
				g_phb_cntx.searched_number_type = MMI_PHB_ENTRY_FIELD_OFFICE;
				return TRUE;
			}
		}
		if(PhoneBook[store_index].field |MMI_PHB_ENTRY_FIELD_FAX)
		{
			if(!mmi_phb_number_has_extension((S8*)PhoneBookOptionalFields.faxNumber) &&
				mmi_phb_number_compare_before_extension(number_in,(S8*)PhoneBookOptionalFields.faxNumber))
			{
				g_phb_cntx.searched_number_type = MMI_PHB_ENTRY_FIELD_FAX;
				return TRUE;
			}
		}
	}
#endif

	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_number_find_first_without_extension()

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				:

 

**************************************************************/
S16 mmi_phb_number_find_first_without_extension(S16 mid, U32 number, S8* number_in)
{
	S16 i, new_mid, min_index;
	S16 counter = 0;
	U16 j;
	BOOL is_match = FALSE;

	new_mid = mid;
	min_index = MAX_PB_ENTRIES;

	/*Compare number before mid*/
	i = mid;
	while ( (i >= 0) && (LookUpTable[i].number == number))
	{
		if(mmi_phb_number_check_full_match_with_extension(i, number_in))
		{
			for(j = 0 ; j < PhoneBookEntryCount ; j++)
				if(g_phb_name_index[j] == LookUpTable[i].store_index)
				{
					is_match = TRUE;
					break;
				}

			if(j < min_index)
			{
				min_index = j;
				new_mid = i;
			}
		}
		i--;
		counter++;

		if(counter >= MAX_PHB_NUMBER_COMP)
		{
			break;
		}
	}

	/*Compare number after mid*/
	i= mid + 1;
	while ( (i < g_phb_cntx.lookup_table_count) && (LookUpTable[i].number == number))
	{
		if(mmi_phb_number_check_full_match_with_extension(i, number_in))
		{
			for(j = 0 ; j < PhoneBookEntryCount ; j++)
				if(g_phb_name_index[j] == LookUpTable[i].store_index)
				{
					is_match = TRUE;
					break;
				}

			if(j < min_index)
			{
				min_index = j;
				new_mid = i;
			}
		}
		i++;
		counter++;
		if(counter >= MAX_PHB_NUMBER_COMP)
		{
			break;
		}
	}

	if(is_match)
		return new_mid;	/*Always return, even only number with extension exists.*/
	else
		return -1;
/*
	if(counter <= 1)  //Only one entry match.
		return new_mid;
	else if(min_index < MAX_PB_ENTRIES)
		return new_mid;
	else
		return -1;
*/
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_number_check_full_match_without_extension()

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				:

 

**************************************************************/
BOOL mmi_phb_number_check_full_match_without_extension(U16 lookup_index, S8* number_list, S8* number_in)
{
	U16 store_index = LookUpTable[lookup_index].store_index;

	/*Check if mobile field is match*/
	if(PhoneBook[store_index].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
	{
		number_list[0] = '+';
		mmi_phb_convert_to_digit((U8*)(number_list+1), PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);
	}
	else
	{
		mmi_phb_convert_to_digit((U8*)number_list, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);
	}

	if(mmi_phb_number_compare_before_extension(number_in,(S8*)number_list) &&
			mmi_phb_number_compare_extension(number_in,(S8*)number_list))
	{
		return TRUE;
	}

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	/*Check number in the optional field*/
	if((store_index < MAX_PB_PHONE_ENTRIES) && \
		((PhoneBook[store_index].field & MMI_PHB_ENTRY_FIELD_HOME) || \
		 (PhoneBook[store_index].field & MMI_PHB_ENTRY_FIELD_OFFICE) || \
		 (PhoneBook[store_index].field & MMI_PHB_ENTRY_FIELD_FAX)))
	{
		S16 pError;
#if 0
		ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);


		if(PhoneBook[store_index].field |MMI_PHB_ENTRY_FIELD_HOME)
		{
			if(mmi_phb_number_compare_before_extension(number_in,(S8*)PhoneBookOptionalFields.homeNumber) &&
				mmi_phb_number_compare_extension(number_in,(S8*)PhoneBookOptionalFields.homeNumber))
			{
				g_phb_cntx.searched_number_type = MMI_PHB_ENTRY_FIELD_HOME;
				return TRUE;
			}
		}
		if(PhoneBook[store_index].field |MMI_PHB_ENTRY_FIELD_OFFICE)
		{
			if(mmi_phb_number_compare_before_extension(number_in,(S8*)PhoneBookOptionalFields.officeNumber) &&
				mmi_phb_number_compare_extension(number_in,(S8*)PhoneBookOptionalFields.officeNumber))
			{
				g_phb_cntx.searched_number_type = MMI_PHB_ENTRY_FIELD_HOME;
				return TRUE;
			}
		}
		if(PhoneBook[store_index].field |MMI_PHB_ENTRY_FIELD_FAX)
		{
			if(mmi_phb_number_compare_before_extension(number_in,(S8*)PhoneBookOptionalFields.faxNumber) &&
				mmi_phb_number_compare_extension(number_in,(S8*)PhoneBookOptionalFields.faxNumber))
			{
				g_phb_cntx.searched_number_type = MMI_PHB_ENTRY_FIELD_FAX;
				return TRUE;
			}
		}
	}
#endif

	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_number_find_first_with_extension()

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				:

 

**************************************************************/
S16 mmi_phb_number_find_first_with_extension(S16 mid, U32 number, S8* number_in)
{
	S16 i, new_mid, min_index,	counter;
	U8 number_list[MAX_PB_NUMBER_LENGTH+1+1];
	U16 j;

	new_mid = mid;
	min_index = MAX_PB_ENTRIES;
	counter = 0;

	/*Compare number before mid*/
	i = mid;
	while ( (i >= 0) && (LookUpTable[i].number == number))
	{
		if(mmi_phb_number_check_full_match_without_extension(i,(S8*)number_list,number_in))
		{
			for(j = 0 ; j < PhoneBookEntryCount ; j++)
				if(g_phb_name_index[j] == LookUpTable[i].store_index)
					break;

			if(j < min_index)
			{
				min_index = j;
				new_mid = i;
			}
		}
		i--;
		counter++;
		if(counter >= MAX_PHB_NUMBER_COMP)
		{
			break;
		}
	}

	/*Compare number after mid*/
	i= mid + 1;
	while ( (i < g_phb_cntx.lookup_table_count) && (LookUpTable[i].number == number))
	{
		if(mmi_phb_number_check_full_match_without_extension(i,(S8*)number_list,number_in))
		{
			for(j = 0 ; j < PhoneBookEntryCount ; j++)
				if(g_phb_name_index[j] == LookUpTable[i].store_index)
					break;

			if(j < min_index)
			{
				min_index = j;
				new_mid = i;
			}
		}
		i++;
		counter++;
		if(counter >= MAX_PHB_NUMBER_COMP)
		{
			break;
		}
	}

	if(min_index == MAX_PB_ENTRIES)
	{
		U16 ext1 = 0;
		mmi_phb_convert_to_digit(number_list, PhoneBook[LookUpTable[new_mid].store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);

		if((counter <=1) && !mmi_phb_number_has_extension((S8*)number_list) &&  
			((ext1 = mmi_phb_number_has_extension(number_in)) > 0) && 
			(number_in[ext1] != '#') && (number_in[ext1] != '*'))
			return mid;
		else
			return -1;
	}
	else
		return new_mid;
}
#endif	/*END of #ifdef __MMI_PHB_ADV_NUM_MATCH_MOBILE__*/

/**************************************************************

	FUNCTION NAME		: mmi_phb_lookup_table_search()

  	PURPOSE				: Search for the number in the lookup table
						  and returns the index of the matched entry.

	INPUT PARAMETERS	: U32 number, S16 low, S16 high

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U16 mmi_phb_lookup_table_search(U32 number, S16 low, S16 high, S8* number_ASCII)
{
	S16 mid;

	while(low <= high)
	{
		mid = (low + high)/2;
		if(number == LookUpTable[mid].number)
		{
			/*
			* Advanced Compare - Compare number longer than 9 digit,
			* with extension, find out the prior one if numbers are the same, and etc.
			*/
#if defined(__MMI_PHB_ADV_NUM_MATCH_MOBILE__	)
			if(!mmi_phb_number_has_extension(number_ASCII))
			{
				mid = mmi_phb_number_find_first_without_extension(mid,number,number_ASCII);
			}
			else
			{
				mid = mmi_phb_number_find_first_with_extension(mid,number,number_ASCII);
			}

			if(mid < 0)
				return 0xffff; /*an invalid number*/
			else
#endif
			return LookUpTable[mid].store_index; /* return storage location in array, begin from 0*/
		}
		else if(number < LookUpTable[mid].number)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return 0xffff;
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_lookup_table_search()

  	PURPOSE				: Search for the number in the lookup table
						  and returns the number of index of the matched entry.

	INPUT PARAMETERS	: U32 number, S16 low, S16 high

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U16 mmi_phb_lookup_table_n_search(U32 number, S16 low, S16 high, S8* number_ASCII, S16 *pfirstindex)
{
	S16 mid;
	U16 matchcount = 0;
	S16 i = 0;

	while(low <= high)
	{
		mid = (low + high)/2;
		if(number == LookUpTable[mid].number)
		{
			/*
			* Advanced Compare - Compare number longer than 9 digit,
			* with extension, find out the prior one if numbers are the same, and etc.
			*/
#if defined(__MMI_PHB_ADV_NUM_MATCH_MOBILE__	)
			if(!mmi_phb_number_has_extension(number_ASCII))
			{
				mid = mmi_phb_number_find_first_without_extension(mid,number,number_ASCII);
			}
			else
			{
				mid = mmi_phb_number_find_first_with_extension(mid,number,number_ASCII);
			}

			if(mid < 0)
				return 0; /*an invalid number*/
			else
#endif
			{		
				for(i = mid + 1; i <= high && i > mid; i++) 
				{
					if(number == LookUpTable[i].number)
					{
						matchcount++;
					}
					else
					{
						break;
					}
				}
				
				for(i = mid; i >= low && i <= mid; i--) 
				{
					if(number == LookUpTable[i].number)
					{
						matchcount++;
						*pfirstindex = i;
					}
					else
					{
						break;
					}
				}
				
				return matchcount;
			}
		}
		else if(number < LookUpTable[mid].number)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return 0xffff;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_util_read_compare_length()
* DESCRIPTION
*   Read the compare length from NVRAM
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_util_read_compare_length(void)
{
	S16 pError;
	U8 comp_len;

	/*Already read value*/
	if (g_phb_compare_length != 0xff)
	{
		return;
	}

	ReadRecord(NVRAM_EF_PHB_COMPARE_DIGIT_LID, 1, (void*)&comp_len, 1, &pError);

	if(comp_len < 6)
	{
		g_phb_compare_length = 6;
	}
	else if (comp_len > 20)
	{
		g_phb_compare_length = 20;
	}

	else
	{
		g_phb_compare_length = comp_len;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_util_convert_number_to_int()
* DESCRIPTION
*   Converts the last n characters of a phone number to digits
*
* PARAMETERS
*  ascii_number	IN	input ascii number
* RETURNS
*  interger of input number
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U32 mmi_phb_util_convert_number_to_int(U8* ascii_number)
{
#if 1  
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 num_len;
	U16 ext_len;
	U16	buff_len = MAX_PB_NUMBER_LENGTH + 1;
	U8 *number;
	U8 temp_number[MAX_PB_NUMBER_LENGTH + 1 + 1];
	U8 compare_length = 9;	/*Maximum convert number length is 9 digits. (4 bytes integer)*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	//PRINT_INFORMATION (("---[PhonebookSearch.c] mmi_phb_util_convert_number_to_int() => number: [%s] \n", ascii_number));
       if(ascii_number != NULL)
	{
		strncpy((S8*)temp_number, (S8*)ascii_number, buff_len);
              memset( (temp_number + buff_len), 0x00, 1);	/*Makre sure there will be terminator*/	}
	else
	{
		return INVALID_NUMBER;
	}

	number = (U8*)temp_number;

	/*Ignore  *31# and #31# case*/
	while( ((number[0] == '*') && (number[1] == '3') && (number[2] == '1') && (number[3] == '#')) ||
			((number[0] == '#') && (number[1] == '3') && (number[2] == '1') && (number[3] == '#'))  )
	{
		number += 4;
	}

	/*Skip the first plus sign*/
	if(number[0] == '+')
	{
		number++;
	}

	/*number empty*/
	if(number[0] == '\0')
	{
		return INVALID_NUMBER;
	}

	/*Find the position of extension number and remove that part*/
	ext_len = strcspn((PS8)number, "+pw*#");
	if(!ext_len)
	{
		return INVALID_NUMBER;
	}
	else
	{
		MMI_ASSERT(ext_len <= buff_len);

		number[ext_len] = '\0';

		/*Read compare length from NVRAM*/
		mmi_phb_util_read_compare_length();

		num_len = strlen((PS8)number);

		/*May compare less number than 9 digits*/
		if(g_phb_compare_length < 9)
		{
			compare_length = g_phb_compare_length;
		}

		/*Convert at most 9 digits integer*/
		if( num_len > compare_length)
		{
			number += num_len - compare_length;
		}

            //all compare only without unnumber and space

		/*return a 32 bits integer by atol()*/
		return atol((PS8)number);
	}
	#endif
}

/*-----------------------------------------------END  General Compare String Functions -------------------------------------------------*/

#define MMI_PHB_SEARCH_EMAIL
#if defined(__MMI_PHB_EMAIL_SORT__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_email_insert_index
* DESCRIPTION
*   This function inserts email to mapping table by its index.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sort_email_insert_index(U16 store_index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 pos;
	U16 i;
	S16 pError;
	U16 g_phb_email_index[MAX_PB_PHONE_ENTRIES];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	ReadRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

	if(g_phb_cntx.phone_used == 1)
	{
		g_phb_email_index[(g_phb_cntx.phone_used - 1)] = store_index;
	}
	else
	{
		pos = mmi_phb_sort_email_find_insert_pos(0, (g_phb_cntx.phone_used-2), store_index, g_phb_email_index);

		for(i = (g_phb_cntx.phone_used - 1); i > pos ; i--)
		{
			g_phb_email_index[i] = g_phb_email_index[i-1];
		}
		g_phb_email_index[pos] = store_index;

	}

	WriteRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_email_find_insert_pos
* DESCRIPTION
*   This function find the insert position for a new adding email.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_sort_email_find_insert_pos(S32 low, S32 high, U16 store_index, U16* mapping_table)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 mid;
	S16 flag;
	U8 same_count;
	S32 distance;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	while(low <= high)
	{
		mid = (low + high)/2;

		mmi_phb_sort_email_compare_by_encoding(store_index,mapping_table[mid], &same_count, &distance);

		if( distance > 0)
		{
			low = mid + 1;
			flag = -1;
		}
		else /*Either > 0 or < 0, not possible ==0*/
		{
			high = mid - 1;
			flag = 1;
		}
	}

	if(flag == -1)
		return (U16)(mid + 1);
	else
		return (U16)mid;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_email_compare_by_encoding
* DESCRIPTION
*   This function compare email precedence by encoding.
*   If no email presents, use name instead.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sort_email_compare_by_encoding(U16 first, U16 second,  U8 *same_count, S32 *distance)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S8* string1, * string2;
	S8 email1[(MAX_PB_EMAIL_LENGTH+1)*ENCODING_LENGTH];
	S8 email2[(MAX_PB_EMAIL_LENGTH+1)*ENCODING_LENGTH];
	S16 pError;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	/*
	* Prepare data for compare string 1
	*/
	if(PhoneBook[first].field & MMI_PHB_ENTRY_FIELD_EMAIL)
	{
    #if 0
	ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(first + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    #endif
 	ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, first*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
   AnsiiToUnicodeString(email1, (S8*)PhoneBookOptionalFields.emailAddress);
		string1 = email1;
	}
	else
		string1 = (S8*)PhoneBook[first].alpha_id.name;

	/*
	* Prepare data for compare string 2
	*/
	if(PhoneBook[second].field & MMI_PHB_ENTRY_FIELD_EMAIL)
	{
   #if 0
	ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(second + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
  #endif
	ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, second*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    AnsiiToUnicodeString(email2, (S8*)PhoneBookOptionalFields.emailAddress);
		string2 = email2;
	}
	else
		string2 = (S8*)PhoneBook[second].alpha_id.name;

	/*
	* compare strings
	*/
	mmi_phb_compare_ucs2_strings(string1, string2, same_count, distance);

	/*
	* If two strings are the same, put smaller store index in front.
	*/
	if(*distance == 0)
	{
		if(first < second)
			*distance = -1;
		else if(first > second)
			*distance = 1;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_email_delete_index
* DESCRIPTION
*   This function
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sort_email_delete_index(U16 store_index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 pos,i;
	S16 pError;
	U16 g_phb_email_index[MAX_PB_PHONE_ENTRIES];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	ReadRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

	for( pos = 0 ; pos < (g_phb_cntx.phone_used+1); pos++)
		if(g_phb_email_index[pos] == store_index)
			break;

	for(i = pos ; i < (g_phb_cntx.phone_used+1) ; i++)
		g_phb_email_index[i] = g_phb_email_index[i+1];

	WriteRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sort_email_delete_all_index
* DESCRIPTION
*   This function
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sort_email_delete_all_index(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S16 pError;
	U16 g_phb_email_index[MAX_PB_PHONE_ENTRIES];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	ReadRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

	memset(g_phb_email_index,0xff,MAX_PB_PHONE_ENTRIES*sizeof(U16));

	WriteRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_search_email_binary_search
* DESCRIPTION
*   This function
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_search_email_binary_search(S32 low, S32 high, S8* ucs2_pattern)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S16 pError;
	U16 g_phb_email_index[MAX_PB_PHONE_ENTRIES];
	S32 mid;
	S16 flag;
	U8 same_count;
	S32 distance;
	U16 index;
	S8* string;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	/*Get email mapping table from NVRAM*/
	ReadRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

	/*Use binary search to find nearest name.*/
	while(low <= high)
	{
		mid = (low + high)/2;

		index = g_phb_email_index[mid];

		/*Use email to compare pattern, if email empty, use name instead*/
    #if 0
		ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    #endif  
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    if(PhoneBookOptionalFields.emailAddress[0] != 0x00)
		{
			AnsiiToUnicodeString(pbEmailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
			string = (S8*)pbEmailAddress;
		}
		else
			string = (S8*)PhoneBook[index].alpha_id.name;

		mmi_phb_compare_ucs2_strings(ucs2_pattern, string, &same_count, &distance);

		if( distance > 0)
		{
			low = mid + 1;
			flag = -1;
		}
		/*
		* Distance == 0 or < 0 here
		* If there are duplicate entry, this will help to move up until first entry.
		*/
		else
		{
			high = mid - 1;
			flag = 1;
		}
	}

	if(flag == -1)
		return (U16)(mid + 1);
	else
		return (U16)mid;
}

#endif
#ifdef __FT_IDLE_SMART_DIAL__
pBOOL mmi__smartdial_search_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	
	U16 store_index;
	S8 temp_number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];	
	
	if(item_index ==0)
	{
               //str_buff[0]='1';
	         pfnUnicodeStrcpy((S8*)str_buff, (S8*)g_idle_context.DialPadCallBuffer);
		  *img_buff_p = get_image(IMG_STORAGE_HANDSET);

	}
	else if(item_index >0)
	{
		 store_index = m_smartDialListTable[item_index-1].store_index;
	      mmi_phb_convert_get_ucs2_number((S8*)temp_number, store_index);
	
	if ( pfnUnicodeStrlen((S8*)temp_number))
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)temp_number);
	
	if(store_index >= MAX_PB_PHONE_ENTRIES)	/*In SIM*/
		*img_buff_p = get_image(IMG_STORAGE_SIM);
	else
		*img_buff_p = get_image(IMG_STORAGE_HANDSET);

	}
	
	return TRUE; 	
	
}

S32 mmi__smartdial_search_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	
	U16 store_index;

	if(item_index ==0)
		return 0;

       if(item_index >0)
       {
            store_index = m_smartDialListTable[item_index-1].store_index;
	     if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name))
		    pfnUnicodeStrcpy((S8*) hint_array[0], (S8*)PhoneBook[store_index].alpha_id.name);
	     else
		mmi_phb_convert_get_ucs2_number((S8*)hint_array[0], store_index);
       }
	
	
	if(pfnUnicodeStrlen((S8*)hint_array))
		return 1;
	else
		return 0;
	
}

S32  OnChangeEdit(U8* KeyWord) 
{
	
	int i,k;
	int j=0;
	U8 tempArray[MAX_DIAL_PAD];
	U8 tempNumber[MAX_PB_NUMBER_LENGTH + 1 + 1];
	U16 store_index;
	
	dialLen = pfnUnicodeStrlen((S8*)KeyWord);
	memset(tempArray,0,sizeof(tempArray));
	//将按键信息中的Unicode转换成ASCII码，并只提取数字部分
	for(i=0;i<dialLen*2;i++){
		if(KeyWord[i]>='0'&&KeyWord[i]<='9')
			tempArray[j++] = KeyWord[i];
	}
	dialLen = j;
	
	if(dialLen == 0){
		//当输入框无内容时，将相关的数据清空
		memset(m_smartDialSearchTable,0,sizeof(m_smartDialSearchTable));
		memset(m_smartDialListTable,0,sizeof(m_smartDialListTable));
		nowHeap = 0;
		list_count = 0;
		preDialLen =0 ;
		
	}
	else if(dialLen > 1) {
		
		
		for (i=0;i<nowHeap;i++){
			if(m_smartDialSearchTable[i].matchCount >= (dialLen-1) &&  dialLen > preDialLen){
				store_index = m_smartDialSearchTable[i].store_index;
				//if(pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name))
				{
					
					memset(tempNumber,0,MAX_PB_NUMBER_LENGTH + 1 + 1);
		                     if(PhoneBook[store_index].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
					  {
						  tempNumber[0] = '+';
						  mmi_phb_convert_to_digit((U8*)(tempNumber + 1), PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
					  }
					  else
					  {
						  
	                      mmi_phb_convert_to_digit(tempNumber, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
					  }
					  
					  if (IsSubStr((S8* )tempNumber,(S8*)tempArray, dialLen) ==1)
						  m_smartDialSearchTable[i].matchCount = dialLen ;
					  else
					  	m_smartDialSearchTable[i].matchCount = dialLen-1 ;						  
						  
				}
			}
		}
		memset(m_smartDialListTable,0,sizeof(m_smartDialListTable));
		list_count =0 ;
		for (j=0;j<nowHeap;j++){
			
			if (m_smartDialSearchTable[j].matchCount >=dialLen){
				
				
				m_smartDialListTable[list_count].store_index= m_smartDialSearchTable[j].store_index;
				m_smartDialListTable[list_count].matchCount = m_smartDialSearchTable[j].matchCount;
				list_count++;
				
			}
		}
	}
	else if(  dialLen == 1){
		//筛选,从电话簿中获取
		
		memset(m_smartDialSearchTable,0,sizeof(m_smartDialSearchTable));
		nowHeap = 0;
		list_count = 0;
		preDialLen =0 ;
		//count = 0;
		//memset(m_tempBlock,0,sizeof(m_tempBlock));
		for(i = 0; i < PhoneBookEntryCount ; i++) 
		{
            store_index = g_phb_name_index[i];
					 
			memset(tempNumber,0,MAX_PB_NUMBER_LENGTH + 1 + 1);

			#ifdef __SLT_MMI_VIP_PHB__
			if(mmi_vip_phb_is_vip(store_index))
			{
				continue;
			}
			#endif
			
			if(PhoneBook[store_index].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
			{
				tempNumber[0] = '+';
				mmi_phb_convert_to_digit((U8*)(tempNumber + 1), PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
			}
			else
	             {
				
				mmi_phb_convert_to_digit(tempNumber, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
			}
			
			
			if (IsSubStr((S8* )tempNumber, (S8* )tempArray,	dialLen)==1){
				
				m_smartDialSearchTable[nowHeap].store_index = store_index;
				m_smartDialSearchTable[nowHeap].matchCount= 1;
				
				nowHeap++;
				
			}
			
			
		}
		/*
	       if(nowHeap ==0){
			   for(i=0 ;i<MAX_PB_ENTRIES;i++){
				  // if(pfnUnicodeStrlen((S8*)PhoneBook[i].alpha_id.name))
				   {
					   //store_index = g_phb_name_index[i];
					   memset(tempNumber,0,MAX_PB_NUMBER_LENGTH + 1 + 1);
					   if(PhoneBook[i].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
					   {
						   tempNumber[0] = '+';
						   mmi_phb_convert_to_digit((U8*)(tempNumber + 1), PhoneBook[i].tel.number, MAX_PB_NUMBER_LENGTH + 1);
					   }
					   else
					   {
						   
						   mmi_phb_convert_to_digit(tempNumber, PhoneBook[i].tel.number, MAX_PB_NUMBER_LENGTH + 1);
					   }
					   
					   
					   if (IsSubStr((S8* )tempNumber, (S8* )tempArray,	dialLen)==1){
						   
						   m_smartDialSearchTable[nowHeap].store_index = i;
						   m_smartDialSearchTable[nowHeap].matchCount= 1;
						   
						   nowHeap++;
						   
					   }
					   
					   
				   }
			   }
		   }
		   
		   */
		   memset(m_smartDialListTable,0,sizeof(m_smartDialListTable));
		   for (k=0;k<nowHeap;k++){
			   m_smartDialListTable[k].store_index= m_smartDialSearchTable[k].store_index;
			   m_smartDialListTable[k].matchCount = m_smartDialSearchTable[k].matchCount;
			   
			   
			   
		   }
		   
		   
		   list_count =nowHeap;
		   
		   
	}	
	preDialLen = dialLen;
	if(PhoneBookEntryCount==0)
	{
		list_count =0;
	}
	SmartDial_number_of_items = list_count;
	
	return (list_count+1);
}

//返回为1表示匹配成功，0表示匹配失败
int IsSubStr(S8 *motherStr, S8 *sonStr, int subLen)
{
	int flag = 0;
	int i,j;
	int motherLen = strlen(motherStr);
	int sonLen = strlen(sonStr);
	
	
	for (i=0; i< (motherLen-sonLen+1);i++){
		for(j=0;j<subLen;j++){
			if (motherStr[i+j] != sonStr[j]){
				flag = 1;
				break;
			}
		}
		if (0 == flag){
			return 1;
		}
		flag = 0;
	}
	return 0;
}

#endif

#endif	/* _PHONEBOOKSEARCH_C*/


