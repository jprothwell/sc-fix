 /***************************************************************************
 *
 * File Name : jwap_interface.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights
 * in and to this Software or any part of this (including without limitation
 * any images, photographs, animations, video, audio, music, text and/or
 * "applets," incorporated into the Software), herein mentioned to as
 * "Software", the accompanying printed materials, and any copies of the
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu")
 * or Jataayu's suppliers as the case may be. The Software is protected by
 * copyright, including without limitation by applicable copyright laws,
 * international treaty provisions, other intellectual property laws and
 * applicable laws in the country in which the Software is being used.
 * You shall not modify, adapt or translate the Software, without prior
 * express written consent from Jataayu. You shall not reverse engineer,
 * decompile, disassemble or otherwise alter the Software, except and
 * only to the extent that such activity is expressly permitted by
 * applicable law notwithstanding this limitation. Unauthorized reproduction
 * or redistribution of this program or any portion of it may result in severe
 * civil and criminal penalties and will be prosecuted to the maximum extent
 * possible under the law. Jataayu reserves all rights not expressly granted.
 *
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT,
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/

 /**
 * @file		jwap_interface.c
 * @ingroup	jWAP
 * @brief
 *
 **/

#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_interface.c"
#ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/


 /***************************************************************************
 * User Include Files
 **************************************************************************/


#include "jwap_inc.h"


#include "jdi_browserengine.h"
#include "jwap_cb.h"
#include "jdi_cutils.h"
#include "jdi_arr.h"
#include "jdi_displayinfo.h"
#include "jddex_platform.h"
#include "jdi_history.h"

#include "jddex_mmieventhandler.h"
#include "jddex_wnddefaults.h"

#include "jdi_communicator.h"
#include "jwap_bookmark.h"
#include "jdd_netdatatype.h"
#include "jdd_netapi.h"
#include "jdd_dataconnection.h"
#include "jddex_dataconnection.h"

#include "custom_nvram_editor_data_item.h"
#include "nvram_user_defs.h"
#include "custom_mmi_default_value.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "ts.h"
#include "jmms_init.h"
#include "jdd_deconfig.h"
#include "custom_nvram_editor_data_item.h"
#include "jprofilehandlertypes.h"
#include "jprofilehandlerprots.h"
#include "settingprofile.h"
#include "jdi_downloadclient.h"
#ifdef CS_DC_ENABLE

#include "jwap_downloadclient.h"
#endif

/***************************************************************************
* Macros
**************************************************************************/


/***************************************************************************
* Global variables declarations
**************************************************************************/
#define JWAP_ADD_COOKIE_HEADER

JWAP_APP_INFO *g_pstJWapAppInfo ;
ST_HISTORY_DOMAIN* g_pstHistoryDomain;
JC_UINT32 g_ScrollPrevCarY = -1;

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/


/***************************************************************************
* All Global Function Definitions
**************************************************************************/
extern JC_HANDLE	jMms_getCommHandle () ;
extern void jMMS_processAppEvent(JC_EVENT_DATA *pEventData) ;

void jWap_setUpdateCore (ST_WAP_SETTINGS	*pstWapSettInfo)
{
	ST_BE_SET_OPTION  					stBeSetOption ;
	ST_BE_OPTION_CACHE  				stCacheOption ;
	ST_BE_OPTION_COOKIE  				stCookieOption ;
	ST_BE_OPTION_GRAPHICS				stGraphics;
	INT iErr = JC_OK ;
       nvram_wap_profile_content_struct *profCont ;
	/* Update Timeout Option */
	if (g_pstJWapAppInfo->stWapSettings.stWAPOptions.uiTimeout !=
		pstWapSettInfo->stWAPOptions.uiTimeout)
	{
		jc_memset (&stBeSetOption, 0, sizeof(ST_BE_SET_OPTION));
		stBeSetOption.iMaxRetries = 2;
		stBeSetOption.iMaxSockets = 3;
		stBeSetOption.iMaxTcpRecvBuf = (1024 * 5) ;
        
              profCont =  mmi_ph_get_activated_wap_profile () ;
              if(PH_CONN_TYPE_CONNECTION_OREINTED == profCont->conn_type || PH_CONN_TYPE_CONNECTION_OREINTED_SECURE== profCont->conn_type)
              {
                // wap connection . 
		   stBeSetOption.iTimeOut = pstWapSettInfo->stWAPOptions.uiTimeout / (stBeSetOption.iMaxRetries +1);
               }
              else
                {
                // http  connection
		    stBeSetOption.iTimeOut = pstWapSettInfo->stWAPOptions.uiTimeout ;
                }
              

		mmi_trace (1, "JDD_LOG: jWap_setUpdateCore >> Setting Timeout to %d", stBeSetOption.iTimeOut) ;
		iErr = jdi_BESetOption(g_pstJWapAppInfo->hBECore, &stBeSetOption);
		mmi_trace (1, "JDD_LOG: jWap_setUpdateCore >> jdi_BESetOption Ret: %X", JRET (iErr)) ;
	}

	/* Update Graphics Option */
	if (g_pstJWapAppInfo->uiWinId != 0 && (g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableGraphics!=
		pstWapSettInfo->stWAPOptions.bEnableGraphics))
	{
		jc_memset (&stGraphics, 0, sizeof(ST_BE_OPTION_GRAPHICS)) ;
		stGraphics.bIsVisibility = pstWapSettInfo->stWAPOptions.bEnableGraphics ;
		stGraphics.bIsSmartImage = E_FALSE ;
		mmi_trace (1, "@@@ Setting Graphics to %d", stGraphics.bIsVisibility) ;
		iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_GRAPHICS, &stGraphics) ;
	}

	/* Update Cache Option */
	if (g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableCache !=
		pstWapSettInfo->stWAPOptions.bEnableCache)
	{
		jc_memset (&stCacheOption, 0, sizeof (ST_BE_OPTION_CACHE)) ;
		stCacheOption.bIsGeneralCacheEnabled = pstWapSettInfo->stWAPOptions.bEnableCache ;
		mmi_trace (1, "JDD_LOG: jWap_setUpdateCore >> Setting Cache to %d", stCacheOption.bIsGeneralCacheEnabled) ;
		iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_CACHE, &stCacheOption) ;
		mmi_trace (1, "JDD_LOG: jWap_setUpdateCore >> jdi_BESetProperties Ret: %X @@@", JRET (iErr)) ;
	}

	/* Update Cookie Option */
	if (g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableCookie !=
		pstWapSettInfo->stWAPOptions.bEnableCookie)
	{
		jc_memset (&stCookieOption, 0, sizeof (ST_BE_OPTION_COOKIE)) ;
		stCookieOption.bIsCookieEnabled = pstWapSettInfo->stWAPOptions.bEnableCookie ;

		mmi_trace (1, "JDD_LOG: jWap_setUpdateCore >> Setting Cookie to %d", stCookieOption.bIsCookieEnabled) ;
		iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_COOKIE, &stCookieOption) ;
		mmi_trace (1, "JDD_LOG: jWap_setUpdateCore >> jdi_BESetProperties Ret: %X @@@", JRET (iErr)) ;
	}

	/* Update Application Context */
	jc_memcpy (&g_pstJWapAppInfo->stWapSettings.stWAPOptions, pstWapSettInfo, sizeof (ST_WAP_SETTINGS)) ;

}


