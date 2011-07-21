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
 * PhoneBookStubsToOthers.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   PhoneBook APIs for other applications
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
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _PHONEBOOKSTUBSTOOTHERS_C
#define _PHONEBOOKSTUBSTOOTHERS_C

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"	/*debug info*/
#include "protocolevents.h"
#include "gui_data_types.h"
#include "allappgprot.h"
#include "phonebookprot.h"
#include "phonebookdef.h"
#include "phonebookgprot.h"
#include "unicodexdcl.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "nvram_data_items.h"
#include "idleappdef.h"
#include "idleappprot.h"
#include "timerevents.h"
#include "globaldefs.h"
#include "speeddial.h"
#include "commonscreens.h"

#if defined(MMS_SUPPORT)/*For MMS*/
#include "nvramtype.h"
#include "wap_ps_struct.h"
#endif

#if defined(__MOD_SMSAL__)
#include "messagesexdcl.h"
#include "messagesresourcedata.h"
#endif

#if defined(__MMI_FILE_MANAGER__) /*Also for __MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__*/
#include "filesystemdef.h"
/*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 11:3 */
#include "filemanagergprot.h"
#include "mmi_trace.h"
/*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 11:3 */
#include "gdi_image.h" 	/* graphic library*/
#endif

#include "settingdefs.h"
#include "settingsgdcl.h"
#include "settinggenum.h"
#include "securitysetup.h"

#if defined(__MMI_EMAIL__)
#include "emailappgprot.h" /* email app list screen*/
#endif

#if defined(__MMI_VRSD_DIAL__)
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "vrsd.h"
#include "vrsdenum.h"
#include "vrsddial.h"	/*voice dial application list screen*/
#endif

#if defined(__MMI_VRSI__)
#include "mdi_datatype.h"
#include "vrsienum.h"
#include "vrsitype.h"
#include "vrsiprot.h"
#endif

#if defined(MMS_SUPPORT)
#include "wapprot.h"
#endif

#ifdef __MMI_PHB_BIRTHDAY_FIELD__
#include "app_datetime.h"
#endif

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "wgui_categories_multimedia.h"
#undef __NEWSIMULATOR
#include "simdetectiongprot.h"
/*
** Local Variable
*/
MMI_PHB_LIST_VIEW phbListView;
U8 g_phb_enter_from;  /*Identify Save number from idle screen*/
FuncPhbListCallback g_phb_list_callback = NULL; /*For generic list call back function*/

/*
** Global Variable
*/
extern U16 PhoneBookEntryCount;
extern MMI_PHB_ENTRY_BCD_STRUCT			PhoneBook[];
extern const U8 AsciiToDefaultArray[];
extern const U8 DefaultToAsciiArray[];

#ifdef __MMI_PHB_BIRTHDAY_FIELD__
extern const kal_uint8 g_dt_day_per_mon[];
extern S8 pbDay[];
extern S8 pbMon[];
extern S8 pbYear[];
#endif /* __MMI_PHB_BIRTHDAY_FIELD__ */

#ifndef __MMI_PHB_NO_OPTIONAL_FIELD__
extern PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;
#endif

extern S8 pbName[];
extern S8 pbNumber[];
extern S8 pbHomeNumber[];
extern S8 pbCompanyName[];
extern S8 pbEmailAddress[];
extern S8 pbOfficeNumber[];
extern S8 pbFaxNumber[];

#if defined(__MMI_FILE_MANAGER__)
extern S8 g_phb_file_path[];	/*Select file from file manager*/
static S8 phb_caller_pic_file_path[MMI_PHB_MAX_PATH_LENGTH];
#endif

extern U16 g_phb_name_index[];

#if defined(__MMI_INCOMING_CALL_VIDEO__)
extern S8 g_phb_video_file_path[];	/*Select video file from file manager*/
#endif

/*
** Global Function
*/
extern void IdleShowScrInvalidLocation(void);
extern void IdleShowScrNoPhoneNumEntry(void);
extern void SendPhbEntryForDivert(U8[], U8[]);
extern void CBackCallDeflectNumberFromPB(PS8 CallDeflectBuffer);
extern void mmi_msg_get_number_from_phb (U8 name[], U8 number[]);
#if defined(__MMI_MESSAGES_CHAT__)
extern void SetNameNumberForChatScreen(U8 name[], U8 number[]);
#endif
extern void mmi_msg_send_msg_to_group (U16, U16*);
extern void mmi_frm_sms_go_back_from_send(void);
extern void mmi_frm_sms_end_key_from_send(void);
extern MMI_PHB_ENTRY_STRUCT* mmi_phb_get_sim_entry(void);
#if defined(__MMI_MESSAGES_INSERT_FROM_PHB__)
extern void SetNameNumberForSMSInsert(U8 name[], U8 number[]);
#endif

/*****************************************************************************
* FUNCTION
*  lookUpName()
* DESCRIPTION
* Search for the number entry for corresponding name input
*
* PARAMETERS
*  name	IN	input name to be serach
* RETURNS
*  result number string if found
* GLOBALS AFFECTED
*   none
*****************************************************************************/
PS8 lookUpName (PS8 name)
{
	U16 i;
	U16 store_index;

	for(i = 0; i < PhoneBookEntryCount; i++)
	{
		store_index = g_phb_name_index[i];

		if (pfnUnicodeStrcmp((PS8)name, (PS8)PhoneBook[store_index].alpha_id.name) == 0)
		{
			mmi_phb_convert_get_ucs2_number(pbNumber, i);	/*BCD number format.*/
			return (S8*)pbNumber;
		}
	}
	return NULL;
}

