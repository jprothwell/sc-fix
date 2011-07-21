
/***************************************************************************
 * $Id: jdi_ProvisionAPI.h,v 1.4 2007/08/14 11:56:31 lumitdevpclient Exp $
 * $Revision: 1.4 $
 * $DateTime: $
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
 *
 * File Description
 * ----------------
 *
 * Purpose			:
 * 
 * Created By		: Lumitaswa Meher
 * Created Date		: January, 2005
 * Current Revision	:
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_ProvisionAPI.h,v $
 * Revision 1.4  2007/08/14 11:56:31  lumitdevpclient
 * Fixes for the jTrack Issue 1248
 *
 * Revision 1.3  2007/01/31 05:39:24  lumitdevpclient
 * Compiler warning and Lint warning fixed as reported by TTPCom
 *
 * Revision 1.2  2006/01/04 06:50:14  lumitdevpclient
 * committ after fixing some continuous provisioning related discripancies
 *
 * Revision 1.6  2005/08/20 12:26:16  guestpclient
 * Updated according to the standard - no hard coded string, magic number and retcode checked
 *
 * Revision 1.5  2005/08/19 13:45:58  guestpclient
 * Code release on 19th August before changing on the comments on TTPcom
 *
 * 
 * Revision 1.4  2005/08/15 18:36:43  arundevpclient
 * Interfaces changed - jdi_ProvisionProcess & jdi_ProvisionAuthenticate.
 *
 *  Revision 1.3  2005/08/15 17:36:43  arundevpclient
 * Added new API jdi_ProvisionGetContConnectivityData.
 *
 *  
 * Revision 1.2  2005/08/09 15:10:31  guestpclient
 * Modified and changed to suit for the doxygen style and new coding standards.
 *
 *
 * Revision 1.1  2005/08/06 10:52:26  arundevpclient
 * Modified it to doxygen standard
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_ProvisionAPI.h
 * @ingroup Provisioning
 * @brief contains the exposed API for jOTA Provisioning Client
 */

#ifndef _JDI_PROVISIONING_H
#define _JDI_PROVISIONING_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

/** @mainpage
 * @section intro_section Introduction
 *
 * This Document gives a detailed description of the interfaces that has to be used by an application. The documentation clearly describes the usage of each function, the significance of each variable, usage of the data structures and enumerations etc. The document will ease the complexity faced while interfacing the application and the jOTA Provisioning Core. <BR>
 *
 * @section startup_code Startup Code
 *
 *    - The application has to develop the UI for jOTA Provisioning client core functionalities
 *    - Receive the Wap Push buffer [XML/WBXML] and retrieve the MimeHeader information & fill the structure
 *    - Pass the Mime structure alongwith the buffer received for Parsing & Generating the Provision Info structure
 *    - Based on the structure received it will perform the various core functionalities
 * 
 * @section basic_flow Basic Flow
 *
 *    - This section describes the basic flow that has to be used in order to use the jOTA Provisioning Client. 
 *    - The initialize function <B><I>jdi_ProvisionInitialize()</I></B> has to be called before calling any Core related functions. 
 *    - After receiving the Wap Push message, application need to segregate the MimeHeader & then pass the buffer to Process using API <B><I>jdi_ProvisionProcess()</I></B>
 *    - Once successfully implement the Processing of buffer, it will go for other core related functionalities.
 *    - Incase using provisioning database, App need to configure the database using <B><I>jdi_ProvisionConfigOTAClient()<I><B>
 *    - Finally application has to Deinitialize the Provisioning client using <B><I>jdi_ProvisionDeinitialize()</I></B>.
 */ 


/**
 * @defgroup Provisioning
 * @{
 */


#include <jdi_provisionstructures.h>
#include <jdi_mimeinfo.h>
#include <provisionjotacore.h>