void jWap_updateDataConnSettings (ST_BE_PROFILE *pstProxySett, \
	ST_GENERIC_DATA_CONN *pstGprsSett, JC_CHAR *pmHomePage)
{
	INT iErr ;

	mmi_trace (1, "@@@ APN: %s", pstGprsSett->uDataConnType.stGPRSDataConn.acAPN) ;
	mmi_trace (1, "@@@ Proxy: %s", pstProxySett->acHostName) ;
	mmi_trace (1, "@@@ Port: %d", pstProxySett->uiPortNum) ;

	/* Clear any Data Connection Info present */
	jdi_BEDisconnect (g_pstJWapAppInfo->hBECore) ;

	iErr = jdi_BESetDataConn (g_pstJWapAppInfo->hBECore, pstGprsSett) ;

/****************************/
	mmi_trace (1, "@@@ jdi_BESetDataConn: %x", JRET (iErr)) ;
	if (!iErr)
	{
		iErr = jdi_BESetProfile (g_pstJWapAppInfo->hBECore, pstProxySett) ;
		mmi_trace (1, "@@@ jdi_BESetProfile: %x", JRET (iErr)) ;
	}

	if (pmHomePage)
	{
		JC_INT8 *pcHome ;
		jdi_CUtilsTcharToChar (NULL, pmHomePage, &pcHome) ;
		//mmi_trace (1, "@@@ Home : %s @@@", pcHome) ;
		//jc_wstrncpy (g_pstJWapAppInfo->amHomePage, pmHomePage, JWAP_MAX_URL_LEN) ;
		jdi_CUtilsCharToTcharMem (pcHome,JWAP_MAX_URL_LEN, g_pstJWapAppInfo->amHomePage) ;
		jdd_MemFree (pcHome) ;
	}
	else
	{
		//jc_memset (g_pstJWapAppInfo->amHomePage, 0, JWAP_MAX_URL_LEN) ;
		JC_CHAR *pmHome ;

		mmi_trace (1, "@@@ Home Page: NULL @@@") ;
		jdi_CUtilsCharToTchar (NULL,"file://t/home.html", &pmHome) ;
		if (pmHome)
		{
			jc_wstrncpy (g_pstJWapAppInfo->amHomePage, pmHome, JWAP_MAX_URL_LEN) ;
			jdd_MemFree (pmHome) ;
		}
	}
}

#ifdef MMI_ON_HARDWARE_P
inline U8*	jWap_getInputBuff ()
{
	return (g_pstJWapAppInfo->pcInputBuff) ;
}
#else
U8*	jWap_getInputBuff ()
{
	return (g_pstJWapAppInfo->pcInputBuff) ;
}
#endif /* #ifdef MMI_ON_HARDWARE_P */

JC_BOOLEAN jWap_isBack ()
{
	//JC_HANDLE hNavig ;
	ST_BE_NAVIGATION_INFO stNavigInfo ;

	//iErr = jdi_BEGetNavigationHandle (g_pstJWapAppInfo->hBECore, &hNavig) ;

	jc_memset (&stNavigInfo, 0, sizeof (ST_BE_NAVIGATION_INFO)) ;
	jdi_BEGetNavigationInfo (g_pstJWapAppInfo->hBECore, &stNavigInfo) ;

	return (stNavigInfo.bBackNavigation) ;
}


JC_BOOLEAN jWap_isNext ()
{
	ST_BE_NAVIGATION_INFO stNavigInfo ;

	//iErr = jdi_BEGetNavigationHandle (g_pstJWapAppInfo->hBECore, &hNavig) ;

	jc_memset (&stNavigInfo, 0, sizeof (ST_BE_NAVIGATION_INFO)) ;
	jdi_BEGetNavigationInfo (g_pstJWapAppInfo->hBECore, &stNavigInfo) ;

	return (stNavigInfo.bForwardNavigation) ;

}


INT jWap_exitApplication ()
{
	INT iErr ;

	mmi_trace (1, "@@@ jWap_exitApplication @@@") ;

	iErr = JC_OK ;

	return (iErr) ;
}

INT jWap_refresh ()
{
	INT iErr ;

	iErr = jdi_BEReloadPage (g_pstJWapAppInfo->hBECore) ;
	mmi_trace (1, "@@@ jdi_BEReloadPage: %x", JRET (iErr)) ;

	return (iErr) ;
}

void jWap_back ()
{
	INT iErr ;

	mmi_trace (1, "@@@ jWap_back @@@") ;

	iErr = jdi_BEHandleNavigation (g_pstJWapAppInfo->hBECore, E_BE_NAVIGATION_BACK) ;
	mmi_trace (1, "@@@ jdi_BEHandleNavigation: %x", JRET (iErr)) ;
}


INT jWap_stop ()
{
	INT iErr ;

	iErr = jdi_BEStopRequest (g_pstJWapAppInfo->hBECore) ;
	mmi_trace (1, "@@@ jdi_BEStopRequest: %x", JRET (iErr)) ;

	return (iErr) ;
}

INT jWap_next ()
{
	INT iErr ;

	mmi_trace (1, "@@@ jWap_next @@@") ;

	iErr = jdi_BEHandleNavigation (g_pstJWapAppInfo->hBECore, E_BE_NAVIGATION_FORWARD) ;
	mmi_trace (1, "@@@ jdi_BEHandleNavigation: %x", JRET (iErr)) ;

	return (iErr) ;
}

#ifdef MMI_ON_HARDWARE_P
void jWAP_SET_STATE(E_JWAP_STATE X)
{
	g_pstJWapAppInfo->eJwapState = X ;
}

E_JWAP_STATE jWAP_GET_STATE ()
{
	return (g_pstJWapAppInfo->eJwapState) ;
}
#else
void jWAP_SET_STATE(E_JWAP_STATE X)
{
	g_pstJWapAppInfo->eJwapState = X ;
}

E_JWAP_STATE jWAP_GET_STATE ()
{
	return (g_pstJWapAppInfo->eJwapState) ;
}
#endif /* #ifdef MMI_ON_HARDWARE_P */

void* jWap_Get_Brw_Handle()
{
	return (g_pstJWapAppInfo->hBECore) ;
}

void jWap_initAppSettingsInfo ()
{
	INT  iErr ;

	iErr = jWap_readFromSettFile (&g_pstJWapAppInfo->stWapSettings) ;

	/* Some problem while Reading. Try to Reset contents */
	if (iErr)
	{
		jWap_updateSettingsFile (&g_pstJWapAppInfo->stWapSettings) ;
	}
}

