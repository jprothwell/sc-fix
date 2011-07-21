/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * DataAccount.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifdef __GPRS_MODE__

#include "dataaccountcommon.h"
#include "protocolevents.h"
#include "debuginitdef.h"
#include "phonebooktypes.h"

#include "abm_soc_enums.h"
#include "abm_util.h"
#include "cswtype.h"
extern wgui_inline_item wgui_inline_items[];

void (*g_DTCNTReadyCallback) (void) = NULL;

DataAccountNode g_DataAccount[MAX_DATA_ACCOUNT_GSM_LIMIT];
static DataAccountNodeDisplay g_DataAccount_Display;

#ifdef __MMI_GPRS_FEATURES__

GPRSDataAccountNode g_GPRS_DataAccount[MAX_DATA_ACCOUNT_GPRS_LIMIT];

#endif /* __MMI_GPRS_FEATURES__ */ 

typedef struct
{
    U8 bearer;
    U8 bearer_enum;
    U16 name;
    FuncPtr func;
} dtcnt_select_op_struct;


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_pre_entry_app_csd_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_dtcnt_pre_entry_app_csd_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DataAccountReadyCheck(EntryAppDTCNTList);
}


#ifdef __MMI_GPRS_FEATURES__
/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_pre_entry_app_gprs_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_dtcnt_pre_entry_app_gprs_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
		    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] mmi_dtcnt_pre_entry_app_gprs_list"));

    DataAccountReadyCheck(EntryGPRSAppDTCNTList);
}
#endif /* __MMI_GPRS_FEATURES__ */ 

const dtcnt_select_op_struct g_dtcnt_selections[] = 
{
    {DATA_ACCOUNT_BEARER_CSD, ABM_E_SOCKET_BEARER_GSM_CSD, SERVICES_DATA_CONNECT_GSM_STRING_ID,
     mmi_dtcnt_pre_entry_app_csd_list},
 #ifdef __MMI_GPRS_FEATURES__
    {DATA_ACCOUNT_BEARER_GPRS, ABM_E_SOCKET_BEARER_GSM_GPRS, SERVICES_DATA_CONNECT_GPRS_STRING_ID,
     mmi_dtcnt_pre_entry_app_gprs_list},
 #endif /* __MMI_GPRS_FEATURES__ */ 
    {0, 0, 0, NULL}
};

void mmi_dtcnt_entry_account_select(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_get_first_account_id
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bearers     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 mmi_dtcnt_get_first_account_id(U8 bearers)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i, account_id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_WIN32
    return 0xFF;
#else
    for (i = 0; g_dtcnt_selections[i].bearer > 0; i++)
    {
        if (bearers & g_dtcnt_selections[i].bearer)
        {
            /* Special case for GPRS */
            if (g_dtcnt_selections[i].bearer_enum == ABM_E_SOCKET_BEARER_GSM_GPRS)
            {
                if (abm_bearer2nw_id(g_dtcnt_selections[i].bearer_enum, 1, &account_id) == TRUE)
                {
                    return account_id;
                }
            }
            else
            {
                if (abm_bearer2nw_id(g_dtcnt_selections[i].bearer_enum, 0, &account_id) == TRUE)
                {
                    return account_id;
                }
            }
        }
    }
    ASSERT(0);
    return 0xFF;
#endif /* MMI_ON_WIN32 */ 
}


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_select_account_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  callback        [IN]        
 *  AppMenuID       [IN]        
 *  bearers         [IN]        
 *  account_id      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_dtcnt_select_account_ext(FuncPtrShort callback, U16 AppMenuID, U8 bearers, U8 account_id)
{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	   // PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] mmi_dtcnt_select_account_ext,bearers: %d",bearers));

    kal_uint8 bearer = 0;
    kal_uint8 id = 0;
    U8 i, count = 0;
    FuncPtr funcs[DATA_ACCOUNT_BEARER_TOTAL];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_WIN32
    PopupNoSupport();
    return;
#endif /* MMI_ON_WIN32 */ 

    g_DataAccount_Display.app_callback = callback;
    g_DataAccount_Display.AppMenuID = AppMenuID;
    g_DataAccount_Display.CurrAppMainSelectItem = 0;
    g_DataAccount_Display.CurrAppSelectItem = 0;
#ifdef __MMI_GPRS_FEATURES__
    g_DataAccount_Display.GPRSCurrAppSelectItem = 0;
#endif /* __MMI_GPRS_FEATURES__ */ 
    g_DataAccount_Display.bearers = bearers;

    for (i = 0; g_dtcnt_selections[i].bearer > 0; i++)
    {
        if (bearers & g_dtcnt_selections[i].bearer)
        {
            funcs[count++] = g_dtcnt_selections[i].func;
        }
    }
	    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] mmi_dtcnt_select_account_ext,count: %d",count));

    if (count == 1)
    {
#ifndef MMI_ON_WIN32
        if (abm_nw2bearer_id(account_id, &bearer, &id) == KAL_TRUE)
        {
            if ((bearers & DATA_ACCOUNT_BEARER_CSD) && (bearer == ABM_E_SOCKET_BEARER_GSM_CSD))
            {
                g_DataAccount_Display.CurrAppSelectItem = id;
            }
        #ifdef __MMI_GPRS_FEATURES__
            else if((bearers & DATA_ACCOUNT_BEARER_GPRS) && (bearer == ABM_E_SOCKET_BEARER_GSM_GPRS))
            {
                g_DataAccount_Display.GPRSCurrAppSelectItem = id - 1;
            }
        #endif /* __MMI_GPRS_FEATURES__ */ 
        }
#endif /* MMI_ON_WIN32 */ 
        
        funcs[0] ();
        return;
    }
    mmi_dtcnt_entry_account_select();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_select_account
 * DESCRIPTION
 *  
 * PARAMETERS
 *  callback        [IN]        
 *  AppMenuID       [IN]        
 *  bearers         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_dtcnt_select_account(FuncPtrShort callback, U16 AppMenuID, U8 bearers)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_dtcnt_select_account_ext(callback, AppMenuID, bearers, 0);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_get_apn
 * DESCRIPTION
 *  
 * PARAMETERS
 *  account_id      [IN]        
 *  dest            [OUT]         
 *  len             [IN]        
 * RETURNS
 *  
 *****************************************************************************/
BOOL mmi_dtcnt_get_apn(U8 account_id, S8 *dest, U8 len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 bearer = 0;
    kal_uint8 id = 0;   /* dummy */
    U8 length = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(MMI_ON_WIN32) || !defined(__MMI_GPRS_FEATURES__)
    return FALSE;
#else
    if(!dest)
    {
        return FALSE;
    }
    
    length = (len < (MAX_GPRS_MMI_APN_LEN+ 1) * ENCODING_LENGTH) ? len : (MAX_GPRS_MMI_APN_LEN + 1) * ENCODING_LENGTH;
        
    if (abm_nw2bearer_id(account_id, &bearer, &id) == KAL_TRUE)
    {
        if(bearer == ABM_E_SOCKET_BEARER_GSM_GPRS)
        {
            pfnUnicodeStrncpy(dest, (S8*) g_GPRS_DataAccount[id - 1].APN, length);
            return TRUE;
        }
    }
    memset(dest, 0, len);
    return FALSE;
#endif /* MMI_ON_WIN32 */ 
}

BOOL mmi_dtcnt_get_user_pass(U8 account_id, S8 *user,S8 * pass,U8 len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 bearer = 0;
    kal_uint8 id = 0;   /* dummy */
    U8 length = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(MMI_ON_WIN32) || !defined(__MMI_GPRS_FEATURES__)
    return FALSE;
#else
    if((!user)||(!pass))
    {
        return FALSE;
    }
    
    length = (len < (MAX_GPRS_USERNAME + 1) ? len : (MAX_GPRS_USERNAME + 1));
        
    if (abm_nw2bearer_id(account_id, &bearer, &id) == KAL_TRUE)
    {
        if(bearer == ABM_E_SOCKET_BEARER_GSM_GPRS)
        {
            strncpy(user, (S8*) g_GPRS_DataAccount[id - 1].UserName, length);
	      strncpy(pass, (S8*) g_GPRS_DataAccount[id - 1].Passwd, length);
            return TRUE;
        }
    }
    memset(user, 0, len);
    memset(pass, 0, len);
    return FALSE;
#endif /* MMI_ON_WIN32 */ 
}


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_get_account_name
 * DESCRIPTION
 *  
 * PARAMETERS
 *  account_id      [IN]        
 *  dest            [OUT]         
 *  len             [IN]        
 * RETURNS
 *  
 *****************************************************************************/