/*****************************************************************************
* FUNCTION
*  lookUpNumber()
* DESCRIPTION
* Search for the name entry for corresponding number input
*
* PARAMETERS
*  number	IN	input number to be serach
* RETURNS
*  result name string if found
* GLOBALS AFFECTED
*   none
*****************************************************************************/
PS8 lookUpNumber (PS8 number)
{
	U16 i;
	U8 flag = 0;
	U32 num;
	U8 number_ASCII[MAX_PB_NUMBER_LENGTH+1+1];
	U32 len = 0;

	MMI_TRACE((MMI_TRACE_G4_PHB, MMI_PHB_FUNC_lookUpNumber));
	len = pfnUnicodeStrlen(number);
	MMI_ASSERT(len <= MAX_PB_NUMBER_LENGTH);
	memset(number_ASCII, 0, ENCODING_LENGTH);
	UnicodeToAnsii((PS8)number_ASCII, (PS8)number);

	/*Lookup into table when (1) lookup table not empty (2)Not Processing (3)All Entries populated and sorted*/
	if(g_phb_cntx.lookup_table_count && !g_phb_cntx.processing && (g_phb_cntx.populate_count == 0xffff))
	{
		num = mmi_phb_util_convert_number_to_int(number_ASCII);
		if(num < INVALID_NUMBER)
		{
			i = mmi_phb_lookup_table_search(num, 0, (U16)(g_phb_cntx.lookup_table_count-1),(S8*)number_ASCII);

			if(i < 0xffff) /* i is storage location in array, begin from 0.*/
				flag = 1;
		}
	}

	if(flag)
		return (PS8)PhoneBook[i].alpha_id.name;
	else
		return NULL;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_generic_enter_list()
* DESCRIPTION
*   Phonebook list interface for other application.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_generic_enter_list(FuncPhbListCallback func_list)
{
	phbListView = MMI_PHB_LIST_FOR_ALL_SHARED;
	g_phb_list_callback = func_list;
	mmi_phb_list_pre_entry_second_level();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_generic_enter_list_result()
* DESCRIPTION
*   choose entry result, send result back to caller app.
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_generic_enter_list_result(void)
{
	S16 pError;
	U16 store_index;
	MMI_PHB_VCARD_STRUCT* result_entry = OslMalloc(sizeof(MMI_PHB_VCARD_STRUCT));

	/*Clear buffer*/
	memset(result_entry,0, sizeof(MMI_PHB_VCARD_STRUCT));

	store_index = g_phb_name_index[g_phb_cntx.active_index_second];

	/*Copy Name*/
	pfnUnicodeStrcpy(result_entry->name, (S8*)PhoneBook[store_index].alpha_id.name);

	/*Copy Number - ASCII Type*/
	mmi_phb_convert_to_digit((U8*)result_entry->number, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	if(store_index < MAX_PB_PHONE_ENTRIES)	/*in NVRAM*/
	{
    #if 0
		ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    #endif
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
		strcpy(result_entry->homeNumber, (PS8)PhoneBookOptionalFields.homeNumber);
		strcpy(result_entry->officeNumber, (PS8)PhoneBookOptionalFields.officeNumber);
		strcpy(result_entry->faxNumber, (PS8)PhoneBookOptionalFields.faxNumber);
		strcpy(result_entry->emailAddress, (S8*)PhoneBookOptionalFields.emailAddress);
		pfnUnicodeStrcpy(result_entry->companyName, (PS8)PhoneBookOptionalFields.companyName);
	}
#endif

	/*Send back to email app.*/
	if(g_phb_list_callback != NULL)
		g_phb_list_callback(result_entry);

	OslMfree(result_entry);
	g_phb_list_callback = NULL;
	DeleteScreenIfPresent(SCR_PBOOK_LIST);
}

#define MMI_PHB_IDLE_SCREEN
/**************************************************************

	FUNCTION NAME		: mmi_phb_idle_enter_phb_list()

  	PURPOSE				: Displays PHB list from RSK of idle screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_idle_enter_phb_list(void)
{
	g_idle_context.ToNameScrFromIdleApp = 1;
	g_idle_context.RskPressedFromIdleApp = 1;
	phbListView = MMI_PHB_LIST_FOR_PHB;
	g_phb_cntx.highlight_entry = 0;

#if defined(__MMI_PHB_RSK_QUICK_SEARCH__)
	mmi_phb_quick_search_list_pre_entry();
#else
	mmi_phb_list_pre_entry();
#endif

#if (defined(__MMI_KEYPAD_LOCK_PATTERN_1__) && !defined(__MMI_DISABLE_KEYPAD_LOCK__))
/*zhangm Add Start For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
//	if(g_idle_context.RskPressedFromIdleApp == 1)
//	{
//		StartTimer(KEYPAD_LOCK_TIMER, KEYPAD_LOCK_TIMEOUT, IdleHandleKeypadLockProcess);
//		SetKeyHandler(IdleHandlePoundKeyForKeypadLock,KEY_POUND,KEY_EVENT_UP);
//	}
/*zhangm endStart For 10821 Ver: GreenStone  on 2008-12-19 16:23 */	
#endif
}


/*****************************************************************************
* FUNCTION
*  SaveNumberFromIdleScrn()
* DESCRIPTION
* Saves number from idle screen
*
* PARAMETERS
*  number	IN	number string to be saved
*  entry	IN	enter location
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SaveNumberFromIdleScrn (PS8 number, U8 enter)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<SaveNumberFromIdleScrn.>\n",__FILE__,__LINE__));

	/*Check number length first*/
	if(pfnUnicodeStrlen((PS8)number) > MAX_PB_NUMBER_LENGTH+1)
	{
		DisplayPopup((PU8)GetString(STR_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
	else
	{
		g_phb_cntx.set_done_flag = 1;
		pfnUnicodeStrcpy(pbNumber, number);

		/*Avoid add entry while copy all and delete all*/
		if(!g_phb_cntx.phb_ready || g_phb_cntx.processing)
		{
			mmi_phb_entry_not_ready();
		}
		else
		{
			g_phb_enter_from = enter;
			mmi_phb_clear_old_history();
			mmi_phb_op_add_pre_entry();
		}
	}
}

#ifdef __MMI_PHB_BIRTHDAY_FIELD__
/*****************************************************************************
 * FUNCTION
 *  mmi_phb_retrieve_bday
 * DESCRIPTION
 *  parse birthday field of a vcard object.s
 * PARAMETERS
 *  b_string        [?]         
 *  pb_year         [?]         
 *  pb_mon          [?]         
 *  pb_day          [?]         
 *  change_flag     [IN]        
 *  String(?)       [IN]        String read from file for one line
 * RETURNS
 *  parse error cause(?)
 *****************************************************************************/
void mmi_phb_retrieve_bday(S8 *b_string, S8 *pb_year, S8 *pb_mon, S8 *pb_day, BOOL change_flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 temp_buf[10 + 1]; /* 19990101 or 1999-01-01 */
    S8 ascii_buff[6];

    U16 b_year;
    U8 b_month;
    U8 b_day;
    U8 is_valid = TRUE;
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* b_string; */
    memset(temp_buf, 0, sizeof(temp_buf));
    strncpy(temp_buf, b_string, 10);

    /* Year */
    strncpy(ascii_buff, temp_buf, 4);
    ascii_buff[4] = '\0';
    mmi_asc_to_ucs2(pb_year, ascii_buff);
    b_year = (U16) gui_atoi((U16*) pb_year);
    i = 4;
    /* Month */
    if (*(temp_buf + i) == '-')
    {
        i++;
    }
    strncpy(ascii_buff, temp_buf + i, 2);
    ascii_buff[2] = '\0';
    mmi_asc_to_ucs2(pb_mon, ascii_buff);
    b_month = (U8) gui_atoi((U16*) pb_mon);
    i += 2;
    /* Day */
    if (*(temp_buf + i) == '-')
    {
        i++;
    }    
    strncpy(ascii_buff, temp_buf + i, 2);
    ascii_buff[2] = '\0';
    mmi_asc_to_ucs2(pb_day, ascii_buff);
    b_day = (U8) gui_atoi((U16*) pb_day);

    if (b_year > MMI_PHB_BDAY_MAX_YEAR_INT || b_year < 1900)
    {
        is_valid = FALSE;
    }
    else if (b_month > 12 || b_month < 1)
    {
        is_valid = FALSE;
    }

    if (b_month == 2)
    {
        if (b_day > (g_dt_day_per_mon[b_month - 1] + applib_dt_is_leap_year(b_year)) || b_day < 1)
            is_valid = FALSE;
    }
    else if (b_day > g_dt_day_per_mon[b_month - 1] || b_day < 1)
    {
        is_valid = FALSE;
    }

    if (change_flag)
    {
        if (is_valid)
        {
            mmi_phb_bday_set_changed();
        }
        else
        {
            mmi_phb_util_clear_bday_buffer();
        }
    }
}
#endif /* __MMI_PHB_BIRTHDAY_FIELD__ */ 

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_sim_entry_by_location()
* DESCRIPTION
* Return the number to idle screen for quick access
*
* PARAMETERS
*  entryNum	IN	location to be search
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_sim_entry_by_location(U16 entryNum)
{
	U16 i;
	U16 record_index = 0;
	U8 hasEntry = 0;

#ifdef __MMI_MULTI_SIM__
	U16 nSimTotal = 0;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!MTPNP_PFAL_Is_CardAbsent(i))
			nSimTotal += g_phb_cntx.sim_total[i];
	}
	if(!g_phb_cntx.phb_ready || g_phb_cntx.processing)
		mmi_phb_entry_not_ready();
	else if((MTPNP_PFAL_Is_CardAbsent(0) == TRUE) && (MTPNP_PFAL_Is_CardAbsent(1) == TRUE))
	{
	}
	else if(entryNum == 0 ||entryNum > nSimTotal)
	{
		IdleShowScrInvalidLocation();
	}

	else
	{
		for(i = 0 ; i < PhoneBookEntryCount ; i++)
		{
			if(g_phb_name_index[i] >= MAX_PB_PHONE_ENTRIES )
			{
				U8 nSimIndex = 0;
				U8 j;
				nSimIndex = (g_phb_name_index[i] - MAX_PB_PHONE_ENTRIES)/MAX_PHB_SIM_ENTRY;
				nSimTotal = 0;
				for(j = 0; j < nSimIndex; j++)
				{
					if(!MTPNP_PFAL_Is_CardAbsent(i))
						nSimTotal += g_phb_cntx.sim_total[i];
				}
				record_index = g_phb_name_index[i] - MAX_PB_PHONE_ENTRIES - (MAX_PHB_SIM_ENTRY *nSimIndex - nSimTotal) + 1;
			}
			mmi_trace(g_sw_PHB, "PHB  FUNC:%s, record_index = %d", __FUNCTION__, record_index);
			if(record_index == entryNum)
			{
				g_phb_cntx.active_index = i;
				hasEntry = 1;
				mmi_phb_entry_op_view_entry();
			}
		}

		if(!hasEntry)
			IdleShowScrNoPhoneNumEntry();

	}
#else
	if(!g_phb_cntx.phb_ready || g_phb_cntx.processing)
		mmi_phb_entry_not_ready();
	else if(entryNum == 0 || entryNum > g_phb_cntx.sim_total)
		IdleShowScrInvalidLocation();
	else
	{
		for(i = 0 ; i < PhoneBookEntryCount ; i++)
		{
			record_index = g_phb_name_index[i] - MAX_PB_PHONE_ENTRIES + 1;
			if(record_index == entryNum)
			{
				g_phb_cntx.active_index = i;
				hasEntry = 1;
				mmi_phb_entry_op_view_entry();
			}
		}

		if(!hasEntry)
			IdleShowScrNoPhoneNumEntry();

	}
#endif	
}

#define MMI_PHB_INTERFACE_FOR_CALL
/*****************************************************************************
* FUNCTION
*  mmi_phb_call_get_data_for_call_mgnt()
* DESCRIPTION
* Returns the names/ringtone/image tag/caller group details for the incoming
* call number if it is present in PHB
* PARAMETERS
*  number	IN	number string to be matched
* RETURNS
*  PHB_CM_INTERFACE	a interface structure
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#ifdef __MMI_MULTI_SIM__
PHB_CM_INTERFACE mmi_phb_call_get_data_for_call_mgnt(PU8 number, UINT8 nSimId)
#else
PHB_CM_INTERFACE mmi_phb_call_get_data_for_call_mgnt(PU8 number)
#endif
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 store_index = 0xffff, grp_id;
	PHB_CM_INTERFACE phbDetails;
	U8 flag = 0;
	U32 num;
	U8 number_ASCII[MAX_PB_NUMBER_LENGTH+1+1];
	U8* number_ptr;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids;
	PHB_CALLER_GROUP_STRUCT *callerGroups;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	U16 video_id;
#endif
	U8 bFdlStatus = MMI_FALSE;
	int i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	//MMI_TRACE((MMI_TRACE_G4_PHB, MMI_PHB_FUNC_GetPHBDataForCM));

	memset(number_ASCII, 0, ENCODING_LENGTH);
	memset((void*)&phbDetails, 0, sizeof(phbDetails));

	phbDetails.pictureId = IMG_PHB_DEFAULT;
	phbDetails.dialInList = MMI_PHB_NONE;

	if(NULL == number)
	{
		mmi_trace(g_sw_CC, "mmi_phb_call_get_data_for_call_mgnt, no number to use");
		return phbDetails;
	}
	UnicodeToAnsii((PS8)number_ASCII, (PS8)number);

	mmi_trace(g_sw_CC,"---[PhonebookStubsToOthers.c] mmi_phb_call_get_data_for_call_mgnt() => number: [%s] dial list:[%d] \n", number_ASCII, g_phb_cntx.dial_from_list);

	/*
	*  Display as Emergency Number
	*/

	number_ptr = number_ASCII;
	/*Ignore  *31# and #31# case*/
	while( ((number_ptr[0] == '*') && (number_ptr[1] == '3') && (number_ptr[2] == '1') && (number_ptr[3] == '#')) ||
			((number_ptr[0] == '#') && (number_ptr[1] == '3') && (number_ptr[2] == '1') && (number_ptr[3] == '#'))  )
	{
		number_ptr += 4;
	}

    if (CheckValidEmergencyNo1((S8*) number))
    {
        AnsiiToUnicodeString((PS8) phbDetails.number, (S8*) number_ptr);
        g_phb_cntx.dial_from_list = MMI_PHB_NONE;   /* Clear dial from list flag */

        pfnUnicodeStrncpy((PS8) phbDetails.name, GetString(STR_EMERGENCY_NUMBER), MAX_PB_NAME_LENGTH);
        if (GetUCS2Flag((PS8) phbDetails.name))
        {
            phbDetails.name_dcs = MMI_PHB_UCS2;
        }
        else
        {
            phbDetails.name_dcs = MMI_PHB_ASCII;
        }

        return phbDetails;
    }

    /* Check if dial out from phonebook list */
    if (g_phb_cntx.dial_from_list == MMI_PHB_PHONEBOOK)
    {
        g_phb_cntx.dial_from_list = MMI_PHB_NONE;
        flag = 1;
        store_index = g_phb_name_index[g_phb_cntx.active_index];
        phbDetails.dialInList = MMI_PHB_PHONEBOOK;
    }
	/*
	*  Check if dial out from FDN list,
	*  note that should enter here only from FDN list screen
	*/
	else if(g_phb_cntx.dial_from_list == MMI_PHB_FDN)
	{
		g_phb_cntx.dial_from_list  = MMI_PHB_NONE;

        /* Fill up structure and return FDN data directly here */
        pfnUnicodeStrcpy(
            (S8*) phbDetails.name,
            (S8*) gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].alpha_id.name);
        phbDetails.name_dcs = gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].alpha_id.name_dcs;

        pfnUnicodeStrcpy(
            (S8*) phbDetails.number,
            (S8*) gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].tel.number);

		phbDetails.dialInList = MMI_PHB_FDN;
		return phbDetails;
	}
	/*
	*  Check if dial out from SDN list,
	*  note that should enter here only from SDN list screen
	*/
	else if(g_phb_cntx.dial_from_list == MMI_PHB_SDN)
	{
		MMI_PHB_ENTRY_STRUCT* sdn_entry = mmi_phb_get_sim_entry();
		g_phb_cntx.dial_from_list  = MMI_PHB_NONE;

		/*Fill up structure and return SDN data directly here*/
		pfnUnicodeStrcpy((S8*)phbDetails.name, (S8*)sdn_entry->alpha_id.name);
		phbDetails.name_dcs = sdn_entry->alpha_id.name_dcs;
		pfnUnicodeStrcpy((S8*)phbDetails.number, (S8*)sdn_entry->tel.number);

		phbDetails.dialInList = MMI_PHB_SDN;
		return phbDetails;
	}
	
	/*
	*	Not dial from list.
	*/
	else
	{
		/*Lookup into table when (1)  lookup table not empty (2) Not Processing (3) All Entries populated and sorted*/
		if(g_phb_cntx.lookup_table_count && !g_phb_cntx.processing&& (g_phb_cntx.populate_count == 0xffff))
		{
			num = mmi_phb_util_convert_number_to_int(number_ASCII);
			if(num < INVALID_NUMBER)
			{
				g_phb_cntx.searched_number_type = 0;
				store_index = mmi_phb_lookup_table_search(num, 0, (U16)(g_phb_cntx.lookup_table_count-1),(S8*)number_ptr);

				if(store_index < 0xffff) /* i is storage location in array, begin from 0.*/
					flag = 1;
			}
		}
	}

	/*
	* Map Number with one enrtry.
	*/
	mmi_trace(g_sw_CC," Func: %s flag=%d", __FUNCTION__,flag);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.FdlStatus[i])
		{
			bFdlStatus = MMI_TRUE;
			break;
		}
	}
	if(flag)
	{
		pfnUnicodeStrcpy((PS8)phbDetails.name, (PS8)PhoneBook[store_index].alpha_id.name);
		phbDetails.name_dcs = PhoneBook[store_index].alpha_id.name_dcs;
		if (phbDetails.dialInList != MMI_PHB_PHONEBOOK) /* CM should use its number if dial from Phonebook list */
		{
			if((store_index < MAX_PB_PHONE_ENTRIES) && g_phb_cntx.searched_number_type != 0)
			{
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
				switch (g_phb_cntx.searched_number_type)
				{
					case MMI_PHB_ENTRY_FIELD_HOME:
						AnsiiToUnicodeString((PS8)phbDetails.number, (S8*)PhoneBookOptionalFields.homeNumber);
						break;
					case MMI_PHB_ENTRY_FIELD_OFFICE:
						AnsiiToUnicodeString((PS8)phbDetails.number, (S8*)PhoneBookOptionalFields.officeNumber);
						break;
					case MMI_PHB_ENTRY_FIELD_FAX:
						AnsiiToUnicodeString((PS8)phbDetails.number, (S8*)PhoneBookOptionalFields.faxNumber);
						break;
					default:
						MMI_ASSERT(0);
						break;
				}
#endif                
			}
			else
			{
				mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
				mmi_phb_convert_get_ucs2_number((S8*)phbDetails.number, store_index);
			}
		}

		/*
		*  Caller Group Setup
		*/
#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		if (store_index < MAX_PB_PHONE_ENTRIES)
#endif
		{
			callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

			mmi_phb_read_optional_ids(store_index); /*i is store index*/
			opt_ids = (PHB_OPTIONAL_IDS_STRUCT*) g_phb_cntx.optional_ids;

			grp_id = opt_ids->callerGroupID;
			if(grp_id)
			{
				phbDetails.alertType	= callerGroups[grp_id-1].alertType;
				phbDetails.backlightId	= callerGroups[grp_id-1].LEDPatternId;
				phbDetails.ringtoneId	= callerGroups[grp_id-1].ringToneID;
				if(callerGroups[grp_id-1].pictureTagID == 1) /*Associate Picture is in the file system.*/
				{
#if defined(__MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__)
					phbDetails.pictureId = 1;
					phbDetails.record_index = 5000 + grp_id;
#endif
				}
				else if (callerGroups[grp_id-1].pictureTagID != 0) /* 0 means default image */
				{
					phbDetails.pictureId	= callerGroups[grp_id-1].pictureTagID;
				}

				/*Incoming Call Video*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
				if(callerGroups[grp_id-1].VideoID)
				{
					phbDetails.videoId = callerGroups[grp_id-1].VideoID;
					phbDetails.video_record_index = (U16)(MAX_PB_PHONE_ENTRIES + grp_id); /* MAX_PB_PHONE_ENTRIES for group 1 and so on.*/
				}
#endif
			}
		}

		/*
		*  Personal Entry Setup, these will overwrite caller group setting.
		*/
		if (store_index < MAX_PB_PHONE_ENTRIES)
		{
			/*Associate Pictures*/
			if(opt_ids->pictureTagID == 1)
			{
#if defined(__MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__)
					phbDetails.pictureId = 1;
					phbDetails.record_index = store_index+1;	/*Record Must in NVRAM, so the record index is store_index + 1*/
#endif
#if defined(__MMI_INCOMING_CALL_VIDEO__)
				phbDetails.videoId = 0; /*If Image Selected, should not play caller group's video*/
#endif
			}
			else if((opt_ids->pictureTagID != IMG_PHB_DEFAULT) && (opt_ids->pictureTagID != 0x0000))
			{
				phbDetails.pictureId = opt_ids->pictureTagID;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
				phbDetails.videoId = 0; /*If Image Selected, should not play caller group's video*/
#endif
			}

			/*Associate Ring Tone*/
			if(opt_ids->ringToneID)
				phbDetails.ringtoneId = opt_ids->ringToneID;

			/*For Incoming Call Video*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
			if((video_id = mmi_phb_video_get_id_by_index(store_index)) != 0)
			{
				phbDetails.videoId = video_id;
				phbDetails.video_record_index = store_index+1; /*Record Must in NVRAM, so the record index is store_index + 1*/
			}