INT jWap_config_connProfile ()
{
	INT						iErr = 0;

	ASSERT (g_pstJWapAppInfo->hBECore) ;

	if (g_pstJWapAppInfo->bIsDCAvailable)
	{
		iErr = JC_OK ;
	}
	else
	{
#if 0
		ST_GENERIC_DATA_CONN    stDataCon ;
		ST_BE_PROFILE           stBEProfile ;

		jc_memset (&stDataCon, 0, sizeof (ST_GENERIC_DATA_CONN)) ;

		stDataCon.eBearerType = E_DATA_CONN_GPRS_BEARER_TYPE ;
		jc_strcpy(stDataCon.uDataConnType.stGPRSDataConn.acAPN,
	          "cmwap") ;
		stDataCon.uiConnID = 1 ;

		iErr = jdi_BESetDataConn (g_pstJWapAppInfo->hBECore, &stDataCon) ;
		mmi_trace (1, (PSTR)"@@@ jdi_BESetDataConn-> %x", JRET(iErr)) ;

		if (!iErr)
		{
			jc_memset (&stBEProfile, 0, sizeof (ST_BE_PROFILE)) ;

			/* Proxy IP and Port */
			/*
			jc_strcpy (stBEProfile.acHostName, "") ;
			stBEProfile.uiPortNum = 80 ;
			*/

			//stBEProfile.eMode = E_BE_WPHTTP_DIRECT ;
			stBEProfile.eMode = E_COMM_WPHTTP_PROXY ;
			jc_strcpy (	stBEProfile.acHostName, "10.0.0.172") ;
			stBEProfile.uiPortNum = 80 ;
			iErr = jdi_BESetProfile (g_pstJWapAppInfo->hBECore, &stBEProfile) ;
			mmi_trace (1, (PSTR)"@@@ jdi_BESetProfile-> %x", JRET(iErr)) ;

			/* URL should be copied from Settings */
			//jdi_CUtilsCharToTchar (NULL,"http://202.54.124.161/home/index.php", &pmDest) ;
			//jdi_CUtilsCharToTcharMem ("file://t/home.html", JWAP_MAX_URL_LEN, &g_pstJWapAppInfo->amHomePage) ;
			//jdi_CUtilsCharToTchar (NULL,"http://wap.yahoo.com", &pmDest) ;

			jdi_CUtilsCharToTcharMem ("file://t/home.html", JWAP_MAX_URL_LEN, g_pstJWapAppInfo->amHomePage) ;
			//mmi_trace (1, "@@@ Home Page: %s @@@", pcHome) ;
		}
#else
{
		ST_BE_SET_OPTION  		stBeSetOption ;
        
              nvram_wap_profile_content_struct *profCont ;

		g_pstJWapAppInfo->bIsDCAvailable = E_TRUE ;
		
		jWap_readUpdateDataConnInfo () ;
		
		jc_memset (&stBeSetOption, 0x00, sizeof(ST_BE_SET_OPTION));
		stBeSetOption.iMaxRetries = 2;
		stBeSetOption.iMaxSockets = 3;
		stBeSetOption.iMaxTcpRecvBuf = (1024 * 5) ;
        
              profCont =  mmi_ph_get_activated_wap_profile () ;
              if(PH_CONN_TYPE_CONNECTION_OREINTED == profCont->conn_type || PH_CONN_TYPE_CONNECTION_OREINTED_SECURE== profCont->conn_type)
              {
                // wap connection . 
		stBeSetOption.iTimeOut = g_pstJWapAppInfo->stWapSettings.stWAPOptions.uiTimeout / (stBeSetOption.iMaxRetries +1);
               }
              else
                {
                // http  connection
		stBeSetOption.iTimeOut = g_pstJWapAppInfo->stWapSettings.stWAPOptions.uiTimeout ;
                }
              

		iErr = jdi_BESetOption(g_pstJWapAppInfo->hBECore, &stBeSetOption);
		#ifdef __MMI_MULTI_SIM__
		mmi_trace (1, "JDD_LOG: jWap_config_connProfile >> jdi_BESetOption: %x. Timeout: %d, simid [%d]",
			JRET (iErr), stBeSetOption.iTimeOut, g_pstJWapAppInfo->simid) ;
		#else
		mmi_trace (1, "JDD_LOG: jWap_config_connProfile >> jdi_BESetOption: %x. Timeout: %d",
			JRET (iErr), stBeSetOption.iTimeOut) ;
		#endif
}
#endif
//		}
	}
	return (iErr) ;
}

INT jWap_config_dispSettings ()
{
	ST_BE_OPTION_AUDIO          		stAudio ;
	ST_BE_OPTION_GRAPHICS      			stGraphics ;
	ST_BE_OPTION_VISIBLE        		stVisibility ;
	ST_BE_OPTION_MAGNIFICATION      	stMagnification ;
	ST_BE_OPTION_IMAGE_SELECTION    	stImageSelection ;
	ST_BE_OPTION_REFERER_HEADER_MODE  	stRefererMode ;
	ST_BE_DISPLAY_MODE_INFO       		stDisplayModeInfo ;
	ST_BE_OPTION_STYLE         			stStyleInfo ;
	ST_BE_OPTION_SCROLL_MODE      		stScrollMode ;
	ST_ARR_RULES            			stArrRules ;
	INT									iErr ;

	ASSERT (g_pstJWapAppInfo->hBECore) ;
	/* Audio Properties */
	stAudio.bEnable       =  E_TRUE ;
	stAudio.bIsSmartAudio = E_FALSE ;
	iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_AUDIO, &stAudio) ;

	/* Image Properties */
	if (!iErr)
	{
	  jc_memset (&stGraphics, 0, sizeof(ST_BE_OPTION_GRAPHICS)) ;
	  stGraphics.bIsVisibility = g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableGraphics ;
	  stGraphics.bIsSmartImage = E_FALSE ;
	  mmi_trace (1, "@@@ Setting Graphics to %d", stGraphics.bIsVisibility) ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_GRAPHICS, &stGraphics) ;
	  mmi_trace (1, "@@@ jdi_BESetProperties Ret: %x @@@", JRET (iErr)) ;
	}
	/* Visibility */
	if (!iErr)
	{
	  jc_memset (&stVisibility, 0, sizeof(ST_BE_OPTION_VISIBLE)) ;
	  stVisibility.bIsVisibility = E_TRUE ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_VISIBILITY, &stVisibility) ;
	}
	/* Magnification */
	if (!iErr)
	{
	  jc_memset (&stMagnification, 0, sizeof(ST_BE_OPTION_MAGNIFICATION)) ;
	  stMagnification.uhMagnification = 100 ;
	  stMagnification.uhTextMagnification = 100 ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_MAGNIFICATION, &stMagnification) ;
	}
	/* Image Selection */
	if (!iErr)
	{
	  jc_memset (&stImageSelection, 0, sizeof(ST_BE_OPTION_IMAGE_SELECTION)) ;
	  stImageSelection.bSelectable = E_TRUE ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_IMAGE_SELECTION, &stImageSelection) ;
	}
	/* DisplayMode */
	if (!iErr)
	{
	  jc_memset (&stDisplayModeInfo, 0, sizeof(ST_BE_DISPLAY_MODE_INFO)) ;
	  stDisplayModeInfo.eDisplayMode = E_DISPLAY_FIT_MODE ;
	  stDisplayModeInfo.uiMaxWidth = 0 ;
	  stDisplayModeInfo.bTextOnly = E_FALSE ;
	  stDisplayModeInfo.bStrictXHTML = E_FALSE ;
	  stDisplayModeInfo.bDisplayDirectImage = E_FALSE ;
	  stDisplayModeInfo.bDisplayAHref = E_FALSE ;
	  stDisplayModeInfo.bMetaRefresh = E_TRUE ;
	  stDisplayModeInfo.bNoDisplayAdjust = E_FALSE ;
	  stDisplayModeInfo.uiMinFontSize =  1 ;
	  stDisplayModeInfo.eLinkAccessKey =  E_ACCESSKEY_NONE ;
	  stDisplayModeInfo.eFormAccessKey = E_ACCESSKEY_NONE ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_DISPLAY_MODE, &stDisplayModeInfo) ;
	}
	/* ARR Rules */
	if (!iErr)
	{
	  jc_memset (&stArrRules, 0, sizeof (ST_ARR_RULES)) ;
	  stArrRules.eBgImage = E_ARR_BGIMAGE_IGNORE ;
      stArrRules.eButton = E_ARR_BUTTON_TRUNCATE_TEXT ;
      stArrRules.eFloatText = E_ARR_FLOATTEXT_MOVE_BEYOND ;
      stArrRules.eFontSize = E_ARR_FONTSIZE_IGNORE ;
      stArrRules.eForm = E_ARR_FORM_FIT_ON_EXCEED ;
      stArrRules.eFrame = E_ARR_FRAME_LINKS ;
      stArrRules.eGeneral = E_ARR_GENERAL_FIT_ON_EXCEED ;
      stArrRules.eHidden = E_ARR_HIDDEN_COLLAPSE ;
      stArrRules.eIFrame = E_ARR_IFRAME_LINK ;
      stArrRules.eImage = E_ARR_IMAGE_ASPECT_FIT ;
      stArrRules.eImageMap = E_ARR_IMAGEMAP_SAME_WINDOW ;
      stArrRules.eIndentation = E_ARR_INDENTATION_MAX_LEVEL ;
      stArrRules.eLineHeight = E_ARR_LINEHEIGHT_OVERFLOW ;
      stArrRules.eLink = E_ARR_LINK_IGNORE ;
      stArrRules.eMargin = E_ARR_MARGIN_SCALE_RATIO ;
      stArrRules.eNoframe = E_ARR_NOFRAME_IGNORE ;
      stArrRules.eOverflow = E_ARR_OVERFLOW_IGNORE ;
      stArrRules.ePadding = E_ARR_PADDING_SCALE_RATIO ;
      stArrRules.ePercentage = E_ARR_PERCENTAGE_IGNORE ;
      stArrRules.ePosition = E_ARR_POSITION_IGNORE ;
      stArrRules.eTable = E_ARR_TABLE_2D_FIT_SCREEN ;
      stArrRules.eTextIndentation = E_ARR_TEXT_INDENTATION_IGNORE ;
      stArrRules.eWrapText = E_ARR_WRAPTEXT_WRAP ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_ARR_RULE, &stArrRules) ;
	}
	/* Refresh Mode */
	if (!iErr)
	{
	  jc_memset (&stRefererMode, 0, sizeof (ST_BE_OPTION_REFERER_HEADER_MODE)) ;
	  stRefererMode.eRefererHeaderMode = E_REFERER_ALL_REQUESTS ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_REFERER_HEADER_MODE, &stRefererMode) ;
	}
	/* Style mode */
	if (!iErr)
	{
		jc_memset (&stStyleInfo, 0, sizeof (ST_BE_OPTION_STYLE)) ;
		stStyleInfo.uiFontSize = 10 ;
		iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_STYLE, &stStyleInfo) ;
	}
	/* Scroll Mode */
	if (!iErr)
	{
	  jc_memset (&stScrollMode, 0, sizeof (ST_BE_OPTION_SCROLL_MODE)) ;
	  stScrollMode.eMode = E_SCROLL_MEDIUM ;
	  stScrollMode.bIntelligentPointer = E_FALSE ;
	  stScrollMode.bFourKeyScrolling = E_TRUE ;
	  stScrollMode.uiLevel = 10 ;
	  iErr = jdi_BESetProperties (g_pstJWapAppInfo->hBECore, E_BE_SET_SCROLL_MODE, &stScrollMode) ;
	}
	/* window size */
	if (!iErr)
	{
	  //iErr = jdi_BESizeTo (g_pstJWapAppInfo->hBECore, 128, 120) ;
	  // to be changed with a macro later
	  //iErr = jdi_BESizeTo (g_pstJWapAppInfo->hBECore, 240, 268) ;
	  /* iErr = jdi_BESizeTo (g_pstJWapAppInfo->hBECore, jwap_GetLcdWidth(), (jwap_GetLcdHeight() \
	  						- jwap_GetSoftkeyHeight() - jwap_GetTitleHeight())) ;*/
	}

	return iErr ;
}