BOOL mmi_dtcnt_get_account_name(U8 account_id, S8 *dest, U8 len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 bearer = 0;
    kal_uint8 id = 0;   /* dummy */
    U8 length =
        len <
        (MAX_DATA_ACCOUNT_NAME_LEN + 1) * ENCODING_LENGTH ? len : (MAX_DATA_ACCOUNT_NAME_LEN + 1) * ENCODING_LENGTH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_WIN32
    return FALSE;
#else
    if(!dest)
    {
        return FALSE;
    }
	mmi_trace(1,"length:%d, account_id:%d",length, account_id);

    if (abm_nw2bearer_id(account_id, &bearer, &id) == KAL_TRUE)
    {
        switch (bearer)
        {
            case ABM_E_SOCKET_BEARER_GSM_CSD:
                pfnUnicodeStrncpy(dest, (S8*) g_DataAccount[id].DataAccountName, length);
                return TRUE;
#ifdef __MMI_GPRS_FEATURES__
            case ABM_E_SOCKET_BEARER_GSM_GPRS:
                pfnUnicodeStrncpy(dest, (S8*) g_GPRS_DataAccount[id - 1].DataAccountName, length);
                return TRUE;
#endif /* __MMI_GPRS_FEATURES__ */ 
            default:
                memset(dest, 0, len);
                return FALSE;
        }
    }

    memset(dest, 0, len);
    return FALSE;
#endif /* MMI_ON_WIN32 */ 
}


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_account_select_highlight_hdlr
 * DESCRIPTION
 *  highlight handler for DTCNTAppList
 *  
 *  Index values start on zero for first Listesd item.
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_dtcnt_account_select_highlight_hdlr(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_DataAccount_Display.CurrAppMainSelectItem = (U8) nIndex;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_account_select_ok
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_dtcnt_account_select_ok(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_DataAccount_Display.bearer_callbacks[g_DataAccount_Display.CurrAppMainSelectItem] ();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_entry_account_select
 * DESCRIPTION
 *  Data account main screen ( including GSM/GPPRS)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_dtcnt_entry_account_select(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;
    U16 nStrItemList[MAX_SUB_MENUS];    /* Stores the strings id of submenus returned */
    U8 *guiBuffer;                      /* Buffer holding history data */
    U16 nNumofItem = 0;                 /* Stores no of children in the submenu */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_DTCNT_MAIN, NULL, mmi_dtcnt_entry_account_select, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_MAIN);

    for (i = 0; g_dtcnt_selections[i].bearer > 0; i++)
    {
        if (g_DataAccount_Display.bearers & g_dtcnt_selections[i].bearer)
        {
            nStrItemList[nNumofItem] = g_dtcnt_selections[i].name;
            g_DataAccount_Display.bearer_callbacks[nNumofItem] = g_dtcnt_selections[i].func;
            nNumofItem++;
        }
    }

    RegisterHighlightHandler(mmi_dtcnt_account_select_highlight_hdlr);

    ShowCategory52Screen(
        SERVICES_DATA_CONNECT_MAIN_STRING_ID,
        GetRootTitleIcon(g_DataAccount_Display.AppMenuID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (U16*) gIndexIconsImageList,
        0,
        0,
        0,
        guiBuffer);
    SetLeftSoftkeyFunction(mmi_dtcnt_account_select_ok, KEY_EVENT_UP);
    SetKeyHandler(mmi_dtcnt_account_select_ok, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_dtcnt_exit_list_hdlr
 * DESCRIPTION
 *  GoBackHistory wrapper for resetting highlight index.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static U8 mmi_dtcnt_exit_list_hdlr(void *p)
{
    g_DataAccount_Display.CurrSelectItem = 0;
#ifdef __MMI_GPRS_FEATURES__
    g_DataAccount_Display.GPRSCurrSelectItem = 0;
#endif
    return FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  SetDynaDataAccountName
 * DESCRIPTION
 *  
 * PARAMETERS
 *  UCS2Name        [?]         
 *  index           [IN]        
 *  void(?)         [IN]        *
 * RETURNS
 *  void
 *****************************************************************************/
void SetDynaDataAccountName(S8 *UCS2Name, U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 LocalBuffer[8];
    S8 StrVal[8];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    sprintf(LocalBuffer, "%d", index + 1);
    AnsiiToUnicodeString((PS8) StrVal, (PS8) LocalBuffer);

    pfnUnicodeStrcpy((PS8) UCS2Name, (PS8) GetString(STR_ID_DTCNT_NAME));

    pfnUnicodeStrcat((PS8) UCS2Name, (PS8) StrVal);
}

//JIASHUO ADD FOR WAP/MMS,START
/*****************************************************************************
 * FUNCTION
 *  InitGetDataAccountCallBack
 * DESCRIPTION
 *  JUST GET DATA ACCOUNT CONTENT,NO NEED CALLBACK,SO JUST A VOID FUNCTION 
 * PARAMETERS
 *  UCS2Name        [?]         
 *  index           [IN]        
 *  void(?)         [IN]        *
 * RETURNS
 *  void
 *****************************************************************************/
void InitGetDataAccountCallBack(void)
{
	//VOID FUNCTION
}
//JIASHUO ADD FOR WAP/MMS,END

/*****************************************************************************
 * FUNCTION
 *  InitDataAccountApp
 * DESCRIPTION
 *  Initialize the Data Account App
 *  
 *  This is used to initialized the Data Account application
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitDataAccountApp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;
    U8 ImageLocation = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_DATA_ACCOUNT_GSM_LIMIT; i++)
    {
        memset(&g_DataAccount[i], '\0', sizeof(DataAccountNode));
        g_DataAccount[i].Present = (U8) '\0';
        g_DataAccount[i].iFdcs = 0;
    }
    for (i = 0; i < MAX_INLINE_ITEM_DATA_ACCOUNT_LIST; i++)
    {
        if (i % 2)
        {
            g_DataAccount_Display.IconList[i] = IMG_ID_DTCNT_LSK_OK;
        }
        else
        {
            g_DataAccount_Display.IconList[i] = IMG_ID_DTCNT_AC_NAME + ImageLocation;
            ImageLocation++;
        }
    }
    g_DataAccount_Display.CurrGetIndex = 0;
    g_DataAccount_Display.CurrSelectItem = 0;
    /* micha0112 */
    g_DataAccount_Display.CurrAppSelectItem = 0;

    /* micha0913 */
#ifdef __MMI_GPRS_FEATURES__
    for (i = 0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
    {
        memset(&g_GPRS_DataAccount[i], '\0', sizeof(GPRSDataAccountNode));
        g_GPRS_DataAccount[i].Present = (U8) '\0';
        g_GPRS_DataAccount[i].iFdcs = 0;
    }

    ImageLocation = 0;
    for (i = 0; i < MAX_INLINE_ITEM_GPRSDATA_ACCOUNT_LIST; i++)
    {
        if (i % 2)
        {
            g_DataAccount_Display.IconList[i] = IMG_ID_DTCNT_LSK_OK;
        }
        else
        {
            g_DataAccount_Display.IconList[i] = IMG_ID_DTCNT_GPRS_AC_NAME + ImageLocation;
            ImageLocation++;
        }
    }
    g_DataAccount_Display.GPRSCurrGetIndex = 0;
    g_DataAccount_Display.GPRSCurrSelectItem = 0;
    /* micha0112 */
    g_DataAccount_Display.GPRSCurrAppSelectItem = 0;
	
	DataAccountGPRSReadyCheck(InitGetDataAccountCallBack);//JIASHUO ADD FOR WAP/MMS,WE NEED DATA ACCOUNT CONTENT AFTER POWERON
	
#endif /* __MMI_GPRS_FEATURES__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  InitializeDataAccountRecords
 * DESCRIPTION
 *  Initialize Data Account Records
 *  
 *  Called from SAT to enter Data Account Main Screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeDataAccountRecords(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DataAccountReadyCheck(EntryDTCNTList);
}


/*****************************************************************************
 * FUNCTION
 *  DataAccountReadyCheck
 * DESCRIPTION
 *  Chec if data account is ready in global array,
 *  execute call back function when ready.
 * PARAMETERS
 *  ready_callback      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void DataAccountReadyCheck(FuncPtr ready_callback)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DataAccountReadyCheck:cal back:0x%x",ready_callback));

    g_DTCNTReadyCallback = ready_callback;
    ClearInputEventHandler(MMI_DEVICE_ALL);
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);    
    DTCNTGetAccountReq(0);
}

//start,jiashuo add for wap/mms,coolsand cannot support gsm csd,so only get gprs data account
/*****************************************************************************
 * FUNCTION
 *  DataAccountReadyCheck
 * DESCRIPTION
 *  Chec if data account is ready in global array,
 *  execute call back function when ready.
 * PARAMETERS
 *  ready_callback      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void DataAccountGPRSReadyCheck(FuncPtr ready_callback)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_DTCNTReadyCallback = ready_callback;
    ClearInputEventHandler(MMI_DEVICE_ALL);
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);    
    DTCNTGPRSGetAccountReq(0);
}
//end,jiashuo add for wap/mms,coolsand cannot support gsm csd,so only get gprs data account

/* micha0112 begin */


/*****************************************************************************
 * FUNCTION
 *  HighlightAppDTCNTList
 * DESCRIPTION
 *  highlight handler for DTCNTAppList
 *  
 *  Index values start on zero for first Listesd item.
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightAppDTCNTList(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_DataAccount_Display.CurrAppSelectItem = (U8) nIndex;
}


/*****************************************************************************
 * FUNCTION
 *  EntryAppDTCNTList
 * DESCRIPTION
 *  shows the main menu list
 *  
 *  This is used to shows the main menu list of data account
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryAppDTCNTList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    U8 i = 0;
    U8 *DataAccountName[MAX_DATA_ACCOUNT_GSM_LIMIT];
    static U8 *PopUpDescription[MAX_DATA_ACCOUNT_GSM_LIMIT];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_DATA_ACCOUNT_GSM_LIMIT; i++)
    {
        /* micha0307 */
        DataAccountName[i] = g_DataAccount[i].DataAccountName;
        if (pfnUnicodeStrlen((PS8) g_DataAccount[i].DialNumber) > 0)
        {
            PopUpDescription[i] = g_DataAccount[i].DialNumber;
        }
        else
        {
            PopUpDescription[i] = (U8*) GetString(STR_GLOBAL_EMPTY_LIST);
        }
    }

    EntryNewScreen(SCR_ID_DTCNT_LIST, NULL, EntryAppDTCNTList, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_LIST);
    RegisterHighlightHandler(HighlightAppDTCNTList);

    ShowCategory53Screen(
        STR_ID_DTCNT_CAPTION,
        (g_DataAccount_Display.AppMenuID ? GetRootTitleIcon(g_DataAccount_Display.AppMenuID) : 0),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        MAX_DATA_ACCOUNT_GSM_LIMIT,
        (U8 **) DataAccountName,
        (PU16) gIndexIconsImageList,
        PopUpDescription,
        0,
        g_DataAccount_Display.CurrAppSelectItem,
        guiBuffer);
    SetLeftSoftkeyFunction(AppDTCNTSelectOK, KEY_EVENT_UP);
    SetKeyHandler(AppDTCNTSelectOK, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void GetAccountId(U8 dataconn_id, U8* data_account)
{
	abm_bearer2nw_id(ABM_E_SOCKET_BEARER_GSM_GPRS, dataconn_id + 1, data_account);
}
/*****************************************************************************
 * FUNCTION
 *  AppDTCNTSelectOK
 * DESCRIPTION
 *  highlight handler for DTCNTAppList
 *  
 *  Index values start on zero for first Listesd item.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void AppDTCNTSelectOK(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifndef MMI_ON_WIN32
    if (g_DataAccount_Display.app_callback)
    {
        kal_uint8 id;

        abm_bearer2nw_id(ABM_E_SOCKET_BEARER_GSM_CSD, g_DataAccount_Display.CurrAppSelectItem, &id);
        g_DataAccount_Display.app_callback((U16) id);
        g_DataAccount_Display.app_callback = NULL;
    }
#endif /* MMI_ON_WIN32 */ 
}

/* micha0112 end */


/*****************************************************************************
 * FUNCTION
 *  HighlightDTCNTList
 * DESCRIPTION
 *  gets current hilite item for data account list
 *  
 *  Index values start on zero for first Listesd item.
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightDTCNTList(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_DataAccount_Display.CurrSelectItem = (U8) nIndex;
}


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTList
 * DESCRIPTION
 *  shows the main menu list
 *  
 *  This is used to shows the main menu list of data account
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    U8 i = 0;
    U8 *DataAccountName[MAX_DATA_ACCOUNT_GSM_LIMIT];
    static U8 *PopUpDescription[MAX_DATA_ACCOUNT_GSM_LIMIT];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_DATA_ACCOUNT_GSM_LIMIT; i++)
    {
        /* micha0307 */
        DataAccountName[i] = g_DataAccount[i].DataAccountName;
        if (pfnUnicodeStrlen((PS8) g_DataAccount[i].DialNumber) > 0)
        {
            PopUpDescription[i] = g_DataAccount[i].DialNumber;
        }
        else
        {
            PopUpDescription[i] = (U8*) GetString(STR_GLOBAL_EMPTY_LIST);
        }
    }

    EntryNewScreen(SCR_ID_DTCNT_LIST, NULL, InitializeDataAccountRecords, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_LIST);
    SetParentHandler(SERVICES_DATA_CONNECT_MAIN_MENU_ID);
    RegisterHighlightHandler(HighlightDTCNTList);

    ShowCategory53Screen(
        STR_ID_DTCNT_CAPTION,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        STR_GLOBAL_EDIT,
        IMG_ID_DTCNT_LSK_EDIT,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        MAX_DATA_ACCOUNT_GSM_LIMIT,
        (U8 **) DataAccountName,
        (PU16) gIndexIconsImageList,
        PopUpDescription,
        0,
        g_DataAccount_Display.CurrSelectItem,
        NULL);

    SetLeftSoftkeyFunction(InitInLineDataAccount, KEY_EVENT_UP);
    SetKeyHandler(InitInLineDataAccount, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    SetDelScrnIDCallbackHandler(SCR_ID_DTCNT_LIST, mmi_dtcnt_exit_list_hdlr);
}


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTEdit
 * DESCRIPTION
 *  Edit Data Account
 *  
 *  This is used to edit Data Account Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTEdit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *inputBuffer;
    U8 *guiBuffer;
    U16 inputBufferSize;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_DTCNT_EDIT, ExitDTCNTEdit, NULL, NULL);

    InitializeCategory57Screen();
    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_EDIT);
    inputBuffer = GetCurrNInputBuffer(SCR_ID_DTCNT_EDIT, &inputBufferSize);     /* added for inline edit history */
    SetParentHandler(MITEM_DATA_ACCOUNT_PARENT_ZERO);
    DataAccountFillInlineStruct();

    if (inputBuffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, MAX_INLINE_ITEM_DATA_ACCOUNT_LIST, inputBuffer);
    }

    if (VerifyDataAcountChanges() && GetInlineDoneFlag(guiBuffer) == 0)
    {
        SetInlineDoneFlag(guiBuffer);
    }

    ShowCategory57Screen(
        STR_GLOBAL_EDIT,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        STR_GLOBAL_EDIT,
        IMG_ID_DTCNT_LSK_EDIT,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        MAX_INLINE_ITEM_DATA_ACCOUNT_LIST,
        g_DataAccount_Display.IconList,
        wgui_inline_items,
        DATA_ACCOUNT_HILITED_ITEM,
        guiBuffer);

    SetCategory57RightSoftkeyFunctions(EntryDTCNTSave, GoBackHistory);
}


