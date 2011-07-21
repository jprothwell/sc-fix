/***************************************************************************
 * $Id: jdi_upp.h,v 1.7 2008/02/21 16:12:10 kumardevhtmlbrow Exp $
 * $Revision: 1.7 $
 * $Date: 2008/02/21 16:12:10 $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_upp.h,v $
 * Revision 1.7  2008/02/21 16:12:10  kumardevhtmlbrow
 * PPP & UPP updated for new plugins.
 *
 * Revision 1.6  2008/01/23 13:50:56  kavithadevhtmlbrow
 * Anti Banner UPP Added
 *
 * Revision 1.5  2006/05/26 10:04:11  kumardevhtmlbrow
 * Updated....
 *
 * Revision 1.4  2006/05/12 13:41:44  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2006/05/12 13:25:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.2  2006/03/01 09:08:37  kavithadevhtmlbrow
 * Context/Non-Context to Only Read & ReadWrite Plugins
 *
 * Revision 1.1  2005/12/30 12:20:50  kavithadevhtmlbrow
 * UPP Initial Version
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_upp.h
 * @ingroup Plugins
 * @brief Defines the exposed enumerations, structures and Callback prototypes
 *		  that can be used to use a UPP function.
 * 
 */

#ifndef JDI_UPP_H
#define JDI_UPP_H


/**
 * @brief The application needs to use this enum to specify the various UPP Read plugin options
 */
typedef enum uppReadPlugin
{
	E_UPP_ZONES, /**< */
	E_UPP_REQUEST_COUNT /**< Indicates the request count */
} E_UPP_READ_PLUGIN ;


/**
 * @brief The application needs to use this enum to specify the various UPP Read Write plugin options
 */
typedef enum uppReadWritePlugin
{
	E_UPP_SMARTIMAGE, /**< Specifies smart image UPP */
	E_UPP_FILTER,     /**< Specifies the filter UPP */ 
	E_UPP_ANTIBANNER  /**< Specifies the Anti-Banner UPP */
} E_UPP_READWRITE_PLUGIN ;

/**
 * @brief The application/display engine needs to use this enum to specify the type of the
 *		  Anti-Banner event.
 */
typedef enum uppAntiBannerEvent
{
	E_UPP_ANTIBANNER_SETFILE, /**< Request for initializing the anti-banner module. The FileName containing the restricted URIs should be passed on to the module using this event type */
	E_UPP_ANTIBANNER_SWITCH, /**< Request for either enabling or disabling the anti-banner feature */
	E_UPP_ANTIBANNER_EXECUTE /**< Request for verifying the passed URI against the restricted URIs present in the specified file */
} E_UPP_ANTIBANNER_EVENT ;

/**
 * @brief Specifies UPP Smart image argument 
 */
typedef struct uppSmartImage
{
	JC_CHAR			*pmCurrURI ; /**< Specifies the URL of the image */
	JC_CHAR			*pmNewURI ; /**< Specifies the URL that the UPP wants the browser to fetch */
} ST_UPP_SMART_IMAGE ;

/**
 * @brief Specifies the data for anti banner event type
 */
typedef union uppAntiBannerData
{
	JC_CHAR					*pmFileName ; /**< Specifies the FileName containing the restricted URIs */
	JC_CHAR					*pmURI ; /**< Specifies the URL that needs to be verified against */
	JC_BOOLEAN				bEnable ; /**< Specifies whether the anti-banner feature is to be enabled or disabled */
} U_UPP_ANTI_BANNER_DATA ;

/**
 * @brief Specifies UPP Anti-Banner argument 
 */
typedef struct uppAntiBanner
{
	E_UPP_ANTIBANNER_EVENT	eEvent ; /**< Specifies the event for executing Anti-Banner */
	U_UPP_ANTI_BANNER_DATA	uData ;
} ST_UPP_ANTI_BANNER ;

/**
 * @brief Specifies UPP Filter argument 
 */