#if defined(__MMI_TOUCH_SCREEN__)
void jWap_Generic_Pen_Handler (mmi_keypads_enum keyCode, mmi_pen_point_struct *stPenCooridinates)
{	
	JC_RETCODE			rCode = JC_OK;
	ST_BE_CONTROL_INFO		stCtrlInfo = {0, } ;
	ST_BE_MOUSE_INFO	stBEMouseInfo = {0, } ;
	SCROLL_INFO			m_stScrollInfo = {0, } ;
	JC_RECT				stStylusCoordinate = {stPenCooridinates->x, stPenCooridinates->y, 0, 0} ;
	JC_RECT				stPrimaryWindowDim = {0, jwap_GetTitleHeight (), jwap_GetLcdWidth () - jwap_GetScrollbarWidth(),
		jwap_GetLcdHeight () - jwap_GetSoftkeyHeight () - jwap_GetTitleHeight ()} ;

	mmi_trace (1, "JDD_LOG: jWap_Generic_Pen_Handler >> stPenCooridinates KEY CODE [%d] [%d], [%d] and Primary [%d], [%d], [%d], [%d]",
		keyCode,
		stPenCooridinates->x, stPenCooridinates->y,
		stPrimaryWindowDim.iLeft, stPrimaryWindowDim.iTop,
		stPrimaryWindowDim.uiHeight, stPrimaryWindowDim.uiWidth) ;
	if ((JC_INT32)stPrimaryWindowDim.iLeft <= stPenCooridinates->x &&
			(JC_INT32)(stPrimaryWindowDim.iLeft + stPrimaryWindowDim.uiWidth) > stPenCooridinates->x &&
			(JC_INT32)stPrimaryWindowDim.iTop <= stPenCooridinates->y &&
			(JC_INT32)(stPrimaryWindowDim.iTop + stPrimaryWindowDim.uiHeight) >= stPenCooridinates->y)
	{
		if (KEY_STAR == keyCode)
		{			
			stBEMouseInfo.eType = E_MOUSE_WINDOW ;
			stBEMouseInfo.uMouseData.stMouseEvent.eAction = E_MOUSE_CLICK ;
			stBEMouseInfo.uMouseData.stMouseEvent.uiWindowID = g_pstJWapAppInfo->uiWinId ;
			stBEMouseInfo.uMouseData.stMouseEvent.uiX = stPenCooridinates->x ;
			stBEMouseInfo.uMouseData.stMouseEvent.uiY = stPenCooridinates->y - stPrimaryWindowDim.iTop ;
			
			//if the end display is recived then widgets like input box/list box.combo box can be allowed to function
			if (E_WAP_END_DISPLAY_RECVD == jWAP_GET_STATE ())
			{
				if (E_FALSE == jddex_widgetHandleKeyPress (g_pstJWapAppInfo->hBECore, E_BE_ACTION, \
						JC_NULL, &stStylusCoordinate))
				{
					rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_MOUSE_EVENT, &stBEMouseInfo) ;
					mmi_trace (1, "JDD_LOG: jWap_Generic_Pen_Handler >> Widgets too consumed the event") ;
				}
			}
			else
			{
				
				JC_RECT			stFocusedCtrlRect = {0, } ;
			   	rCode = jdi_BEGetFocusedControlInfo (g_pstJWapAppInfo->hBECore, &stCtrlInfo) ;

				if (rCode == JC_OK )
				{
					if( stCtrlInfo.eType == E_LIST_BOX || stCtrlInfo.eType == E_COMBO_BOX)
					{
						JDDEX_DEVICE_CHAR	stDeviceChar = {0, } ;
						jddex_GetDeviceProp (JDDEX_BROWSER_APP, &stDeviceChar) ;
						jddex_GetWidgetRect(stCtrlInfo.uiCtrlID,&stFocusedCtrlRect);
						
						stFocusedCtrlRect.iLeft += stDeviceChar.uiLeft ;
						stFocusedCtrlRect.iTop += stDeviceChar.uiTop ;
						
						if( stStylusCoordinate.iLeft >= stFocusedCtrlRect.iLeft &&
						stStylusCoordinate.iLeft < stFocusedCtrlRect.iLeft + (JC_INT32)stFocusedCtrlRect.uiWidth &&
						stStylusCoordinate.iTop >= stFocusedCtrlRect.iTop &&
						stStylusCoordinate.iTop < stFocusedCtrlRect.iTop + (JC_INT32)stFocusedCtrlRect.uiHeight)
						{
							//ignore the keys if the focussed control is list/combo box and there is a stylus action in that region
						}
						else
						{
							rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_MOUSE_EVENT, &stBEMouseInfo) ;	
						}
					}
					else
					{
						rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_MOUSE_EVENT, &stBEMouseInfo) ;	
					}
				}
			}
		}
	}
	else if ((JC_INT32)(stPrimaryWindowDim.iLeft + stPrimaryWindowDim.uiWidth) < stPenCooridinates->x &&
			(JC_INT32)stPrimaryWindowDim.iTop <= stPenCooridinates->y &&
			(JC_INT32)(stPrimaryWindowDim.iTop + stPrimaryWindowDim.uiHeight) >= stPenCooridinates->y)
	{
		JC_RECT		m_stScrollCar ={0,} ;
		JC_DOUBLE	dFactor = 0;
		
		jdd_MMIGetScrollInfo (g_pstJWapAppInfo->uiWinId,
			g_pstJWapAppInfo->uiScrollBarWinId, &m_stScrollInfo) ;
		jdd_MMIGetScrollCarInfo (g_pstJWapAppInfo->uiWinId,
			g_pstJWapAppInfo->uiScrollBarWinId, &m_stScrollCar) ;

		/*	Check what proper condition to be used and then replace this	*/		
		if (0 == m_stScrollInfo.uiMaxPos || 0 == m_stScrollInfo.uiPageSize)
		{
			return ;
		}
		if (m_stScrollInfo.uiMaxPos > m_stScrollInfo.uiPageSize)
		{
			dFactor =  ((JC_DOUBLE)m_stScrollInfo.uiPageSize) /
				((JC_DOUBLE)m_stScrollCar.uiHeight) ;
			mmi_trace(1,"jdd_log: factor[%d] maxpos[%d] Carht[%d]",\
						(JC_UINT32)dFactor, m_stScrollInfo.uiMaxPos, m_stScrollCar.uiHeight);			
		}
		else
		{
			dFactor = 1.0 ;
		}

		if (KEY_STAR == keyCode || KEY_POUND == keyCode)
		{
			if (KEY_STAR == keyCode)
			{
			 	g_ScrollPrevCarY = -1;	
				mmi_trace(1,"jdd_log: penco y[%d] type[%d] Action[%d] event co[%d]", \
							stPenCooridinates->y, stBEMouseInfo.eType, \
							stBEMouseInfo.uMouseData.stMouseEvent.eAction, \
							stBEMouseInfo.uMouseData.stMouseEvent.uiY);
				stBEMouseInfo.uMouseData.stMouseEvent.uiWindowID = g_pstJWapAppInfo->uiWinId ;				
				stBEMouseInfo.eType = E_MOUSE_MOVEEVENT ;
				stBEMouseInfo.uMouseData.stMouseEvent.eAction = E_MOUSE_CLICK ;
				if (stPenCooridinates->y < m_stScrollCar.iTop)
				{					
					stBEMouseInfo.uMouseData.stMouseEvent.uiY = -1 * jdd_DEConfigGetMediumScroll () ;
					rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_MOUSE_EVENT, &stBEMouseInfo) ;
				}
				else if (stPenCooridinates->y > m_stScrollCar.iTop + m_stScrollCar.uiHeight)
				{
					stBEMouseInfo.uMouseData.stMouseEvent.uiY = jdd_DEConfigGetMediumScroll () ;
					rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_MOUSE_EVENT, &stBEMouseInfo) ;
				}				
			}
			if (KEY_POUND == keyCode)//E_FALSE == bIsSendEvent && eWapPenDownPos == E_WAP_PEN_DOWN_INSIDE_SCROLL_TOP)
			{
				JC_UINT32 uiScrollNewCarY = 0;
				JC_INT32 iPixelstoMove = 0;				
				
				uiScrollNewCarY = stPenCooridinates->y - stPrimaryWindowDim.iTop ;
				if( g_ScrollPrevCarY  == -1)
				{
					g_ScrollPrevCarY = uiScrollNewCarY ;
					return;
				}
				else
				{
					iPixelstoMove = uiScrollNewCarY - g_ScrollPrevCarY;
					g_ScrollPrevCarY = uiScrollNewCarY ;
				}
				stBEMouseInfo.uMouseData.stMouseEvent.uiWindowID = g_pstJWapAppInfo->uiWinId ;				
				stBEMouseInfo.eType = E_MOUSE_MOVEEVENT ;
				stBEMouseInfo.uMouseData.stMouseEvent.eAction = E_MOUSE_CLICK ;
							
				stBEMouseInfo.uMouseData.stMouseEvent.uiY = (iPixelstoMove * dFactor);
				mmi_trace(1,"jdd_log: tomove[%d] cordi[%d]", \
							 iPixelstoMove, stBEMouseInfo.uMouseData.stMouseEvent.uiY);
				rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_MOUSE_EVENT, &stBEMouseInfo) ;
				mmi_trace (1, "JDD_LOG: jWap_Generic_Pen_Handler Scrollbar>> jdi_BESetEvent: %X", JRET (rCode)) ;
			}
		}
	}
	else
	{
		/*	The pen is outside the primary mmi and scroll bar area	*/
		mmi_trace (1, "JDD_LOG: Pen Event For SOFTKEY") ;
		if (KEY_STAR == keyCode)
		{
			wgui_general_SK_pen_down_hdlr(*stPenCooridinates);
		}
		if (KEY_ENTER == keyCode)
		{
			wgui_general_SK_pen_up_hdlr(*stPenCooridinates);
		}
	}
}
#endif
void jWap_Generic_Key_Handler (U16 keyCode)
{
	E_BE_EVENT_TYPE		eEventType ;
	JC_RETCODE				rCode = JC_ERR_UNKNOWN ;
	ST_BE_CONTROL_INFO	stBEControlInfo = {0, } ;

	switch (keyCode)
	{
		case KEY_UP_ARROW:
		{
			eEventType = E_BE_SCROLL_UP ;
		}
		break ;

		case KEY_DOWN_ARROW:
		{
			eEventType = E_BE_SCROLL_DOWN ;
		}
		break ;

		case KEY_LEFT_ARROW:
		{
			eEventType = E_BE_SCROLL_LEFT ;
		}
		break ;

		case KEY_RIGHT_ARROW:
		{
			eEventType = E_BE_SCROLL_RIGHT ;
		}
		break ;

		case KEY_ENTER:
		{
			eEventType = E_BE_ACTION ;
		}
		break ;

		default:
		{
			return ;
			mmi_trace (1, "JDD_LOG: Invalid key -> %d", __FILE__, keyCode) ;
		}
		break ;
	}
	if (E_WAP_END_DISPLAY_RECVD == jWAP_GET_STATE ())
	{
		if (E_TRUE == jddex_widgetHandleKeyPress (g_pstJWapAppInfo->hBECore, eEventType, JC_NULL, JC_NULL))
		{
			mmi_trace (1, "JDD_LOG: Widgets consumed the event @@@") ;
		}
	}
	if (eEventType == E_BE_ACTION)
	{
        rCode = jdi_BEGetFocusedControlInfo (g_pstJWapAppInfo->hBECore, &stBEControlInfo) ;
        if (JC_OK == rCode &&
			(E_INPUT_BOX == stBEControlInfo.eType ||
			E_LIST_BOX == stBEControlInfo.eType ||
			E_COMBO_BOX == stBEControlInfo.eType))
		{
			/* Widget would have already consumed the event if E_BE_END_PAGE_LISTENER is received */
		}
		else
		{
			rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, eEventType, JC_NULL) ;
			mmi_trace (1, "JDD_LOG: jdi_BESetEvent: %x", JRET (rCode)) ;
		}
	}
	else
	{
		rCode = jdi_BESetEvent (g_pstJWapAppInfo->hBECore, eEventType, JC_NULL) ;
		mmi_trace (1, "JDD_LOG: jdi_BESetEvent: %x", JRET (rCode)) ;
	}
	return ;
}
JC_RETCODE InitBrwHistory()
{
	JC_RETCODE rCode = JC_OK;
	U16 *pHstFileName ;
	ST_BE_HISTORY_COMPONENT stBeHistoryComp ;
	jc_memset(&stBeHistoryComp,0, sizeof(ST_BE_HISTORY_COMPONENT)) ;
	jdi_CUtilsCharToTchar (NULL, CS_WAP_HISTORY_FILE_NAME, &pHstFileName) ;

	stBeHistoryComp.pmHistoryFileName = pHstFileName ;
	stBeHistoryComp.iNoOfDomains = CS_WAP_HISTORY_MAX_DOMAIN ;
	stBeHistoryComp.iNoOfHosts = CS_WAP_HISTORY_MAX_HOST ;

	rCode =jdi_BEInitComponent(g_pstJWapAppInfo->hBECore, E_BE_HISTORY, &stBeHistoryComp) ;

	if(rCode == JC_OK)
	{
		rCode = jdi_BEGetHistoryContext(g_pstJWapAppInfo->hBECore, &g_pstJWapAppInfo->pvHistory);
	}
	mmi_trace (1, "@@@ jBR_HANDLE:in InitBrwHistory %x rCode %d", g_pstJWapAppInfo->hBECore, rCode) ;
	jdd_MemFree(pHstFileName);
	return rCode ;
}

