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
 *	NITZ.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements interface and message handler for NITZ.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************
	FILENAME	: NITZ.c
  	PURPOSE		: NITZ Source File
 
 
	DATE		: Restructured on Dec-01-2003
**************************************************************/
#ifndef _MMI_NITZ_C
#define _MMI_NITZ_C

#include "mmi_features.h"
#if defined(__MMI_NITZ__)

#define __NEWSIMULATOR 
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
//#include "mainmenudef.h"
#include "alarmframeworkprot.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_datetime.h"
#include "wgui_categories_sublcd.h"
#include "globaldefs.h"
#include "idleappdef.h"
#include "hardwareevents.h"
#include "debuginitdef.h"
#include "audioinc.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "callsdefs.h"
#include "protocolevents.h"
#include "settingstructs.h"
#include "settingsgexdcl.h"
#include "settinggprots.h"
#include "settingprot.h"
#include "simdetectiongprot.h"
#include "queuegprot.h"
#include "protocolevents.h"
#include "settingprofile.h"
#include "l4dr.h"
//#include "commonscreens.h"
//#include "worldclock.h"
#include "alarmgprot.h"
#include "idleappprot.h"
#include "datetimegprot.h"
#include "wallpaperdefs.h"
//#include "downloaddefs.h"
#include "screensaverdefs.h"
//#include "phonesetup.h"
#include "phonesetupgprots.h"
#include "nitz.h"
#undef __NEWSIMULATOR
#include "mmi_trace.h"
#include "gsm7bitdefaultalphabet.h"
#include "keybrd.h"
#include "simdetectiondef.h"
#include "gpioinc.h"
#include "downloaddefs.h"
#include "phonesetup.h"
/* 
** Define
*/
#define NITZ_SIGN_BIT	0x08
#define NITZ_HIGH_BIT	0xF0
#define NITZ_LOW_BIT	0x0F
#define NITZ_TZ_BIT	0x07

#define NITZ_TIME_LEN		20*ENCODING_LENGTH
#define NITZ_MAX_ALLOW_DIFF	15
/* 
** Typedef 
*/

/* 
** Local Variable
*/
nitz_context_struct g_nitz_cntx;
static U8 nitz_plmn[MAX_PLMN_LEN + 1];
static S16 nitzTempTZ;

#ifdef __MMI_DUAL_SIM__
static U8 nitz_plmn_2[MAX_PLMN_LEN + 1];
#endif


/* 
** Local Function
*/

/* 
** Global Variable
*/
extern wgui_inline_item wgui_inline_items[];
extern U16   gKeyPadLockFlag;
extern MYTIME MyTime;
extern rtc_format_struct gclockData;

/* 
** Global Function
*/
extern void PhnsetSetDT(void);
extern void EntryPhnsetTimeAndDate(void);
extern void EntryIdleScreen(void);
//add by zough 061121 start
#ifndef __MOD_ORGANIZER__
extern FLOAT GetTimeZone(U8 cityIndex);
#else
extern S32 GetTimeZone(U8 cityIndex);
#endif
//add by zough 061121 end
extern void EntryPhnsetSetDT(void);
extern void PhnsetSetTimeResHdlr(void *info);

void mmi_nitz_get_nitz_nw_name(U8* plmn, U8* nwname, U8 len, BOOL* add_ci);
U8 mmi_nitz_get_nw_name(U8 dcs, U8 len, BOOL add_ci, S8 *dstStr, S8 *srcStr);
U8 mmi_nitz_check_dt();
void mmi_NITZ_set_rtc_dt(MYTIME *t);
void mmi_nitz_write_nw_name_to_nvram(U8 sim_type, nitz_nw_name_struct *nw_name);
/*****************************************************************************
 * FUNCTION
 *  NITZHandlerInit
 * DESCRIPTION
 *  NITZ Initlization function,
 *  Set the global variable of NITZ to default and register NITZ indication handler.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void NITZHandlerInit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetProtocolEventHandler(NITZHandler,PRT_MSG_ID_MMI_NW_TIME_ZONE_IND);
    memset(&g_nitz_cntx, 0, sizeof(nitz_context_struct)); 
    g_nitz_cntx.NITZTimeZone = (S16)0xffff;
    g_nitz_cntx.state_flag = 0;
    g_nitz_cntx.near_flag = 0;
    /* init PLMN */
    mmi_nitz_get_nitz_nw_name(NULL, g_nitz_cntx.NWShortName, MAX_NW_LEN *ENCODING_LENGTH, NULL);

}


