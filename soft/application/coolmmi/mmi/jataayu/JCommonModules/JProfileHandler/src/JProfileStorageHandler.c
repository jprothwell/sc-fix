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
 * JProfileStorageHandler.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is source file for NVRAM storage related part for the profiles (BROWSER & MMS .....)
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
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

#ifndef _JPROFILESTORAGEHANDLER_C
#define _JPROFILESTORAGEHANDLER_C

#include "mmi_features.h"

#if defined (JATAAYU_SUPPORT)// && (defined (WAP_SUPPORT) || (defined (MMS_SUPPORT)))
#include "jwap_inc.h"
#include "jmms_inc.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "jprovwap.h"

#include "jprofilehandlerdefs.h"
#include "jprofilehandlertypes.h"
#include "jprofilehandlerprots.h"
#include "phonebookgprot.h"     /* for extern pbool getucs2flag(ps8 buffer) fun */

#define MMI_ID_TYPE  U16
#ifdef MMI_ON_HARDWARE_P
#include "nvram_user_defs.h"
#endif
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#if 0
#include "globaldefs.h"
#include "commonscreens.h"

#endif
#ifdef __MMI_MULTI_SIM__
#ifdef JATAAYU_SUPPORT_WAP
extern JWAP_APP_INFO *g_pstJWapAppInfo ;
#endif
#ifdef JATAAYU_SUPPORT_MMS
extern JMMS_APP_INFO *g_pstJMmsAppInfo ;
#endif
#endif
extern nvram_wap_profile_content_struct NVRAM_EF_WAP_PROFILE_CONTENT_DEFAULT[];
extern nvram_wap_profile_name_struct NVRAM_EF_WAP_PROFILE_NAMES_DEFAULT[];
extern nvram_mms_profile_content_struct NVRAM_EF_MMS_PROFILE_CONTENT_DEFAULT[];
extern nvram_mms_profile_name_struct NVRAM_EF_MMS_PROFILE_NAMES_DEFAULT[];


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_get_brw_profile_name_list
 * DESCRIPTION
 *  This reads the browser profile name list from the NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_get_brw_profile_name_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_ph_cntx.ph_wap_prof_name_arr_p == NULL)
    {
        g_ph_cntx.ph_wap_prof_name_arr_p =
            (nvram_wap_profile_name_array_struct*) jdd_MemAlloc(NVRAM_EF_WAP_PROFILE_NAMES_SIZE, 1);
    }
    /* Itishree Adds */
    if (g_ph_cntx.ph_wap_prof_name_arr_p == NULL)
    {
        ASSERT(0);
    }
    /* Itishree Ends */

#ifdef JATAAYU_SUPPORT_WAP	
#ifdef __MMI_MULTI_SIM__

	    ReadRecord(
	        NVRAM_EF_WAP_PROFILE_NAMES_LID + g_pstJWapAppInfo->ph_edit_simid,
	        1,
	        g_ph_cntx.ph_wap_prof_name_arr_p,
	        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
	        &error);
#else
	ReadRecord(
	    NVRAM_EF_WAP_PROFILE_NAMES_LID,
	    1,
	    g_ph_cntx.ph_wap_prof_name_arr_p,
	    NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
	    &error);
#endif
#endif

}


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_get_mms_profile_name_list
 * DESCRIPTION
 *  This reads the MMS profile name list from the NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_get_mms_profile_name_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_ph_cntx.ph_mms_prof_name_arr_p == NULL)
    {
        g_ph_cntx.ph_mms_prof_name_arr_p =
            (nvram_mms_profile_name_array_struct*) jdd_MemAlloc(NVRAM_EF_MMS_PROFILE_NAMES_SIZE, 1);
    }
    /* Itishree Adds */
    if (g_ph_cntx.ph_mms_prof_name_arr_p == NULL)
    {
        ASSERT(0);
    }
    /* Itishree Ends */