/**
 * @ingroup    Provisioning jdi_ProvisionInitialize
 * @brief	This API is used to initialize jOTA Provisioning Client.
 * @param[out] pHandle Pointer to the provisioning handle for interface APIs.
 * 
 * This API is used to the provisioning library by initializing the necessary structures. 
 * The applications (Browser, MMS, jOTA.) must initialize the library before invoking any of the exposed provisioning APIs. 
 * This API can be called by MMS, browser and jOTA Provisioning Client, etc.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *
 * @sa jdi_ProvisionDeinitialize()
 *
 * @note The application must initialize the library before invoking any of the exposed Provisioning APIs
 * 
 */


JC_RETCODE jdi_ProvisionInitialize (JC_HANDLE *pHandle) ;


/**
 * @ingroup    Provisioning jdi_ProvisionProcess
 * @brief	This API will process the XML/WBXML buffer and attach the parsed profile information in the  Provisioning Handle.
 * @param[in] vHandle Provisioning handle
 * @param[in] pucBuffer Pointer to the buffer containing provisioning data
 * @param[in] uiBufferLen Length of the buffer containing provisioning data
 * @param[in] pHeadersList This is a list of MIME header values
 * @param[in, out] peProvType Pointer to the provisioning content type
 * @param[in, out] ppcProvURL Pointer to the Provisioning URL (for continuous provision)
 * @param[out] peSecurityType Here it will inform us which type of authentication is required
 * 
 * It will also return the type of the provisioning content such as Bootstrap (with continuous provision) or 
 * continuous provisioning. This function is used to process both BOOTSTRAP and CONTINUOUS provisioning data.  
 * For processing the CONTINUOUS provisioning data, application should pass the provision URL (PROVURL) as 
 * input but for the BOOTSTRAP, it should pass NULL as input for PROVURL through parameter ppcProvURL. 
 * If the provisioning data contains a provision URL, the function should return it to the application through 
 * the ppcProvURL parameter. This API needs to be called by jOTA Provisioning Client only for processing the 
 * buffer received by WAP PUSH/PULL. The Return value is either JC_OK (for success) or one of the appropriate 
 * error codes as described below.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *	  - JC_ERR_PROV_INVALID_DOCUMENT If the XML doc is not well formed. XML parser informs this
 *    - JC_ERR_PROFILENAME_EXISTS If the profile name is already is present
 *
 * @sa jdi_ProvisionProcessSIM()
 *
 * @note 
 * 
 */

JC_RETCODE jdi_ProvisionProcess (JC_HANDLE vHandle, 
								 JC_UINT8 *pucBuffer, 
								 JC_UINT32 uiBufferLen, 
								 ST_MIME_HEADERS *pstHeadersList,
								 E_PROV_DATA_TYPE *peProvType, 
								 JC_INT8 **ppcProvURL, 
								 E_SEC_TYPE *piSecurityType);

/**
 * @ingroup    Provisioning jdi_ProvisionGetCharactersticCount
 * @brief	This API is used to count the number of specified characteristic blocks present in the current provisioning context.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] pcCharacteristicName The characteristic name to be count. Ex-PXLOGICAL
 * @param[out] uiCount Return the count of the characteristic name ( specified in Input parameter) present in the provisioning context
 * 
 * This API can be called by MMS, browser, jOTA Provisioning Client, etc.. to count the number of characteristic
 * blocks present in the provisioning context in continuous as well as bootstrap provisioning.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *
 * @sa 
 *
 * @note 
 * 
 */

JC_RETCODE jdi_ProvisionGetCharactersticCount( JC_HANDLE vHandle, 
											  JC_INT8 *pcCharacteristicName, 
											  JC_UINT32 *uiCount) ;

/**
 * @ingroup    Provisioning jdi_ProvisionGetProfileNames
 * @brief	This API is used to list all the profiles [i.e. combination of AppId & AppName] present in the current provisioning context.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[out] ppstProfName double pointer to contain the list of profile names
 * 
 * If multiple profiles are present in a single document, it will give the linked list of the profile names. 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *
 * @sa jdi_ProvisionFreeProfileNames()
 *
 * @note 
 * 
 */

JC_RETCODE jdi_ProvisionGetProfileNames ( JC_HANDLE vHandle, 
										  ST_PROFILE_NAME_LIST **ppstProfName) ;