INT jWap_refreshPriMmi ()
{
	INT	iErr ;

	ASSERT (g_pstJWapAppInfo->hBECore) ;

	iErr = jdi_BERefreshDisplay (g_pstJWapAppInfo->hBECore) ;
	mmi_trace (1, "@@@ jdi_BERefreshDisplay: %x", JRET (iErr)) ;

	return (iErr) ;
}

INT jWap_init_engine ()
{
	JC_TASK_INFO stWapInfo = {0, } ;
	JC_TASK_INFO stTrInfo = {0, } ;
	JC_RETCODE	 rCode ;
#ifdef JWAP_ADD_COOKIE_HEADER
	ST_BE_SESSION_HEADER stSessionHeader ;
#endif

	mmi_trace (1, "JDD_LOG jWap_init_engine: stWapState.eTrConnState is [%d]",
		g_pstJWapAppInfo->stWapState.eTrConnState) ;

	if (NULL == g_pstJWapAppInfo->hBECore)
	{
		stTrInfo.iTaskID = E_TASK_TRANSPORT ;
		stTrInfo.iAppID = E_TASK_TRANSPORT ;

		stWapInfo.iTaskID = E_TASK_BROWSER_CORE ;
		stWapInfo.iAppID = E_TASK_BROWSER_UI ;

		//rCode = jdd_TimerInitialize () ;
		//if (!rCode)
		{
			rCode = jdd_NetInitialize () ;
		}


		if (!rCode)
		{

			rCode = jdi_BEInitialize (JC_NULL, jwap_app_callback, stWapInfo, \
				stTrInfo, &g_pstJWapAppInfo->hBECore) ;
			mmi_trace (1, "@@@ jdi_BEInitialize:%d @@@",rCode) ;
		}

		if(g_pstJWapAppInfo->hBECore && !rCode)
		{
			InitBrwHistory();
			mmi_trace (1, "@@@ History Initialized @@@") ;

			/* Init Cache Cookie irrespective of settings.
				Enable/Disable them based  on settings. Bug: 31122 */
			//if(g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableCookie)
			{
				//mmi_trace (1, "@@@ COOKIE INIT@@@") ;
				jWap_Init_Cookie() ;
				mmi_trace (1, "@@@ Cookie Initialized @@@") ;
			}

			//if(g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableCache)
			{
				//mmi_trace (1, "@@@ CACHE INIT @@@") ;
				jWap_Init_Cache() ;
				mmi_trace (1, "@@@ Cache Initialized @@@") ;
			}

			jWap_setUpdateCore (&g_pstJWapAppInfo->stWapSettings) ;
			jWap_bmInitBookMark() ;
			/* Data Connection Settings */
			rCode = jWap_config_connProfile () ;
#ifdef JWAP_ADD_COOKIE_HEADER
			jc_memset (&stSessionHeader, 0, sizeof (ST_BE_SESSION_HEADER)) ;
			jc_strcpy(stSessionHeader.acName, "Cookie2");

			stSessionHeader.pucHeader = "$Version=1" ;
			stSessionHeader.uiHeaderLen = jc_strlen("$Version=1");

			rCode = jdi_BESendSessionHeaders (g_pstJWapAppInfo->hBECore, &stSessionHeader) ;
			mmi_trace (1, "@@@ Session Header: %x", JRET (rCode) ) ;
#endif
		}
	}
	else
	{
		rCode = JC_OK ;
	}

	return (rCode) ;
}