#ifdef __MMI_MULTI_SIM__
#ifdef JATAAYU_SUPPORT_MMS
	 ReadRecord(
	        NVRAM_EF_MMS_PROFILE_NAMES_LID + g_pstJMmsAppInfo->ph_edit_simid,
	        1,
	        g_ph_cntx.ph_mms_prof_name_arr_p,
	        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
	        &error);
#endif
#else
    ReadRecord(
        NVRAM_EF_MMS_PROFILE_NAMES_LID,
        1,
        g_ph_cntx.ph_mms_prof_name_arr_p,
        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
        &error);
#endif		
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_get_profile_nvram_content_data
 * DESCRIPTION
 *  This reads the content data for the given profile index from the NVRAM
 * PARAMETERS
 *  index       [IN]        Index of the profile
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_get_profile_nvram_content_data(U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
	if (g_ph_cntx.profile_module == PH_BROWSER_PROFILE)
	{ 
	#ifdef JATAAYU_SUPPORT_WAP
		ReadRecord(
			NVRAM_EF_WAP_PROFILE_CONTENT_LID + g_pstJWapAppInfo->ph_edit_simid,
			(U16) (index + 1),
			g_ph_cntx.ph_wap_profile_p,
			NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
			&error);
	#endif
	}
	else if (g_ph_cntx.profile_module == PH_MMS_PROFILE)	
	{
	#ifdef JATAAYU_SUPPORT_MMS
		ReadRecord(
			    NVRAM_EF_MMS_PROFILE_CONTENT_LID + g_pstJMmsAppInfo->ph_edit_simid,
			    (U16) (index + 1),
			    g_ph_cntx.ph_mms_profile_p,
			    NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
			    &error);
	#endif
	}
#else
    if (g_ph_cntx.profile_module == PH_BROWSER_PROFILE)
    {
        ReadRecord(
            NVRAM_EF_WAP_PROFILE_CONTENT_LID,
            (U16) (index + 1),
            g_ph_cntx.ph_wap_profile_p,
            NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
            &error);
    }
    else if (g_ph_cntx.profile_module == PH_MMS_PROFILE)
    {
        ReadRecord(
            NVRAM_EF_MMS_PROFILE_CONTENT_LID,
            (U16) (index + 1),
            g_ph_cntx.ph_mms_profile_p,
            NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
            &error);
    }
#endif	
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_get_activated_profile_data
 * DESCRIPTION
 *  This gets the activated profile content data from the NVRAM into the global context of the activated profile
 * PARAMETERS
 *  profile     [IN]        Profile type(Browser,MMS.....)
 *  index       [IN]        Activated profile index
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_get_activated_profile_data(ph_profile_enum profile, U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    #ifdef __MMI_MULTI_SIM__
	if (profile == PH_BROWSER_PROFILE)
	{
	#ifdef JATAAYU_SUPPORT_WAP
		ReadRecord(
			NVRAM_EF_WAP_PROFILE_CONTENT_LID + g_pstJWapAppInfo->ph_edit_simid,
			(U16) (index + 1),
			g_ph_cntx.ph_activated_wap_profile[g_pstJWapAppInfo->ph_edit_simid],
			NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
			&error);
	#endif
	}
	else if (profile == PH_MMS_PROFILE)
	{
	#ifdef JATAAYU_SUPPORT_MMS
		ReadRecord(
			NVRAM_EF_MMS_PROFILE_CONTENT_LID + g_pstJWapAppInfo->ph_edit_simid,
			(U16) (index + 1),
			g_ph_cntx.ph_activated_mms_profile[g_pstJWapAppInfo->ph_edit_simid],
			NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
			&error);
	#endif
	}
    #else
            if (profile == PH_BROWSER_PROFILE)
            {
                ReadRecord(
                    NVRAM_EF_WAP_PROFILE_CONTENT_LID,
                    (U16) (index + 1),
                    g_ph_cntx.ph_activated_wap_profile,
                    NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
                    &error);
            }
            else if (profile == PH_MMS_PROFILE)
            {
                ReadRecord(
                    NVRAM_EF_MMS_PROFILE_CONTENT_LID,
                    (U16) (index + 1),
                    g_ph_cntx.ph_activated_mms_profile,
                    NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
                    &error);
            }
  #endif
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_update_nvram_wap_profile
 * DESCRIPTION
 *  This API updates the NVRAM data for the browser profile (both Name & Content)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_update_nvram_wap_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S16 error = 0;

    U32 index = g_ph_cntx.profile_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (UCS2Strlen((S8*) g_ph_cntx.profile_display_p->profile_name))
    {
        if (GetUCS2Flag((S8*) g_ph_cntx.profile_display_p->profile_name))
        {
            g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].dcs = PH_DCS_UCS;
            UCS2Strncpy(
                (S8*) (g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].profile_name),
                (S8*) g_ph_cntx.profile_display_p->profile_name,
                NVRAM_WAP_PROFILE_PROFILE_NAME_LEN);
        }
        else
        {
            g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].dcs = PH_DCS_ASCII;
            UnicodeToAnsii(
                (S8*) (g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].profile_name),
                (S8*) g_ph_cntx.profile_display_p->profile_name);
        }
    }
    else
    {
        S8 temp_unicode[8];
        S8 temp[4];
        S8 *profile_name_p = NULL;

        profile_name_p = (S8*) GetString((MMI_ID_TYPE) STR_ID_PROF_HDLR_PROFILE);


        sprintf(temp, "%d", (index + 1));
        AnsiiToUnicodeString((S8*) temp_unicode, (S8*) temp);
        UCS2Strcpy((S8*) g_ph_cntx.profile_display_p->profile_name, profile_name_p);
        UCS2Strcat((S8*) g_ph_cntx.profile_display_p->profile_name, temp_unicode);

        g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].dcs = PH_DCS_UCS;
        UCS2Strncpy(
            (S8*) (g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].profile_name),
            (S8*) g_ph_cntx.profile_display_p->profile_name,
            NVRAM_WAP_PROFILE_PROFILE_NAME_LEN);
    }

    UnicodeToAnsii((S8*) g_ph_cntx.ph_wap_profile_p->homepage_url, (S8*) (g_ph_cntx.profile_display_p->homepage_url));
    UnicodeToAnsii((S8*) (g_ph_cntx.ph_wap_profile_p->username), (S8*) (g_ph_cntx.profile_display_p->username));
    UnicodeToAnsii((S8*) (g_ph_cntx.ph_wap_profile_p->password), (S8*) (g_ph_cntx.profile_display_p->password));

    g_ph_cntx.ph_wap_profile_p->empty = 0x00;   /* Added By Itishree */