/*****************************************************************************
 * FUNCTION
 *  ExitDTCNTEdit
 * DESCRIPTION
 *  Exit from Edit
 *  
 *  This is used to exit screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitDTCNTEdit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history_t h;
    U16 inputBufferSize;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();
    h.scrnID = SCR_ID_DTCNT_EDIT;
    h.entryFuncPtr = EntryDTCNTEdit;
    GetCategoryHistory(h.guiBuffer);
    inputBufferSize = (U16) GetCategory57DataSize();
    GetCategory57Data((U8*) h.inputBuffer);
    AddNHistory(h, inputBufferSize);
}


/*****************************************************************************
 * FUNCTION
 *  InitInLineDataAccount
 * DESCRIPTION
 *  Fill the Inline Structure.
 *  
 *  This is used to edit Data Account Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitInLineDataAccount(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = g_DataAccount_Display.CurrSelectItem;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pfnUnicodeStrcpy((PS8) g_DataAccount_Display.DataAccountName, (PS8) g_DataAccount[i].DataAccountName);
    pfnUnicodeStrcpy((PS8) g_DataAccount_Display.DialNumber, (PS8) g_DataAccount[i].DialNumber);
    AnsiiToUnicodeString((PS8) g_DataAccount_Display.UserName, (PS8) g_DataAccount[i].UserName);
    AnsiiToUnicodeString((PS8) g_DataAccount_Display.Passwd, (PS8) g_DataAccount[i].Passwd);

    ReturnThreeDigitString(g_DataAccount_Display.IP1, g_DataAccount[i].DnsAddress[0]);
    ReturnThreeDigitString(g_DataAccount_Display.IP2, g_DataAccount[i].DnsAddress[1]);
    ReturnThreeDigitString(g_DataAccount_Display.IP3, g_DataAccount[i].DnsAddress[2]);
    ReturnThreeDigitString(g_DataAccount_Display.IP4, g_DataAccount[i].DnsAddress[3]);

    g_DataAccount_Display.DialType = (S32) g_DataAccount[i].DialType;
    g_DataAccount_Display.DataRate = (S32) g_DataAccount[i].DataRate;

    g_DataAccount_Display.DialTypeOption[0] = (U8*) GetString(STR_ID_DTCNT_ANALOGUE_STR);
    g_DataAccount_Display.DialTypeOption[1] = (U8*) GetString(STR_ID_DTCNT_ISDN_STR);
    g_DataAccount_Display.DialRateOption[0] = (U8*) GetString(STR_ID_DTCNT_RATE_STR_ONE);
    g_DataAccount_Display.DialRateOption[1] = (U8*) GetString(STR_ID_DTCNT_RATE_STR_TWO);
    g_DataAccount_Display.DialRateOption[2] = (U8*) GetString(STR_ID_DTCNT_RATE_STR_THREE);

    EntryDTCNTEdit();
}


/*****************************************************************************
 * FUNCTION
 *  DataAccountFillInlineStruct
 * DESCRIPTION
 *  Call the Inline Functions.
 *  
 *  This is used to edit Data Account Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DataAccountFillInlineStruct(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 BufferSize = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetInlineItemActivation((wgui_inline_items + DATA_ACCOUNT_NAME), KEY_LSK, KEY_EVENT_UP);
    SetInlineItemActivation((wgui_inline_items + DATA_ACCOUNT_DIAL_NUMBER), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemActivation((wgui_inline_items + DATA_ACCOUNT_USER_NAME), KEY_LSK, KEY_EVENT_UP);
    SetInlineItemActivation((wgui_inline_items + DATA_ACCOUNT_PASSWD), KEY_LSK, KEY_EVENT_UP);
    SetInlineItemActivation((wgui_inline_items + DATA_ACCOUNT_DNS_ADDRESS), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemActivation((wgui_inline_items + DATA_ACCOUNT_DIAL_TYPE), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemActivation((wgui_inline_items + DATA_ACCOUNT_DATA_RATE), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

    SetInlineItemCaption((wgui_inline_items + DATA_ACCOUNT_NAME_STR), (U8*) GetString(STR_ID_DTCNT_RENAME));
    SetInlineItemCaption(
        (wgui_inline_items + DATA_ACCOUNT_DIAL_NUMBER_STR),
        (U8*) GetString(STR_ID_DTCNT_DIAL_NUMBER));
    SetInlineItemCaption((wgui_inline_items + DATA_ACCOUNT_USER_NAME_STR), (U8*) GetString(STR_ID_DTCNT_USER_NAME));
    SetInlineItemCaption((wgui_inline_items + DATA_ACCOUNT_PASSWD_STR), (U8*) GetString(STR_ID_DTCNT_PASSWD));
    SetInlineItemCaption((wgui_inline_items + DATA_ACCOUNT_DIAL_TYPE_STR), (U8*) GetString(STR_ID_DTCNT_DIAL_TYPE));
    SetInlineItemCaption((wgui_inline_items + DATA_ACCOUNT_DATA_RATE_STR), (U8*) GetString(STR_ID_DTCNT_DATA_RATE));
    SetInlineItemCaption(
        (wgui_inline_items + DATA_ACCOUNT_DNS_ADDRESS_STR),
        (U8*) GetString(STR_ID_DTCNT_DNS_ADDRESS));

    SetInlineItemSelect(
        (wgui_inline_items + DATA_ACCOUNT_DIAL_TYPE),
        (S32) DATA_ACCOUNT_INLINE_DIAL_TYPE_OPTION,
        (U8 **) g_DataAccount_Display.DialTypeOption,
        (S32*) & g_DataAccount_Display.DialType);
    SetInlineItemSelect(
        (wgui_inline_items + DATA_ACCOUNT_DATA_RATE),
        (S32) DATA_ACCOUNT_INLINE_DATA_RATE_OPTION,
        (U8 **) g_DataAccount_Display.DialRateOption,
        (S32*) & g_DataAccount_Display.DataRate);

    BufferSize = MAX_DATA_ACCOUNT_NAME_LEN + 1;

    SetInlineItemFullScreenEdit(
        (wgui_inline_items + DATA_ACCOUNT_NAME),
        STR_ID_DTCNT_RENAME,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        (U8*) (g_DataAccount_Display.DataAccountName),
        BufferSize,
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH);
    SetInlineFullScreenEditCustomFunction(wgui_inline_items + DATA_ACCOUNT_NAME, DataAccountFullLineEditScreen);

    BufferSize = MAX_DIAL_NUM_LEN - 1;  /* 1 because + is handled by UI */
    SetInlineItemTextEdit(
        (wgui_inline_items + DATA_ACCOUNT_DIAL_NUMBER),
        (U8*) (g_DataAccount_Display.DialNumber),
        BufferSize,
        INPUT_TYPE_PHONE_NUMBER | INPUT_TYPE_PLUS_CHARACTER_HANDLING);

    BufferSize = MAX_USER_LEN + 1;

    SetInlineItemFullScreenEdit(
        (wgui_inline_items + DATA_ACCOUNT_USER_NAME),
        STR_ID_DTCNT_USER_NAME,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        (U8*) (g_DataAccount_Display.UserName),
        BufferSize,
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES);
    SetInlineFullScreenEditCustomFunction(wgui_inline_items + DATA_ACCOUNT_USER_NAME, DataAccountFullLineEditScreen);

    BufferSize = MAX_PASSWD_LEN + 1;
    SetInlineItemFullScreenEdit(
        (wgui_inline_items + DATA_ACCOUNT_PASSWD),
        STR_ID_DTCNT_PASSWD,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        (U8*) (g_DataAccount_Display.Passwd),
        BufferSize,
        INPUT_TYPE_ALPHANUMERIC_PASSWORD | INPUT_TYPE_USE_ONLY_ENGLISH_MODES);
    SetInlineFullScreenEditCustomFunction(wgui_inline_items + DATA_ACCOUNT_PASSWD, DataAccountFullLineEditScreen);

    SetInlineItemIP4(
        (wgui_inline_items + DATA_ACCOUNT_DNS_ADDRESS),
        (U8*) g_DataAccount_Display.IP1,
        (U8*) g_DataAccount_Display.IP2,
        (U8*) g_DataAccount_Display.IP3,
        (U8*) g_DataAccount_Display.IP4,
        PreparedIPAddressString);
}