typedef struct uppFilter
{
	JC_CHAR			*pmCurrURI ; /**< Specifies the URL of the image */
	JC_CHAR			*pmNewURI ; /**< Specifies the URL that the UPP wants the browser to fetch */
} ST_UPP_FILTER ;


/**
 * @brief	    Prototype of UPP Plugin initialize callback.
 *
 * @param[in]	vHandle Specifies the handle to the plug in function.
 * @param[in]	pvUPPContext Once the plug in function is intiailized the callback
 *				function needs to return its context in this parameter.
 * @retval		JC_OK if the plug in is initialized successfully.
 *
 * The browser will call this callback to initialize a UPP function. The callback
 * needs to initialize the UPP and return the context of UPP in pvUPPContext. The 
 * browser will use this context while calling the UPP.
 *
 * For each UPP it has to follow the steps of initialization the plugin, Use of 
 * plugin and the deinitialize the plugin. This callback will be called to initialize
 * the various plugins (read or read write plugin).
 */
typedef JC_RETCODE (* CB_UPP_INITIALIZE) (JC_HANDLE			vHandle,
										  JC_HANDLE			*pvUPPContext) ;

/**
 * @brief	    Prototype of UPP Read Plugin callback.
 *
 * @param[in]	vHandle Specifies the handle to the plug in function.
 * @param[in]	pvUPPContext Specifies the UPP context which was returned when the
 *				plug in was initialized.
 * @param[in]	vUPPArg Specifies the arguement to the read plug in callback. 
 * @retval 		JC_OK if the read plug in is executed successfully.
 *
 * The browser will call this callback to execute the read plugin function which 
 * was initialized. The information needed for the plug in is specified
 * in the vUPPArg.
 */
typedef JC_RETCODE (* CB_UPP_READ_PLUGIN) (JC_HANDLE			vHandle,
										   JC_HANDLE			vUPPContext,
										   JC_HANDLE			vUPPArg) ;

/**
 * @brief	    Prototype of UPP Read write Plugin callback.
 *
 * @param[in]	vHandle Specifies the handle to the plug in function.
 * @param[in]	pvUPPContext Specifies the UPP context which was returned when the
 *				plug in was initialized.
 * @param[in]	vUPPArg Specifies the arguement to the read write plug in callback. 
 * @retval 		JC_OK if the read write plug in is executed successfully.
 *
 * The browser will call this callback to execute the read write plugin function which 
 * was initialized. The information needed for the plug in is specified
 * in the vUPPArg.
 */
typedef JC_RETCODE (* CB_UPP_READWRITE_PLUGIN) (JC_HANDLE			vHandle,
												JC_HANDLE			vUPPContext,
												JC_HANDLE			vUPPArg) ;

/**
 * @brief	    Prototype of UPP Plugin deinitialize callback.
 *
 * @param[in]	pvUPPContext Specifies the context of the UPP function which 
 *				is to be deinitialized.
 * @retval		JC_OK if the plug in is deinitialized successfully.
 *
 * The browser will call this callback to deinitialize a UPP function. 
 */
typedef JC_RETCODE (* CB_UPP_DEINITIALIZE) (JC_HANDLE		vUPPContext) ;


/**
 * @brief The error is returned when the antibanner file path has not been set by the application.
 */
#define JC_ERR_UPP_ANTIBANNER_SETFILE		TERMINAL_ERR_CODE(DE_MODULE, 0xAF)

/**
 * @brief The error is returned when the URL sent for anti-banner verification is found to be restricted.
 */
#define JC_ERR_UPP_ANTIBANNER_RESTRICTED	TERMINAL_ERR_CODE(DE_MODULE, 0xAE)

/**
 * @brief The error is returned when the URL is blocked
 */
#define JC_ERR_UPP_FILTER_BLOCKED			TERMINAL_ERR_CODE(DE_MODULE, 0xAD)

#endif