#ifdef __MMI_MULTI_SIM__
    if (index == g_ph_cntx.wap_actived_profile_index[g_pstJWapAppInfo->ph_edit_simid])
    {
        memcpy(g_ph_cntx.ph_activated_wap_profile[g_pstJWapAppInfo->ph_edit_simid], g_ph_cntx.ph_wap_profile_p, NVRAM_EF_WAP_PROFILE_CONTENT_SIZE);
    }

	WriteRecord(
	        NVRAM_EF_WAP_PROFILE_CONTENT_LID + g_pstJWapAppInfo->ph_edit_simid,
	        (U16) (index + 1),
	        g_ph_cntx.ph_wap_profile_p,
	        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
	        &error);
	WriteRecord(
	        NVRAM_EF_WAP_PROFILE_NAMES_LID + g_pstJWapAppInfo->ph_edit_simid,
	        1,
	        g_ph_cntx.ph_wap_prof_name_arr_p,
	        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
	        &error);
#else
    if (index == g_ph_cntx.wap_actived_profile_index)
    {
        memcpy(g_ph_cntx.ph_activated_wap_profile, g_ph_cntx.ph_wap_profile_p, NVRAM_EF_WAP_PROFILE_CONTENT_SIZE);
    }

    WriteRecord(
        NVRAM_EF_WAP_PROFILE_CONTENT_LID,
        (U16) (index + 1),
        g_ph_cntx.ph_wap_profile_p,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        &error);
    WriteRecord(
        NVRAM_EF_WAP_PROFILE_NAMES_LID,
        1,
        g_ph_cntx.ph_wap_prof_name_arr_p,
        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
        &error);