#endif
		}
	}
	/*
	*   If entry is not found, try to search in FDN list to see if number can be match and dial out.
	*   This should be search when FDN is enable.
	*   Note: FDN list is stored in ADN storage when FDN is enable.
	*/
	else if(bFdlStatus)
	{
		//Vivian add for FDL 2007/06/19
		mmi_trace(1, "else bFdlStatus is TRUE");
		
#ifdef __MMI_MULTI_SIM__
		if(g_SecuritySetupContext.FdlStatus[nSimId])
		{
			gpFDLBDLContext = &gFDLBDLContext[nSimId];
		}
#endif

		/*Fill up structure and return FDN data directly here*/
		for(i = 0;i<gpFDLBDLContext->FDNBDNCount;i++)
		{
			if(!pfnUnicodeStrcmp(number,gpFDLBDLContext->FDLBDLEntries[i].tel.number))
			{
				mmi_trace(1, "yes");
					pfnUnicodeStrcpy((S8*)phbDetails.name, (S8*)gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name);
			   		phbDetails.name_dcs = gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_dcs;
					pfnUnicodeStrcpy((S8*)phbDetails.number, (S8*)gpFDLBDLContext->FDLBDLEntries[i].tel.number);
			}
		}
		//Vivian add end for FDL 2007/06/19
		
		/*Vivian Del for FDL 2007/06/19
		UnicodeToAnsii((PS8)number_ASCII, (PS8)number);
		store_index = mmi_phb_fdn_search_by_number((S8*)number_ASCII);
		if(store_index < 0xffff)
		{
			pfnUnicodeStrcpy((PS8)phbDetails.name, (PS8)PhoneBook[store_index].alpha_id.name);
			phbDetails.name_dcs = PhoneBook[store_index].alpha_id.name_dcs;

			mmi_phb_convert_get_ucs2_number((S8*)phbDetails.number, store_index);
		}
		//Vivian Del end for FDL 2007/06/19 */
	}
	UnicodeToAnsii((PS8)number_ASCII, (PS8)phbDetails.number);

	mmi_trace(g_sw_CC,"---[PhonebookStubsToOthers.c] mmi_phb_call_get_data_for_call_mgnt() => number: [%s] dial list:[%d] \n", number_ASCII, g_phb_cntx.dial_from_list);

	return phbDetails;
}

extern MMI_PHB_LOOKUP_NODE_STRUCT LookUpTable[];
UINT8 mmi_phb_get_callergroup_by_number(PUINT8 number)
{
    UINT8 number_ASCII[MAX_PB_NUMBER_LENGTH+1+1];
    UINT32 num;
    U16 store_index = 0xffff;
    S16 matchcount = 0;  
    U16 firstindex = 0;  	 
    UINT8 g_group_id = 0;		
    U8 optid_record = 0;
    U8 optid_index = 0;
    S16 pError;
    PHB_OPTIONAL_IDS_STRUCT *pPhbOptIDs = NULL;
    PHB_OPTIONAL_IDS_STRUCT *pCurrentPhbOptIDs = NULL;

    UnicodeToAnsii((PUINT8)number_ASCII, (PUINT8)number);
    num = mmi_phb_util_convert_number_to_int(number_ASCII);
    //get the number index in phoneBook
    matchcount = mmi_phb_lookup_table_n_search(num, 0, (U16)(g_phb_cntx.lookup_table_count-1),(S8*)number_ASCII, &firstindex);

    mmi_trace(g_sw_PHB, "PHB Func:%s matchcount is %d", __FUNCTION__, matchcount);

    if(matchcount > 0)
    {
        pPhbOptIDs = (PHB_OPTIONAL_IDS_STRUCT *)OslMalloc(NVRAM_PHB_IDS_TOTAL * OPTIONAL_IDS_RECORD_SIZE);
        if(pPhbOptIDs == NULL)
        {
            return g_group_id;
        }
        else
        {
            memset(pPhbOptIDs , 0, (NVRAM_PHB_IDS_TOTAL * OPTIONAL_IDS_RECORD_SIZE));
        }
        
        for(; optid_record < NVRAM_PHB_IDS_TOTAL;optid_record++)
        {
            ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (pPhbOptIDs + optid_record*OPTIONAL_IDS_RECORD_TOTAL), OPTIONAL_IDS_RECORD_SIZE, &pError);
        }
    }

    for(; matchcount > 0; matchcount--)
    {
        store_index = LookUpTable[firstindex].store_index;

        if(store_index < 0xffff)
        {
            optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
            optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;
            
            pCurrentPhbOptIDs = pPhbOptIDs + optid_record*OPTIONAL_IDS_RECORD_TOTAL + optid_index;
            if(pCurrentPhbOptIDs->callerGroupID > MAX_PB_CALLER_GROUPS)
            {
                pCurrentPhbOptIDs->callerGroupID  = 0;
            }

            g_group_id |= 1 << pCurrentPhbOptIDs->callerGroupID;

            mmi_trace(g_sw_PHB, "PHB mmi_phb_get_callergroup_by_number, store_index = %d, opt_ids->callerGroupID is %d", store_index, pCurrentPhbOptIDs->callerGroupID);
        }
        firstindex++;	
    }
    
    if(pPhbOptIDs != NULL)
    {
        OslMfree(pPhbOptIDs);
    }
    
    return g_group_id;
}