/*****************************************************************************
 * FUNCTION
 *  DataAccountFullLineEditScreen
 * DESCRIPTION
 *  Data Account Name for Full Line Edit Screen.
 *  
 *  This is used to edit Data Account.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void GBSetInputboxLSKFunction(FuncPtr f);//JIASHUO ADD FOR WAP/MMS,20090213
void DataAccountFullLineEditScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);

    SetInputMethodAndDoneCaptionIcon(GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID));

//JIASHUO ADD FOR WAP/MMS,20090213
#if defined(__MMI_GB__)                                                          //add by yaosq 20061110 for leftsoft in profile
    mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
#endif
//JIASHUO ADD FOR WAP/MMS,20090213
    SetLeftSoftkeyFunction(EntryScrForInputMethodAndDone, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  PreparedIPAddressString
 * DESCRIPTION
 *  Prepared the Data Account IP Address to Show on Screen.
 * PARAMETERS
 *  string_buffer       [?]     
 *  IP1                 [?]     
 *  IP2                 [?]     
 *  IP3                 [?]     
 *  IP4                 [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void PreparedIPAddressString(U8 *string_buffer, U8 *IP1, U8 *IP2, U8 *IP3, U8 *IP4)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 Point[MAX_PART_IP_ADDRESS];
    S32 IPAddress[MAX_PART_IP_ADDRESS];
    U8 LocalBuffer[8];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    AnsiiToUnicodeString((S8*) Point, (S8*) ".");

    IPAddress[0] = (S32) gui_atoi((UI_string_type) IP1);
    IPAddress[1] = (S32) gui_atoi((UI_string_type) IP2);
    IPAddress[2] = (S32) gui_atoi((UI_string_type) IP3);
    IPAddress[3] = (S32) gui_atoi((UI_string_type) IP4);

    ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[0]);
    pfnUnicodeStrcpy((PS8) string_buffer, (PS8) LocalBuffer);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) Point);

    ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[1]);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) LocalBuffer);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) Point);

    ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[2]);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) LocalBuffer);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) Point);

    ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[3]);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) LocalBuffer);
}


/*****************************************************************************
 * FUNCTION
 *  ReturnThreeDigitString
 * DESCRIPTION
 *  This Fun Convert the integer value to String with prefix zero.
 *  
 *  Works only for three digit integer.(0-256)
 * PARAMETERS
 *  StringVal       [IN]        
 *  DigitVal        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReturnThreeDigitString(PU8 StringVal, U8 DigitVal)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 LocalBuffer[8];
    S8 StrVal[8];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (DigitVal == 0)
    {
        AnsiiToUnicodeString((PS8) StrVal, (PS8) "000");
    }
    else if (DigitVal < 10)
    {
        sprintf(LocalBuffer, "00%d", DigitVal);
        AnsiiToUnicodeString((PS8) StrVal, (PS8) LocalBuffer);
    }
    else if (DigitVal < 100)
    {
        sprintf(LocalBuffer, "0%d", DigitVal);
        AnsiiToUnicodeString((PS8) StrVal, (PS8) LocalBuffer);
    }
    else
    {
        sprintf(LocalBuffer, "%d", DigitVal);
        AnsiiToUnicodeString((PS8) StrVal, (PS8) LocalBuffer);
    }
    pfnUnicodeStrcpy((PS8) StringVal, (PS8) StrVal);
}


/*****************************************************************************
 * FUNCTION
 *  VerifyDataAcountChanges
 * DESCRIPTION
 *  Verify the Data Account fields.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
U8 VerifyDataAcountChanges(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = g_DataAccount_Display.CurrSelectItem;
    S8 UserName_UCS2[(MAX_USER_LEN + 1) * ENCODING_LENGTH];
    S8 Passwd_UCS2[(MAX_PASSWD_LEN + 1) * ENCODING_LENGTH];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    AnsiiToUnicodeString((PS8) UserName_UCS2, (PS8) g_DataAccount[i].UserName);
    AnsiiToUnicodeString((PS8) Passwd_UCS2, (PS8) g_DataAccount[i].Passwd);

    if (pfnUnicodeStrcmp((PS8) g_DataAccount[i].DataAccountName, (PS8) g_DataAccount_Display.DataAccountName))
    {
        return TRUE;
    }
    if (pfnUnicodeStrcmp((PS8) g_DataAccount[i].DialNumber, (PS8) g_DataAccount_Display.DialNumber))
    {
        return TRUE;
    }
    if (pfnUnicodeStrcmp((PS8) UserName_UCS2, (PS8) g_DataAccount_Display.UserName))
    {
        return TRUE;
    }
    if (pfnUnicodeStrcmp((PS8) Passwd_UCS2, (PS8) g_DataAccount_Display.Passwd))
    {
        return TRUE;
    }
    if (((S32) g_DataAccount[i].DialType) != g_DataAccount_Display.DialType)
    {
        return TRUE;
    }
    if (((S32) g_DataAccount[i].DataRate) != g_DataAccount_Display.DataRate)
    {
        return TRUE;
    }
    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  DataAccountSaveChanges
 * DESCRIPTION
 *  Save the Changed fields.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DataAccountSaveChanges(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = g_DataAccount_Display.CurrSelectItem;
    U8 PrevIfdcs = 0;
    U8 StrDigit[MAX_IPADDRES_LEN * ENCODING_LENGTH];
    S32 WordCount = 0;
    S8 AccountNameStr[30 * ENCODING_LENGTH];    /* Assumption:  string "Data Account " length < 30 */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    stopRequestedTone(WARNING_TONE);

    PrevIfdcs = g_DataAccount[i].iFdcs;

    memset(&g_DataAccount[i], '\0', sizeof(DataAccountNode));

    WordCount = GetInlineEditUCS2Count(&wgui_inline_items[1]);

    if (WordCount < 0)
    {
        g_DataAccount[i].iFdcs = PrevIfdcs;
    }
    else
    {
        g_DataAccount[i].iFdcs = (U8) WordCount;
    }

    SetDynaDataAccountName(AccountNameStr, g_DataAccount_Display.CurrSelectItem);

    if (pfnUnicodeStrlen((PS8) g_DataAccount_Display.DataAccountName) == 0)
    {
        pfnUnicodeStrcpy((PS8) g_DataAccount[i].DataAccountName, (PS8) AccountNameStr);
        g_DataAccount[i].iFdcs = pfnUnicodeStrlen((PS8) g_DataAccount[i].DataAccountName);
    }
    else
    {
        pfnUnicodeStrcpy((PS8) g_DataAccount[i].DataAccountName, (PS8) g_DataAccount_Display.DataAccountName);
    }

    pfnUnicodeStrcpy((PS8) g_DataAccount[i].DialNumber, (PS8) g_DataAccount_Display.DialNumber);
    UnicodeToAnsii((PS8) g_DataAccount[i].UserName, (PS8) g_DataAccount_Display.UserName);
    UnicodeToAnsii((PS8) g_DataAccount[i].Passwd, (PS8) g_DataAccount_Display.Passwd);

    UnicodeToAnsii((PS8) StrDigit, (PS8) g_DataAccount_Display.IP1);
    g_DataAccount[i].DnsAddress[0] = (U8) atoi((S8*) StrDigit);
    UnicodeToAnsii((PS8) StrDigit, (PS8) g_DataAccount_Display.IP2);
    g_DataAccount[i].DnsAddress[1] = (U8) atoi((S8*) StrDigit);
    UnicodeToAnsii((PS8) StrDigit, (PS8) g_DataAccount_Display.IP3);
    g_DataAccount[i].DnsAddress[2] = (U8) atoi((S8*) StrDigit);
    UnicodeToAnsii((PS8) StrDigit, (PS8) g_DataAccount_Display.IP4);
    g_DataAccount[i].DnsAddress[3] = (U8) atoi((S8*) StrDigit);
    g_DataAccount[i].DialType = (U8) g_DataAccount_Display.DialType;
    g_DataAccount[i].DataRate = (U8) g_DataAccount_Display.DataRate;
    g_DataAccount[i].Present = 'P';
    DTCNTSetAccountReq(i);
}


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTSave
 * DESCRIPTION
 *  Notification Screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTSave(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_GLOBAL_SAVE),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(DataAccountSaveChanges, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoToMainDataAccountScreen, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  GoToMainDataAccountScreen
 * DESCRIPTION
 *  Go back 2 history screens to Data Account Main
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToMainDataAccountScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    stopRequestedTone(WARNING_TONE);
    GoBacknHistory(1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowDataAccountOKMessage
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ShowDataAccountOKMessage(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, DATA_ACCOUNT_NOTIFY_TIMEOUT, SUCCESS_TONE);
    /* micha0427 */
    DeleteScreenIfPresent(SCR_ID_DTCNT_EDIT);
    DeleteScreenIfPresent(SCR_ID_DTCNT_POPUP);
#ifdef __MMI_GPRS_FEATURES__
    DeleteScreenIfPresent(SCR_ID_DTCNT_GPRS_EDIT);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  DTCNTSetAccountReq
 * DESCRIPTION
 
 * PARAMETERS
 *  ItemNum     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTSetAccountReq(U8 ItemNum)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mmi_cc_set_csd_profile_req_struct *myMsgPtr;

    /* micha0820 */
    U8 ref_count;
    U16 msg_len;

    MYQUEUE Message;
    U8 DataAccountName_ASCII[(MAX_DATA_ACCOUNT_NAME_LEN + 1) * ENCODING_LENGTH];
    U8 pbNumber_ASCII[(MAX_DIAL_NUM_LEN + 1) * ENCODING_LENGTH];
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ItemNum >= MAX_DATA_ACCOUNT_GSM_LIMIT)
    {
        return;
    }

    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTSetAccountReq"));

    memset(DataAccountName_ASCII, '\0', (MAX_DATA_ACCOUNT_NAME_LEN + 1) * ENCODING_LENGTH);
    memset(pbNumber_ASCII, '\0', (MAX_DIAL_NUM_LEN + 1) * ENCODING_LENGTH);

    myMsgPtr = (mmi_cc_set_csd_profile_req_struct*) OslConstructDataPtr(sizeof(mmi_cc_set_csd_profile_req_struct));

    /* micha0820 */
    ref_count = myMsgPtr->ref_count;
    msg_len = myMsgPtr->msg_len;
    memset(myMsgPtr, '\0', sizeof(mmi_cc_set_csd_profile_req_struct));
    myMsgPtr->ref_count = ref_count;
    myMsgPtr->msg_len = msg_len;

    myMsgPtr->csd_module = 0;
    myMsgPtr->auth_type = 0;
    myMsgPtr->prof_id = ItemNum;

    if (g_DataAccount[ItemNum].DialType == DIAL_TYPE_ANALOGUE)
    {
        myMsgPtr->type = (U8) CSMCC_ITC_3_1_K_AUDIO;
    }
    else
    {
        myMsgPtr->type = (U8) CSMCC_ITC_UDI;
    }

    if (g_DataAccount[ItemNum].DataRate == DATA_RATE_FOUR_EIGHT)
    {
        myMsgPtr->rate = (U8) CSD_PROF_RATE_4800;
    }
    else if (g_DataAccount[ItemNum].DataRate == DATA_RATE_NINE_SIX)
    {
        myMsgPtr->rate = (U8) CSD_PROF_RATE_9600;
    }
    else if (g_DataAccount[ItemNum].DataRate == DATA_RATE_FOURTEEN_FOUR)
    {
        myMsgPtr->rate = (U8) CSD_PROF_RATE_14400;
    }

    for (i = 0; i < 4; i++)
    {
        myMsgPtr->dns[i] = (U8) g_DataAccount[ItemNum].DnsAddress[i];
    }

    if (g_DataAccount[ItemNum].iFdcs > 0)
    {
        g_DataAccount[ItemNum].iFdcs = pfnUnicodeStrlen((PS8) g_DataAccount[ItemNum].DataAccountName) * ENCODING_LENGTH;
        myMsgPtr->name.name_length = g_DataAccount[ItemNum].iFdcs;
        myMsgPtr->name.name_dcs = DATA_ACCOUNT_UCS; /* 0x8 */
        i = 0;
        while (i != g_DataAccount[ItemNum].iFdcs)
        {
            myMsgPtr->name.name[i] = g_DataAccount[ItemNum].DataAccountName[i + 1];
            myMsgPtr->name.name[i + 1] = g_DataAccount[ItemNum].DataAccountName[i];
            i += 2;
        }
        myMsgPtr->name.name[i] = '\0';
        myMsgPtr->name.name[i + 1] = '\0';
    }
    else
    {
        myMsgPtr->name.name_dcs = DATA_ACCOUNT_ASCII;
        UnicodeToAnsii((PS8) DataAccountName_ASCII, (PS8) g_DataAccount[ItemNum].DataAccountName);
        myMsgPtr->name.name_length = strlen((PS8) DataAccountName_ASCII);
        memcpy(myMsgPtr->name.name, DataAccountName_ASCII, strlen((PS8) DataAccountName_ASCII) /* +1 */ );
        g_DataAccount[ItemNum].iFdcs = 0;
    }

    memcpy(myMsgPtr->user_id, g_DataAccount[ItemNum].UserName, strlen((PS8) g_DataAccount[ItemNum].UserName) + 1);

    memcpy(myMsgPtr->passwd, g_DataAccount[ItemNum].Passwd, strlen((PS8) g_DataAccount[ItemNum].Passwd) + 1);

    if (g_DataAccount[ItemNum].DialNumber[0] == '+')
    {
        UnicodeToAnsii((PS8) pbNumber_ASCII, (PS8) g_DataAccount[ItemNum].DialNumber + ENCODING_LENGTH);
        myMsgPtr->dest.type = MMI_CSMCC_INTERNATIONAL_ADDR; /* INTERNATIONAL */
        myMsgPtr->dest.length = strlen((PS8) pbNumber_ASCII) + 1;
        memcpy(myMsgPtr->dest.number, pbNumber_ASCII, strlen((PS8) pbNumber_ASCII) + 1);
    }
    else
    {
        UnicodeToAnsii((PS8) pbNumber_ASCII, (PS8) g_DataAccount[ItemNum].DialNumber);
        myMsgPtr->dest.type = MMI_CSMCC_DEFAULT_ADDR_TYPE;  /* DEFAULT */
        myMsgPtr->dest.length = strlen((PS8) pbNumber_ASCII) + 1;
        memcpy(myMsgPtr->dest.number, pbNumber_ASCII, strlen((PS8) pbNumber_ASCII) + 1);

    }

    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId = PRT_MMI_CC_SET_CSD_PROFILE_REQ;
    Message.oslDataPtr = (oslParaType*) myMsgPtr;
    Message.oslPeerBuffPtr = NULL;
    SetProtocolEventHandler(DTCNTSetAccountRsp, PRT_MMI_CC_SET_CSD_PROFILE_RSP);
    OslMsgSendExtQueue(&Message);
    EntryDTCNTProgress();
}


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTProgress
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTProgress(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_DTCNT_POPUP, NULL, EntryDTCNTProgress, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_POPUP);

    ShowCategory66Screen
        (STR_ID_DTCNT_CAPTION, GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID), 0, 0, 0, 0,
         (PU8) GetString(STR_GLOBAL_SAVING), IMG_GLOBAL_SAVE, NULL);
}


/*****************************************************************************
 * FUNCTION
 *  DTCNTSetAccountRsp
 * DESCRIPTION
 *  Handler when set data account complete
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTSetAccountRsp(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_cc_set_csd_profile_rsp_struct *localPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    localPtr = (mmi_cc_set_csd_profile_rsp_struct*) info;

    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTSetAccountRsp"));

    if (localPtr->result == KAL_TRUE /* OK */ )
    {
        g_DataAccount[g_DataAccount_Display.CurrSelectItem].Present = 'P';
        ShowDataAccountOKMessage();
    }
    else
    {
        DisplayPopup(
            (PU8) GetString(STR_GLOBAL_NOT_DONE),
            IMG_GLOBAL_UNFINISHED,
            1,
            DATA_ACCOUNT_NOTIFY_TIMEOUT,
            ERROR_TONE);
        /* micha0427 */
        DeleteScreenIfPresent(SCR_ID_DTCNT_EDIT);
        DeleteScreenIfPresent(SCR_ID_DTCNT_POPUP);
    }

}