#endif

    if(error==NVRAM_WRITE_SUCCESS)
         DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
    else
        DisplayPopup((PU8) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
    DeleteNHistory(1);

}


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_update_nvram_mms_profile
 * DESCRIPTION
 *  This API updates the NVRAM data for the MMS profile (both Name & Content)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_update_nvram_mms_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error = 0;

    U32 index = g_ph_cntx.profile_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (UCS2Strlen((S8*) g_ph_cntx.profile_display_p->profile_name))
    {
        if (GetUCS2Flag((S8*) g_ph_cntx.profile_display_p->profile_name))
        {
            g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].dcs = PH_DCS_UCS;
            UCS2Strncpy(
                (S8*) (g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].profile_name),
                (S8*) g_ph_cntx.profile_display_p->profile_name,
                NVRAM_WAP_PROFILE_PROFILE_NAME_LEN);
        }
        else
        {
            g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].dcs = PH_DCS_ASCII;
            UnicodeToAnsii(
                (S8*) (g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].profile_name),
                (S8*) g_ph_cntx.profile_display_p->profile_name);
        }
    }
    else
    {
        S8 temp_unicode[8];
        S8 temp[4];
        S8 *profile_name_p;

        profile_name_p = (S8*) GetString((MMI_ID_TYPE) STR_ID_PROF_HDLR_PROFILE);
        sprintf(temp, "%d", (index + 1));
        AnsiiToUnicodeString((S8*) temp_unicode, (S8*) temp);
        UCS2Strcpy((S8*) g_ph_cntx.profile_display_p->profile_name, profile_name_p);
        UCS2Strcat((S8*) g_ph_cntx.profile_display_p->profile_name, temp_unicode);

        g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].dcs = PH_DCS_UCS;
        UCS2Strncpy(
            (S8*) (g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].profile_name),
            (S8*) g_ph_cntx.profile_display_p->profile_name,
            NVRAM_WAP_PROFILE_PROFILE_NAME_LEN);

    }

    UnicodeToAnsii((S8*) g_ph_cntx.ph_mms_profile_p->mmsc_url, (S8*) (g_ph_cntx.profile_display_p->homepage_url));
    UnicodeToAnsii((S8*) (g_ph_cntx.ph_mms_profile_p->username), (S8*) (g_ph_cntx.profile_display_p->username));
    UnicodeToAnsii((S8*) (g_ph_cntx.ph_mms_profile_p->password), (S8*) (g_ph_cntx.profile_display_p->password));

    g_ph_cntx.ph_mms_profile_p->empty = 0x00;   /* Added By Itishree */

#ifdef __MMI_MULTI_SIM__
    if (index == g_ph_cntx.mms_actived_profile_index[g_pstJMmsAppInfo->ph_edit_simid])
    {
        memcpy(g_ph_cntx.ph_activated_mms_profile[g_pstJMmsAppInfo->ph_edit_simid], g_ph_cntx.ph_mms_profile_p, NVRAM_EF_MMS_PROFILE_CONTENT_SIZE);
    #if defined(MMS_SUPPORT)        /* dilip */
        mmi_jmms_set_profile();
    #endif 

    }

	WriteRecord(
	        NVRAM_EF_MMS_PROFILE_CONTENT_LID + g_pstJMmsAppInfo->ph_edit_simid ,
	        (U16) (index + 1),
	        g_ph_cntx.ph_mms_profile_p,
	        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
	        &error);
	WriteRecord(
	        NVRAM_EF_MMS_PROFILE_NAMES_LID + g_pstJMmsAppInfo->ph_edit_simid ,
	        1,
	        g_ph_cntx.ph_mms_prof_name_arr_p,
	        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
	        &error);