UINT8 mmi_phb_get_black_list_flag_by_group(UINT8 group_id)
{
    UINT8 ret = 0;
    PHB_CALLER_GROUP_STRUCT* caller_group;

    if(group_id <= 0)
    {
        return ret;    
    }
	
    caller_group = (PHB_CALLER_GROUP_STRUCT*)&((PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group)[group_id - 1];
    ret = caller_group->black_list_flag_Id;

    return ret;
}

UINT8 g_red_list;
UINT8 mmi_phb_get_red_list_flag_by_group(UINT8 group_id)
{
    UINT8 ret = 0;
    PHB_CALLER_GROUP_STRUCT* caller_group;

    if(group_id <= 0)
    {
        return ret;    
    }	
	
    caller_group = (PHB_CALLER_GROUP_STRUCT*)&((PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group)[group_id - 1];
    ret = caller_group->red_list_flag_Id;

    g_red_list = ret;
    return ret;
}

UINT8 mmi_phb_group_in_black_list(PUINT8 number)
{
    UINT8 group;
    UINT8 flag = 0;
    UINT8 i;
  
    group = mmi_phb_get_callergroup_by_number( number);
	
    for(i = 1; i < 6; i++)
    {
        if(group & (1 << i))
        {
            flag |= mmi_phb_get_black_list_flag_by_group(i);
        }
    }
	
    mmi_trace(g_sw_PHB, "PHB Func:%s flag = %x", __FUNCTION__, flag);	
    return flag;
}

UINT8 mmi_phb_group_in_red_list(PUINT8 number)
{
    UINT8 group;
    UINT8 flag = 0;
    UINT8 i;
   
    group = mmi_phb_get_callergroup_by_number( number);

    for(i = 1; i < 6; i++)
    {
        if(group & (1 << i))
        {
            flag |= mmi_phb_get_red_list_flag_by_group(i);
        }
    }

    mmi_trace(g_sw_PHB, "PHB Func:%s flag = %x", __FUNCTION__, flag);	
    return flag;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_call_set_dial_from_list()
* DESCRIPTION
* Let call management to set dial from list flag.
*
* PARAMETERS
*  value	IN	TRUE/FALSE value
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_call_set_dial_from_list(U8 value)
{
	g_phb_cntx.dial_from_list = value;
}

#if defined(__MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__) || defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_image_get_path_from_id()
* DESCRIPTION
*   This function returns the phonebook associate picture image path by index.
*
* PARAMETERS
*  index	IN	index of a phonebook record. (if >5000, it is a caller group image)
*
* RETURNS
*  full path of image
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
#include "nvram_user_defs.h"
#include "nvramtype.h"
#ifdef MMI_ON_WIN32
#define NVRAM_EF_PHB_PIC_SIZE 25600
#endif

BOOL mmi_phb_image_read_nvram(U16 record_index, S8* path)
{
	S16 err;
	S8* pb_pics;
       S32 writen;
	Trace("mmi_phb_image_read_nvram %d",record_index);
	if(record_index<0||record_index> PHONE_PHB_ENTRY_COUNT-1)
	{
		Trace("mmi_phb_image_read_nvram record_index %d",record_index);
		return FALSE;
	}
	pb_pics = COS_MALLOC(NVRAM_EF_PHB_PIC_SIZE);
	if(!pb_pics)
	{
		memset(path,0,MMI_PHB_MAX_PATH_LENGTH);
		//MMI_ASSERT(0);
		return FALSE;
	}
	memset( pb_pics, 0, NVRAM_EF_PHB_PIC_SIZE);

	writen=ReadRecord(NVRAM_EF_PHB_PIC_LID, 0, pb_pics, NVRAM_EF_PHB_PIC_SIZE, &err);
	//save the image path to nvram
	//MMI_ASSERT(writen == NVRAM_EF_PHB_PIC_SIZE);

	memcpy(path,&(pb_pics[record_index*128]),128);

      COS_FREE(pb_pics);
       Trace("mmi_phb_image_read_nvram %d",err);

	if(err!=NVRAM_READ_SUCCESS)
	{
		memset(path,0,ENCODING_LENGTH);
        return FALSE;
	}
    return TRUE;
}
#endif
S8* mmi_phb_image_get_path_from_id(U16 record_index)
{
	S16 drive;
	S8* path;
	FS_HANDLE fh;
	S32 width, height;

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	MMI_TRACE((MMI_TRACE_G4_PHB, MMI_PHB_IMAGE_GET_PATH_FROM_ID_1, record_index));

	//drive = MMI_PHB_DRV;
	drive = 67;// dingjian 20080329 device name 'C'
	TRACE_FMGR_FUNCTION();
	if (drive > 0)
	{
		path = (S8*)phb_caller_pic_file_path;	/*Use global static buffer to store path*/

#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
		if(!mmi_phb_image_read_nvram(record_index-1,path))
    		mmi_phb_image_get_full_path_by_index(record_index,path);
#else
		mmi_phb_image_get_full_path_by_index(record_index,path);
#endif

		/*Try to open file and check image format. If can't open, use default picture to display.*/
		if(pfnUnicodeStrlen(path) > 0)
		{
			fh = MMI_FS_Open((U8*)path, FS_READ_ONLY);

			MMI_TRACE((MMI_TRACE_G4_PHB, MMI_PHB_IMAGE_GET_PATH_FROM_ID_2, fh));
			if(fh >= 0)
			{
				MMI_FS_Close(fh);
				if (gdi_image_get_dimension_file((S8*)path, &width, &height) >= 0)
				{
					MMI_TRACE((MMI_TRACE_G4_PHB, MMI_PHB_IMAGE_GET_PATH_FROM_ID_3));
					return path;
				}
			}
		}
	}

	return NULL;

}
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_video_get_path_from_id()
* DESCRIPTION
*   This function returns the phonebook associate video path by index.
*
* PARAMETERS
*  index	IN	index of a phonebook record.
*
* RETURNS
*  full path of image
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8* mmi_phb_video_get_path_from_id(U16 record_index)
{
	U16 store_index = record_index - 1;
	U16 video_id = 0;

	PHB_CALLER_GROUP_STRUCT* callerGroups;
    mmi_trace(g_sw_PHB, "MMI_Call: Func: %s store_index:%d", __FUNCTION__, store_index);

	/*In phonebook entry*/
	if(store_index < MAX_PB_PHONE_ENTRIES)
	{
		video_id = mmi_phb_video_get_id_by_index(store_index);
	}
	/*In caller group*/
	else if ((store_index < (MAX_PB_PHONE_ENTRIES + MAX_PB_CALLER_GROUPS)) && \
			  (store_index >= MAX_PB_PHONE_ENTRIES))
	{
		callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;
		video_id = callerGroups[(store_index - MAX_PB_PHONE_ENTRIES)].VideoID;
	}
	return mmi_phb_caller_video_get_path_by_index(video_id);
}
#endif

/**************************************************************

	FUNCTION NAME		: mmi_phb_call_enter_from_divert()

  	PURPOSE				: Displays the PHB list for call divert

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_call_enter_from_divert(void)
{
	phbListView = MMI_PHB_LIST_FOR_SETTINGS;
	//mmi_phb_list_pre_entry_second_level();
	g_phb_cntx.highlight_entry = 0;

	mmi_phb_entry_list(PhoneBookEntryCount, /*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OK, /*LSK*/
							  IMG_GLOBAL_OK, /*LSK*/
							  mmi_phb_get_index_second_level, /*Highlight Callback*/
							  mmi_phb_call_send_data_for_divert, /*LSK Callback*/
							  mmi_phb_call_send_data_for_divert, /*SEND Key Callback*/
							  mmi_phb_list_get_item, /*List Callback*/
							  mmi_phb_list_get_hint, /*Hint Callback*/
							  mmi_phb_call_enter_from_divert, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_call_send_data_for_divert()

  	PURPOSE				: Sends the names & numbers info of the selected
						  PHB entry to call divert

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_call_send_data_for_divert(void)
{
	U16 store_index;

	store_index = g_phb_name_index[g_phb_cntx.active_index_second];

	mmi_phb_convert_get_ucs2_number(pbNumber, store_index);	/*BCD number format.*/
	SendPhbEntryForDivert(PhoneBook[store_index].alpha_id.name, (U8*)pbNumber);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_call_enter_from_active_call()

  	PURPOSE				: Displays PHB main menu from active call options

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_call_enter_from_active_call(void)
{
	if(g_phb_cntx.processing)
		mmi_phb_entry_not_ready();
	else
	{
		mmi_phb_clear_old_history();
		mmi_phb_entry_main_menu();
	}
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_call_enter_from_deflect_option()

  	PURPOSE				: Displays PHB list from incoming call deflect option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_call_enter_from_deflect_option(void)
{
	if(g_phb_cntx.processing)
		mmi_phb_entry_not_ready();
	else
	{
		phbListView = MMI_PHB_LIST_FOR_CM;
		//mmi_phb_list_pre_entry_second_level();
		g_phb_cntx.highlight_entry = 0;

		mmi_phb_entry_list(PhoneBookEntryCount, /*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OK, /*LSK*/
							  IMG_GLOBAL_OK, /*LSK*/
							  mmi_phb_get_index_second_level, /*Highlight Callback*/
							  mmi_phb_call_send_data_for_deflect, /*LSK Callback*/
							  mmi_phb_call_send_data_for_deflect, /*SEND Key Callback*/
							  mmi_phb_list_get_item, /*List Callback*/
							  mmi_phb_list_get_hint, /*Hint Callback*/
							  mmi_phb_call_enter_from_deflect_option, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/

	}
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_call_send_data_for_deflect()

  	PURPOSE				: Sends the numbers info of the selected
						  entry for Call deflect

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_call_send_data_for_deflect(void)
{
	mmi_phb_convert_get_ucs2_number(pbNumber, g_phb_name_index[g_phb_cntx.active_index_second]);	/*BCD number format.*/
	CBackCallDeflectNumberFromPB(pbNumber);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_enter_first_from_active_call()
* DESCRIPTION
*   Enter phonebook list from active call to first item.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_enter_first_from_active_call(void)
{
	mmi_phb_list_enter_from_active_call(0);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_enter_last_from_active_call()
* DESCRIPTION
*   Enter phonebook list from active call to last item.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_enter_last_from_active_call(void)
{
	mmi_phb_list_enter_from_active_call((U16)(PhoneBookEntryCount - 1));
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_enter_from_active_call()
* DESCRIPTION
*   enter phonebook list from active call, highlight on specific entry.
*   (Note this function call will clear all phonebook history if exists)
* PARAMETERS
*  highlight_pos	IN	highlight entry position.
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_enter_from_active_call(U16 highlight_pos)
{
	if(g_phb_cntx.phb_ready&& !g_phb_cntx.processing)
	{
		if(PhoneBookEntryCount)
		{
			phbListView = MMI_PHB_LIST_FOR_PHB;
			g_phb_cntx.highlight_entry= highlight_pos;

			mmi_phb_clear_old_history();	/*Clear old phb history*/
			mmi_phb_list_pre_entry();
		}
		else
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
	else
		mmi_phb_entry_not_ready();
}

#define MMI_PHB_INTERFACE_FOR_SMS
/**************************************************************

	FUNCTION NAME		: mmi_phb_sms_enter_send_entry()

  	PURPOSE				: Displays the PHB list for SMS to send SMS
						  to one or many numbers

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
#if defined(__PROJECT_GALLITE_C01__)
#define MMI_ENTRY_QUICK_SEARCH_FOR_PHB_SEARH
#endif
extern void mmi_phb_entry_quick_search_list_for_app(U16 STR_TITLE, U16 IMG_TITLE,U16 STR_LSK, U16 IMG_LSK, void (*HighlightPtr)(S32), FunctionPtr LSKPtr, FunctionPtr SendKeyPtr, FunctionPtr EntryPtr);
extern void mmi_phb_qsearch_get_index_by_store_location_second(S32 nIndex);

void mmi_phb_sms_enter_send_entry(void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_sms_enter_send_entry.>\n",__FILE__,__LINE__));
	#ifndef MMI_ENTRY_QUICK_SEARCH_FOR_PHB_SEARH
	phbListView = MMI_PHB_LIST_FOR_SMS;
	//mmi_phb_list_pre_entry_second_level();

	g_phb_cntx.highlight_entry = 0;

	mmi_phb_entry_list(PhoneBookEntryCount, /*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OK, /*LSK*/
							  IMG_GLOBAL_OK, /*LSK*/
							  mmi_phb_get_index_second_level, /*Highlight Callback*/
							  mmi_phb_sms_send_data_choose_entry, /*LSK Callback*/
							  mmi_phb_sms_send_data_choose_entry, /*SEND Key Callback*/
							  mmi_phb_list_get_item, /*List Callback*/
							  mmi_phb_list_get_hint, /*Hint Callback*/
							  mmi_phb_sms_enter_send_entry, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/
	#else
	if(!g_phb_cntx.phb_ready||g_phb_cntx.processing)
	{
		mmi_phb_entry_not_ready();
		return;
	}

	if(!PhoneBookEntryCount)
	{
		DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		return;
	}
		
	phbListView = MMI_PHB_LIST_FOR_SMS;	
	mmi_phb_entry_quick_search_list_for_app(STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
											IMG_SCR_PBOOK_CAPTION, /*Title Image*/
											STR_GLOBAL_OK, /*LSK*/
											IMG_GLOBAL_OK, /*LSK*/
											mmi_phb_qsearch_get_index_by_store_location_second, /*Highlight Callback*/
											mmi_phb_sms_send_data_choose_entry, /*LSK Callback*/
											mmi_phb_sms_send_data_choose_entry,/*SEND Key Callback*/
											mmi_phb_sms_enter_send_entry/*Re-Entry Callback*/
											);
	#endif

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_sms_choose_number()
* DESCRIPTION
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_sms_choose_number(void)
{
	mmi_phb_entry_list_choose_number(g_phb_cntx.active_index_second,
						mmi_phb_entry_sms_choose_number,
						mmi_phb_sms_send_data_for_entry_list,
						mmi_phb_sms_send_data_for_entry,
						STR_SMS_NUM_CANNOT_BE_EMPTY,
						FALSE);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_frm_enter_send_entry()

  	PURPOSE				: Displays the PHB list for SMS to send SMS
						  to one or many numbers

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_frm_enter_send_entry(void)
{
#ifndef MMI_ENTRY_QUICK_SEARCH_FOR_PHB_SEARH
	U16 i;
	U16 EntryCount = 0;
	U16 store_index;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_frm_enter_send_entry.>\n",__FILE__,__LINE__));

	if (g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		/*Allocate buffer for phonebook memeber list filter*/
		mmi_phb_list_allocate_filter();

		/*Build list according to filter*/
		for (i=0 ; i < PhoneBookEntryCount ; i++)
		{
			store_index = g_phb_name_index[i];
//			if (PhoneBook[store_index].field & 0x1E) //if the number is empty, list it too. guojian modified for 10106 at 20081019
			{
				g_phb_cntx.list_filter[EntryCount] = store_index;
				EntryCount++;
			}
		}

		if (EntryCount)
		{
			g_phb_cntx.highlight_entry = 0;
			phbListView = MMI_PHB_LIST_FOR_FRM;

			mmi_phb_entry_list(EntryCount, 				/*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OK, /*LSK*/
							  IMG_GLOBAL_OK, /*LSK*/
							  mmi_phb_get_index_by_store_location_second, /*Highlight Callback*/
							  mmi_phb_entry_sms_choose_number, /*LSK Callback*/
							  mmi_phb_entry_sms_choose_number, /*SEND Key Callback*/
							  mmi_phb_filter_list_get_item, /*List Callback*/
							  mmi_phb_filter_list_get_hint, /*Hint Callback*/
							  mmi_phb_frm_enter_send_entry, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/
			SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
			//SetKeyHandler(mmi_frm_sms_end_key_from_send, KEY_END, KEY_EVENT_DOWN);  // zrx del 20060612
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_ID_PHB_NO_ENTRY_TO_SELECT), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
			mmi_phb_list_free_filter();
		}
	}
	else
	{
		mmi_phb_entry_not_ready();
	}
#else
	if(!g_phb_cntx.phb_ready||g_phb_cntx.processing)
	{
		mmi_phb_entry_not_ready();
		return;
	}

	if(!PhoneBookEntryCount)
	{
		DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		return;
	}
		
	phbListView = MMI_PHB_LIST_FOR_FRM;	
	mmi_phb_entry_quick_search_list_for_app(STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
											IMG_SCR_PBOOK_CAPTION, /*Title Image*/
											STR_GLOBAL_OK, /*LSK*/
											IMG_GLOBAL_OK, /*LSK*/
											mmi_phb_qsearch_get_index_by_store_location_second, /*Highlight Callback*/
											mmi_phb_entry_sms_choose_number, /*LSK Callback*/
											mmi_phb_entry_sms_choose_number,/*SEND Key Callback*/
											mmi_phb_frm_enter_send_entry/*Re-Entry Callback*/
											);
			

#endif
}


#if defined(__MMI_MESSAGES_SEND_BY_GROUP__)
/**************************************************************

	FUNCTION NAME		: mmi_phb_sms_enter_send_by_group()

  	PURPOSE				: Displays the Caller Groups list for SMS to send message
						  to all numbers belonging to this group

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_sms_enter_send_by_group(void)
{
	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		if(PhoneBookEntryCount)
		{
			phbListView = MMI_PHB_LIST_FOR_SMS;
			mmi_phb_entry_callergroup();	/*Entry caller group main screen.*/
		}
		else
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
	else
		mmi_phb_entry_not_ready();
}
#endif


/*****************************************************************************
* FUNCTION
*  mmi_phb_sms_send_data_choose_entry()
* DESCRIPTION
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sms_send_data_choose_entry(void)
{
	mmi_phb_entry_list_choose_number(g_phb_name_index[g_phb_cntx.active_index_second],
													mmi_phb_sms_send_data_choose_entry,
													mmi_phb_sms_send_data_for_entry_list,
													mmi_phb_sms_send_data_for_entry,
													STR_SMS_NUM_CANNOT_BE_EMPTY,
													FALSE);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sms_send_data_name_only()
* DESCRIPTION
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sms_send_data_for_entry_name_only(void)
{
	mmi_phb_sms_send_data_for_entry (NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sms_send_data_for_entry_list()
* DESCRIPTION
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sms_send_data_for_entry_list(void)
{
	//DeleteNScrId(SCR_PBOOK_LIST);
	mmi_phb_sms_send_data_for_entry((PS8)g_phb_cntx.number_to_dial[g_phb_cntx.active_index_third]);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sms_send_data_for_entry()
* DESCRIPTION
*   Sends the name & number info of the selected PHB entry to SMS
* PARAMETERS
*  number	IN	input number string
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_frm_sms_get_phb_number(U8 name[], U8 number[]);
void mmi_phb_sms_send_data_for_entry (S8* number)
{
#if 1 // zrx del 20060612
	U8 type=0;
	U16 store_index;
	store_index = g_phb_name_index[g_phb_cntx.active_index_second];

//chenhe use for jasper
#if defined(__MMI_MESSAGES_CHAT__)
	if((phbListView == MMI_PHB_LIST_FOR_CHAT) || (phbListView == MMI_PHB_LIST_FOR_CHAT_NO_CHECK))
	{
		SetNameNumberForChatScreen(PhoneBook[store_index].alpha_id.name, (U8*)number);
		return;
	}
#endif
#if defined(__MMI_MESSAGES_INSERT_FROM_PHB__)
	if(phbListView == MMI_PHB_LIST_FOR_SMS_INSERT ||phbListView == MMI_PHB_LIST_FOR_SMS_INSERT_NO_CHECK)
	{
		SetNameNumberForSMSInsert(PhoneBook[store_index].alpha_id.name, (U8*) number);
		return;
	}
#endif

	if(number[0]=='+')
	{
		type=1;
	}
	if(pfnUnicodeStrlen ((PS8)number) <= ((MAX_PB_NUMBER_LENGTH/ENCODING_LENGTH)+type))
	{
		if(pfnUnicodeStrlen ((PS8)number) == 0)
			DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		else if(phbListView == MMI_PHB_LIST_FOR_FRM)
			mmi_frm_sms_get_phb_number (PhoneBook[store_index].alpha_id.name, (U8*)number);
		else
			mmi_msg_get_number_from_phb (PhoneBook[store_index].alpha_id.name, (U8*)number);
	}
	else
	{
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
	}
	#endif
}

#ifdef __MMI_PHB_SEND_MMS_FROM_PHB__
extern void jMMS_SendFromApp(S8* number);
/*****************************************************************************
* FUNCTION
*  mmi_phb_mms_send_data_for_entry()
* DESCRIPTION
*   Sends the name & number info of the selected PHB entry to SMS
* PARAMETERS
*  number	IN	input number string
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_mms_send_data_for_entry (S8* number)
{
#ifdef JATAAYU_SUPPORT
	if (pfnUnicodeStrlen ((PS8)number) <= MAX_PB_NUMBER_LENGTH)
	{
		if(pfnUnicodeStrlen ((PS8)number) == 0)
			DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		else
		{
		#ifdef JATAAYU_SUPPORT_MMS
		    jMMS_SendFromApp(number);
		#endif
		}
	}
	else
#endif /* JATAAYU_SUPPORT */
	{
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_mms_send_data_for_entry_list()
* DESCRIPTION
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_mms_send_data_for_entry_list(void)
{
	mmi_phb_mms_send_data_for_entry((PS8)g_phb_cntx.number_to_dial[g_phb_cntx.active_index_third]);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_mms_send_data_choose_entry()
* DESCRIPTION
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_mms_send_data_choose_entry(void)
{
    phbListView = MMI_PHB_LIST_FOR_MMS_ALL;

    mmi_phb_entry_choose_field(
        g_phb_name_index[g_phb_cntx.active_index_second],
        MMI_PHB_ENTRY_FIELD_MMS,
        mmi_phb_mms_send_data_choose_entry,
        mmi_phb_mms_send_data_for_entry_list,
        GoBackHistory,
        NULL,
        MMI_FALSE);
}
#endif /* __MMI_PHB_SEND_MMS_FROM_PHB__ */

#if defined(__MMI_MESSAGES_SEND_BY_GROUP__)
/**************************************************************

	FUNCTION NAME		: mmi_phb_sms_send_data_for_group()

  	PURPOSE				: Sends the names & numbers info of the selected
						  caller group to SMS

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_sms_send_data_for_group(void)
{
	U16* indices;
	U16 i, j = 0;
	S16 pError;
	U16 store_index;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	/*Clear key immediately to avoid user press twice.*/
	ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);

#if (MAX_PB_ENTRIES >= 1000)
	indices = (U16*)g_phb_list_filter;
#else
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
	indices = OslMalloc(MAX_PB_ENTRIES*sizeof(U16));
#else
	indices = OslMalloc(MAX_PB_PHONE_ENTRIES*sizeof(U16));
#endif
#endif /* MAX_PB_ENTRIES >= 1000 */

	for (i = 0; i < PhoneBookEntryCount ; ++i)
	{
		store_index = g_phb_name_index[i];

#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
			if(store_index < MAX_PB_PHONE_ENTRIES)
#endif
			{
				/*Get optional IDs record.*/
				optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
				optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

				if(optid_record != current_record)
				{
					ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
					current_record = optid_record;
				}
				/*Compare caller group ID*/
				if(PhbOptIDs[optid_index].callerGroupID == (g_phb_cntx.active_index_second+1))
				indices[j++] = store_index;
				}
			}

	MMI_TRACE((MMI_TRACE_G4_PHB, MMI_PHB_FUNC_SendDataForCallerGrp, j ));

	mmi_msg_send_msg_to_group(j, indices);  // zrx del 20060612                //open it by yaosq  for SMS groups send 2006.11.30
#if (MAX_PB_ENTRIES < 1000)
		OslMfree(indices);
#endif /* MAX_PB_ENTRIES < 1000 */
	}
#endif

#define MMI_PHB_UPDATE_INLINE_LIST
/**************************************************************

	FUNCTION NAME		: mmi_phb_update_delete_image()

  	PURPOSE				: Deletes the image id from PHB records if
						  one of the downloaded/factory/system images
						  are deleted.

	INPUT PARAMETERS	: U16 imageId

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_update_delete_image(U16 imageId)
{
	U16 i;
	U8 flag = 0;
	S16 pError;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	for(i = 0; i < MAX_PB_PHONE_ENTRIES; ++i)
	{
		/*Get optional IDs record.*/
		optid_record = (i / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = i - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

		if(optid_record != current_record)
		{
			if(current_record != 0 && flag == 1)	/*Write result back for previous optional ID record.*/
			{
				WriteRecord(NVRAM_EF_PHB_IDS_LID, current_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
				flag = 0;
			}

			/*Read out next record.*/
			ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
			current_record = optid_record;
		}

		if(PhbOptIDs[optid_index].pictureTagID == imageId)
		{
			PhbOptIDs[optid_index].pictureTagID = IMG_PHB_DEFAULT;
			flag = 1;
		}
	}

	/*Write back for last record.*/
	if(flag)
		WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_update_delete_image_caller_group()

  	PURPOSE				: Deletes the image id from caller group if
						  one of the downloaded/factory/system images
						  are deleted.

	INPUT PARAMETERS	: U16 imageId

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_update_delete_image_caller_group(U16 imageId)
{
	U8 i;
	U8 flag = 0;
	S16 pError;
	PHB_CALLER_GROUP_STRUCT* callerGroups;

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

	for(i = 0; i < MAX_PB_CALLER_GROUPS; ++i)
	{
		if(callerGroups[i].pictureTagID== imageId)
		{
			callerGroups[i].pictureTagID= IMG_PHB_DEFAULT;
			flag = 1;
		}
	}

	if(flag)
		WriteRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);

}


/**************************************************************

	FUNCTION NAME		: mmi_phb_update_delete_audio()

  	PURPOSE				: Deletes the audio id from PHB records if
						  one of the downloaded/factory/system/composed
						  images are deleted.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_update_delete_audio(U16 audioId)
{
	U16 i;
	U8 flag = 0;
	S16 pError;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	for(i = 0; i < MAX_PB_PHONE_ENTRIES; ++i)
	{
		/*Get optional IDs record.*/
		optid_record = (i / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = i - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

		if(optid_record != current_record)
		{
			if(current_record != 0 && flag == 1)	/*Write result back for previous optional ID record.*/
			{
				WriteRecord(NVRAM_EF_PHB_IDS_LID, current_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
				flag = 0;
			}

			/*Read out next record.*/
			ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
			current_record = optid_record;
		}

		if(PhbOptIDs[optid_index].ringToneID == audioId)
		{
			PhbOptIDs[optid_index].ringToneID = 0;
			flag = 1;
		}
	}

	/*Write back for last record.*/
	if(flag)
		WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_update_delete_audio_caller_group()

  	PURPOSE				: Deletes the audio id from CallerGroup records if
						  one of the downloaded/factory/system/composed
						  images are deleted.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_update_delete_audio_caller_group(U16 audioId)
{
	U8 i,flag = 0;
	S16 pError;
	PHB_CALLER_GROUP_STRUCT* callerGroups;

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

	for(i = 0; i < MAX_PB_CALLER_GROUPS; ++i)
	{
		if(callerGroups[i].ringToneID == audioId)
		{
			callerGroups[i].ringToneID = 0;
			flag = 1;
		}
	}

	if(flag)
		WriteRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);
}

#define MMI_PHB_INTERFACE_FOR_SPEED_DIAL
/**************************************************************

	FUNCTION NAME		: mmi_phb_speed_dial_enter_phb()

  	PURPOSE				: Displays the PHB list for Speed key mapping

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void mmi_phb_speed_dial_enter_phb(void)
{
	phbListView = MMI_PHB_LIST_FOR_SPEED_DIAL;
	mmi_phb_list_pre_entry_second_level();
}

/*------------------------------------------------------  Interface for MMS  ------------------------------------------------------*/
#define MMI_PHB_INTERFACE_FOR_MMS
#ifdef JATAAYU_SUPPORT  //fengzb 2009-3-3
/*****************************************************************************
* FUNCTION
*  EntryPhbFromMMSNumber()
* DESCRIPTION
*   Entry phonebook list from MMS, list entry with number field only
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryPhbFromMMSNumber(void)
{
	phbListView = MMI_PHB_LIST_FOR_MMS_NUMBER;
	mmi_phb_list_pre_entry_for_mms_number_and_email();
}
/*****************************************************************************
* FUNCTION
*  EntryPhbFromMMSEmail()
* DESCRIPTION
*   Entry phonebook list from MMS, list entry with email field only
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryPhbFromMMSEmail(void)
{
	phbListView = MMI_PHB_LIST_FOR_MMS_EMAIL;
	mmi_phb_list_pre_entry_for_mms_number_and_email();
}
/*****************************************************************************
* FUNCTION
*  EntryPhbFromMMSAll()
* DESCRIPTION
*   Entry phonebook list from MMS, list all entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryPhbFromMMSAll(void)
{
	phbListView = MMI_PHB_LIST_FOR_MMS_ALL;
	mmi_phb_list_pre_entry_second_level();
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_mms_choose_number()
* DESCRIPTION
*   Entry Choose entry screen if there are more then one entry with number field.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_mms_choose_number(void)
{
	U16 i;
	U8* guiBuffer;
	U16 numberTypeImageList[5];
	S16 pError;
	U16 store_index;

	if( (phbListView == MMI_PHB_LIST_FOR_MMS_EMAIL) || (phbListView == MMI_PHB_LIST_FOR_MMS_NUMBER))
		store_index = g_phb_cntx.active_index_second;
	else
		store_index = g_phb_name_index[g_phb_cntx.active_index_second];

	pfnUnicodeStrcpy(pbName, (PS8)PhoneBook[store_index].alpha_id.name);
	mmi_phb_convert_get_ucs2_number(pbNumber, store_index);	/*BCD number format.*/

	if((phbListView != MMI_PHB_LIST_FOR_MMS_EMAIL) && (store_index >= MAX_PB_PHONE_ENTRIES)) /*In SIM*/
	{
		if(!pfnUnicodeStrlen(pbNumber))
			DisplayPopup((PU8)GetString(STR_NO_NUMBER_TO_DIAL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		else
			mmi_phb_mms_send_data_to_mms(pbName, pbNumber);
	}
	else
	{
		i = 0;

		if((phbListView != MMI_PHB_LIST_FOR_MMS_EMAIL) && pfnUnicodeStrlen(pbNumber) )
		{
				g_phb_cntx.number_to_dial[i] = (U8*)pbNumber;
				numberTypeImageList[i++] = IMG_MOBILE_NUMBER;
		}
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
#if 0
		ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
		if(phbListView != MMI_PHB_LIST_FOR_MMS_EMAIL)
		{
			AnsiiToUnicodeString(pbHomeNumber, (PS8)PhoneBookOptionalFields.homeNumber);
			if(pfnUnicodeStrlen(pbHomeNumber))
			{
				g_phb_cntx.number_to_dial[i] = (PU8)pbHomeNumber;
				numberTypeImageList[i++] = IMG_HOME_NUMBER;
			}
			AnsiiToUnicodeString(pbOfficeNumber, (PS8)PhoneBookOptionalFields.officeNumber);
			if(pfnUnicodeStrlen(pbOfficeNumber))
			{
				g_phb_cntx.number_to_dial[i] = (PU8)pbOfficeNumber;
				numberTypeImageList[i++] = IMG_OFFICE_NUMBER;
			}
			AnsiiToUnicodeString(pbFaxNumber, (PS8)PhoneBookOptionalFields.faxNumber);
			if(pfnUnicodeStrlen(pbFaxNumber))
			{
				g_phb_cntx.number_to_dial[i] = (PU8)pbFaxNumber;
				numberTypeImageList[i++] = IMG_FAX_NUMBER;
			}
		}

		if(phbListView != MMI_PHB_LIST_FOR_MMS_NUMBER)
		{
			AnsiiToUnicodeString(pbEmailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
			if(pfnUnicodeStrlen(pbEmailAddress))
			{
				g_phb_cntx.number_to_dial[i] = (PU8)pbEmailAddress;
				numberTypeImageList[i++] = IMG_EMAIL_ADDRESS;
			}
		}
#endif

		if(!i)
			DisplayPopup((PU8)GetString(STR_NO_NUMBER_TO_DIAL), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		else if(i==1)
		{
			mmi_phb_mms_send_data_to_mms(pbName, (S8*)g_phb_cntx.number_to_dial[0]);
		}
		else
		{
			EntryNewScreen(SCR_ID_PHB_MMS_CHOOSE_ENTRY, mmi_phb_exit_mms_choose_number, mmi_phb_entry_mms_choose_number, NULL);
			guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_MMS_CHOOSE_ENTRY);
			RegisterHighlightHandler(mmi_phb_get_index_third_level);
			ShowCategory53Screen(STR_CHOOSE_NUMBER_CAPTION, IMG_SCR_PBOOK_CAPTION,
				  STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				  i, g_phb_cntx.number_to_dial, numberTypeImageList, NULL, 0, 0, guiBuffer);

			SetLeftSoftkeyFunction(mmi_phb_mms_pre_send_data_to_mms, KEY_EVENT_UP);
			SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_mms_choose_number
* DESCRIPTION
*   exit function
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_mms_choose_number(void)
{
	g_phb_cntx.end_scr_id = SCR_ID_PHB_MMS_CHOOSE_ENTRY;
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_mms_pre_send_data_to_mms()
* DESCRIPTION
*   Send choosed number to MMS
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_mms_pre_send_data_to_mms(void)
{
	mmi_phb_mms_send_data_to_mms(pbName, (S8*)g_phb_cntx.number_to_dial[g_phb_cntx.active_index_third]);
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_mms_send_data_to_mms()
* DESCRIPTION
*   send entry name and choosed number to MMS by sending primitives
*
* PARAMETERS
*  name	IN	name field to be sent
*  number	IN	number field to be sent.
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void jMms_AddressInputPhoneBookGetValue(S8* number) ;
void mmi_phb_mms_send_data_to_mms(S8* name, S8* number)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<SendDataForMMS>\n",__FILE__,__LINE__));
	ClearAllKeyHandler(); /*Clear Key Handler to avoid resend message.*/
#ifdef JATAAYU_SUPPORT
	jMms_AddressInputPhoneBookGetValue(number) ;
#endif
#ifdef WAP_SUPPORT
	widget_MMI_fullscreen_editor_update_from_phonebook((kal_uint8*)number);
#endif
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_mms_email_list_get_item()
* DESCRIPTION
*   dynamic list call back function
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_phb_mms_email_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	U16 i;

	i = g_phb_cntx.list_filter[item_index];

	if (pfnUnicodeStrlen((S8*)PhoneBook[i].alpha_id.name))
		pfnUnicodeStrcpy((S8*) str_buff, (S8*)PhoneBook[i].alpha_id.name);
	else
		mmi_phb_convert_get_ucs2_number((S8*)str_buff, i);
	
#ifdef __MMI_MULTI_SIM__
	if (i < MAX_PB_PHONE_ENTRIES)
	{
		*img_buff_p = get_image(IMG_STORAGE_HANDSET);
	}
	else
	{
		U8 nSimIndex = (i - MAX_PB_PHONE_ENTRIES) / MAX_PB_SIM_ENTRIES;
		*img_buff_p = get_image(IMG_STORAGE_SIM_1 + nSimIndex);
	}
#else
	if (i < MAX_PB_PHONE_ENTRIES)
		*img_buff_p = get_image(IMG_STORAGE_HANDSET);
	else
		*img_buff_p = get_image(IMG_STORAGE_SIM);
#endif
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_mms_email_list_get_hint()
* DESCRIPTION
*
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_phb_mms_email_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	U16 store_index;
	S8 temp_number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];

	store_index = g_phb_cntx.list_filter[item_index];

	mmi_phb_convert_get_ucs2_number((S8*)temp_number, store_index);

	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name) && pfnUnicodeStrlen((S8*)temp_number))
		pfnUnicodeStrcpy((S8*)hint_array[0], (S8*)temp_number);
	else
		return 0; /*No Hint Data*/

	return 1;  /*One hint data only, can be more hints.*/
}

#endif
/*--------------------------------------------------END Interface for MMS  ------------------------------------------------------*/

/*------------------------------------------------------  Interface for File Manager Image---------------------------------------------------*/
#define MMI_PHB_INTERFACE_FOR_FMGR
#if defined(__MMI_FILE_MANAGER__)

#if defined(__MMI_INCOMING_CALL_VIDEO__)
//guojian added for jassmine 20080617
void mmi_phb_fmgr_from_file_video_CallBack(S32 result)  
{
	S8* error_str;
	if(result>=0){
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	}
	else
	{
		ChangeLeftSoftkey(0,0);
		ChangeRightSoftkey(0,0);

		error_str = GetString(STR_GLOBAL_UNSUPPORTED_FORMAT);
		if(error_str)
		   DisplayPopup((PU8)error_str, \
                                                   IMG_GLOBAL_UNFINISHED, 1, \
                                                   UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		/* delete to file select screen again */
		DeleteNHistory(1);

 	}

}


/*****************************************************************************
* FUNCTION
*  PhnsetWpUsrDefPreView
* DESCRIPTION
*   This function is to preview user define wallpaper/screensaver
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void mmi_phb_fmgr_from_file_video_preview(void)
{
	U8		*gui_buffer;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	EntryNewScreen(SCR_VDO_DISP_PHB_PREVIEW, NULL, NULL, NULL);
	gui_buffer = GetCurrGuiBuffer(SCR_VDO_DISP_PHB_PREVIEW);


		ShowCategory225Screen(
										STR_GLOBAL_VIEW,	/* title_str */
										IMG_SCR_SETTING_CAPTION,/* title_icon */
										STR_GLOBAL_OK,				/* lsk_str */
										IMG_GLOBAL_OK,				/* lsk_icon */
										STR_GLOBAL_BACK,			/* rsk_str */
										IMG_GLOBAL_BACK,			/* rsk_icon */
										0,								/* video_id */
										(S8*)g_phb_video_file_path,			/* video_filename */
										1,								/* repeat_count */
										TRUE,							/* is_visual_update */
										TRUE,							/* is_play_audio */
										TRUE,							/* is_lcd_no_sleep */
										GDI_COLOR_WHITE,			/* bg_color */
										mmi_phb_fmgr_from_file_video_CallBack,
										gui_buffer);


	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetLeftSoftkeyFunction (mmi_phb_entry_fmgr_save_option, KEY_EVENT_UP);


}

#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_fmgr_from_file_mgr()
* DESCRIPTION
*   forward function for file manager application
*
* PARAMETERS
*  path IN
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_fmgr_from_file_mgr(S8* path)
{
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	PS8			ext_ptr;
	S8			ext_name[20];
	S32			i;
#endif
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	if(path == NULL)
	{
		DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_NO_SELECT), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return;
	}

#if defined(__MMI_INCOMING_CALL_VIDEO__)
	/*Check video content*/
	ext_ptr = mmi_fmgr_extract_ext_file_name(path);

	memset(ext_name, 0, sizeof(ext_name));
	UnicodeNToAnsii(ext_name, ext_ptr, 5 * ENCODING_LENGTH);

	/* make upper */
	for (i = 0 ; i < 12 ; i++)
	{
		if (ext_name[i] != 0)
		{
			if (ext_name[i] >= 'a')
				ext_name[i] -= 'a' - 'A';
		}
	}

	if (strcmp(ext_name, "3GP") == 0 ||strcmp(ext_name, "MP4") == 0 ||
		strcmp(ext_name, "AVI") == 0)
	{
		if (mmi_phb_video_check_data_space(path))
		{
			pfnUnicodeStrcpy(g_phb_video_file_path, path);

			/*Clear Previous PHB screen, and set phb new entry point here.*/
			mmi_phb_clear_old_history();
			g_phb_cntx.start_scr_id = SCR_ID_PHB_FMGR_SAVE_OPTION;

			g_phb_cntx.recv_res_from = MMI_PHB_RECV_RES_VIDEO;
	//		mmi_phb_entry_fmgr_save_option();
			mmi_phb_fmgr_from_file_video_preview();
		}
	}
	/*Check image content*/
	else if(mmi_phb_image_check_valid_image(path))
#else

	/*Check image content*/
	if(mmi_phb_image_check_valid_image(path))
#endif
	{
		pfnUnicodeStrcpy(g_phb_file_path, path);
		/*Clear Previous PHB screen, and set phb new entry point here.*/
		mmi_phb_clear_old_history();
		g_phb_cntx.start_scr_id = SCR_ID_PHB_FMGR_SAVE_OPTION;
		g_phb_cntx.recv_res_from = MMI_PHB_RECV_RES_IMAGE;
		mmi_phb_entry_fmgr_save_option();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_fmgr_add_entry()
* DESCRIPTION
* Highlight function to associate image from file manager to a new phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_fmgr_add_entry(void)
{
	SetLeftSoftkeyFunction(mmi_phb_fmgr_pre_add_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_fmgr_pre_add_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_fmgr_edit_entry()
* DESCRIPTION
*  Highlight function for associate image to a existing phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_fmgr_edit_entry(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	SetLeftSoftkeyFunction(mmi_phb_list_pre_entry_for_fmgr_in_nvram,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_list_pre_entry_for_fmgr_in_nvram, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	phbListView = MMI_PHB_LIST_FOR_SAVE_FROM_FILE_MGR;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_edit_callergroup()
* DESCRIPTION
* Highlight function for associate image to a existing phonebook caller group
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_edit_callergroup(void)
{
	SetLeftSoftkeyFunction(mmi_phb_entry_callergroup,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_callergroup, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	phbListView = MMI_PHB_LIST_FOR_SAVE_FROM_FILE_MGR;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_fmgr_save_option()
* DESCRIPTION
*  Entry function for save image from file manager option menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_fmgr_save_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[3];
	U16 nNumofItem;

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	EntryNewScreen(SCR_ID_PHB_FMGR_SAVE_OPTION, mmi_phb_exit_fmgr_save_option, mmi_phb_entry_fmgr_save_option, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_FMGR_SAVE_OPTION);
	nNumofItem = GetNumOfChild(MENU_ID_PHB_FMGR_SAVE_OPTION);
	GetSequenceStringIds(MENU_ID_PHB_FMGR_SAVE_OPTION, nStrItemList);
	SetParentHandler(MENU_ID_PHB_FMGR_SAVE_OPTION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ShowCategory15Screen(STR_SCR_VIEW_OPTIONS_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_fmgr_save_option()
* DESCRIPTION
*  Exit function
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_fmgr_save_option(void)
{
	g_phb_cntx.end_scr_id = SCR_ID_PHB_FMGR_SAVE_OPTION;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_fmgr_pre_add_entry()
* DESCRIPTION
*  pre check for associate image from file manager to a new phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_fmgr_pre_add_entry(void)
{
	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		if(g_phb_cntx.phone_used >= g_phb_cntx.phone_total)	/*No spcace in NVRAM*/
		{
			DisplayPopup((PU8)GetString(STR_PBOOK_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else
		{
			mmi_phb_util_clear_buffer(TRUE);

			if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_IMAGE)
			{
				mmi_phb_build_image_list();
				g_phb_cntx.selected_pic_index = g_phb_cntx.total_image_id - 1;
				g_phb_cntx.image_location = MMI_PHB_IMAGE_SELECT_PATH;
			}
			else if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_AUDIO)
			{
				mmi_phb_build_ring_tone_list();
				g_phb_cntx.selected_ring_index = mmi_phb_get_ring_tone_index(g_phb_cntx.recv_res_id);
			}
#if defined(__MMI_INCOMING_CALL_VIDEO__)
			else if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_VIDEO)
			{
				mmi_phb_build_video_list();
				g_phb_cntx.selected_video_index = g_phb_cntx.total_video_id - 1;
				g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;
			}
#endif

			g_phb_cntx.set_done_flag = 2;
			g_phb_cntx.selected_storage = MMI_NVRAM;
			mmi_phb_entry_op_add_entry();
		}
	}
	else
	{
		mmi_phb_entry_not_ready();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_fmgr_pre_edit_entry()
* DESCRIPTION
*  pre check for associate image to a existing phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_fmgr_pre_edit_entry(void)
{
	S16 pError;
	U16 i,store_index;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids;

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	store_index = g_phb_cntx.active_index;

 	if (store_index >= MAX_PB_PHONE_ENTRIES) /*ERROR!, Record in NVRAM only*/
 	{
 		//Print Error Log Here.
		GoBackHistory( );
		return;
 	}

	/*set active index for edit entry*/
	for(i = 0 ; i < PhoneBookEntryCount ; i++)
	{
		if(g_phb_name_index[i] == store_index)
		{
			g_phb_cntx.active_index = i;
			break;
		}
	}

	/*Clear All Buffer First.*/
	mmi_phb_util_clear_buffer(TRUE);

	/*Assign value for each field for update*/
	pfnUnicodeStrcpy(pbName, (S8*)PhoneBook[store_index].alpha_id.name);
	mmi_phb_convert_get_ucs2_number(pbNumber, store_index);

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
#if 0
	ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
	ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
 	AnsiiToUnicodeString(pbHomeNumber, (PS8)PhoneBookOptionalFields.homeNumber);
	pfnUnicodeStrcpy(pbCompanyName, (PS8)PhoneBookOptionalFields.companyName);
 	AnsiiToUnicodeString(pbEmailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
	AnsiiToUnicodeString(pbOfficeNumber, (PS8)PhoneBookOptionalFields.officeNumber);
 	AnsiiToUnicodeString(pbFaxNumber, (PS8)PhoneBookOptionalFields.faxNumber);
 #endif

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
		mmi_phb_bday_read_data_to_buff(store_index);
#endif

	mmi_phb_build_image_list();
	mmi_phb_build_ring_tone_list();

 	mmi_phb_read_optional_ids(store_index);
 	opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;

	/*Image setting*/
	if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_IMAGE)
	{
		g_phb_cntx.selected_pic_index = g_phb_cntx.total_image_id - 1;
		g_phb_cntx.image_location = MMI_PHB_IMAGE_SELECT_PATH;
	}
	else	/*Keep original image*/
	{
	 	g_phb_cntx.selected_pic_index = mmi_phb_get_image_index(opt_ids->pictureTagID);
		if(opt_ids->pictureTagID == 1)
		{
			g_phb_cntx.image_location = MMI_PHB_IMAGE_FOR_ENTRY;
		}
		else
		{
			g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
		}
	}

	/*Audio Setting*/
	if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_AUDIO)
	{
		g_phb_cntx.selected_ring_index = mmi_phb_get_ring_tone_index(g_phb_cntx.recv_res_id);
	}
	else	/*Keep original audio*/
	{
		g_phb_cntx.selected_ring_index = mmi_phb_get_ring_tone_index(opt_ids->ringToneID);
	}

	/*Video Setting*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	mmi_phb_build_video_list();
	if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_VIDEO)
	{
		g_phb_cntx.selected_video_index = g_phb_cntx.total_video_id - 1;
		g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;
		g_phb_cntx.video_audio = 1;	/*Turn on video's audio by default*/
	}
	else	/*Keep original video*/
	{
		U16 video_id = mmi_phb_video_get_id_by_index(store_index);
		g_phb_cntx.selected_video_index = mmi_phb_get_video_index(video_id);

		g_phb_cntx.video_audio = ((video_id == 0) || (video_id & 0x4000)) ? 1:0;

		if((video_id & 0x8000) && mmi_phb_video_get_path_by_index(video_id)) /*Check if the video is a file*/
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;
		}
		else
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_NO_SELECT;
		}
	}
#endif

	/*Caller Group Setting, keep original caller group*/
	g_phb_cntx.selected_grp_index = opt_ids->callerGroupID;

	mmi_phb_entry_op_edit_save_confirm();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_fmgr_pre_edit_callergrp()
* DESCRIPTION
* pre check for associate image to a existing phonebook caller group
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_fmgr_pre_edit_callergrp(void)
{
	PHB_CALLER_GROUP_STRUCT* callerGroups;

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;
	pfnUnicodeStrcpy(pbName, (PS8)callerGroups[g_phb_cntx.active_index].groupName);

	mmi_phb_build_led_and_alert_list();
	mmi_phb_build_ring_tone_list();
	mmi_phb_build_image_list();

	/*Image Setting*/
	if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_IMAGE)
	{
		g_phb_cntx.selected_pic_index = g_phb_cntx.total_image_id - 1;
		g_phb_cntx.image_location = MMI_PHB_IMAGE_SELECT_PATH;
	}
	else	/*Keep original image*/
	{
		 g_phb_cntx.selected_pic_index = mmi_phb_get_image_index(callerGroups[g_phb_cntx.active_index].pictureTagID);
		if(callerGroups[g_phb_cntx.active_index].pictureTagID == 1)
		{
			g_phb_cntx.image_location = MMI_PHB_IMAGE_FOR_ENTRY;
		}
		else
		{
			g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
		}
	}

	/*Audio Setting*/
	if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_AUDIO)
	{
		g_phb_cntx.selected_ring_index = mmi_phb_get_ring_tone_index(g_phb_cntx.recv_res_id);
	}
	else	/*Keep original audio*/
	{
		g_phb_cntx.selected_ring_index =  mmi_phb_get_ring_tone_index(callerGroups[g_phb_cntx.active_index].ringToneID);
	}

	/*Video Setting*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	mmi_phb_build_video_list();
	if(g_phb_cntx.recv_res_from == MMI_PHB_RECV_RES_VIDEO)
	{
		g_phb_cntx.selected_video_index = g_phb_cntx.total_video_id - 1;
		g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;
		g_phb_cntx.video_audio = 1;	/*Turn on video's audio by default*/
	}
	else	/*Keep original video*/
	{
		U16 video_id = callerGroups[g_phb_cntx.active_index].VideoID;

		g_phb_cntx.selected_video_index = mmi_phb_get_video_index(video_id);

		g_phb_cntx.video_audio = ((video_id == 0) || (video_id & 0x4000)) ? 1:0;

		if((video_id & 0x8000) && mmi_phb_video_get_path_by_index((U16)(g_phb_cntx.active_index + MAX_PB_PHONE_ENTRIES)))
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;
		}
		else
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_NO_SELECT;
		}
	}
#endif

	/*Keep other settings*/
	g_phb_cntx.selected_alert_index = callerGroups[g_phb_cntx.active_index].alertType;
	g_phb_cntx.selected_pattern_index = callerGroups[g_phb_cntx.active_index].LEDPatternId;

	mmi_phb_callergroup_detail_pre_save();
}
/*------------------------------------------------------  END Interface for File Manager Image------------------------------------------------*/
#endif
#define MMI_PHB_INTERFACE_FOR_EMAIL
#if defined(__MMI_EMAIL__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_email_enter_list()
* DESCRIPTION
*   Phonebook list interface for email application.
*
* PARAMETERS
*  void
* RETURNS
*  full path of image
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_email_enter_list(void)
{
	phbListView = MMI_PHB_LIST_FOR_EMAIL_APP;
	mmi_phb_list_pre_entry_second_level();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_email_choose_entry()
* DESCRIPTION
*   choose entry result, send result back to email app.
*
* PARAMETERS
*	void
* RETURNS
*  full path of image
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_email_choose_entry(void)
{
	S16 pError;
	U16 store_index;

	store_index = g_phb_name_index[g_phb_cntx.active_index_second];

	/*Clear buffer*/
	memset(pbEmailAddress,0,2);

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	if(store_index < MAX_PB_PHONE_ENTRIES)	/*in NVRAM*/
	{
    #if 0
		ReadRecord(NVRAM_EF_PHB_FIELDS_LID, store_index + 1, (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    #endif
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    AnsiiToUnicodeString(pbEmailAddress, (S8*)PhoneBookOptionalFields.emailAddress);
	}
#endif

	/*Send back to email app.*/
	mmi_email_phb_get_addr_callback((S8*)PhoneBook[store_index].alpha_id.name,(S8*) pbEmailAddress);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_email_save_entry_to_phb()
* DESCRIPTION
*   save name and email to phonebook as a new entry
*
* PARAMETERS
*	name	IN	input name string pointer
*	email	IN	input email string pointer
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_email_save_entry_to_phb (S8* ucs2_name, S8* ucs2_email)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_email_save_entry_to_phb.>\n",__FILE__,__LINE__));

	if(g_phb_cntx.phb_ready&& !g_phb_cntx.processing)
	{
		if(PhoneBookEntryCount == g_phb_cntx.sim_total + g_phb_cntx.phone_total)
			DisplayPopup((PU8)GetString(STR_PBOOK_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		else
		{
			/*Ensure the length will not exceed phonebook buffer.*/
			memset((ucs2_name+MAX_PB_NAME_LENGTH*ENCODING_LENGTH),0,2);
			memset((ucs2_email+MAX_PB_EMAIL_LENGTH*ENCODING_LENGTH),0,2);

			/*Clear other field*/
			mmi_phb_util_clear_buffer(TRUE);

			/*Bring in data and enter save entry screen.*/
			g_phb_cntx.set_done_flag = 1;
			pfnUnicodeStrcpy(pbName, ucs2_name);
			pfnUnicodeStrcpy(pbEmailAddress, ucs2_email);

			mmi_phb_clear_old_history();
			mmi_phb_entry_op_add_choose_storage();
		}
	}
	else
		mmi_phb_entry_not_ready();
}
#endif


#define MMI_PHB_INTERFACE_FOR_VRSD_DIAL
#if defined(__MMI_VRSD_DIAL__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_vrsddial_enter_tag_list()
* DESCRIPTION
*   Phonebook list interface for voice dial application.
*
* PARAMETERS
*  void
* RETURNS
*  full path of image
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_vrsddial_enter_tag_list(void)
{
	U16 i,j;
	U16 EntryCount = 0;
	U16 store_index;
	vrsd_tag_struct tag_list[MMI_VRSD_MAX_TAG];
	S16 pError;

	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		/*Allocate buffer for phonebook memeber list filter*/
		mmi_phb_list_allocate_filter();

		/*Read voice tag list from NVRAM*/
		ReadRecord(NVRAM_EF_VRSD_TAG_LID, MMI_VRSD_GROUP_ALL, (void*)tag_list, MMI_VRSD_MAX_TAG*sizeof(vrsd_tag_struct), &pError);

		/*Find out entry with tag and put it into filter list*/
		for(i=0 ; i < PhoneBookEntryCount ; i++)
		{
			store_index = g_phb_name_index[i];

			for(j = 0 ; j < MMI_VRSD_MAX_TAG ; j++)
			{
				if((tag_list[j].appref_id < 0xffff) && (tag_list[j].app_id == MMI_VRSD_APP_DIAL) && (tag_list[j].appref_id == store_index))
				{
					g_phb_cntx.list_filter[EntryCount] = store_index;
					EntryCount++;
					break;
				}
			}
		}

		/*If voice tag exists, enter phonebook list*/
		if(EntryCount)
		{
			g_phb_cntx.highlight_entry = 0;

			mmi_phb_entry_list(EntryCount, 				/*Total Entry*/
							  STR_ID_VRSD_DIAL, /*Title String*/
							  IMG_ID_VRSD_APP, /*Title Image*/
							  STR_GLOBAL_OPTIONS, 				/*LSK*/
							  IMG_GLOBAL_OPTIONS, 				/*LSK*/
							  mmi_phb_vrsddial_get_tag_list_index, 		/*Highlight Callback*/
							  mmi_vrsddial_main_menu_all_option, 	/*LSK Callback*/
							  NULL, 									/*SEND Key Callback*/
							  mmi_phb_filter_list_get_item, 	/*List Callback*/
							  mmi_phb_filter_list_get_hint, 	/*Hint Callback*/
							  mmi_phb_vrsddial_enter_tag_list,	/*Re-Entry Callback*/
							  FALSE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/

		}
		else /*No voice Tag exists, Go to menu directly.*/
		{
			mmi_vrsddial_entry_empty_list();
			mmi_phb_list_free_filter();
		}
	}
	else
		mmi_phb_entry_not_ready();


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
void mmi_phb_vrsddial_get_tag_list_index(S32 nIndex)
{
	g_phb_cntx.active_index = g_phb_cntx.list_filter[nIndex];	/*This is store_index in filter list*/

	StopTimer(VRSD_DIAL_PLAYBACK_TIMER);
	StartTimer(VRSD_DIAL_PLAYBACK_TIMER, MMI_VRSD_PLAY_TAG_DELAY, mmi_phb_vrsddial_playback_by_store_index);
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
void mmi_phb_vrsddial_playback_by_store_index(void)
{
	U16 tag_id;

	tag_id = mmi_vrsd_util_get_tag_id(MMI_VRSD_APP_DIAL, g_phb_cntx.active_index);
	mmi_vrsd_playback_req(tag_id, NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_vrsddial_enter_tag_list()
* DESCRIPTION
*   Phonebook list interface for voice dial application.
*
* PARAMETERS
*  void
* RETURNS
*  full path of image
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_vrsddial_enter_no_tag_list(void)
{
	U16 i,j;
	U16 EntryCount = 0;
	U16 store_index;
	vrsd_tag_struct tag_list[MMI_VRSD_MAX_TAG];
	S16 pError;
	BOOL has_tag;

	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		/*Allocate buffer for phonebook memeber list filter*/
		mmi_phb_list_allocate_filter();

		/*Read voice tag list from NVRAM*/
		ReadRecord(NVRAM_EF_VRSD_TAG_LID, MMI_VRSD_GROUP_ALL, (void*)tag_list, MMI_VRSD_MAX_TAG*sizeof(vrsd_tag_struct), &pError);

		/*Find out entry without voice tag and put it into filter list*/
		for(i=0 ; i < PhoneBookEntryCount ; i++)
		{
			store_index = g_phb_name_index[i];

			has_tag = FALSE;
			for(j = 0 ; j < MMI_VRSD_MAX_TAG ; j++)
			{
				if((tag_list[j].app_id == MMI_VRSD_APP_DIAL) && (tag_list[j].appref_id == store_index))
			{
					has_tag = TRUE;
					break;
				}
			}

			/*Add to list if the entry has no tag.*/
			if(!has_tag)
			{
				g_phb_cntx.list_filter[EntryCount] = store_index;
				EntryCount++;
			}
		}

		/*If voice tag exists, enter phonebook list*/
		if(EntryCount)
		{
			g_phb_cntx.highlight_entry = 0;

			mmi_phb_entry_list(EntryCount, 				/*Total Entry*/
							  STR_ID_VRSD_DIAL, /*Title String*/
							  IMG_ID_VRSD_APP, /*Title Image*/
							  STR_GLOBAL_OK, 				/*LSK*/
							  IMG_GLOBAL_OK, 				/*LSK*/
							  mmi_phb_get_index_by_store_location_second, 		/*Highlight Callback*/
							  mmi_vrsddial_add_tag_from_list, 	/*LSK Callback*/
							  NULL, 									/*SEND Key Callback*/
							  mmi_phb_filter_list_get_item, 	/*List Callback*/
							  mmi_phb_filter_list_get_hint, 	/*Hint Callback*/
							  mmi_phb_vrsddial_enter_no_tag_list,	/*Re-Entry Callback*/
							  FALSE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/

		}
		else /*All Entries has voice tag.*/
		{
			DisplayPopup((PU8)GetString(STR_ID_PHB_NO_ENTRY_TO_SELECT), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
			mmi_phb_list_free_filter();

		}
	}
	else
		mmi_phb_entry_not_ready();


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
/*
void mmi_phb_vrdial_entry_option(void)
{
	U16 i;

	for(i = 0 ; i < PhoneBookEntryCount ; i++)
		if(g_phb_name_index[i] == g_vrdial_cntx.store_index)
		{
				g_phb_cntx.active_index = i;
				break;
		}

	mmi_phb_entry_op_option();
}
*/

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
void mmi_phb_highlight_op_voice_dial(void)
{
	SetLeftSoftkeyFunction(mmi_vrsddial_phb_main_menu_pre_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_vrsddial_phb_main_menu_pre_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_vrsddial_auto_dial
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
void mmi_phb_vrsddial_auto_dial(U16 store_index)
{
	U16 i;

	g_phb_cntx.active_index = 0;

	for(i = 0 ; i < PhoneBookEntryCount ; i++)
	{
		if(g_phb_name_index[i] == store_index)
		{
			g_phb_cntx.active_index = i;
			break;
		}
	}

	mmi_phb_choose_number_normal();
}

#endif	/*__MMI_VRSD_DIAL__*/


#define MMI_PHB_INTERFACE_FOR_VRSI
#if defined(__MMI_VRSI__) && defined(__MMI_VRSI_TRAIN_TAG__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_vrsi_pre_entry_list
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
void mmi_phb_vrsi_pre_entry_list(void)
{
	g_phb_cntx.highlight_entry = 0;

	mmi_phb_entry_list(PhoneBookEntryCount, /*Total Entry*/
							  STR_ID_VRSI_APP, /*Title String*/
							  IMG_ID_VRSI_APP, /*Title Image*/
							  STR_GLOBAL_OK, /*LSK*/
							  IMG_GLOBAL_OK, /*LSK*/
							  mmi_phb_get_index_second_level, /*Highlight Callback*/
							  mmi_phb_vrsi_entry_list_callback, /*LSK Callback*/
							  NULL, /*SEND Key Callback*/
							  mmi_phb_list_get_item, /*List Callback*/
							  mmi_phb_list_get_hint, /*Hint Callback*/
							  mmi_phb_vrsi_pre_entry_list, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_vrsi_entry_list_callback
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
void mmi_phb_vrsi_entry_list_callback(void)
{
	U16 store_index;

	store_index = g_phb_name_index[g_phb_cntx.active_index_second];
	mmi_vrsi_ndial_train_list_callback(store_index);
}

#endif

/*Convert function for phonebook numbers between BCD and ASCII format.*/
#define MMI_PHB_GENERAL_UTIL
/*****************************************************************************
* FUNCTION
*  mmi_phb_util_check_entry_exist()
* DESCRIPTION
*   check if an entry exists
*
* PARAMETERS
*  store_index	IN	the store location id of the entry
* RETURNS
*  TRUE if the location has the entry, else FALSE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_util_check_entry_exist(U16 store_index)
{
	U16 i;

	for(i = 0 ; i < PhoneBookEntryCount ; i++)
		if(g_phb_name_index[i] == store_index)
			return TRUE;

	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_to_bcd_get_byte()
* DESCRIPTION
*   convert ASCII character to BCD
*
* PARAMETERS
*  digit	IN	ascii character
* RETURNS
*  bcd character
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_convert_to_bcd_get_byte (U8 digit)
{
	U8 half_byte;

	if (digit <= '9' && digit >= '0')
	{
		half_byte = digit - '0';
	}
	else
	{
		switch (digit)
		{
			case '*':
			half_byte = 0x0a;
			break;

			case '#':
			half_byte = 0x0b;
			break;

			case 'p':
			case 'P':
			half_byte = 0x0c;
			break;

			case 'w':
			case 'W':
			half_byte = 0x0d;
			break;

			default:
			half_byte = 0x0f;
			break;
		}
	}
	return half_byte;
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_to_bcd()
* DESCRIPTION
 
*
* PARAMETERS
*  	source				IN 			An ASCII encoding string.
*	dest 				IN/OUT	An BCD encoding string.
*	max_dest_len 	IN			the byte size of dest array.
* RETURNS
*  byte number after convert.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_convert_to_bcd (U8 *dest, U8 *source,  U8 max_dest_len )
{
	U8 i=0;
	U8 lower_byte, upper_byte;

	while (source[i] != '\0')
	{

		lower_byte = mmi_phb_convert_to_bcd_get_byte (source[i]);
		upper_byte = mmi_phb_convert_to_bcd_get_byte (source[i+1]);

		if( (i>>1) >= max_dest_len)
			return (i>>1);

		*((U8*)dest + (i>>1)) = (upper_byte << 4) + lower_byte;

		if (source[i+1] == '\0')
		{
			if( (i>>1) >= max_dest_len)
				return (i>>1)+1;

			*((U8*)dest + (i>>1)) |= 0xf0;
			return (i>>1)+1;
		}

		i+=2;
	}

	if( (i>>1) >= max_dest_len)
		return (i>>1);

	*((U8 *)dest+(i>>1)) = 0xff;
		return (i>>1);

}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_convert_to_digit_get_byte_by_storage
 * DESCRIPTION
 *  convert BCD character to ASCII
 * PARAMETERS
 *  bcd     [IN]        Bcd character
 * RETURNS
 *  ascii character
 *****************************************************************************/
U8 mmi_phb_convert_to_digit_get_byte_by_storage(U8 bcd, U8 storage)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 digit;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/    
    if (!(storage == MMI_NVRAM || storage == MMI_SIM
    #ifdef __MMI_DUAL_SIM_MASTER__
        || storage == MMI_SIM2
    #endif
        ))
    {
        ASSERT(0);
    }

    if (bcd <= 9)
    {
        digit = bcd + '0';
    }
    else
    {
        switch (bcd)
        {
            case 0x0a:
                digit = '*';
                break;

            case 0x0b:
                digit = '#';
                break;

            case 0x0c:
                digit = 'p';
                break;

            case 0x0d:
                if (storage == MMI_NVRAM)
                {
                    digit = 'w';
                }
                else
                {
                    digit = '?';
                }
                break;

            case 0x0e:
                digit = '+';
                break;

            default:
                digit = '\0';
                break;
        }
    }
    return digit;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_convert_to_digit
 * DESCRIPTION
 *  convert BCD string to ASCII
 * PARAMETERS
 *  dest                [IN/OUT]        An ASCII encoding string.
 *  source              [IN]            An BCD encoding string.
 *  max_dest_len        [IN]            The byte size of dest array.(include null terminate '\0')
 * RETURNS
 *  byte number after convert.
 *****************************************************************************/
U8 mmi_phb_convert_to_digit_by_storage(U8 *dest, U8 *source, U8 max_dest_len, U8 storage)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 ch1, ch2;
    U8 i = 0, j = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(storage == MMI_NVRAM || storage == MMI_SIM
    #ifdef __MMI_DUAL_SIM_MASTER__
        || storage == MMI_SIM2
    #endif
        ))
    {
        ASSERT(0);
    }
    
    while ((source[i] != 0xff) && (j < max_dest_len - 1))
    {
        ch1 = source[i] & 0x0f;
        ch2 = (source[i] & 0xf0) >> 4;

        *((U8*) dest + j) = mmi_phb_convert_to_digit_get_byte_by_storage(ch1, storage);

        if (ch2 == 0x0f)
        {
            *((U8*) dest + j + 1) = '\0';
            return j + 1;
        }
        else
        {
            *((U8*) dest + j + 1) = mmi_phb_convert_to_digit_get_byte_by_storage(ch2, storage);
        }
        i++;
        j += 2;
    }

    *((U8*) dest + j) = '\0';
    return j;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_convert_to_digit
 * DESCRIPTION
 *  convert BCD string to ASCII
 * PARAMETERS
 *  dest                [IN/OUT]        An ASCII encoding string.
 *  source              [IN]            An BCD encoding string.
 *  max_dest_len        [IN]            The byte size of dest array.(include null terminate '\0')
 * RETURNS
 *  byte number after convert.
 *****************************************************************************/
U8 mmi_phb_convert_to_digit_by_store_index(U8 *dest, U8 *source, U8 max_dest_len, U16 store_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 storage;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (store_index >= MAX_PB_ENTRIES)
        ASSERT(0);

    if (store_index < MAX_PB_PHONE_ENTRIES)
    {
        storage = MMI_NVRAM;
    }
#ifdef __MMI_DUAL_SIM_MASTER__
    else if (store_index >= (MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES))
    {
        storage =  MMI_SIM2;
    }
#endif
    else
    {
        storage =  MMI_SIM;
    }
    
    return mmi_phb_convert_to_digit_by_storage(dest, source, max_dest_len, storage);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_to_digit_get_byte
* DESCRIPTION
*   convert BCD character to ASCII
*
* PARAMETERS
*  bcd	IN	bcd character
* RETURNS
*  ascii character
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_convert_to_digit_get_byte (U8 bcd)
{
	U8 digit;

	if (bcd <= 9)
	{
		digit = bcd + '0';
	}
	else
	{
		switch (bcd)
		{
			case 0x0a:
			digit = '*';
			break;

			case 0x0b:
			digit = '#';
			break;

			case 0x0c:
			digit = 'p';
			break;

			case 0x0d:
			digit = 'w';
			break;

			default:
			digit = '\0';
			break;
		}
	}
	return digit;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_to_digit()
* DESCRIPTION
*   convert BCD string to ASCII
*
* PARAMETERS
*	source		IN				An BCD encoding string.
*	dest		IN/OUT		An ASCII encoding string.
*	max_dest_len	IN			the byte size of dest array.(include null terminate '\0')
* RETURNS
*  byte number after convert.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_convert_to_digit (U8 *dest, U8 *source, U8 max_dest_len)
{
	U8 ch1, ch2;
	U8 i=0, j=0;

	while ( (source[i] != 0xff) && (j < max_dest_len - 1))
	{
		ch1 = source[i] & 0x0f;
		ch2 = (source[i] & 0xf0) >> 4;

		*((U8*)dest + j) = mmi_phb_convert_to_digit_get_byte (ch1);

		if (ch2 == 0x0f)
		{
			*((U8 *)dest + j + 1) = '\0';
			return j+1;
		}
		else
		{
			*((U8*)dest + j + 1) = mmi_phb_convert_to_digit_get_byte (ch2);
		}
		i++;
		j+=2;
	}

	*((U8*)dest+j) = '\0';
	return j;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_get_ucs2_number()
* DESCRIPTION
*   This function returns the UCS2 number string by specifying store_index
*
* PARAMETERS
*  pString	IN/OUT	buffter to store convert result
*  index	IN		store index of the phonebook entry to be converted.
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_convert_get_ucs2_number(S8* pString,U16 index)
{
	U8 tempNumber[MAX_PB_NUMBER_LENGTH + 1 + 1];

	if(PhoneBook[index].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
	{
		tempNumber[0] = '+';
		mmi_phb_convert_to_digit((U8*)(tempNumber + 1), PhoneBook[index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
	}
	else
	{
		mmi_phb_convert_to_digit(tempNumber, PhoneBook[index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
	}

	AnsiiToUnicodeString(pString, (S8*)tempNumber);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_to_0x81_get_offset()
* DESCRIPTION
*   This function get the offset according to the base for a two bytes UCS2 character.
*
* PARAMETERS
*  base	IN	base of the character.
*  code	IN	character for getting offset
* RETURNS
*  offset
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_convert_to_0x81_get_offset(U16 base, U16 code)
{
	if(code < 0x80)
		return (U8)code;
	else
		return (U8)(code - base + 0x80);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_to_0x81_get_base()
* DESCRIPTION
*   This function get the base for a two bytes UCS2 chatacters.(For Thai only now.)
*
* PARAMETERS
*  code	IN	input character
* RETURNS
*  the offset base
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_convert_to_0x81_get_base(U16 code)
{
	if((code >= 0x0E00) && (code <= 0x0E5F )) /*Thai 0E00 ~ 0E5F*/
		return 0x0E00;
	if((code >= 0x0080) && (code <= 0x00FF )) /*C1 Controls and Latin-1 Supplement*/
		return 0x0080;
	if((code >= 0x0100) && (code <= 0x017F ))	/*Latin Extended - A*/
		return 0x0100;
	if((code >= 0x0180) && (code <= 0x01FF )) /*Latin Extended - B (Partial)*/
		return 0x0180;
	if((code >= 0x0400) && (code <= 0x04FF )) /*Russian 0400 ~ 04FF*/
		return 0x0400;
	if((code >= 0x0600) && (code <= 0x06FF )) /*Arabic 0600 ~ 06FF*/
		return 0x0600; 

	return 0x0000;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_convert_to_0x81()
* DESCRIPTION
*   This convert a 0x80 UCS2 string to 0x81 format. (Only for Thai Language right now.)
*
* PARAMETERS
*  input_name		IN	the input string.(Assume the input name is in high byte - low byte order)
* RETURNS
*  converted result length(in byte) for input string.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_convert_to_0x81(S8* input_name, BOOL is_convert)
{
	U16 code;
	U16 base = 0;
	BOOL flag = TRUE;
	S8 result_name[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
	S8* temp_name;
	U8 i = 0;

	temp_name = input_name;


	/*Find first UCS2 character as base*/
	while(!((*temp_name == 0) && (*(temp_name +1) == 0)))
	{
		memcpy(&code,temp_name,2);

		/*Already GSM Encoding, need to exchange byte order*/
		/*if(is_convert)
		{
			code = (code << 8) | (code >> 8);
		}
		*/

		if(code >= 0x80 && !UI_TEST_8895_1_CHAR_IN_GSM_DEF_CHAR(code))
		{
			base = mmi_phb_convert_to_0x81_get_base(code);
			break;
		}
		temp_name += 2;
	}

	/*Check if whole string can be encoded as 0x81.*/
	temp_name = input_name;
	if(base > 0)
	{
		memset(result_name,0xFF,(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH);
		result_name[0] = (U8)0x81;
		result_name[1] = (U8)pfnUnicodeStrlen(input_name);
		result_name[2] = (U8)(base >> 7);

		i = 3;
		while(!((*temp_name == 0) && (*(temp_name +1) == 0)))
		{
			memcpy(&code,temp_name,2);

			/*Already GSM Encoding, need to exchange byte order*/
			/*
			if(is_convert)
			{
				code = (code << 8) | (code >> 8);
			}
			*/
			if((code >= 0x80) && (mmi_phb_convert_to_0x81_get_base(code) != base))
			{
				flag = FALSE;
				break;
			}

			result_name[i] = mmi_phb_convert_to_0x81_get_offset(base, code);
			temp_name += 2;
			i++;
		}

		if(flag)
		{
			/*Wrtie result to the same buffer*/
			if(is_convert)
			{
				memcpy(input_name,result_name,(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH );
			}
			return i;
		}
	}

	return 0;
}

/*****************************************************************************
* FUNCTION
*  PhbAsciiToGSM7Bit()
* DESCRIPTION
*   Converts Ascii to GSM 7-Bit encoding
*
* PARAMETERS
*  buffer	IN/OUT	result buffer
* RETURNS
* void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PhbAsciiToGSM7Bit(PU8 buffer)
{
#if 0 // zrx del 20060612
	U16 i, length;

	length = strlen((PS8)buffer);
	for(i=0; i < length; ++i)
		buffer[i] = AsciiToDefaultArray[buffer[i]];
	#endif
}

/*****************************************************************************
* FUNCTION
*  PhbGSM7BitToAscii()
* DESCRIPTION
*   Converts GSM 7-Bit to Ascii encoding
*
* PARAMETERS
*  buffer	IN/OUT	result buffer
*  length	IN		result buffer size
* RETURNS
* void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PhbGSM7BitToAscii(PU8 buffer, U8 length)
{
#if 0 // zrx del 20060612
	U16 i;

	for(i=0; i < length; ++i)
	{
		if(buffer[i] == 27) /*treat escape char as a space*/
			buffer[i] = 32;
		else
			buffer[i] = DefaultToAsciiArray[buffer[i]];
	}
	#endif
}

/*****************************************************************************
* FUNCTION
*  GetUCS2Flag()
* DESCRIPTION
*   Checks if the buffer contains Chinese character
*
* PARAMETERS
*  buffer	IN	string to be checked
* RETURNS
* TRUE is a UCS2 two bytes string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL GetUCS2Flag(PS8 buffer)
{
	pBOOL UCS2Flag = FALSE;
	U8 i, bufferLen;
	UI_character_type ch;

	bufferLen = pfnUnicodeStrlen(buffer)*ENCODING_LENGTH;
	for(i = 1; i < bufferLen; i += 2)
	{
	   mmi_trace(g_sw_CC," Func: %s buffer[%d]=0x%x,buffer[%d]=0x%x", __FUNCTION__,i,buffer[i],i-1,buffer[i-1]);
	   if(buffer[i] || (buffer[i-1] & 0x80))
	   {
	   	  ch = ((UI_character_type)buffer[i] << 8)|((UI_character_type)buffer[i-1] );
	         if(!UI_TEST_8895_1_CHAR_IN_GSM_DEF_CHAR(ch))
	         {
		   UCS2Flag = TRUE;
		   break;
	         }
	    }
	}
	return UCS2Flag;
}

/*****************************************************************************
 * FUNCTION
 *  GetUCS2ExtendedNum
 * DESCRIPTION
 *  Checks if the buffer contains Chinese character
 * PARAMETERS
 *  buffer      [IN]        String to be checked
 * RETURNS
 *  TRUE is a UCS2 two bytes string
 *****************************************************************************/
U8 GetUCS2ExtendedNum(PS8 buffer, U8 max_buffer_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 extend_num = 0;
    UI_character_type ch;
    U8 ex_count = 0;
    U8 ex_len = 1; /* need second bytes */
    U8 buffer_len = mmi_ucs2strlen(buffer) * ENCODING_LENGTH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (ex_count < max_buffer_len && ex_len < buffer_len)
    {
        ch = ((UI_character_type) buffer[ex_len] << 8) | ((UI_character_type) buffer[ex_len - 1]);
        if (UI_TEST_GSM_EXTENDED(ch))
        {
            ex_count++;
            extend_num++;
        }
        ex_count++;
        ex_len += 2;
    }
    return extend_num;
}

/*****************************************************************************
* FUNCTION
*  BigEndianToLittleEndian()
* DESCRIPTION
*   Changes big endian to be little endian and vice versa
*
* PARAMETERS
*  dstBuffer	IN	destination buffer
*  srcBuffer	IN	source buffer
* RETURNS
* result string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
PS8 BigEndianToLittleEndian(PS8 dstBuffer, PS8 srcBuffer)
{
	U8 i = 0;
	U8 len;

	len = pfnUnicodeStrlen(srcBuffer)*ENCODING_LENGTH;
	while(i != len)
	{
		dstBuffer[i]	= srcBuffer[i+1];
		dstBuffer[i+1]	= srcBuffer[i];
		i += 2;
	}
	dstBuffer[i] = '\0';
	dstBuffer[i+1] = '\0';

	return dstBuffer;
}

#endif /* _PHONEBOOKSTUBSTOOTHERS_C */