/*****************************************************************************
 * FUNCTION
 *  DTCNTGetAccountReq
 * DESCRIPTION
 
 * PARAMETERS
 *  ItemNum     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTGetAccountReq(U8 ItemNum)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mmi_cc_get_csd_profile_req_struct *myMsgPtr;
    MYQUEUE Message;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGetAccountReq,response id: %d",PRT_MMI_CC_GET_CSD_PROFILE_RSP));

    myMsgPtr = (mmi_cc_get_csd_profile_req_struct*) OslConstructDataPtr(sizeof(mmi_cc_get_csd_profile_req_struct));
    myMsgPtr->prof_id = ItemNum;

    SetProtocolEventHandler(DTCNTGetAccountRsp, PRT_MMI_CC_GET_CSD_PROFILE_RSP);
    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId = PRT_MMI_CC_GET_CSD_PROFILE_REQ;
    Message.oslDataPtr = (oslParaType*) myMsgPtr;
    Message.oslPeerBuffPtr = NULL;
    OslMsgSendExtQueue(&Message);
}


/*****************************************************************************
 * FUNCTION
 *  DTCNTGetAccountRsp
 * DESCRIPTION
 *  Handler when get data account complete
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTGetAccountRsp(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_cc_get_csd_profile_rsp_struct *localPtr;
    U8 ItemNum = 0, i = 0;
    S8 AccountNameStr[30 * ENCODING_LENGTH];    /* Assumption:  string "Data Account " length < 30 */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGetAccountRsp"));
    localPtr = (mmi_cc_get_csd_profile_rsp_struct*) info;
    ItemNum = g_DataAccount_Display.CurrGetIndex;
    memset(&g_DataAccount[ItemNum], '\0', sizeof(DataAccountNode));
    g_DataAccount[ItemNum].Present = 'P';

    if (localPtr->type == (U8) CSMCC_ITC_3_1_K_AUDIO)
    {
        g_DataAccount[ItemNum].DialType = DIAL_TYPE_ANALOGUE;
    }
    else
    {
        g_DataAccount[ItemNum].DialType = DIAL_TYPE_ISDN;
    }

    if (localPtr->rate == (U8) CSD_PROF_RATE_4800)
    {
        g_DataAccount[ItemNum].DataRate = DATA_RATE_FOUR_EIGHT;
    }
    else if (localPtr->rate == (U8) CSD_PROF_RATE_9600)
    {
        g_DataAccount[ItemNum].DataRate = DATA_RATE_NINE_SIX;
    }
    else if (localPtr->rate == (U8) CSD_PROF_RATE_14400)
    {
        g_DataAccount[ItemNum].DataRate = DATA_RATE_FOURTEEN_FOUR;
    }

    for (i = 0; i < 4; i++)
    {
        g_DataAccount[ItemNum].DnsAddress[i] = (U8) localPtr->dns[i];
    }

    SetDynaDataAccountName(AccountNameStr, ItemNum);

    /* micha0307 */
    if (localPtr->name.name_dcs == DATA_ACCOUNT_UCS)
    {
        i = 0;
        g_DataAccount[ItemNum].iFdcs = localPtr->name.name_length + 1;

        if (localPtr->name.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            while (i != localPtr->name.name_length)
            {
                g_DataAccount[ItemNum].DataAccountName[i] = localPtr->name.name[i + 1];
                g_DataAccount[ItemNum].DataAccountName[i + 1] = localPtr->name.name[i];
                i += 2;
            }
            g_DataAccount[ItemNum].DataAccountName[localPtr->name.name_length] = 0x00;
            g_DataAccount[ItemNum].DataAccountName[localPtr->name.name_length + 1] = 0x00;
        }
    }
    else if (localPtr->name.name_dcs == DATA_ACCOUNT_ASCII)
    {
        if (localPtr->name.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            AnsiiNToUnicodeString(
                (PS8) g_DataAccount[ItemNum].DataAccountName,
                (PS8) localPtr->name.name,
                localPtr->name.name_length);
        }
        g_DataAccount[ItemNum].iFdcs = 0;
    }

    strcpy((PS8) g_DataAccount[ItemNum].UserName, (PS8) localPtr->user_id);
    strcpy((PS8) g_DataAccount[ItemNum].Passwd, (PS8) localPtr->passwd);

    if ((localPtr->dest.type) & 0x10)
    {
        g_DataAccount[ItemNum].DialNumber[0] = '+';
        AnsiiToUnicodeString((PS8) g_DataAccount[ItemNum].DialNumber + 2, (PS8) localPtr->dest.number);
    }
    else
    {
        AnsiiToUnicodeString((PS8) g_DataAccount[ItemNum].DialNumber, (PS8) localPtr->dest.number);
    }

    if ((pfnUnicodeStrlen((PS8) g_DataAccount[ItemNum].DataAccountName) == 0) &&
        (pfnUnicodeStrlen((PS8) g_DataAccount[ItemNum].DialNumber) == 0))
    {
        g_DataAccount[ItemNum].Present = 'E';
    }

    if (g_DataAccount_Display.CurrGetIndex < MAX_DATA_ACCOUNT_GSM_LIMIT - 1)
    {
        g_DataAccount_Display.CurrGetIndex++;
        DTCNTGetAccountReq(g_DataAccount_Display.CurrGetIndex);
    }
    else
    {
        g_DataAccount_Display.CurrGetIndex = 0;

    #ifdef __MMI_GPRS_FEATURES__
        DTCNTGPRSGetAccountReq(0);
    #else /* __MMI_GPRS_FEATURES__ */ 
        ClearInputEventHandler(MMI_DEVICE_ALL);
        g_DTCNTReadyCallback();
        g_DTCNTReadyCallback = NULL;
    #endif /* __MMI_GPRS_FEATURES__ */ 
    }
}


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTMain
 * DESCRIPTION
 *  Data account main screen ( including GSM/GPPRS)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTMain(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nStrItemList[MAX_SUB_MENUS];    /* Stores the strings id of submenus returned */
    U8 *guiBuffer;                      /* Buffer holding history data */
    U16 nNumofItem;                     /* Stores no of children in the submenu */
    U16 nDispAttribute;                 /* Stores display attribue */
    U8 *u8GPRSCount[MAX_SUB_MENUS];     /* sms counters for display hint popups */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_WIN32
    PopupNoSupport();
    return;
#endif /* MMI_ON_WIN32 */ 

#ifdef __MMI_GPRS_FEATURES__
    mmi_frm_hide_menu_item(SERVICES_DATA_CONNECT_MENU_ID);
#endif
    /* Retrieve no of child of menu item to be displayed */
    nNumofItem = GetNumOfChild_Ext(SERVICES_DATA_CONNECT_MAIN_MENU_ID);

#ifndef __MMI_GPRS_FEATURES__
    if(nNumofItem == 1)
    {
        InitializeDataAccountRecords();
        return;
    }
    #else
    if(nNumofItem == 1)
    {
        InitializeGPRSDataAccountRecords();
        return;
    }
#endif

#if 0
    EntryNewScreen(SCR_ID_DTCNT_MAIN, NULL, EntryDTCNTMain, NULL);

    /* Get current screen to gui buffer  for history purposes */
    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_MAIN);
    /* Retrieve no of child of menu item to be displayed */
    nNumofItem = GetNumOfChild(SERVICES_DATA_CONNECT_MAIN_MENU_ID);
    /* Get attribute of menu to be displayed */
    nDispAttribute = GetDispAttributeOfItem(SERVICES_DATA_CONNECT_MAIN_MENU_ID);
    /* Retrieve string ids in sequence of given menu item to be displayed */
    GetSequenceStringIds(SERVICES_DATA_CONNECT_MAIN_MENU_ID, nStrItemList);
    /* Set current parent id */
    SetParentHandler(SERVICES_DATA_CONNECT_MAIN_MENU_ID);
    /* Register highlight handler to be called in menu screen */
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    /* construct hint popups for the menu items */
    ConstructHintsList(SERVICES_DATA_CONNECT_MAIN_MENU_ID, u8GPRSCount);

    /* Display Category52 Screen */
    ShowCategory52Screen(
        SERVICES_DATA_CONNECT_MAIN_STRING_ID,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (U16*) gIndexIconsImageList,
        u8GPRSCount,
        0,
        0,
        guiBuffer);

    /* Register left arrow key */
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    /* Register function with left/right softkey */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
#endif
    return;
}

#ifdef __MMI_GPRS_FEATURES__

/*****************************************************************************
 * FUNCTION
 *  InitializeGPRSDataAccountRecords
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeGPRSDataAccountRecords(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DataAccountGPRSReadyCheck(EntryDTCNTGPRSList);
}


/*****************************************************************************
 * FUNCTION
 *  DTCNTGPRSGetAccountReq
 * DESCRIPTION
 
 * PARAMETERS
 *  ItemNum     [IN]        //item id
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTGPRSGetAccountReq(U8 ItemNum)
{
#ifdef __GPRS_MODE__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mmi_ps_get_gprs_data_account_req_struct *myMsgPtr;

    MYQUEUE Message;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGPRSGetAccountReq"));

    myMsgPtr =
        (mmi_ps_get_gprs_data_account_req_struct*)
        OslConstructDataPtr(sizeof(mmi_ps_get_gprs_data_account_req_struct));

    myMsgPtr->profile_id = ItemNum + 1;

    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId = PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ;
    Message.oslDataPtr = (oslParaType*) myMsgPtr;
    Message.oslPeerBuffPtr = NULL;

    SetProtocolEventHandler(DTCNTGPRSGetAccountRsp, PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP);
    OslMsgSendExtQueue(&Message);

#endif /* __GPRS_MODE__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  DTCNTGPRSGetAccountRsp
 * DESCRIPTION
 *  Handle data account get response
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTGPRSGetAccountRsp(void *info)
{
#ifdef __GPRS_MODE__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mmi_ps_get_gprs_data_account_rsp_struct *localPtr;
    U8 ItemNum = 0, i = 0;
    S8 AccountNameStr[30 * ENCODING_LENGTH];    /* Assumption:  string "Data Account " length < 30 */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGPRSGetAccountRsp"));

    localPtr = (mmi_ps_get_gprs_data_account_rsp_struct*) info;

    ItemNum = g_DataAccount_Display.GPRSCurrGetIndex;

    memset(&g_GPRS_DataAccount[ItemNum], 0x00, sizeof(GPRSDataAccountNode));

    g_GPRS_DataAccount[ItemNum].Present = 'P';

    g_GPRS_DataAccount[ItemNum].AuthType = localPtr->gprs_account.authentication_type;

    for (i = 0; i < 4; i++)
    {
        g_GPRS_DataAccount[ItemNum].DnsAddress[i] = (U8) localPtr->gprs_account.dns[i];
    }

    SetDynaDataAccountName(AccountNameStr, ItemNum);

    /* micha0307 */
    if (localPtr->gprs_account.name_dcs == DATA_ACCOUNT_UCS)
    {
        i = 0;
        g_GPRS_DataAccount[ItemNum].iFdcs = localPtr->gprs_account.name_length + 1;

        if (localPtr->gprs_account.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            while (i != localPtr->gprs_account.name_length)
            {
                g_GPRS_DataAccount[ItemNum].DataAccountName[i] = localPtr->gprs_account.name[i + 1];
                g_GPRS_DataAccount[ItemNum].DataAccountName[i + 1] = localPtr->gprs_account.name[i];
                i += 2;
            }
            g_GPRS_DataAccount[ItemNum].DataAccountName[localPtr->gprs_account.name_length] = 0x00;
            g_GPRS_DataAccount[ItemNum].DataAccountName[localPtr->gprs_account.name_length + 1] = 0x00;
        }
    }
    else if (localPtr->gprs_account.name_dcs == DATA_ACCOUNT_ASCII)
    {
        if (localPtr->gprs_account.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            AnsiiNToUnicodeString(
                (PS8) g_GPRS_DataAccount[ItemNum].DataAccountName,
                (PS8) localPtr->gprs_account.name,
                localPtr->gprs_account.name_length);
        }
        g_GPRS_DataAccount[ItemNum].iFdcs = 0;
    }
    else
    {
        pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        g_GPRS_DataAccount[ItemNum].iFdcs = 0;
    }

    memset(g_GPRS_DataAccount[ItemNum].UserName, '\0', sizeof(g_GPRS_DataAccount[ItemNum].UserName));
    strcpy((PS8) g_GPRS_DataAccount[ItemNum].UserName, (PS8) localPtr->gprs_account.user_name);

    memset(g_GPRS_DataAccount[ItemNum].Passwd, '\0', sizeof(g_GPRS_DataAccount[ItemNum].Passwd));
    strcpy((PS8) g_GPRS_DataAccount[ItemNum].Passwd, (PS8) localPtr->gprs_account.password);
    
    AnsiiNToUnicodeString(
        (PS8) g_GPRS_DataAccount[ItemNum].APN,
        (PS8) localPtr->gprs_account.apn,
        (U32) localPtr->gprs_account.apn_length);

    if ((pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName) == 0) &&
        (pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[ItemNum].APN) == 0))
    {
        g_GPRS_DataAccount[ItemNum].Present = 'E';
    }

    if (g_DataAccount_Display.GPRSCurrGetIndex < MAX_DATA_ACCOUNT_GPRS_LIMIT - 1)
    {
        g_DataAccount_Display.GPRSCurrGetIndex++;
        DTCNTGPRSGetAccountReq(g_DataAccount_Display.GPRSCurrGetIndex);
    }
    else
    {
        g_DataAccount_Display.GPRSCurrGetIndex = 0;
        //ClearInputEventHandler(MMI_DEVICE_ALL); //wangzl: 090511 delete for bug12226
        g_DTCNTReadyCallback();
        g_DTCNTReadyCallback = NULL;
    }

#endif /* __GPRS_MODE__ */ 
}

/* micha0112 begin */


/*****************************************************************************
 * FUNCTION
 *  HighlightGPRSAppDTCNTList
 * DESCRIPTION
 *  highlight handler for DTCNTAppList
 *  
 *  Index values start on zero for first Listesd item.
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightGPRSAppDTCNTList(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_DataAccount_Display.GPRSCurrAppSelectItem = (U8) nIndex;
}


/*****************************************************************************
 * FUNCTION
 *  EntryGPRSAppDTCNTList
 * DESCRIPTION
 *  shows the main menu list
 *  
 *  This is used to shows the main menu list of data account
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryGPRSAppDTCNTList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	//	    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] EntryGPRSAppDTCNTList"));
    U8 *guiBuffer = NULL;
    U8 i = 0;
    U8 *DataAccountName[MAX_DATA_ACCOUNT_GPRS_LIMIT];
    static U8 *PopUpDescription[MAX_DATA_ACCOUNT_GPRS_LIMIT];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
    {
        /* micha0307 */
        DataAccountName[i] = g_GPRS_DataAccount[i].DataAccountName;
        if (pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[i].APN) > 0)
        {
            PopUpDescription[i] = g_GPRS_DataAccount[i].APN;
        }
        else
        {
            PopUpDescription[i] = (U8*) GetString(STR_GLOBAL_EMPTY_LIST);
        }
    }

    EntryNewScreen(SCR_ID_DTCNT_GPRS_LIST, NULL, EntryGPRSAppDTCNTList, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_GPRS_LIST);

    RegisterHighlightHandler(HighlightGPRSAppDTCNTList);

    ShowCategory53Screen(
        STR_ID_DTCNT_CAPTION,
        (g_DataAccount_Display.AppMenuID ? GetRootTitleIcon(g_DataAccount_Display.AppMenuID) : 0),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        MAX_DATA_ACCOUNT_GPRS_LIMIT,
        (U8 **) DataAccountName,
        (PU16) gIndexIconsImageList,
        PopUpDescription,
        0,
        g_DataAccount_Display.GPRSCurrAppSelectItem,
        guiBuffer);

    /* Register function for left soft key */
    SetLeftSoftkeyFunction(GPRSAppDTCNTSelectOK, KEY_EVENT_UP);
    SetKeyHandler(GPRSAppDTCNTSelectOK, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  GPRSAppDTCNTSelectOK
 * DESCRIPTION
 *  highlight handler for DTCNTAppList
 *  
 *  Index values start on zero for first Listesd item.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GPRSAppDTCNTSelectOK(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
		    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] GPRSAppDTCNTSelectOK"));
#ifndef MMI_ON_WIN32
    if (g_DataAccount_Display.app_callback)
    {
        kal_uint8 id;

        abm_bearer2nw_id(ABM_E_SOCKET_BEARER_GSM_GPRS, g_DataAccount_Display.GPRSCurrAppSelectItem + 1, &id);
        g_DataAccount_Display.app_callback((U16) id);
        g_DataAccount_Display.app_callback = NULL; 
    }
#endif /* MMI_ON_WIN32 */ 
}