INT jWap_clearCache ()
{
	ST_BE_OPTION_CACHE_CLEAR	stCacheClear ;

	jc_memset (&stCacheClear, 0, sizeof(ST_BE_OPTION_CACHE_CLEAR)) ;

	stCacheClear.bClearGeneralCache = E_TRUE ;
	return (jdi_BESetEvent(g_pstJWapAppInfo->hBECore, E_BE_CACHE_CLEAR, &stCacheClear)) ;
}

INT jWap_clearCookie ()
{
	return (jdi_BESetEvent(g_pstJWapAppInfo->hBECore, E_BE_COOKIE_CLEAR, JC_NULL)) ;
}


JC_RETCODE	jWap_Init_Cache()
{
	ST_BE_CACHE_COMPONENT	stCache ;
	JC_RETCODE				rCode ;

	jc_memset (&stCache, 0, sizeof (stCache)) ;
	rCode = JC_OK ;
	stCache.pmGeneralStorage = (JC_CHAR*) jdd_MemAlloc (jc_strlen (CS_WAP_CACHE_FILE_NAME) + 1 , sizeof (JC_CHAR)) ;

	if (NULL != stCache.pmGeneralStorage)
    {
		jdi_CUtilsTcsStrCat (CS_WAP_CACHE_FILE_NAME,stCache.pmGeneralStorage) ;
		//pstBrowserSettings = poAppUI->GetBrSettingsPtr () ;
		stCache.uiGeneralCacheSize = CS_WAP_CACHE_SIZE_MAX ;
		rCode = jdi_BEInitComponent (g_pstJWapAppInfo->hBECore, E_BE_CACHE, &stCache) ;

		jdd_MemFree (stCache.pmGeneralStorage) ;
		mmi_trace (1, "@@@ jWap_Init_Cache: %x", JRET (rCode)) ;
	}
	else
	{
		rCode = JC_ERR_MEMORY_ALLOCATION ;
	}

	return rCode ;
}