/*****************************************************************************
 * FUNCTION
 *  NITZHandler
 * DESCRIPTION
 *  NITZ Inidcation Handler, Thi function checks the diffrence in time zone,
 *  if diffrence is not zero, it set NITZ occurance to true and entry NITZ screen.
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void NITZHandler(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_nw_time_zone_ind_struct *msgReq;
    S16 tmpTimeZone ;
    MYTIME refTime1, refTime2, incTime;
    FLOAT NetTimeZone;
    U8 DeaultNWName[MAX_NW_LEN *ENCODING_LENGTH];
    U8 updateShortNW = 0, updateFullNW = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_nitz_cntx.state_flag & MMI_NITZ_STATE_MASK)
    {
        mmi_trace(g_sw_NW, "NITZ func:%s is busy!!!", __FUNCTION__);
        return ;
    }
    
    memset(&incTime, 0, sizeof(MYTIME));
    memset(&refTime1, 0, sizeof(MYTIME));
    memset(&refTime2, 0, sizeof(MYTIME));
    memset(&(g_nitz_cntx.NITZTime), 0, sizeof(MYTIME));    
    mmi_nitz_get_nitz_nw_name(NULL, DeaultNWName, MAX_NW_LEN *ENCODING_LENGTH, NULL);
    
    if (!PhnsetGetNITZFuncStatus())
    {
        return;
    }
    
    if (GetActiveScreenId() == SCR_ID_PHNSET_SET_DT)
    {
        return;
    }
    g_nitz_cntx.state_flag |= MMI_NITZ_STATE_MASK;
    
    msgReq = (mmi_nw_time_zone_ind_struct*)msg;

    if(msgReq->nw_time_zone_timeP)
    {
        /* compare network timezone and last recevied timezone */
        refTime1.nYear = NITZConvertTime(msgReq->nw_time_zone_time.year) + YEARFORMATE;
        refTime1.nMonth = NITZConvertTime(msgReq->nw_time_zone_time.month);
        refTime1.nDay = NITZConvertTime(msgReq->nw_time_zone_time.day);
        refTime1.nHour = NITZConvertTime(msgReq->nw_time_zone_time.hour);	
        refTime1.nMin = NITZConvertTime(msgReq->nw_time_zone_time.min);
        refTime1.nSec = NITZConvertTime(msgReq->nw_time_zone_time.sec);

        tmpTimeZone = NITZGetTimeZone(msgReq->nw_time_zone_time.time_zone);
        NetTimeZone = (FLOAT)tmpTimeZone;
        NetTimeZone /= 4;

        g_nitz_cntx.NITZTime = mmi_dt_utc_to_rtc(NetTimeZone, refTime1);
    }
    else	/* no time information, return */
    {
        memset(&g_nitz_cntx.NITZTime, 0, sizeof(MYTIME));
        return;
    }
    
    if (msgReq->short_nw_nameP)
    {
        updateShortNW = mmi_nitz_get_nw_name(
                            msgReq->short_nw_name_dcs,
                            msgReq->short_nw_name_len,
                            (BOOL) msgReq->short_nw_name_add_ci,
                            (S8*) g_nitz_cntx.NWShortName,
                            (S8*) msgReq->short_nw_name);
    }
    else    /* no time information, return */
    {
        memset(g_nitz_cntx.NWShortName, 0, MAX_NW_LEN * ENCODING_LENGTH);
        msgReq->short_nw_name_len = 0;
    }
    
    if ((msgReq->short_nw_nameP == 0 || msgReq->short_nw_name_len == 0) && msgReq->full_nw_nameP)
    {
        updateFullNW = mmi_nitz_get_nw_name(
                        msgReq->full_nw_name_dcs,
                        msgReq->full_nw_name_len,
                        (BOOL) msgReq->full_nw_name_add_ci,
                        (S8*) g_nitz_cntx.NWFullName,
                        (S8*) msgReq->full_nw_name);
    }
    else
    {
        memset(g_nitz_cntx.NWFullName, 0, MAX_NW_LEN * ENCODING_LENGTH);
        msgReq->full_nw_name_len = 0;
    }
    
    if (strlen((PS8)msgReq->plmn))
    {
        U8 sim_type;
        sim_type = g_nitz_cntx.state_flag & MMI_NITZ_SIM_MASK;
        
        if (sim_type == 1)
        {
            memset(nitz_plmn, 0x00, MAX_PLMN_LEN + 1);
            memcpy(nitz_plmn, msgReq->plmn, MAX_PLMN_LEN + 1);
        }
#ifdef __MMI_DUAL_SIM_MASTER__
        else
        {
            memset(nitz_plmn_2, 0x00, MAX_PLMN_LEN + 1);
            memcpy(nitz_plmn_2,msgReq->plmn, MAX_PLMN_LEN + 1);	
        }
#endif /* __MMI_DUAL_SIM_MASTER__ */
    }
    
    /* DTGetRTCTime(&g_nitz_cntx.RecvTime); */
    
    if (!mmi_nitz_check_dt())
    {
    /* if short name matched, it means short name has been displayed in idle screen */
        if (updateShortNW == MMI_FALSE && 
        updateFullNW == MMI_FALSE)
        {			
            g_nitz_cntx.state_flag = 0;
            g_nitz_cntx.near_flag = 0;
            return;
        }        
        else if (msgReq->short_nw_name_len == 0 && 
        pfnUnicodeStrcmp((S8*)DeaultNWName, (S8*) g_nitz_cntx.NWFullName) == 0)
        {
            g_nitz_cntx.state_flag = 0;
            g_nitz_cntx.near_flag = 0;
            return;
        }
        /* The two NITZ are too closer, skip the later one */
        else if (g_nitz_cntx.near_flag)
        {
            g_nitz_cntx.state_flag = 0;
            g_nitz_cntx.near_flag = 0;
            return;
        }
    }
    
    g_nitz_cntx.IsNITZPending = MMI_TRUE;
    if (g_idle_context.IsOnIdleScreen)
    {
        EntryIdleScreen();
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_nitz_get_nw_name
 * DESCRIPTION
 *  This function is used to get NITZ network name.
 * PARAMETERS
 *  dcs         [IN]        
 *  len         [IN]        
 *  add_ci      [IN]        
 *  dstStr      [?]         
 *  srcStr      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
U8 mmi_nitz_get_nw_name(U8 dcs, U8 len, BOOL add_ci, S8 *dstStr, S8 *srcStr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U8 *nwStr = NULL, *tmpStr, *tempStr2;
    U8 count = 0;
    U16 nwStrLen = 0, srcStr_len = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_NW, "NITZ func:%s", __FUNCTION__);
    if (len == 0)
    {
        if (pfnUnicodeStrlen((S8*)dstStr) > 0)
        {
            memset(dstStr, 0, MAX_NW_LEN * ENCODING_LENGTH);
            return MMI_TRUE;
        }
        else
        {
            return MMI_FALSE;
        }
    }


    mmi_trace(g_sw_NW, "NITZ func:%s len = %d, dcs = %d", __FUNCTION__, len, dcs);
    switch (dcs)
    {
        case SMSAL_DEFAULT_DCS:
            srcStr_len = strlen(srcStr);
            if (srcStr_len < MAX_NW_LEN)
            {
                nwStr = (U8*)CovertStringForPlatform((U8*)srcStr, (U16)srcStr_len, (U8)SMSAL_DEFAULT_DCS, (U16*)&nwStrLen);
            }
            else
            {
                return MMI_FALSE;
            }
            break;
        case SMSAL_UCS2_DCS:
            srcStr_len = pfnUnicodeStrlen(srcStr);
            if (srcStr_len < MAX_NW_LEN)
            {
                memcpy(nwStr, srcStr, srcStr_len * ENCODING_LENGTH);
            }
            else
            {
                return MMI_FALSE;
            }
            
        default:
            break;
    }
    g_nitz_cntx.AddCI = add_ci;

    /* compare PLMN name */
    tmpStr = (U8*)dstStr;
    tempStr2 = nwStr;
    while (count < MAX_NW_LEN)
    {
        count++;
        if ((*tmpStr == *tempStr2) && (*(tmpStr + 1) == *(tempStr2 + 1)))
        {
            tmpStr += 2;
            tempStr2 += 2;
        }
        else
        {
            break;
        }
    }   /* End of while */

    if (count < MAX_NW_LEN)
    {
        memcpy(dstStr, nwStr, MAX_NW_LEN * ENCODING_LENGTH);
        OslMfree(nwStr);
        return MMI_TRUE;
    }
    else
    {
        OslMfree(nwStr);
        return MMI_FALSE;
    }

}