/* micha0112 end */


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTGPRSList
 * DESCRIPTION
 *  Main screen for GPRS data account
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTGPRSList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    U8 i = 0;
    U8 *DataAccountName[MAX_DATA_ACCOUNT_GPRS_LIMIT];
    static U8 *PopUpDescription[MAX_DATA_ACCOUNT_GPRS_LIMIT];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
    {
        /* micha0307 */
        DataAccountName[i] = g_GPRS_DataAccount[i].DataAccountName;
        if (pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[i].APN) > 0)
        {
            PopUpDescription[i] = g_GPRS_DataAccount[i].APN;
        }
        else
        {
            PopUpDescription[i] = (U8*) GetString(STR_GLOBAL_EMPTY_LIST);
        }
    }

    EntryNewScreen(SCR_ID_DTCNT_GPRS_LIST, NULL, InitializeGPRSDataAccountRecords, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_GPRS_LIST);

    SetParentHandler(SERVICES_DATA_CONNECT_GPRS_MENU_ID);

    RegisterHighlightHandler(HighlightDTCNTGPRSList);

    ShowCategory53Screen(
        STR_ID_DTCNT_CAPTION,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        STR_GLOBAL_EDIT,
        IMG_ID_DTCNT_LSK_EDIT,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        MAX_DATA_ACCOUNT_GPRS_LIMIT,
        (U8 **) DataAccountName,
        (PU16) gIndexIconsImageList,
        PopUpDescription,
        0,
        g_DataAccount_Display.GPRSCurrSelectItem,
        NULL);


    /* Register function for left soft key */
    SetLeftSoftkeyFunction(InitInLineGPRSDataAccount, KEY_EVENT_UP);
    SetKeyHandler(InitInLineGPRSDataAccount, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    SetDelScrnIDCallbackHandler(SCR_ID_DTCNT_GPRS_LIST, mmi_dtcnt_exit_list_hdlr);
}


/*****************************************************************************
 * FUNCTION
 *  HighlightDTCNTGPRSList
 * DESCRIPTION
 *  get current GPRS menu item index
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightDTCNTGPRSList(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_DataAccount_Display.GPRSCurrSelectItem = (U8) nIndex;
}


/*****************************************************************************
 * FUNCTION
 *  InitInLineGPRSDataAccount
 * DESCRIPTION
 *  Set account data to display
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitInLineGPRSDataAccount(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U8 i = g_DataAccount_Display.GPRSCurrSelectItem;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pfnUnicodeStrcpy((PS8) g_DataAccount_Display.DataAccountName, (PS8) g_GPRS_DataAccount[i].DataAccountName);
    pfnUnicodeStrcpy((PS8) g_DataAccount_Display.APN, (PS8) g_GPRS_DataAccount[i].APN);
    AnsiiToUnicodeString((PS8) g_DataAccount_Display.UserName, (PS8) g_GPRS_DataAccount[i].UserName);
    AnsiiToUnicodeString((PS8) g_DataAccount_Display.Passwd, (PS8) g_GPRS_DataAccount[i].Passwd);

    ReturnThreeDigitString(g_DataAccount_Display.IP1, g_GPRS_DataAccount[i].DnsAddress[0]);
    ReturnThreeDigitString(g_DataAccount_Display.IP2, g_GPRS_DataAccount[i].DnsAddress[1]);
    ReturnThreeDigitString(g_DataAccount_Display.IP3, g_GPRS_DataAccount[i].DnsAddress[2]);
    ReturnThreeDigitString(g_DataAccount_Display.IP4, g_GPRS_DataAccount[i].DnsAddress[3]);

    g_DataAccount_Display.AuthType = (S32) g_GPRS_DataAccount[i].AuthType;

    g_DataAccount_Display.AuthTypeOption[0] = (U8*) GetString(STR_ID_DTCNT_GPRS_PAP_AUTH);
    g_DataAccount_Display.AuthTypeOption[1] = (U8*) GetString(STR_ID_DTCNT_GPRS_CHAP_AUTH);

    EntryDTCNTGPRSEdit();
}


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTGPRSEdit
 * DESCRIPTION
 *  GPRS account editing screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTGPRSEdit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *inputBuffer;    /* added for inline edit history */
    U8 *guiBuffer;      /* Buffer holding history data */
    U16 inputBufferSize;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_DTCNT_EDIT, ExitDTCNTGPRSEdit, NULL, NULL);

    InitializeCategory57Screen();

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_GPRS_EDIT);
    inputBuffer = GetCurrNInputBuffer(SCR_ID_DTCNT_GPRS_EDIT, &inputBufferSize);        /* added for inline edit history */

    SetParentHandler(MITEM_DATA_ACCOUNT_PARENT_ZERO);
    GPRSDataAccountFillInlineStruct();

    if (inputBuffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, MAX_INLINE_ITEM_GPRSDATA_ACCOUNT_LIST, inputBuffer);
    }

    if (VerifyGPRSDataAcountChanges() && GetInlineDoneFlag(guiBuffer) == 0)
    {
        SetInlineDoneFlag(guiBuffer);
    }

    ShowCategory57Screen(
        STR_GLOBAL_EDIT,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        STR_GLOBAL_EDIT,
        IMG_ID_DTCNT_LSK_EDIT,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        MAX_INLINE_ITEM_GPRSDATA_ACCOUNT_LIST,
        g_DataAccount_Display.IconList,
        wgui_inline_items,
        DATA_ACCOUNT_HILITED_ITEM,
        guiBuffer);

    SetCategory57RightSoftkeyFunctions(EntryDTCNTGPRSSave, GoBackHistory);

}


/*****************************************************************************
 * FUNCTION
 *  ExitDTCNTGPRSEdit
 * DESCRIPTION
 *  exit handler for GPRS account edit screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitDTCNTGPRSEdit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history_t h;
    U16 inputBufferSize;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();
    h.scrnID = SCR_ID_DTCNT_GPRS_EDIT;
    h.entryFuncPtr = EntryDTCNTGPRSEdit;
    GetCategoryHistory(h.guiBuffer);
    inputBufferSize = (U16) GetCategory57DataSize();
    GetCategory57Data((U8*) h.inputBuffer);
    AddNHistory(h, inputBufferSize);

}


/*****************************************************************************
 * FUNCTION
 *  GPRSDataAccountFillInlineStruct
 * DESCRIPTION
 *  fill inline structure for GPRS edit screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GPRSDataAccountFillInlineStruct(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 BufferSize = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetInlineItemActivation((wgui_inline_items + GPRS_DATA_ACCOUNT_NAME), KEY_LSK, KEY_EVENT_UP);
    SetInlineItemActivation((wgui_inline_items + GPRS_DATA_ACCOUNT_APN), KEY_LSK, KEY_EVENT_UP);
    SetInlineItemActivation((wgui_inline_items + GPRS_DATA_ACCOUNT_USER_NAME), KEY_LSK, KEY_EVENT_UP);
    SetInlineItemActivation((wgui_inline_items + GPRS_DATA_ACCOUNT_PASSWD), KEY_LSK, KEY_EVENT_UP);
    SetInlineItemActivation(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_AUTH_TYPE),
        INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
        0);
    //DNS menu is currently disabled
    //      SetInlineItemActivation((wgui_inline_items+GPRS_DATA_ACCOUNT_DNS_ADDRESS),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);

    SetInlineItemCaption((wgui_inline_items + GPRS_DATA_ACCOUNT_NAME_STR), (U8*) GetString(STR_ID_DTCNT_RENAME));
    SetInlineItemCaption((wgui_inline_items + GPRS_DATA_ACCOUNT_APN_STR), (U8*) GetString(STR_ID_DTCNT_GPRS_APN));
    SetInlineItemCaption(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_USER_NAME_STR),
        (U8*) GetString(STR_ID_DTCNT_USER_NAME));
    SetInlineItemCaption((wgui_inline_items + GPRS_DATA_ACCOUNT_PASSWD_STR), (U8*) GetString(STR_ID_DTCNT_PASSWD));
    SetInlineItemCaption(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_AUTH_TYPE_STR),
        (U8*) GetString(STR_ID_DTCNT_GPRS_AUTH_TYP));
    /* SetInlineItemCaption((wgui_inline_items+GPRS_DATA_ACCOUNT_DNS_ADDRESS_STR),(U8*)GetString(STR_ID_DTCNT_DNS_ADDRESS)); */

    SetInlineItemSelect(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_AUTH_TYPE),
        (S32) GPRS_DATA_ACCOUNT_INLINE_AUTH_TYPE_OPTION,
        (U8 **) g_DataAccount_Display.AuthTypeOption,
        (S32*) & g_DataAccount_Display.AuthType);

    BufferSize = MAX_GPRS_DATAACCOUNT_NAME + 1;

    SetInlineItemFullScreenEdit(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_NAME),
        STR_ID_DTCNT_RENAME,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        (U8*) (g_DataAccount_Display.DataAccountName),
        BufferSize,
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH);
    SetInlineFullScreenEditCustomFunction(wgui_inline_items + GPRS_DATA_ACCOUNT_NAME, DataAccountFullLineEditScreen);

    /* micha0202 */
    BufferSize = MAX_GPRS_MMI_APN_LEN + 1;

    SetInlineItemFullScreenEdit(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_APN),
        STR_ID_DTCNT_GPRS_APN,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        (U8*) (g_DataAccount_Display.APN),
        BufferSize,
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES);
    SetInlineFullScreenEditCustomFunction(wgui_inline_items + GPRS_DATA_ACCOUNT_APN, DataAccountFullLineEditScreen);

    BufferSize = MAX_GPRS_USERNAME + 1;

    SetInlineItemFullScreenEdit(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_USER_NAME),
        STR_ID_DTCNT_USER_NAME,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        (U8*) (g_DataAccount_Display.UserName),
        BufferSize,
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES);
    SetInlineFullScreenEditCustomFunction(
        wgui_inline_items + GPRS_DATA_ACCOUNT_USER_NAME,
        DataAccountFullLineEditScreen);

    BufferSize = MAX_GPRS_USERPASSWD + 1;
    SetInlineItemFullScreenEdit(
        (wgui_inline_items + GPRS_DATA_ACCOUNT_PASSWD),
        STR_ID_DTCNT_PASSWD,
        GetRootTitleIcon(SERVICES_DATA_CONNECT_MAIN_MENU_ID),
        (U8*) (g_DataAccount_Display.Passwd),
        BufferSize,
        INPUT_TYPE_ALPHANUMERIC_PASSWORD | INPUT_TYPE_USE_ONLY_ENGLISH_MODES);
    SetInlineFullScreenEditCustomFunction(wgui_inline_items + GPRS_DATA_ACCOUNT_PASSWD, DataAccountFullLineEditScreen);

    /* SetInlineItemIP4((wgui_inline_items+GPRS_DATA_ACCOUNT_DNS_ADDRESS),(U8*)g_DataAccount_Display.IP1,(U8*)g_DataAccount_Display.IP2,(U8*)g_DataAccount_Display.IP3,(U8*)g_DataAccount_Display.IP4,PreparedIPAddressString); */
}