#else
    if (index == g_ph_cntx.mms_actived_profile_index)
    {
        memcpy(g_ph_cntx.ph_activated_mms_profile, g_ph_cntx.ph_mms_profile_p, NVRAM_EF_MMS_PROFILE_CONTENT_SIZE);
    #if defined(MMS_SUPPORT)        /* dilip */
        mmi_jmms_set_profile();
    #endif 

    }

    WriteRecord(
        NVRAM_EF_MMS_PROFILE_CONTENT_LID,
        (U16) (index + 1),
        g_ph_cntx.ph_mms_profile_p,
        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
        &error);
    WriteRecord(
        NVRAM_EF_MMS_PROFILE_NAMES_LID,
        1,
        g_ph_cntx.ph_mms_prof_name_arr_p,
        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
        &error);
#endif

    if(error==NVRAM_WRITE_SUCCESS)
         DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
    else
        DisplayPopup((PU8) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
    DeleteNHistory(1);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_update_nvram_activated_wap_profile
 * DESCRIPTION
 *  Updates the NVRAM browser activated profile content data
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_update_nvram_activated_wap_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		WriteRecord(
				NVRAM_EF_WAP_PROFILE_CONTENT_LID + i,
				(U16) (g_ph_cntx.wap_actived_profile_index[i] + 1),
				g_ph_cntx.ph_activated_wap_profile[i],
				NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
				&error);
	}
#else
    WriteRecord(
        NVRAM_EF_WAP_PROFILE_CONTENT_LID,
        (U16) (g_ph_cntx.wap_actived_profile_index + 1),
        g_ph_cntx.ph_activated_wap_profile,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        &error);
#endif
}

#if 0
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */ 

#if 0//fengzb
/*****************************************************************************
 * FUNCTION
 *  mmi_ph_update_prov_nvram_wap_profile
 * DESCRIPTION
 *  Updates the NVRAM browser profile data(Both Name & Content) with the provisioing data for the index selected
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_update_prov_nvram_wap_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S16 error = 0;
    U8 conn_type;
    U32 len = 0;

    U32 index = g_ph_cntx.profile_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ph_cntx.ph_wap_profile_p->data_account = g_ph_cntx.profile_display_p->data_account;

    WriteRecord(
        NVRAM_EF_WAP_PROFILE_CONTENT_LID,
        (U16) (index + 1),
        g_ph_cntx.ph_wap_profile_p,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        &error);

    if (index == g_ph_cntx.wap_actived_profile_index)
    {
        memcpy(g_ph_cntx.ph_activated_wap_profile, g_ph_cntx.ph_wap_profile_p, NVRAM_EF_WAP_PROFILE_CONTENT_SIZE);
    }

    conn_type = g_ph_cntx.ph_wap_profile_p->conn_type;
    g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].conn_type = conn_type;

    len = strlen((PS8) g_prov_profile_name_p);
    if ((g_prov_profile_name_p != NULL) && (len > 0))
    {
        g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].dcs = PH_DCS_ASCII;
        if (len >= PROV_PROFILE_PROFILE_NAME_LEN)
        {
            len = PROV_PROFILE_PROFILE_NAME_LEN - 1;
        }

        memcpy(
            g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].profile_name,
            g_prov_profile_name_p,
            len);
        g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].profile_name[len] = '\0';
        jdd_MemFree(g_prov_profile_name_p);
        g_prov_profile_name_p = NULL;
    }
    else
    {

        S8 temp_unicode[8] = {0, };
        S8 temp[4] = {0, };
        S8 *profile_name_p = NULL;
        S8 temp_name[PROV_PROFILE_PROFILE_NAME_LEN] = {0, };

        profile_name_p = (S8*) get_string((MMI_ID_TYPE) STR_ID_PROF_HDLR_PROFILE);

        len = UCS2Strlen((S8*) profile_name_p);
        len += 2;   /* For index */

        if (GetUCS2Flag((S8*) profile_name_p))
        {
            if (len >= (PROV_PROFILE_PROFILE_NAME_LEN / 2))
            {
                len = (PROV_PROFILE_PROFILE_NAME_LEN / 2) - 1;
            }
            sprintf(temp, "%d", (index + 1));
            AnsiiToUnicodeString((S8*) temp_unicode, (S8*) temp);
            UCS2Strncpy(temp_name, profile_name_p, (len - 2));
            UCS2Strcat((S8*) temp_name, (S8*) temp_unicode);

            g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].dcs = PH_DCS_UCS;
            memcpy(
                g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].profile_name,
                temp_name,
                (len + 1) * ENCODING_LENGTH);
        }
        else
        {
            if (len >= (PROV_PROFILE_PROFILE_NAME_LEN))
            {
                len = PROV_PROFILE_PROFILE_NAME_LEN - 1;
            }
            UnicodeNToAnsii((S8*) temp_name, (S8*) profile_name_p, len - 2);
            /* temp_name[len] = '\0'; */

            sprintf(temp, "%d", (index + 1));
            strcat(temp_name, temp);
            len = strlen(temp_name);
            memcpy(
                g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].profile_name,
                temp_name,
                len + 1);
            g_ph_cntx.ph_wap_prof_name_arr_p->nvram_wap_profile_name_array[index].dcs = PH_DCS_ASCII;
        }

    }

    WriteRecord(
        NVRAM_EF_WAP_PROFILE_NAMES_LID,
        1,
        g_ph_cntx.ph_wap_prof_name_arr_p,
        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
        &error);

}