JC_RETCODE	jWap_Init_Cookie()
{
	ST_BE_COOKIE_COMPONENT	stCookie ;
	JC_RETCODE				rCode ;
	jc_memset (&stCookie, 0, sizeof (stCookie)) ;

	rCode = JC_OK ;
	stCookie.pmCookieFileName = (JC_CHAR*) jdd_MemAlloc (jc_strlen (CS_WAP_COOKIE_FILE_NAME) + 1, sizeof (JC_CHAR)) ;

	if (NULL != stCookie.pmCookieFileName)
	{
		rCode = jdi_CUtilsTcsStrCat (CS_WAP_COOKIE_FILE_NAME, stCookie.pmCookieFileName) ;
		stCookie.uiMaxNoOfCookies = CS_WAP_COOKIE_DEFAULT_MAX ;
		stCookie.uiMaxCookieSize = CS_WAP_COOKIE_MAX_SIZE ;
		rCode = jdi_BEInitComponent(g_pstJWapAppInfo->hBECore, E_BE_COOKIE, &stCookie) ;

		jdd_MemFree (stCookie.pmCookieFileName) ;
		mmi_trace (1, "@@@ jWap_Init_Cookie: %x", JRET (rCode)) ;
	}
	else
	{
		rCode = JC_ERR_MEMORY_ALLOCATION ;
	}

	return rCode ;
}

#ifdef CS_DC_ENABLE

extern ST_DOWNLOAD_CLIENT	*pstDownloadClient ;
#endif

INT jWap_deInit_engine ()
{
	mmi_trace(TRUE,"JDD_LOG: Func: %s g_pstJWapAppInfo->hBECore=%d", __FUNCTION__, g_pstJWapAppInfo->hBECore);
	if (g_pstJWapAppInfo->hBECore)
	{
		jdi_BEDeinitComponent(g_pstJWapAppInfo->hBECore, E_BE_CACHE);
		jdi_BEDeinitComponent(g_pstJWapAppInfo->hBECore, E_BE_COOKIE);
		jdi_BEDeinitComponent(g_pstJWapAppInfo->hBECore, E_BE_HISTORY);
		jdi_BEDeinitialize (g_pstJWapAppInfo->hBECore) ;
		g_pstJWapAppInfo->hBECore = JC_NULL ;
		g_pstJWapAppInfo->bIsDCAvailable = E_FALSE ;
		//jdd_TimerDeInitialize () ;
		//jdd_NetDeinitialize () ;
	}
	if (g_pstJWapAppInfo->vBookMarkHandle)
	{
		jdi_BookmarkDeInitialize (g_pstJWapAppInfo->vBookMarkHandle) ;
		g_pstJWapAppInfo->vBookMarkHandle = JC_NULL ;
	}
#ifdef CS_DC_ENABLE

	if(g_pstJWapAppInfo->pstDownloadClient) 
	{
		JC_RETCODE rCode = JC_OK;
		rCode = jwap_DCDeInit ();
		mmi_trace(1,"jdd_log: jwap_DCDeInit rcode[%d]", rCode);
	}
#endif	
	return JC_OK ;
}


INT jWap_init_priMmi ()
{
	JC_RECT					stRect ;
	JC_WINDOW_PROPERTIES 	stWinProp ;
	JC_RETCODE 				rCode ;
	JC_CONTROL_PROPERTIES   stControlProperty = {0, } ;
	DEVICE_CHARACTERISTICS 	stDeviceChar;
	JC_UINT32				uiScrollBarID ;

	ASSERT (g_pstJWapAppInfo->hBECore) ;
	if (0 == g_pstJWapAppInfo->uiWinId)
	{
		jc_memset (&stRect, 0, sizeof (JC_RECT)) ;
		jc_memset (&stWinProp, 0, sizeof (JC_WINDOW_PROPERTIES)) ;
		jc_memset (&stDeviceChar, 0, sizeof (JDDEX_DEVICE_CHAR)) ;
		jc_memset (&stControlProperty, 0, sizeof (JC_CONTROL_PROPERTIES)) ;

		stWinProp.eWindowStyle = E_APPLICATION_WINDOW ;
		jdd_MMIGetDeviceCharacteristics (&stDeviceChar);
		stRect.iLeft = stDeviceChar.uiWindowX ;
		stRect.iTop = stDeviceChar.uiWindowY;
		stRect.uiWidth = stDeviceChar.uiRenderingWidth - stDeviceChar.uiScrollBarWidth ;
		stRect.uiHeight = stDeviceChar.uiRenderingHeight ;

		rCode = jdd_MMICreateWindow (&stWinProp, &stRect, &g_pstJWapAppInfo->uiWinId) ;
		if (JC_OK == rCode)
		{
			/* Create VScroll bar */
			stControlProperty.typeOfControl.scrollBar.eScrollBarType = E_VERTICAL_SCROLLBAR;
			stRect.iLeft = stDeviceChar.uiRenderingWidth - stDeviceChar.uiScrollBarWidth ;
			stRect.iTop  = 0;
			stRect.uiWidth = stDeviceChar.uiScrollBarWidth ;
			stRect.uiHeight = stDeviceChar.uiScrollBarHeight ;

			rCode = jdd_MMICreateControl (g_pstJWapAppInfo->uiWinId, E_SCROLLBAR,
			              &stControlProperty, &stRect, &uiScrollBarID);
			g_pstJWapAppInfo->uiScrollBarWinId = uiScrollBarID ;
			rCode = jdi_BEOpen (g_pstJWapAppInfo->hBECore, g_pstJWapAppInfo->uiWinId) ;
			mmi_trace(1, "JDD_LOG: jWap_init_priMmi: jdi_BEOpen rCode=%X", -rCode);
		}
	}
	else
	{
		rCode = JC_OK ;
	}
	return rCode ;

}


INT jWap_deInit_priMmi ()
{
	INT iErr ;

	iErr = jdi_BEClose (g_pstJWapAppInfo->hBECore, g_pstJWapAppInfo->uiWinId) ;
	mmi_trace (1, "JDD_LOG: jWap_deInit_priMmi jdi_BEClose: %x", JRET (iErr)) ;

	iErr =	jdd_MMIDestroyWindow (g_pstJWapAppInfo->uiWinId) ;
	mmi_trace (1, "JDD_LOG: jWap_deInit_priMmi jdd_MMIDestroyWindow: %x", JRET (iErr)) ;
	g_pstJWapAppInfo->uiWinId = 0 ;
	if(NULL != g_pstJWapAppInfo->pszTitleString)
	{
		jdd_MemFree(g_pstJWapAppInfo->pszTitleString) ;
		g_pstJWapAppInfo->pszTitleString = NULL ;
		ChangeTitleString (NULL) ;
	}
	return (iErr) ;
}


INT jWap_send_fetch_request (U16	*pmDest)
{
	JC_RETCODE			rCode ;
	ST_BE_FETCH_REQ	stFetchReq = {0,} ;

	if (pmDest)
	{
		jc_memset (&stFetchReq, 0, sizeof (ST_BE_FETCH_REQ)) ;
		stFetchReq.pmURI = pmDest ;
		rCode = jdi_BEFetchRequest (g_pstJWapAppInfo->hBECore, &stFetchReq) ;
	}
	else if (g_pstJWapAppInfo->pstFetchReq)
	{
		{
			JC_INT8	*pcAuthURL = JC_NULL ;
			JC_INT8	*pcAuthUserName = JC_NULL ;
			JC_INT8	*pcAuthPassword = JC_NULL ;
			
			jdi_CUtilsTcharToChar (JC_NULL, g_pstJWapAppInfo->pstFetchReq->pmURI, &pcAuthURL) ;
			jdi_CUtilsTcharToChar (JC_NULL, g_pstJWapAppInfo->pstFetchReq->pstAuthInfo->amName, &pcAuthUserName) ;
			jdi_CUtilsTcharToChar (JC_NULL, g_pstJWapAppInfo->pstFetchReq->pstAuthInfo->amPassword, &pcAuthPassword) ;
			if (pcAuthURL)
			{
				mmi_trace (1, "JDD_LOG: jWap_send_fetch_request >> Auth URL: [%s]", pcAuthURL) ;
				jdd_MemFree (pcAuthURL) ;
			}
			if (pcAuthUserName)
			{
				mmi_trace (1, "JDD_LOG: jWap_send_fetch_request >> Auth Name: [%s]", pcAuthUserName) ;
				jdd_MemFree (pcAuthUserName) ;
			}
			if (pcAuthPassword)
			{
				mmi_trace (1, "JDD_LOG: jWap_send_fetch_request >> Auth PWD: [%s]", pcAuthPassword) ;
				jdd_MemFree (pcAuthPassword) ;
			}
		}
		rCode = jdi_BEFetchRequest (g_pstJWapAppInfo->hBECore, g_pstJWapAppInfo->pstFetchReq) ;
		jWap_freeFetchInfo (g_pstJWapAppInfo->pstFetchReq) ;
		g_pstJWapAppInfo->pstFetchReq = JC_NULL ;
	}
	else
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	mmi_trace (1, "JDD_LOG: jWap_send_fetch_request >> rCode [%X]", -rCode) ;
	return rCode ;
}