/*****************************************************************************
 * FUNCTION
 *  VerifyGPRSDataAcountChanges
 * DESCRIPTION
 *  verify changes in GPRS editing screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
U8 VerifyGPRSDataAcountChanges(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = g_DataAccount_Display.GPRSCurrSelectItem;
    S8 UserName_UCS2[(MAX_GPRS_USERNAME + 1) * ENCODING_LENGTH];
    S8 Passwd_UCS2[(MAX_GPRS_USERPASSWD + 1) * ENCODING_LENGTH];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    AnsiiToUnicodeString((PS8) UserName_UCS2, (PS8) g_GPRS_DataAccount[i].UserName);
    AnsiiToUnicodeString((PS8) Passwd_UCS2, (PS8) g_GPRS_DataAccount[i].Passwd);

    if (pfnUnicodeStrcmp((PS8) g_GPRS_DataAccount[i].DataAccountName, (PS8) g_DataAccount_Display.DataAccountName))
    {
        return TRUE;
    }

    if (pfnUnicodeStrcmp((PS8) g_GPRS_DataAccount[i].APN, (PS8) g_DataAccount_Display.APN))
    {
        return TRUE;
    }

    if (pfnUnicodeStrcmp((PS8) UserName_UCS2, (PS8) g_DataAccount_Display.UserName))
    {
        return TRUE;
    }

    if (pfnUnicodeStrcmp((PS8) Passwd_UCS2, (PS8) g_DataAccount_Display.Passwd))
    {
        return TRUE;
    }

    if (((S32) g_GPRS_DataAccount[i].AuthType) != g_DataAccount_Display.AuthType)
    {
        return TRUE;
    }

    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  EntryDTCNTGPRSSave
 * DESCRIPTION
 *  confirm window for saving GPRS account item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryDTCNTGPRSSave(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_GLOBAL_SAVE),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(GPRSDataAccountSaveChanges, KEY_EVENT_UP);

    SetRightSoftkeyFunction(GoToMainDataAccountScreen, KEY_EVENT_UP);

}


/*****************************************************************************
 * FUNCTION
 *  GPRSDataAccountSaveChanges
 * DESCRIPTION
 *  prepare GPRS data account content to be saved
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GPRSDataAccountSaveChanges(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = g_DataAccount_Display.GPRSCurrSelectItem;
    S8 AccountNameStr[30 * ENCODING_LENGTH];    /* Assumption:  string "Data Account " length < 30 */

    //micha0901
    //DNS menu is currently disabled
#if (0)
/* under construction !*/
#endif /* (0) */ 

    /* micha1103 begin */
    U8 PrevIfdcs = 0;
    S32 WordCount = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    stopRequestedTone(WARNING_TONE);

    PrevIfdcs = g_GPRS_DataAccount[i].iFdcs;

    memset(&g_GPRS_DataAccount[i], '\0', sizeof(GPRSDataAccountNode));

    WordCount = GetInlineEditUCS2Count(&wgui_inline_items[1]);

    if (WordCount < 0)
    {
        g_GPRS_DataAccount[i].iFdcs = PrevIfdcs;
    }
    else
    {
        g_GPRS_DataAccount[i].iFdcs = (U8) WordCount;
    }

    /* micha1103 end */

    SetDynaDataAccountName(AccountNameStr, g_DataAccount_Display.GPRSCurrSelectItem);

    /* micha0307 */
    if (pfnUnicodeStrlen((PS8) g_DataAccount_Display.DataAccountName) == 0)
    {
        pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[i].DataAccountName, (PS8) AccountNameStr);
        g_GPRS_DataAccount[i].iFdcs = pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[i].DataAccountName);
    }
    else
    {
        pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[i].DataAccountName, (PS8) g_DataAccount_Display.DataAccountName);
    }

    pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[i].APN, (PS8) g_DataAccount_Display.APN);
    UnicodeToAnsii((PS8) g_GPRS_DataAccount[i].UserName, (PS8) g_DataAccount_Display.UserName);
    UnicodeToAnsii((PS8) g_GPRS_DataAccount[i].Passwd, (PS8) g_DataAccount_Display.Passwd);

    /* DNS menu is currently disabled */
#if (0)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* (0) */ 

    g_GPRS_DataAccount[i].AuthType = (U8) g_DataAccount_Display.AuthType;
    g_GPRS_DataAccount[i].Present = 'P';

    DTCNTGPRSSetAccountReq(i);

}


/*****************************************************************************
 * FUNCTION
 *  DTCNTGPRSSetAccountReq
 * DESCRIPTION
 
 * PARAMETERS
 *  ItemNum     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTGPRSSetAccountReq(U8 ItemNum)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

#ifdef __GPRS_MODE__

    mmi_ps_set_gprs_data_account_req_struct *myMsgPtr;

    /* micha0820 */
    U8 ref_count;
    U16 msg_len;

    MYQUEUE Message;
    U8 DataAccountName_ASCII[(MAX_GPRS_DATAACCOUNT_NAME + 1) * ENCODING_LENGTH];
    U8 DataAccountAPN_ASCII[MAX_APN_LEN];
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ItemNum >= MAX_DATA_ACCOUNT_GPRS_LIMIT)
    {
        return;
    }

    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGPRSSetAccountReq"));

    memset(DataAccountName_ASCII, '\0', (MAX_GPRS_DATAACCOUNT_NAME + 1) * ENCODING_LENGTH);
    memset(DataAccountAPN_ASCII, '\0', MAX_APN_LEN);

    myMsgPtr =
        (mmi_ps_set_gprs_data_account_req_struct*)
        OslConstructDataPtr(sizeof(mmi_ps_set_gprs_data_account_req_struct));

    /* micha0820 */
    ref_count = myMsgPtr->ref_count;
    msg_len = myMsgPtr->msg_len;
    memset(myMsgPtr, '\0', sizeof(mmi_ps_set_gprs_data_account_req_struct));
    myMsgPtr->ref_count = ref_count;
    myMsgPtr->msg_len = msg_len;

    myMsgPtr->gprs_account.context_id = ItemNum + 1;
    myMsgPtr->gprs_account.authentication_type = g_GPRS_DataAccount[ItemNum].AuthType;

    if (g_GPRS_DataAccount[ItemNum].iFdcs > 0)
    {
        g_GPRS_DataAccount[ItemNum].iFdcs = pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName) * ENCODING_LENGTH;
        myMsgPtr->gprs_account.name_length = g_GPRS_DataAccount[ItemNum].iFdcs;
        myMsgPtr->gprs_account.name_dcs = DATA_ACCOUNT_UCS; /* 0x8 */
        i = 0;
        while (i != g_GPRS_DataAccount[ItemNum].iFdcs)
        {
            myMsgPtr->gprs_account.name[i] = g_GPRS_DataAccount[ItemNum].DataAccountName[i + 1];
            myMsgPtr->gprs_account.name[i + 1] = g_GPRS_DataAccount[ItemNum].DataAccountName[i];
            i += 2;
        }
        myMsgPtr->gprs_account.name[i] = '\0';
        myMsgPtr->gprs_account.name[i + 1] = '\0';
    }
    else
    {
        myMsgPtr->gprs_account.name_dcs = DATA_ACCOUNT_ASCII;
        UnicodeToAnsii((PS8) DataAccountName_ASCII, (PS8) g_GPRS_DataAccount[ItemNum].DataAccountName);
        myMsgPtr->gprs_account.name_length = strlen((PS8) DataAccountName_ASCII);
        memcpy(myMsgPtr->gprs_account.name, DataAccountName_ASCII, strlen((PS8) DataAccountName_ASCII));
        g_GPRS_DataAccount[ItemNum].iFdcs = 0;
    }

    UnicodeToAnsii((PS8) DataAccountAPN_ASCII, (PS8) g_GPRS_DataAccount[ItemNum].APN);
    /* micha0202 */
    memcpy(myMsgPtr->gprs_account.apn, DataAccountAPN_ASCII, strlen((PS8) DataAccountAPN_ASCII) + 1);
    myMsgPtr->gprs_account.apn_length = strlen((PS8) DataAccountAPN_ASCII);

    memcpy(
        myMsgPtr->gprs_account.user_name,
        g_GPRS_DataAccount[ItemNum].UserName,
        strlen((PS8) g_GPRS_DataAccount[ItemNum].UserName) + 1);

    memcpy(
        myMsgPtr->gprs_account.password,
        g_GPRS_DataAccount[ItemNum].Passwd,
        strlen((PS8) g_GPRS_DataAccount[ItemNum].Passwd) + 1);

    for (i = 0; i < 4; i++)
    {
        myMsgPtr->gprs_account.dns[i] = 0;
    }

    ClearInputEventHandler(MMI_DEVICE_ALL);
    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId = PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ;
    Message.oslDataPtr = (oslParaType*) myMsgPtr;
    Message.oslPeerBuffPtr = NULL;
    SetProtocolEventHandler(DTCNTGPRSSetAccountRsp, PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP);
    OslMsgSendExtQueue(&Message);
    EntryDTCNTProgress();

#endif /* __GPRS_MODE__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  DTCNTGPRSSetAccountRsp
 * DESCRIPTION
 *  message handler for mmi_ps_set_gprs_data_account_rsp_struct
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DTCNTGPRSSetAccountRsp(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

#ifdef __GPRS_MODE__

    mmi_ps_set_gprs_data_account_rsp_struct *localPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    localPtr = (mmi_ps_set_gprs_data_account_rsp_struct*) info;

    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGPRSSetAccountReq"));

    if (localPtr->result == KAL_TRUE /* OK */ )
    {
        g_GPRS_DataAccount[g_DataAccount_Display.GPRSCurrSelectItem].Present = 'P';
        ShowDataAccountOKMessage();
    }
    else
    {
        DisplayPopup(
            (PU8) GetString(STR_GLOBAL_NOT_DONE),
            IMG_GLOBAL_UNFINISHED,
            1,
            DATA_ACCOUNT_NOTIFY_TIMEOUT,
            ERROR_TONE);
        /* micha0427 */
        DeleteScreenIfPresent(SCR_ID_DTCNT_GPRS_EDIT);
        DeleteScreenIfPresent(SCR_ID_DTCNT_POPUP);
    }

#endif /* __GPRS_MODE__ */ 
}


//add for QQ APN setting by wuys 2010-08-26
#ifdef  __MMI_SUPPORT_JME__

U32 DataAccountReadyCheck_QQApn(void);
void DTCNTGetAccountReq_QQApn(U8 ItemNum);
void DTCNTGPRSGetAccountReq_QQApn(U8 ItemNum);
void jmetocSetApn(const char* apn);
void jmetocSetPwd(const char* pwd);
void jmetocSetUser(const char* user);
void VM_writeApnNum(U8 simNum);

UINT32 CFW_SetJmetocApn(const INT8 *apn);
//UINT32 CFW_GetJmetocApn(INT8 * apn);
UINT32 CFW_SetJmetocUser(const INT8 *user);
//UINT32 CFW_GetJmetocUser(INT8 * user);
UINT32 CFW_SetJmetocPwd(const INT8 *pwd);
//UINT32 CFW_GetJmetocPwd(INT8 * pwd);
void GPRSAppDTCNTSelectOK_QQApn(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	//	PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] GPRSAppDTCNTSelectOK_QQApn"));
	//	PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] GPRSAppDTCNTSelectOK_QQApn,index: %d;APN:%s",	g_DataAccount_Display.GPRSCurrAppSelectItem,g_GPRS_DataAccount[g_DataAccount_Display.GPRSCurrAppSelectItem].APN));
	#ifdef MMI_ON_HARDWARE_P

		S8 tmpstr[100] = {0x00,};

              VM_writeApnNum(g_DataAccount_Display.GPRSCurrAppSelectItem);
		UnicodeToAnsii(tmpstr,g_GPRS_DataAccount[g_DataAccount_Display.GPRSCurrAppSelectItem].APN);
             jmetocSetApn(tmpstr);
		CFW_SetJmetocApn(tmpstr);
		
		memset(tmpstr,0x00,100);
		strcpy(tmpstr,g_GPRS_DataAccount[g_DataAccount_Display.GPRSCurrAppSelectItem].UserName);
		jmetocSetUser(tmpstr);
		CFW_SetJmetocUser(tmpstr);

		memset(tmpstr,0x00,100);
		strcpy(tmpstr,g_GPRS_DataAccount[g_DataAccount_Display.GPRSCurrAppSelectItem].Passwd);
		jmetocSetPwd(tmpstr);
		CFW_SetJmetocPwd(tmpstr);
		 PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] GPRSAppDTCNTSelectOK_QQApn,clean APN:%s",tmpstr));

		//CFW_GetJmetocApn(tmpstr);
		//PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] GPRSAppDTCNTSelectOK_QQApn,read reg APN:%s",tmpstr));

            GoBackHistory();