/*****************************************************************************
 * FUNCTION
 *  mmi_ph_update_prov_nvram_mms_profile
 * DESCRIPTION
 *  Updates the NVRAM MMS profile data(Both Name & Content) with the provisioing data for the index selected
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ph_update_prov_nvram_mms_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error = 0;
    U32 len = 0;

    U32 index = g_ph_cntx.profile_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ph_cntx.ph_mms_profile_p->data_account = g_ph_cntx.profile_display_p->data_account;
    WriteRecord(
        NVRAM_EF_MMS_PROFILE_CONTENT_LID,
        (U16) (index + 1),
        g_ph_cntx.ph_mms_profile_p,
        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
        &error);

    if (index == g_ph_cntx.mms_actived_profile_index)
    {
        memcpy(g_ph_cntx.ph_activated_mms_profile, g_ph_cntx.ph_mms_profile_p, NVRAM_EF_MMS_PROFILE_CONTENT_SIZE);
    #if defined(MMS_SUPPORT)        /* dilip */
        mmi_jmms_set_profile();
    #endif 
    }

    len = strlen((PS8) g_prov_profile_name_p);
    if ((g_prov_profile_name_p != NULL) && (len > 0))
    {
        g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].dcs = PH_DCS_ASCII;
        if (len >= PROV_PROFILE_PROFILE_NAME_LEN)
        {
            len = PROV_PROFILE_PROFILE_NAME_LEN - 1;
        }
        memcpy(
            g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].profile_name,
            g_prov_profile_name_p,
            len);
        g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].profile_name[len] = '\0';
        jdd_MemFree(g_prov_profile_name_p);
        g_prov_profile_name_p = NULL;
    }
    else
    {

        S8 temp_unicode[8] = {0, };
        S8 temp[4] = {0. };
        S8 *profile_name_p = NULL;
        S8 temp_name[PROV_PROFILE_PROFILE_NAME_LEN] = {0, };

        profile_name_p = (S8*) GetString((MMI_ID_TYPE) STR_ID_PROF_HDLR_PROFILE);
        len = UCS2Strlen((S8*) profile_name_p);
        len += 2;   /* For index */

        if (GetUCS2Flag((S8*) profile_name_p))
        {
            if (len >= (PROV_PROFILE_PROFILE_NAME_LEN / 2))
            {
                len = (PROV_PROFILE_PROFILE_NAME_LEN / 2) - 1;
            }
            sprintf(temp, "%d", (index + 1));
            AnsiiToUnicodeString((S8*) temp_unicode, (S8*) temp);
            UCS2Strncpy(temp_name, profile_name_p, (len - 2));
            UCS2Strcat((S8*) temp_name, (S8*) temp_unicode);

            g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].dcs = PH_DCS_UCS;
            memcpy(
                g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].profile_name,
                temp_name,
                (len + 1) * ENCODING_LENGTH);
        }
        else
        {
            if (len >= (PROV_PROFILE_PROFILE_NAME_LEN))
            {
                len = PROV_PROFILE_PROFILE_NAME_LEN - 1;
            }
            UnicodeNToAnsii((S8*) temp_name, (S8*) profile_name_p, len - 2);
            /* temp_name[len] = '\0'; */

            sprintf(temp, "%d", (index + 1));
            strcat(temp_name, temp);
            len = strlen(temp_name);
            memcpy(
                g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].profile_name,
                temp_name,
                len + 1);
            g_ph_cntx.ph_mms_prof_name_arr_p->nvram_mms_profile_name_array[index].dcs = PH_DCS_ASCII;
        }

    }

    WriteRecord(
        NVRAM_EF_MMS_PROFILE_NAMES_LID,
        1,
        g_ph_cntx.ph_mms_prof_name_arr_p,
        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
        &error);

}
#endif