/**
 * @ingroup    Provisioning jdi_ProvisionFreeProfileNames
 * @brief	This function is used to release the memory allocated for the profile name list.
 * @param[in] pstProfName pointer to contain the list of profile names
 * 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *
 * @sa jdi_ProvisionGetProfileNames()
 *
 * @note jdi_ProvisionFreeProfileNames only frees the memory pointed by pstProfNameList. 
 *		 It is not making pstProfNameList NULL. It is the application, which should take care of this.
 * 
 */

JC_RETCODE jdi_ProvisionFreeProfileNames (ST_PROFILE_NAME_LIST *pstProfName) ;


/**
 * @ingroup    Provisioning jdi_ProvisionGetProfileDetailInfo
 * @brief	This API is used to get the Detail Information of the Characteristic blocks present in a particular profile.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] pcAppID The Application ID
 * @param[in] pcAppName The Profile Name
 * @param[out] ppstProfileInfo double pointer to structure contains detail info of profile
 * 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *
 * @sa jdi_ProvisionFreeProfileDetailInfo()
 *
 * @note 
 *    - Here the return type is JC_OK, but we need to check the buffer content (ppstProfileDetail) before processing further, as some fields may be NULL because they may not be present in the provisioning document. The jdi_ProvisionFreeProfileDetailInfo need to be called to free ppstProfileDetail if this function returns JC_OK.
 *    - If both pcAppId and pcName are NULL then it will return the whole document. If only pcAppId or both of these are present then it will return this profile specific information if the document contains multiple profiles. So if Application wants to get individual profile information then it needs to pass the specific APPID and APPNAME. If APPNAME is NULL then it will get information for that application only.
 */

JC_RETCODE jdi_ProvisionGetProfileDetailInfo (JC_HANDLE vHandle, 
											  JC_INT8 *pcAppID,
											  JC_INT8 *pcAppName, 
											  ST_PROFILE_INFO **ppstProfileInfo) ;


/**
 * @ingroup    Provisioning jdi_ProvisionFreeProfileDetailInfo
 * @brief	This API is used to Release the memory occupied by the Profile Detail Info Structure.
 * @param[in] ppstProfileInfo double pointer to structure contains detail info of profile
 * 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *
 * @sa jdi_ProvisionGetProfileDetailInfo()
 *
 * @note 
 *    jdi_ProvisionFreeProfileDetailInfo only frees the memory pointed by pstProfileDetail. It is not making pstProfileDetail NULL. It is the application, which should take care of this.
 */


JC_RETCODE jdi_ProvisionFreeProfileDetailInfo (ST_PROFILE_INFO *pstProfileInfo) ;


/**
 * @ingroup    Provisioning jdi_ProvisionGetContConnectivityData
 * @brief	This API is used to get the continuous connectivity information of the Characteristic blocks present in a particular profile.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[out] ppstConnInfo double pointer to structure contains detail info of profile
 * 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_UNKNOWN If return information not found or any not known error
 *
 * @sa 
 *
 * @note 
 *    The variable need to be freed for both JC_OK and JC_ERR_UNKNOWN error codes by calling the jdi_ProvisionFreeProfileDetailInfo API.
 */

JC_RETCODE jdi_ProvisionGetContConnectivityData (JC_HANDLE vHandle, 
												 ST_PROFILE_INFO **ppstConnInfo) ;

/**
 * @ingroup    Provisioning jdi_ProvisionAuthenticate
 * @brief	This API will authenticate the User Input PIN MAC Value with the existing MAC depending on the security type.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] pcUserInput User Input PIN Value
 * @param[in] uiDataLen User Input Data Length
 * 
 * Security types can be: E_NETWPIN, E_USERPIN, E_USERNETWPIN, E_USERPINMAC. 
 * This API can be call by jOTA Provisioning Client only to authenticate the user wants to save or modify an existing profile.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_UNKNOWN If the authentication algorithm fails to generate the MAC value or any not known error
 *    - JC_ERR_AUTHENTICATION When The MAC from server is not same after the processing the buffer and SEC PIN
 *    - JC_ERR_NULL_POINTER If the PIN or BUFFER is NULL
 *
 * @sa 
 *
 * @note 
 *    
 */