void jWap_init_context ()
{
	JC_RETCODE	rCode ;
	JC_CHAR		*pmFolder ;
#ifdef __MMI_MULTI_SIM__
  	S16 error = 0;
#endif

	g_pstJWapAppInfo = jdd_MemAlloc (1, sizeof (JWAP_APP_INFO)) ;

	if (g_pstJWapAppInfo)
	{
		jc_memset (g_pstJWapAppInfo, 0, SIZEOF (JWAP_APP_INFO)) ;
		g_pstJWapAppInfo->pcInputBuff = jdd_MemAlloc (jWAP_MAX_INPUT_SIZE + 1, sizeof(JC_CHAR)) ;
#ifdef __MMI_MULTI_SIM__
		ReadValue(NVRAM_MTPNP_WAP_PROFILE_SELECT_SIMTYPE, &g_pstJWapAppInfo->simType, 1, &error);

		switch(g_pstJWapAppInfo->simType)
		{
			case WAP_SIMTYPE_ALWAYSASK:
				g_pstJWapAppInfo->simid = MMI_SIM_1;
				break;
			case WAP_SIMTYPE_SIM1:
			case WAP_SIMTYPE_SIM2:
			case WAP_SIMTYPE_SIM3:
			case WAP_SIMTYPE_SIM4:
				if(MTPNP_PFAL_Is_Card_Usable(g_pstJWapAppInfo->simType - WAP_SIMTYPE_SIM1))
				{
					g_pstJWapAppInfo->simid = g_pstJWapAppInfo->simType - WAP_SIMTYPE_SIM1;
				}
				else
				{
					g_pstJWapAppInfo->simid = MTPNP_AD_GET_UsableSide_Index();
				}
				break;
			default:
				g_pstJWapAppInfo->simid = MMI_SIM_1;
				break;
		}
#endif
		rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_PERSISTENT_PATH, &pmFolder) ;
		if (JC_OK == rCode)
		{
			/* Not checking for the directory existence as jdd_FSMakeDir (),
				does it internally */
			/*rCode = */jdd_FSMakeDir (JC_NULL, pmFolder) ;
			/* Not printing the rCode as it is startup code, mmi_trace ()
				will delay the launch */
			jdd_MemFree (pmFolder) ;
		}		
	}

}


void jWap_deInit_context ()
{
	if (g_pstJWapAppInfo)
	{
		if (g_pstJWapAppInfo->pcInputBuff)
		{
			jdd_MemFree (g_pstJWapAppInfo->pcInputBuff) ;
			g_pstJWapAppInfo->pcInputBuff = JC_NULL ;
		}
		jdd_MemFree (g_pstJWapAppInfo) ;
		g_pstJWapAppInfo = JC_NULL ;
	}
}

JC_RETCODE jWap_LaunchFromIdle (E_WAP_LAUNCH_MODE eLaunchMode, JC_UINT8 eSimID, JC_CHAR *pmURL)
{
	JC_RETCODE retCode = JC_OK ;

	mmi_trace (1, "JDD_LOG: LaunchFromIdle mode[%d], simid[%d]", eLaunchMode, eSimID);
	if (g_pstJWapAppInfo)
	{
		if (pmURL)
		{
			g_pstJWapAppInfo->pmPushURL = jdi_CUtilsTcsDuplicate (JC_NULL, pmURL) ;
			if (JC_NULL == g_pstJWapAppInfo->pmPushURL)
			{
				retCode = JC_ERR_MEMORY_ALLOCATION ;
			}
		}
		if (JC_OK == retCode)
		{
			#ifdef __MMI_MULTI_SIM__
			g_pstJWapAppInfo->original_simid = g_pstJWapAppInfo->simid ;
			g_pstJWapAppInfo->simid = eSimID ;
			#endif
			g_pstJWapAppInfo->eWapLaunchMode = eLaunchMode ;
			jWap_mainMenu_entry_handler () ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_LaunchFromIdle >> g_pstJWapAppInfo is NULL") ;
		retCode = JC_ERR_INVALID_STATE ;
	}
	mmi_trace (1, "JDD_LOG: jWap_LaunchFromIdle returns [%X]", -retCode) ;
	return retCode ;
}

void jWap_processAppEvent(JC_EVENT_DATA *pstEventData)
{
	if (g_pstJWapAppInfo)
	{
		if (E_WAP_EXIT_KEY_QUIT == g_pstJWapAppInfo->stWapState.eWapExitKeyPressed)
		{
			jWap_deInit_priMmi () ;
			jWap_exitOptionsScreen () ;
		}
		else if (E_WAP_EXIT_KEY_END == g_pstJWapAppInfo->stWapState.eWapExitKeyPressed)
		{
			jWap_deInit_priMmi () ;
			jWap_deInit_engine () ;
			jWap_SetWapAppStatus (E_FALSE) ;
			jWap_exitDisconnectingScreen () ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: DANGER jWap_processAppEvent () >> eWapExitKeyPressed [%d]",
				g_pstJWapAppInfo->stWapState.eWapExitKeyPressed) ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: DANGER jWap_processAppEvent () >> g_pstJWapAppInfo is NULL") ;
	}
}


JC_BOOLEAN jMMS_IsWapConnected (void)
{
	extern JC_USER_INFO_CONNID *gpstConIdInfoHead ;
	if (JC_NULL == gpstConIdInfoHead)
	{
		mmi_trace (1, "jMMS_IsWapConnected returns FALSE");
		return E_FALSE ;
	}
	else
	{
		mmi_trace (1, "jMMS_IsWapConnected returns TRUE");
		return E_TRUE;
	}
}

#if 0 //defined(__PROJECT_M003__)

#include "simdetectiondef.h"
BOOL HasValidSIM(void);

void mmi_mainmenu_wap_browser(char* url)
{
	char unicode_url[256];
	//MMI_SIM_ID_T sim;
	mmi_trace(1,"yypp mmi_mainmenu_wap_browser url: %s", url);
	AnsiiToUnicodeString((S8 *)unicode_url, (S8*) url);
	if(!HasValidSIM())
	{
		DisplayPopup((U8*)GetString(STR_SIM_ACCESS_ERROR_MSG),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);
		return;
	}
#if defined(MMI_ON_HARDWARE_P)
	jWap_LaunchFromIdle(1/*E_WAP_LAUNCH_URL*/, 0, (JC_CHAR *)unicode_url);
#endif
}
#endif


#endif //__JATAAYU_APP__