//fengzb add for jataayu profile begin
/*****************************************************************************
 * FUNCTION
 *  mmi_brw_restore_default_profile_settings
 * DESCRIPTION
 *  Restore the default profile settings
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_brw_restore_default_profile_settings(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 index = 0;
	S16 error = 0;
	U16 activateWapProfileIndex = 0;
	U8 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
mmi_trace(1,"mmi_brw_restore_default_profile_settings");
#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-30 */
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		for (index = 1; index <= NVRAM_WAP_PROFILE_MAX; index++)
		{
			WriteRecord(
				NVRAM_EF_WAP_PROFILE_CONTENT_LID + i,
				index,
				&NVRAM_EF_WAP_PROFILE_CONTENT_DEFAULT[index - 1],
				NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
				&error);
		}
		
		WriteRecord(
			NVRAM_EF_WAP_PROFILE_NAMES_LID + i,
			1,
			NVRAM_EF_WAP_PROFILE_NAMES_DEFAULT,
			NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
			&error);
	}

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		WriteValue(NVRAM_PH_ACTIVATED_WAP_PROFILE_INDEX + i, &activateWapProfileIndex, DS_BYTE, &error);
	}
	
#endif

}


/*****************************************************************************
 * FUNCTION
 *  mmi_brw_mms_restore_default_profile_settings
 * DESCRIPTION
 *  Restore the default profile settings
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_brw_mms_restore_default_profile_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 index = 0;
    S16 error = 0;
	
    U16 activateMMSProfileIndex = 0;
	U8 i;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-30 */
	mmi_trace(1,"mmi_brw_mms_restore_default_profile_settings");
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		for (index = 1; index <= NVRAM_MMS_PROFILE_MAX; index++)
		{
			WriteRecord(
					NVRAM_EF_MMS_PROFILE_CONTENT_LID + i,
					index,
					&NVRAM_EF_MMS_PROFILE_CONTENT_DEFAULT[index - 1],
					NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
					&error);
		}

		WriteRecord(
					NVRAM_EF_MMS_PROFILE_NAMES_LID + i,
					1,
					NVRAM_EF_MMS_PROFILE_NAMES_DEFAULT,
					NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
					&error);
	}
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		WriteValue(NVRAM_PH_ACTIVATED_MMS_PROFILE_INDEX + i, &activateMMSProfileIndex, DS_BYTE, &error);
	}
#endif
}
//fengzb add for jataayu profile end

#endif /* defined (JATAAYU_SUPPORT) && (defined (WAP_SUPPORT) || (defined (MMS_SUPPORT))) */ 
#endif /* _JPROFILESTORAGEHANDLER_C */ 