JC_RETCODE jdi_ProvisionAuthenticate (JC_HANDLE vHandle, 
									  JC_UINT8 	*pcUserInput, 
									  JC_UINT32 	uiDataLen);

/**
 * @ingroup    Provisioning jdi_ProvisionRegisterCallback
 * @brief	This API is used to update the callback functions in the present provisioning handle.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] eCallbckType Type of Callback
 * @param[in] puCallBack This is a Union value contains the pointer to the function invoked during the Authentication
 * 
 * The registered callback function get invoked when the jOTA client authenticating the bootstrap message.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_UNKNOWN Failure - Unknown Error
 *
 * @sa 
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionRegisterCallback (JC_HANDLE vHandle, 
										  E_PROV_CALLBACK_TYPE eCallbckType,
										  U_PROV_CALLBACK *puCallBack) ;

/**
 * @ingroup    Provisioning jdi_ProvisionGetData
 * @brief	This API is used to retrieves the parameter values and the characteristic data after the selection of a profile.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] pcCharPath Pointer to the characteristic path of the parameter
 * @param[in] pcParmName Pointer to the name of the parameter to be retrieved
 * @param[in] pcCondition Pointer to the condition to be satisfied to get the data
 * @param[out] pvReturnValue pointer to the value to be get
 * @param[out] puiReturnSize The size of the value to be get
 * 
 * An application can get data (both parm or characteristic value from the provisioning doc) be passing the parameters which should uniquely identify it. 
 * To identify a particular characteristic among many, it is needed to have a proper condition and in order to get a parm value among multiple parm values one has to pass an index value for that.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *    - JC_ERR_PROV_INVALID_TAG_INPUT If invalid tag is entered as input
 *
 * @sa jdi_ProvisionSetData()
 *
 * @note 
 *    
 */


JC_RETCODE jdi_ProvisionGetData (JC_HANDLE vHandle, JC_INT8 *pcCharPath, 
								 JC_INT8 *pcParmName, JC_INT8 *pcCondition, 
								 JC_HANDLE *pvReturnValue, JC_UINT32 *puiReturnSize) ;

/**
 * @ingroup    Provisioning jdi_ProvisionSetData
 * @brief	This API will update the given parameter value or characteristic structures in the given configuration file.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] pcCharPath Pointer to the characteristic path of the parameter
 * @param[in] pcParmName Pointer to the name of the parameter to be retrieved
 * @param[in] pcCondition Pointer to the condition to be satisfied to get the data
 * @param[out] vValue pointer to the value to be set
 * @param[out] uiValueSize The size of the value to be set
 * 
 * This API can be called by MMS, browser, jOTA Provisioning Client, etc. 
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_INVALID_PARAMETER On invalid or NULL input parameter
 *    - JC_ERR_PROV_INVALID_TAG_INPUT If invalid tag is entered as input
 *
 * @sa jdi_ProvisionGetData()
 *
 * @note 
 *    
 */


JC_RETCODE jdi_ProvisionSetData (JC_HANDLE vHandle, JC_INT8 *pucCharPath, JC_INT8 *pucParmName, 
								 JC_INT8 *pucCondition, JC_HANDLE vValue, JC_UINT32 uiValueSize ) ;

/**
 * @ingroup    Provisioning jdi_ProvisionDeinitialize
 * @brief	This API frees the provisoining structure.
 * @param[in] vHandle Provisioning handle for the current context
 * 
 * This API can be called by MMS, browser, jOTA Provisioning Client, etc. 
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing.
 *
 * @sa jdi_ProvisionInitialize()
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionDeinitialize (JC_HANDLE vHandle) ;



/**< New functions added by Arun for DB support */

#ifdef ENABLE_PROV_DATABASE