#endif
		
}
extern UINT8 VM_readApnNum(void);
void EntryGPRSAppDTCNTList_QQApn(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	//	    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] EntryGPRSAppDTCNTList_QQApn"));
    U8 *guiBuffer = NULL;
    U8 i = 0;
    U8 *DataAccountName[MAX_DATA_ACCOUNT_GPRS_LIMIT];
    static U8 *PopUpDescription[MAX_DATA_ACCOUNT_GPRS_LIMIT];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
    {
        /* micha0307 */
        DataAccountName[i] = g_GPRS_DataAccount[i].DataAccountName;
        if (pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[i].APN) > 0)
        {
            PopUpDescription[i] = g_GPRS_DataAccount[i].APN;
        }
        else
        {
            PopUpDescription[i] = (U8*) GetString(STR_GLOBAL_EMPTY_LIST);
        }
    }

    EntryNewScreen(SCR_ID_DTCNT_GPRS_LIST, NULL, EntryGPRSAppDTCNTList_QQApn, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_DTCNT_GPRS_LIST);

    RegisterHighlightHandler(HighlightGPRSAppDTCNTList);
 #ifdef MMI_ON_HARDWARE_P
    g_DataAccount_Display.GPRSCurrAppSelectItem = VM_readApnNum();
#endif
    ShowCategory53Screen(
        STR_ID_DTCNT_CAPTION,
        (g_DataAccount_Display.AppMenuID ? GetRootTitleIcon(g_DataAccount_Display.AppMenuID) : 0),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        MAX_DATA_ACCOUNT_GPRS_LIMIT,
        (U8 **) DataAccountName,
        (PU16) gIndexIconsImageList,
        PopUpDescription,
        0,
        g_DataAccount_Display.GPRSCurrAppSelectItem,
        guiBuffer);

    /* Register function for left soft key */
    SetLeftSoftkeyFunction(GPRSAppDTCNTSelectOK_QQApn, KEY_EVENT_UP);
    SetKeyHandler(GPRSAppDTCNTSelectOK_QQApn, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}
void DTCNTGPRSGetAccountRsp_QQApn(void *info)
{
#ifdef __GPRS_MODE__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mmi_ps_get_gprs_data_account_rsp_struct *localPtr;
    U8 ItemNum = 0, i = 0;
    S8 AccountNameStr[30 * ENCODING_LENGTH];    /* Assumption:  string "Data Account " length < 30 */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGPRSGetAccountRsp_QQApn"));

    localPtr = (mmi_ps_get_gprs_data_account_rsp_struct*) info;

    ItemNum = g_DataAccount_Display.GPRSCurrGetIndex;

    memset(&g_GPRS_DataAccount[ItemNum], 0x00, sizeof(GPRSDataAccountNode));

    g_GPRS_DataAccount[ItemNum].Present = 'P';

    g_GPRS_DataAccount[ItemNum].AuthType = localPtr->gprs_account.authentication_type;

    for (i = 0; i < 4; i++)
    {
        g_GPRS_DataAccount[ItemNum].DnsAddress[i] = (U8) localPtr->gprs_account.dns[i];
    }

    SetDynaDataAccountName(AccountNameStr, ItemNum);

    /* micha0307 */
    if (localPtr->gprs_account.name_dcs == DATA_ACCOUNT_UCS)
    {
        i = 0;
        g_GPRS_DataAccount[ItemNum].iFdcs = localPtr->gprs_account.name_length + 1;

        if (localPtr->gprs_account.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            while (i != localPtr->gprs_account.name_length)
            {
                g_GPRS_DataAccount[ItemNum].DataAccountName[i] = localPtr->gprs_account.name[i + 1];
                g_GPRS_DataAccount[ItemNum].DataAccountName[i + 1] = localPtr->gprs_account.name[i];
                i += 2;
            }
            g_GPRS_DataAccount[ItemNum].DataAccountName[localPtr->gprs_account.name_length] = 0x00;
            g_GPRS_DataAccount[ItemNum].DataAccountName[localPtr->gprs_account.name_length + 1] = 0x00;
        }
    }
    else if (localPtr->gprs_account.name_dcs == DATA_ACCOUNT_ASCII)
    {
        if (localPtr->gprs_account.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            AnsiiNToUnicodeString(
                (PS8) g_GPRS_DataAccount[ItemNum].DataAccountName,
                (PS8) localPtr->gprs_account.name,
                localPtr->gprs_account.name_length);
        }
        g_GPRS_DataAccount[ItemNum].iFdcs = 0;
    }
    else
    {
        pfnUnicodeStrcpy((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        g_GPRS_DataAccount[ItemNum].iFdcs = 0;
    }

    memset(g_GPRS_DataAccount[ItemNum].UserName, '\0', sizeof(g_GPRS_DataAccount[ItemNum].UserName));
    strcpy((PS8) g_GPRS_DataAccount[ItemNum].UserName, (PS8) localPtr->gprs_account.user_name);

    memset(g_GPRS_DataAccount[ItemNum].Passwd, '\0', sizeof(g_GPRS_DataAccount[ItemNum].Passwd));
    strcpy((PS8) g_GPRS_DataAccount[ItemNum].Passwd, (PS8) localPtr->gprs_account.password);
    
   // strcpy((PS8) g_GPRS_DataAccount[ItemNum].APN,(PS8) localPtr->gprs_account.apn);
	    AnsiiNToUnicodeString(
        (PS8) g_GPRS_DataAccount[ItemNum].APN,
        (PS8) localPtr->gprs_account.apn,
        (U32) localPtr->gprs_account.apn_length);


    if ((pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[ItemNum].DataAccountName) == 0) &&
        (pfnUnicodeStrlen((PS8) g_GPRS_DataAccount[ItemNum].APN) == 0))
    {
        g_GPRS_DataAccount[ItemNum].Present = 'E';
    }

    if (g_DataAccount_Display.GPRSCurrGetIndex < MAX_DATA_ACCOUNT_GPRS_LIMIT - 1)
    {
        g_DataAccount_Display.GPRSCurrGetIndex++;
        DTCNTGPRSGetAccountReq_QQApn(g_DataAccount_Display.GPRSCurrGetIndex);
    }
    else
    {
        g_DataAccount_Display.GPRSCurrGetIndex = 0;
		EntryGPRSAppDTCNTList_QQApn();
        //ClearInputEventHandler(MMI_DEVICE_ALL); //wangzl: 090511 delete for bug12226
       // g_DTCNTReadyCallback();
      //  g_DTCNTReadyCallback = NULL;
    }

#endif /* __GPRS_MODE__ */ 
}
void DTCNTGPRSGetAccountReq_QQApn(U8 ItemNum)
{
#ifdef __GPRS_MODE__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mmi_ps_get_gprs_data_account_req_struct *myMsgPtr;

    MYQUEUE Message;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGPRSGetAccountReq_QQApn"));

    myMsgPtr =
        (mmi_ps_get_gprs_data_account_req_struct*)
        OslConstructDataPtr(sizeof(mmi_ps_get_gprs_data_account_req_struct));

    myMsgPtr->profile_id = ItemNum + 1;

    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId = PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ;
    Message.oslDataPtr = (oslParaType*) myMsgPtr;
    Message.oslPeerBuffPtr = NULL;

    SetProtocolEventHandler(DTCNTGPRSGetAccountRsp_QQApn, PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP);
    OslMsgSendExtQueue(&Message);

#endif /* __GPRS_MODE__ */ 

}

void DTCNTGetAccountRsp_QQApn(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_cc_get_csd_profile_rsp_struct *localPtr;
    U8 ItemNum = 0, i = 0;
    S8 AccountNameStr[30 * ENCODING_LENGTH];    /* Assumption:  string "Data Account " length < 30 */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGetAccountRsp_QQApn"));
    localPtr = (mmi_cc_get_csd_profile_rsp_struct*) info;
    ItemNum = g_DataAccount_Display.CurrGetIndex;
    memset(&g_DataAccount[ItemNum], '\0', sizeof(DataAccountNode));
    g_DataAccount[ItemNum].Present = 'P';

    if (localPtr->type == (U8) CSMCC_ITC_3_1_K_AUDIO)
    {
        g_DataAccount[ItemNum].DialType = DIAL_TYPE_ANALOGUE;
    }
    else
    {
        g_DataAccount[ItemNum].DialType = DIAL_TYPE_ISDN;
    }

    if (localPtr->rate == (U8) CSD_PROF_RATE_4800)
    {
        g_DataAccount[ItemNum].DataRate = DATA_RATE_FOUR_EIGHT;
    }
    else if (localPtr->rate == (U8) CSD_PROF_RATE_9600)
    {
        g_DataAccount[ItemNum].DataRate = DATA_RATE_NINE_SIX;
    }
    else if (localPtr->rate == (U8) CSD_PROF_RATE_14400)
    {
        g_DataAccount[ItemNum].DataRate = DATA_RATE_FOURTEEN_FOUR;
    }

    for (i = 0; i < 4; i++)
    {
        g_DataAccount[ItemNum].DnsAddress[i] = (U8) localPtr->dns[i];
    }

    SetDynaDataAccountName(AccountNameStr, ItemNum);

    /* micha0307 */
    if (localPtr->name.name_dcs == DATA_ACCOUNT_UCS)
    {
        i = 0;
        g_DataAccount[ItemNum].iFdcs = localPtr->name.name_length + 1;

        if (localPtr->name.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            while (i != localPtr->name.name_length)
            {
                g_DataAccount[ItemNum].DataAccountName[i] = localPtr->name.name[i + 1];
                g_DataAccount[ItemNum].DataAccountName[i + 1] = localPtr->name.name[i];
                i += 2;
            }
            g_DataAccount[ItemNum].DataAccountName[localPtr->name.name_length] = 0x00;
            g_DataAccount[ItemNum].DataAccountName[localPtr->name.name_length + 1] = 0x00;
        }
    }
    else if (localPtr->name.name_dcs == DATA_ACCOUNT_ASCII)
    {
        if (localPtr->name.name_length == 0)
        {
            pfnUnicodeStrcpy((PS8) g_DataAccount[ItemNum].DataAccountName, (PS8) AccountNameStr);
        }
        else
        {
            AnsiiNToUnicodeString(
                (PS8) g_DataAccount[ItemNum].DataAccountName,
                (PS8) localPtr->name.name,
                localPtr->name.name_length);
        }
        g_DataAccount[ItemNum].iFdcs = 0;
    }

    strcpy((PS8) g_DataAccount[ItemNum].UserName, (PS8) localPtr->user_id);
    strcpy((PS8) g_DataAccount[ItemNum].Passwd, (PS8) localPtr->passwd);

    if ((localPtr->dest.type) & 0x10)
    {
        g_DataAccount[ItemNum].DialNumber[0] = '+';
        AnsiiToUnicodeString((PS8) g_DataAccount[ItemNum].DialNumber + 2, (PS8) localPtr->dest.number);
    }
    else
    {
        AnsiiToUnicodeString((PS8) g_DataAccount[ItemNum].DialNumber, (PS8) localPtr->dest.number);
    }

    if ((pfnUnicodeStrlen((PS8) g_DataAccount[ItemNum].DataAccountName) == 0) &&
        (pfnUnicodeStrlen((PS8) g_DataAccount[ItemNum].DialNumber) == 0))
    {
        g_DataAccount[ItemNum].Present = 'E';
    }

    if (g_DataAccount_Display.CurrGetIndex < MAX_DATA_ACCOUNT_GSM_LIMIT - 1)
    {
        g_DataAccount_Display.CurrGetIndex++;
        DTCNTGetAccountReq_QQApn(g_DataAccount_Display.CurrGetIndex);
    }
    else
    {
        g_DataAccount_Display.CurrGetIndex = 0;

    #ifdef __MMI_GPRS_FEATURES__
        DTCNTGPRSGetAccountReq_QQApn(0);
    #else /* __MMI_GPRS_FEATURES__ */ 
        ClearInputEventHandler(MMI_DEVICE_ALL);
       // g_DTCNTReadyCallback();
       // g_DTCNTReadyCallback = NULL;
    #endif /* __MMI_GPRS_FEATURES__ */ 
    }
}
void DTCNTGetAccountReq_QQApn(U8 ItemNum)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mmi_cc_get_csd_profile_req_struct *myMsgPtr;
    MYQUEUE Message;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DTCNTGetAccountReq_QQApn"));

    myMsgPtr = (mmi_cc_get_csd_profile_req_struct*) OslConstructDataPtr(sizeof(mmi_cc_get_csd_profile_req_struct));
    myMsgPtr->prof_id = ItemNum;

    SetProtocolEventHandler(DTCNTGetAccountRsp_QQApn, PRT_MMI_CC_GET_CSD_PROFILE_RSP);
    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId = PRT_MMI_CC_GET_CSD_PROFILE_REQ;
    Message.oslDataPtr = (oslParaType*) myMsgPtr;
    Message.oslPeerBuffPtr = NULL;
    OslMsgSendExtQueue(&Message);
}
U32 DataAccountReadyCheck_QQApn(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "[DataAccount.c] DataAccountReadyCheck_QQApn"));

  //  g_DTCNTReadyCallback = ready_callback;
    ClearInputEventHandler(MMI_DEVICE_ALL);
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);    
    DTCNTGetAccountReq_QQApn(0);
    return 0;
}

#endif
//add end

#endif /* __MMI_GPRS_FEATURES__ */ 

#endif /* __TCPIP__ */ 