/*****************************************************************************
 * FUNCTION
 *  EntryNitzScreen
 * DESCRIPTION
 *  Show the NITZ chnage in time popup,
 *  if NITZ happend and control is on the idle screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryNitzScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_NITZ, ExitNitzScreen, NULL, NULL);

    ClearKeyEvents();

    guiBuffer = GetCurrGuiBuffer(SCR_ID_NITZ);

    NITZMessageString();
    if (gKeyPadLockFlag == 0)
    {
        /* start the back light */
        TurnOnBacklight(1);

        ShowCategory7Screen(
            CHANGE_TIME_CAPTION,
            CHANGE_TIME_IMAGE,
            STR_GLOBAL_YES,
            IMG_GLOBAL_YES,
            STR_GLOBAL_NO,
            IMG_GLOBAL_NO, 
            (U8*)g_nitz_cntx.Message,
            guiBuffer);
        playRequestedTone(GENERAL_TONE);
        SetLeftSoftkeyFunction(NITZAccept, KEY_EVENT_UP);	
        SetRightSoftkeyFunction(NITZIgnore, KEY_EVENT_UP);	
        SetKeyHandler(NITZIgnore, KEY_END, KEY_EVENT_DOWN);
    }
    else
    {
        /*ShowCategory141Screen(
            0,
            0,
            g_keylock_context.KeyLockLSKStringID,
            g_keylock_context.KeyLockLSKIconID,
            g_keylock_context.KeyLockRSKStringID,
            g_keylock_context.KeyLockRSKIconID,
            (U8*) g_nitz_cntx.Message,
            0,
            NULL);*/
        OslMfree(g_nitz_cntx.Message);
        g_nitz_cntx.Message = NULL;
    }    
}