/**
 * @ingroup    Provisioning jdi_ProvisionSetProfileToContext
 * @brief	This API is used to set a profile (AppID  and AppName) to the present context.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] pcAppID Application ID
 * @param[in] pcAppName Application Name
 * 
 * Its just takes the profile as an input then adds that profile into the handle so that any further processing can be done on that profile.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *
 * @sa jdi_ProvisionGetAllProfileNames()
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionSetProfileToContext (JC_HANDLE vHandle, 
											 JC_INT8 *pcAppID,
											 JC_INT8 *pcAppName) ;

/**
 * @ingroup    Provisioning jdi_ProvisionGetAllProfileNames
 * @brief	This function is used to fetch all the profile names from the existing provisioning database.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[out] ppstProfDetailList Structure which holds detail info of all profiles in the database
 * 
 * This is intended to get all the application profile names that are stored in the provisioning database.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *
 * @sa jdi_ProvisionFreeAllProfileNames()
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionGetAllProfileNames (JC_HANDLE vHandle, 
											ST_PROFILE_DETAILS **ppstProfDetailList) ;

/**
 * @ingroup    Provisioning jdi_ProvisionFreeAllProfileNames
 * @brief	This function is used to free the list of all profile names in the database.
 * @param[in] ppstProfDetailList Structure which holds detail info of all profiles in the database
 * 
 *
 * @return 
 *    - JC_OK Success on processing
 *
 * @sa jdi_ProvisionGetAllProfileNames()
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionFreeAllProfileNames (ST_PROFILE_DETAILS *pstProfileDetails) ;

/**
 * @ingroup    Provisioning jdi_ProvisionSaveProfile
 * @brief	This function will write the profile information into a file.
 * @param[in] vHandle Provisioning handle for the current context
 * 
 * If the provisioning URL/Profile name is matching it will update the existing profile. 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *    - JC_ERR_PROV_INDEX_OUT_OF_RANGE If the index range exceeds
 *    - JC_ERR_PROV_MAX_DOC_EXCEED If the maximum file length exceeds
 *    - JC_ERR_PROFILENAME_EXISTS If the profile name already exists
 *
 * @sa jdi_ProvisionFreeAllProfileNames()
 *
 * @note 
 *    
 */


JC_RETCODE jdi_ProvisionSaveProfile   (JC_HANDLE vHandle);

/**
 * @ingroup    Provisioning jdi_ProvisionSelectProfile
 * @brief	This function is used to select a particular profile by its Application ID & application Name that are stored in the provisioning database.
 * @param[in] vHandle Provisioning handle for the current context
 * @param[in] pcAppID Application Id
 * @param[in] pcAppName Application Name
 * 
 * This API is needed when application uses jOTA internal database. After initialization, user has to select a profile from the profile name list before invoking Get, Set or Delete APIs. 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *
 * @sa jdi_ProvisionDeleteProfile()
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionSelectProfile (JC_HANDLE vHandle, 
									   JC_INT8 *pcAppID,
									   JC_INT8 *pcAppName) ;

/**
 * @ingroup    Provisioning jdi_ProvisionDeleteProfile
 * @brief	This function will delete the non-active profiles.
 * @param[in] vHandle Provisioning handle for the current context
 * 
 * This function delete the profile with the given index. Before this the profile should be selected into the context.
 * This API can be call by jOTA Provisioning Client only to delete any non-active profile.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *
 * @sa jdi_ProvisionSelectProfile()
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionDeleteProfile (JC_HANDLE vHandle) ;


/**
 * @ingroup    Provisioning jdi_ProvisionSetActiveConfig
 * @brief	This function will set the provided profile name as the active profile.
 * @param[in]  vHandle  provisioning handle
 * @param[in]  pcAppID	Applicatlion ID of the profile to be made active 
 * @param[in]  pcAppName Applicatlion name of the profile to be made active 
 * @param[in]  bActiveFlag the flag value to set: 1 for set, 0 to reset
 * 
 * The same API can also be used to de-activate the active profile.
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *    - JC_ERR_ALREADY_ACTIVE  If already active
 *
 * @sa 
 *
 * @note 
 *    
 */

 

JC_RETCODE jdi_ProvisionSetActiveConfig (JC_HANDLE vHandle, 
										 JC_INT8 *pcAppID,
										 JC_INT8 *pcAppName, 
										 JC_BOOLEAN bActiveFlag)  ;

/**
 * @ingroup    Provisioning jdi_ProvisionFreeGetDataBuffer
 * @brief	This function checks if the given ID is present in the database or not.
 * @param[in]  vHandle  provisioning handle
 * @param[in]  pcAppID  Application ID of the profile
 * @param[in]  pcAppName  Application name of the profile.
 * 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *
 * @sa 
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionFreeGetDataBuffer (JC_INT8 *pcDataType,
										   JC_HANDLE vDataBuffer) ;

/**
 * @ingroup    Provisioning jdi_ProvisionIsProfileExist
 * @brief	This function will release the buffer received after calling jdi_ProvisionGetData.
 * @param[in]  vHandle  provisioning handle
 * @param[in]  pcAppID  Application ID of the profile
 * @param[in]  pcAppName  Application name of the profile.
 * 
 * This API can be call by MMS, browser, jOTA Provisioning Client, etc..
 *
 * @return 
 *    - E_TRUE If profile is present in the database
 *    - E_FALSE If profile not present in the database
 *
 * @sa 
 *
 * @note 
 *    
 */

JC_BOOLEAN jdi_ProvisionIsProfileExist (JC_HANDLE vHandle, JC_INT8 *pcAppID, JC_INT8 *pcAppName) ;


/**
 * @ingroup    Provisioning jdi_ProvisionConfigOTAClient
 * @brief	This function needs to be called if the application uses provisioning database.
 * @param[in] vHandle pointer to the handle
 * @param[in] pstProvConfData  provisioning handle
 * 
 * It gets the device specific configuration parameters through the structure ST_PROV_CONFIG_DATA. 
 * This needs to be called before the jdi_ProvisionProcess and after initialization. Application can release pstProvConfData parameter after this function.
 * This function is crusial for the DATAPOOL & Profile management support. It initialzes the configuration parameters life paths, and gets relevent information.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *    - JC_ERR_MEMORY_ALLOCATION Failure in memory allocation
 *
 * @sa 
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionConfigOTAClient (JC_HANDLE hHandle, 
										 ST_PROV_CONFIG_DATA *pstProvConfData) ;

/**
 * @ingroup    Provisioning jdi_ProvisionProcessSIM
 * @brief	This API will update the content of SIM card if needed, provided the API to read data from SIM card is available.
 * @param[in]  vHandle  provisioning handle
 * @param[in]  pucBuffer  Prov Buffer retrieved from the SIM card 
 * @param[in]  uiBufLen  Length of the buffer
 * @param[in]  eMsgType  Type of the buffer to generate XML\WBXML
 * @param[out]  ppucOutBuffer  Output buffer generated after updating SIM data with the newly recieved data
 * @param[out]  puiOutBufLen  Length of the output buffer
 * 
 * This API needs to be called after the jdi_ProvisionProcess is called to process the newly arrived data. 
 * If the process succeeds, calling to this API with the buffer in SIM will process and give the updated output buffer.
 *
 * @return 
 *    - JC_OK Success on processing
 *    - JC_ERR_INVALID_HANDLE On invalid parameter passing
 *    - JC_ERR_PROV_SIM_NO_BUF_FOR_UPDATE If the context has no buffer to update SIM data
 *
 * @sa jdi_ProvisionProcess()
 *
 * @note 
 *    
 */

JC_RETCODE jdi_ProvisionProcessSIM(JC_HANDLE	vHandle,
								   JC_UINT8		*pucBuffer,
								   JC_UINT32	uiBufLen,
								   E_MESSAGE_TYPE eMsgType,
								   JC_UINT8		**ppucOutBuffer,
								   JC_UINT32	*puiOutBufLen) ;


#endif /**< ENABLE_PROV_DATABASE */


#endif /**< JOTA_PROVISION_CORE_LIBRARY */


#endif /**< _JDI_PROVISIONING_H */


/*** END OF FILE ***/