/*****************************************************************************
 * FUNCTION
 *  ExitNitzScreen
 * DESCRIPTION
 *  Exit function of NITZ screen, free memory.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitNitzScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_nitz_cntx.Message != NULL)
    {
        OslMfree(g_nitz_cntx.Message);
        g_nitz_cntx.Message = NULL;
    }
}

/*****************************************************************************
 * FUNCTION
 *  NITZIgnore
 * DESCRIPTION
 *  RSK handler of NITZ screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void NITZIgnore(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_nitz_cntx.IsNITZPending = FALSE;
    g_nitz_cntx.state_flag = 0;
    g_nitz_cntx.near_flag = 0;
    GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  NITZAccept
 * DESCRIPTION
 *  This API calls on pressing OK on NITZ indication screen.
 *  This API fills the time and date set screen and passes control to the same screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void NITZAccept(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYTIME incTime;
    nitz_nw_name_struct nw_name;
    MYTIME currTime;
#ifdef __MMI_NITZ_TZ_SUPPORT__
    MYTIME resultTime;
    FLOAT nitz_tz, diff_tz;   
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_NW, "NITZ func:%s", __FUNCTION__);

    memset(&nw_name, 0, sizeof(nitz_nw_name_struct));

    if (mmi_dt_is_valid(&g_nitz_cntx.NITZTime))
    {
        g_nitz_cntx.NITZTimeZone = nitzTempTZ;
        memset(&incTime, 0, sizeof(MYTIME));
                
        DTGetRTCTime(&currTime);

        GetTimeDifference(&currTime, &g_nitz_cntx.RecvTime, &incTime);

        g_nitz_cntx.NITZTime = IncrementTime(g_nitz_cntx.NITZTime, incTime);
        
	SetProtocolEventHandler(PhnsetSetTimeResHdlr, HW_SET_TIME_REQ_SUCCESS); 
        mmi_NITZ_set_rtc_dt(&g_nitz_cntx.NITZTime);
    }
    
    //mmi_nitz_update_operator_name(&nw_name);
    g_nitz_cntx.state_flag = 0;
    g_nitz_cntx.near_flag = 0;
    g_nitz_cntx.IsNITZPending = FALSE;
    GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_NITZ_set_rtc_dt
 * DESCRIPTION
 
 *  Functionality:
 * PARAMETERS
 *  t       [?]     [?]
 *  a(?)        [IN/OUT](?)
 *  b(?)        [IN](?)
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_NITZ_set_rtc_dt(MYTIME *t)
{
#ifdef MMI_ON_HARDWARE_P
    MYQUEUE msgStruct;
    RTCTIMEFORMAT *time;

    g_phnset_cntx_p->settime.hour        = t->nHour;
    g_phnset_cntx_p->settime.minutes	=t->nMin;
    g_phnset_cntx_p->settime.month     =t->nMonth;
    g_phnset_cntx_p->settime.date		=t->nDay;
    g_phnset_cntx_p->settime.year		=t->nYear;

    time = OslConstructDataPtr(sizeof (RTCTIMEFORMAT));
    time->info.alarm_format = 0;
    time->info.alarm_index = 0;
    time->info.type = 0;
    time->info.text[0] = '\0';
    time->info.recurr = 0;
    time->info.data_time.rtc_sec = t->nSec;
    time->info.data_time.rtc_min = g_phnset_cntx_p->settime.minutes;
    time->info.data_time.rtc_hour = g_phnset_cntx_p->settime.hour;

    time->rtc_type = RTC_TIME_CLOCK_IND;
    time->set_type = RTC_SETTING_TYPE_DATETIME;
    time->info.data_time.rtc_day = g_phnset_cntx_p->settime.date;
    time->info.data_time.rtc_mon = g_phnset_cntx_p->settime.month;
    time->info.data_time.rtc_wday = gclockData.rtc_wday;
    time->info.data_time.rtc_year =  g_phnset_cntx_p->settime.year - YEARFORMATE;

    msgStruct.oslSrcId = MOD_MMI;
    msgStruct.oslDestId = MOD_L4C;
    msgStruct.oslMsgId = HW_SET_TIME_REQ;
    msgStruct.oslSapId = 0;
    msgStruct.oslDataPtr = (oslParaType *)time;
    msgStruct.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&msgStruct);
	#endif


}

/*****************************************************************************
 * FUNCTION
 *  mmi_nitz_update_operator_name
 * DESCRIPTION
 *  This API to update operator name
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_nitz_update_operator_name(nitz_nw_name_struct* nwk_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 sim_type;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    sim_type = g_nitz_cntx.state_flag & MMI_NITZ_SIM_MASK;
    MMI_ASSERT(nwk_ptr != NULL);
	
    if (strlen((S8*) g_nitz_cntx.NWShortName) != 0)
    {        
        memcpy(&(nwk_ptr->name), &g_nitz_cntx.NWShortName, MAX_NW_LEN * ENCODING_LENGTH);
        nwk_ptr->add_ci = (U8) g_nitz_cntx.AddCI;
        if (sim_type == 1)
        {
            memcpy(&(nwk_ptr->plmn), nitz_plmn, MAX_PLMN_LEN_MMI);	
        }
#ifdef __MMI_DUAL_SIM__
        else
        {
            memcpy(&(nwk_ptr->plmn), nitz_plmn_2, MAX_PLMN_LEN_MMI);	
        }
#endif /* __MMI_DUAL_SIM_MASTER__ */
        mmi_nitz_write_nw_name_to_nvram(sim_type, nwk_ptr);
        //mmi_idle_update_opname_from_nitz();
    }
    else if (strlen((S8*) g_nitz_cntx.NWFullName) != 0)
    {
        memcpy(&nwk_ptr->name, &g_nitz_cntx.NWFullName, MAX_NW_LEN * ENCODING_LENGTH);
        nwk_ptr->add_ci = (U8) g_nitz_cntx.AddCI;

        if (sim_type == 1)
        {
            memcpy(&(nwk_ptr->plmn),nitz_plmn, MAX_PLMN_LEN_MMI);	
        }
#ifdef __MMI_DUAL_SIM_MASTER__
        else
        {
            memcpy(&(nwk_ptr->plmn),nitz_plmn_2, MAX_PLMN_LEN_MMI);	
        }
#endif /* __MMI_DUAL_SIM_MASTER__ */     
        mmi_nitz_write_nw_name_to_nvram(sim_type, nwk_ptr);
        //mmi_idle_update_opname_from_nitz();
    }
    else
    {
        memset(&(nwk_ptr->plmn),0, MAX_PLMN_LEN_MMI);    
        memset(&nwk_ptr->name, 0, MAX_NW_LEN * ENCODING_LENGTH);
        nwk_ptr->add_ci = 0;
        mmi_nitz_write_nw_name_to_nvram(sim_type, nwk_ptr);        
        //mmi_idle_update_opname_from_nitz();
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_nitz_set_sim_id
 * DESCRIPTION
 *  This API set sim id to stat flag
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_nitz_set_sim_id(U8 sim_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	g_nitz_cntx.state_flag &= ~MMI_NITZ_SIM_MASK;
	g_nitz_cntx.state_flag |= (1 << (sim_id - 1));
}


/*****************************************************************************
 * FUNCTION
 *  mmi_nitz_get_nitz_nw_name
 * DESCRIPTION
 *  This API read nitz netowrk name information from NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_nitz_get_nitz_nw_name(U8* plmn, U8* nwname, U8 len, BOOL* add_ci)
{
    nitz_nw_name_struct nw_name; 
    S16 error;
    
    ReadRecord(NVRAM_EF_NITZ_NW_NAME_LID, 1, &nw_name, sizeof(nitz_nw_name_struct), &error);

    if (plmn != NULL)
    {
        memcpy(plmn, nw_name.plmn, MAX_PLMN_LEN_MMI);
    }

    if (nwname != NULL)
    {
        pfnUnicodeStrncpy((S8*) nwname, (S8*)nw_name.name, len);
    }

    if (add_ci != NULL)
    {
        memcpy(add_ci, &(nw_name.add_ci), sizeof(U8));    
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_nitz_write_nw_name_to_nvram
 * DESCRIPTION
 *  Write SIM2 network name into NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_nitz_write_nw_name_to_nvram(U8 sim_type, nitz_nw_name_struct *nw_name)
{    
    S16 error;    
    
    WriteRecord(NVRAM_EF_NITZ_NW_NAME_LID, sim_type, nw_name, sizeof(nitz_nw_name_struct), &error);
}

#ifdef __MMI_DUAL_SIM__
/*****************************************************************************
 * FUNCTION
 *  mmi_nitz_get_nitz_nw_name
 * DESCRIPTION
 *  This API read nitz netowrk name information from NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_nitz_get_nitz_nw_name_sim2(U8* plmn, U8* nwname, U8 len, BOOL* add_ci)
{
    nitz_nw_name_struct nw_name; 
    S16 error;
    
    ReadRecord(NVRAM_EF_NITZ_NW_NAME_LID, 2, &nw_name, sizeof(nitz_nw_name_struct), &error);

    if (plmn != NULL)
    {
        memcpy(plmn, nw_name.plmn, MAX_PLMN_LEN_MMI);
    }

    if (nwname != NULL)
    {
        pfnUnicodeStrncpy((S8*) nwname, (S8*)nw_name.name, len);
    }

    if (add_ci != NULL)
    {
        memcpy(add_ci, &(nw_name.add_ci), sizeof(U8));    
    }
}
#endif /*__MMI_DUAL_SIM__*/

/*****************************************************************************
 * FUNCTION
 *  mmi_nitz_check_dt
 * DESCRIPTION
 *  
 *  This function is used to check if the date/time need to be updated from NITZ.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
U8 mmi_nitz_check_dt()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYTIME refTime1, refTime2, incTime;
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(&incTime, 0, sizeof(MYTIME));
    memset(&refTime1, 0, sizeof(MYTIME));
    memset(&refTime2, 0, sizeof(MYTIME));

    incTime.nMin = NITZ_MAX_ALLOW_DIFF;
    g_nitz_cntx.near_flag = 0;
    /* init timezone */
    ReadValue(NVRAM_TIMEZONE, &(g_nitz_cntx.NITZTimeZone), DS_SHORT, &error);		    

    refTime1 = IncrementTime(g_nitz_cntx.RecvTime, incTime);
    DecrementTime(g_nitz_cntx.RecvTime, incTime, &refTime2);

    mmi_trace(g_sw_NW, "NITZ func:%s %d, %d, %d", __FUNCTION__, (S32) refTime1.nHour, (S32) refTime1.nMin, (S32) refTime1.nSec);

    mmi_trace(g_sw_NW, "NITZ func:%s %d, %d, %d", __FUNCTION__, (S32) refTime2.nHour, (S32) refTime2.nMin, (S32) refTime2.nSec);
    if (!mmi_dt_is_valid(&g_nitz_cntx.NITZTime))
    {
        mmi_trace(g_sw_NW, "NITZ func:%s invaild!!!", __FUNCTION__);
        return MMI_FALSE;
    }
	else if ((CompareTime(g_nitz_cntx.NITZTime, refTime1, NULL) == TIME_LESS) &&
             (CompareTime(g_nitz_cntx.NITZTime, refTime2, NULL) == TIME_GREATER))
    {   
 
        /* Check if time zone field in date-time is changed */
        if (nitzTempTZ != (g_nitz_cntx.NITZTimeZone
    #ifdef __MMI_NITZ_TZ_SUPPORT__
         + (nitzTempDST * 4)
    #endif
         ) && g_nitz_cntx.isValidTZ != MMI_FALSE)
        {   
            mmi_trace(g_sw_NW, "NITZ func:%s MMI_RTC_NITZ_CHECK_DT_IGNOER1!!!", __FUNCTION__);
           return MMI_TRUE;
        }
        else
        {
            g_nitz_cntx.near_flag = 1;
            mmi_trace(g_sw_NW, "NITZ func:%s MMI_RTC_NITZ_CHECK_DT_IGNOER2!!!", __FUNCTION__);
            return MMI_FALSE;            
        }
    }
#ifdef __MMI_CHECK_DUPLICATED_NITZ__
    else if (nitzTempTZ == g_nitz_cntx.NITZTimeZone)
    {
        mmi_trace(g_sw_NW, "NITZ func:%s MMI_RTC_NITZ_CHECK_DT_IGNOER3!!!", __FUNCTION__);
        return MMI_FALSE;
    }
#endif    
    else
    {
        mmi_trace(g_sw_NW, "NITZ func:%s MMI_RTC_NITZ_CHECK_DT_IGNOER4!!!", __FUNCTION__);
        return MMI_TRUE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  NITZMessageString
 * DESCRIPTION
 *  Return the new time zone string, need to be display using in IDLE screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S8 *NITZMessageString(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *tmpStr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_nitz_cntx.Message == NULL)
    {
        g_nitz_cntx.Message = OslMalloc(NITZ_NOTIFY_LENGTH);
    }

    tmpStr = GetString(NITZTIMECHANGESTR);

    pfnUnicodeStrncpy((S8*) g_nitz_cntx.Message, tmpStr, pfnUnicodeStrlen((S8*) tmpStr));

    return g_nitz_cntx.Message;
}

/*****************************************************************************
 * FUNCTION
 *  NITZTimeZoneString
 * DESCRIPTION
 *  
 * PARAMETERS
 *  timezone        [IN]        
 *  string          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void NITZTimeZoneString(S16 timezone, S8 *string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 tmpBuff[NITZ_TIME_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (timezone < 0)
    {
        timezone = 0 - timezone;
        sprintf(tmpBuff, "-%02d:%02d", (timezone / 4), (timezone % 4) * 15);
    }
    else
    {
        sprintf(tmpBuff, "+%02d:%02d", (timezone / 4), (timezone % 4) * 15);
    }

    AnsiiToUnicodeString(string, tmpBuff);
}

/*****************************************************************************
 * FUNCTION
 *  NITZConvertTime
 * DESCRIPTION
 *  Utility function require by NITZ to convert BCD Date to decimal.
 * PARAMETERS
 *  time        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
U8 NITZConvertTime(U8 time)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 tmp1 = 0, tmp2 = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmp1 = time & NITZ_HIGH_BIT;
    tmp1 = tmp1 >> 4;
    tmp2 = time & NITZ_LOW_BIT;
    return tmp2 * 10 + tmp1;
}

/*****************************************************************************
 * FUNCTION
 *  NITZGetTimeZone
 * DESCRIPTION
 *  Utility function require by NITZ to convert BCD Date to decimal.
 * PARAMETERS
 *  t       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
S16 NITZGetTimeZone(U8 t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 tmp;
    U8 NegFlag;
    S16 result;
    U8 timezone = t;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmp = (t & NITZ_HIGH_BIT) >> 4;

    if (t & NITZ_SIGN_BIT)  /* negative */
    {
        NegFlag = TRUE;
    }
    else    /* positive */
    {
        NegFlag = FALSE;
    }

    timezone &= NITZ_TZ_BIT;
    timezone = timezone * 10 + tmp;

    result = (S16) timezone;

    if (NegFlag)
    {
        result = 0 - result;
    }

    return result;
}

#endif//__MMI_NITZ__

#endif // #ifndef _MMI_NITZ_C

