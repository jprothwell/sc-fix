/***************************************************************************
 * $Id: jdd_dataconnection.h,v 1.6 2008/08/14 08:25:27 browserdevjtbase Exp $
 * $Revision: 1.6 $
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
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdd_dataconnection.h,v $
 * Revision 1.6  2008/08/14 08:25:27  browserdevjtbase
 * Core Updates
 *
 * Revision 1.5  2008/01/28 05:56:45  browserdevjtbase
 * Core Updated.
 *
 * Revision 1.4  2008/01/15 11:57:45  browserdevjtbase
 * Updated the jtbase.
 *
 * Revision 1.3  2007/06/29 07:33:40  browserdevjtbase
 * Updated the new values in dataconnection.
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.12  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.10  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.9  2005/08/10 05:16:55  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/06/14 09:50:18  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/06/08 14:44:47  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/06/08 13:16:05  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/06 13:26:20  kumardevhtmlbrow
 * Added the data connection id ...
 *
 * Revision 1.4  2005/05/18 15:34:23  kumardevhtmlbrow
 * added the isDataConnectionAvailable
 *
 * Revision 1.3  2005/05/17 08:33:12  kumardevhtmlbrow
 * changed for convention
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/

#ifndef _JDD_DATACONNECTION_H
#define _JDD_DATACONNECTION_H



#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @file jdd_dataconnection.h
 * @ingroup DataConnection
 * @brief Defines the data connection API's.
 *
 */

/**
 * @brief This is the handle associated with the data connection
 */
typedef void * JDD_DATA_CONN_HANDLE ;

/** @cond */
#define MAX_DATA_CONN_PHONE_NUMBER_SIZE		255
#define MAX_DATA_CONN_APN_SIZE				255
#define MAX_DATA_CONN_USERNAME_SIZE			255
#define MAX_DATA_CONN_PASSWORD_SIZE			255
/** @endcond */

/**
 * @brief This enumeration contains the different CSD call type.
 */
typedef enum e_csd_call_type
{
	E_CSD_ISDN_DATA_TYPE, /**< Specifies the ISDN CSD call type */
	E_CSD_ANALOG_DATA_TYPE /**< Specifies the Analog CSD call Type */
} E_DATA_CONN_CSD_CALL_TYPE ; 

/**
 * @brief This structure provides the information about CSD connection.
 */
typedef struct st_csd_data_conn
{
	JC_INT8					acPhoneNumber[MAX_DATA_CONN_PHONE_NUMBER_SIZE + 1] ; /**< Specifies the phone number which needs to be dialled */
	JC_UINT32				uiBaudRate ; /**< Specifies the baud rate which needs to be used for the CSD connection */
	E_DATA_CONN_CSD_CALL_TYPE	eCallType ; /**< Specifies the call type either analog or ISDN */
} ST_DATA_CONN_CSD ;

/**
 * @brief This structure provides the information about the GPRS connection.
 */
typedef struct st_gprs_data_conn 
{
	JC_INT8			acAPN [MAX_DATA_CONN_APN_SIZE] ; /**< Specifies the APN information */
} ST_DATA_CONN_GPRS ;

/**
 * @brief This enumeration specifies the bearer type 
 */
typedef enum e_bearer_type
{
	E_DATA_CONN_CSD_BEARER_TYPE, /**< Specifies the bearer type is of CSD. */
	E_DATA_CONN_GPRS_BEARER_TYPE, /**< Specifies the bearer type is of GPRS. */
	E_DATA_CONN_OTHER_BEARER_TYPE /**< Specifies the bearer type is not CSD or GPRS. */
} E_DATA_CONN_BEARER_TYPE ; 

/**
 * @brief Specifies the CSD or GPRS connection information.
 */
typedef union u_data_conn_type
{
	ST_DATA_CONN_CSD	stCSDDataConn ; /**< Specifies the CSD data connection information */
	ST_DATA_CONN_GPRS	stGPRSDataConn ; /**< Specifies the GPRS data connection information */
} U_DATA_CONN_TYPE ;

#ifndef JCF_ALLOW_PLATFORM_VALUE
/**
 * @brief Specifies the generic data connection information which is used to make the bearer up.
 */
typedef struct st_generic_data_connection
{
	E_DATA_CONN_BEARER_TYPE	eBearerType ; /**< Specifies the bearer type (CSD/GPRS/Other). */	
	JC_UINT32				uiTimeout ; /**< Specifies the time out for this specific data connection */
	JC_INT8 				acUserName [MAX_DATA_CONN_USERNAME_SIZE] ; /**< Specifies the user name for the data connection */
	JC_INT8					acPassword [MAX_DATA_CONN_PASSWORD_SIZE] ; /**< Specifies the password for the data connection */
	U_DATA_CONN_TYPE		uDataConnType ; /**< Specifies the CSD or GPRS specific information */
	JC_UINT32				uiConnID ; /**< Specifies the unique connection identifier, that can be used by the platform
									     to provide other values, which is left in the generic data connection and it is platform specific.
										 Generally recommended this connection id should be unique ID, which can be obtained from 
										 JDD layer, when the core invokes make connection. This should not be the memory
										 allocated as the core does not call any specific API to free this ID.*/
} ST_GENERIC_DATA_CONN ;
#endif

/**
 * @brief This enumeration specifies the different data connection status.
 * Few platform does not provide few status (say E_DATA_CONN_REGISTERING), these information are
 * generally used for the progress information. The main status information required is success 
 * and failure to make the CSD or GPRS data connection.
 */
typedef enum e_data_connection_status 
{
	E_DATA_CONN_CONNECTING_GPRS, /**< Specifies the GPRS data connection is in connecting state. */
	E_DATA_CONN_CONNECTING_CSD, /**< Specifies the CSD data connection is in connecting state. */
	E_DATA_CONN_CONNECTING, /**< Specifies the other data connection (not GPRS or CSD) is in connecting state. */
	E_DATA_CONN_AUTHORIZING, /**< Specifies the data connection is in the authorizing state. */
	E_DATA_CONN_REGISTERING, /**< Specifies the data connection is in the registering state. */
	E_DATA_CONN_CONNECTED_GPRS, /**< Specifies the GPRS data connection is successfully connected. */
	E_DATA_CONN_CONNECTED_CSD, /**< Specifies the CSD data connection is successfully connected. */
	E_DATA_CONN_CONNECTED, /**< Specifies the other data connection (not GPRS or CSD) is in connected state. */
	E_DATA_CONN_FAIL_GPRS, /**< Specifies the GPRS data connection is failed */
	E_DATA_CONN_FAIL_CSD, /**< Specifies the CSD data connection is failed */
	E_DATA_CONN_FAIL, /**< Specifies the other data connection (not GPRS or CSD) is failed. */
	E_DATA_CONN_DISCONNECTING_GPRS, /**< Specifies the GPRS data connection is going down */
	E_DATA_CONN_DISCONNECTING_CSD, /**< Specifies the CSD data connection is going down */
	E_DATA_CONN_DISCONNECTING, /**< Specifies the other data connection (not GPRS or CSD) is going down. */
	E_DATA_CONN_DISCONNECTED_GPRS, /**< Specifies the GPRS data connection is shut down/disconnected */
	E_DATA_CONN_DISCONNECTED_CSD, /**< Specifies the CSD data connection is shut down/disconnected */
	E_DATA_CONN_DISCONNECTED /**< Specifies the other data connection (not GPRS or CSD) is shut down/disconnected. */
} E_DATA_CONN_STATUS ;

/**
 * @brief This callback function is used to update the data connection progress
 * @param[in] pvAppArg Specifies the callback argument, which has been passed by the jdd_DataConnInitialize
 * @param[in] eConnectionStatus Specifies the current status of the data connection.
 * @param[in] uiConnID Specifies the connection identifier.
 */
typedef JC_RETCODE (* JC_DATA_CONN_PROGRESS_CALLBACK) (void		*pvAppArg, 
										E_DATA_CONN_STATUS		eConnectionStatus,
										JC_UINT32				uiConnID) ;

/**
 * @brief This callback function is used to intimate when the data connection is dropped.
 * @param[in] pvAppArg Specifies the callback argument, which has been passed by the jdd_DataConnInitialize.
 * @param[in] uiConnID Specifies the connection identifier.
 */
typedef void (* JC_DATA_CONN_DISCONNECT_CALLBACK) (void			*pvAppArg,
												   JC_UINT32	uiConnID) ;

/**
 * @brief This callback function is used when the data connection could not be established with the specific error code.
 * @param[in] pvAppArg Specifies the callback argument, which has been passed by the jdd_DataConnInitialize
 * @param[in] uiConnID Specifies the connection identifier.
 * @param[in] rErrCode Specifies the specific error code.
 */
typedef void (* JC_DATA_CONN_STATUS_CALLBACK) (void			*pvAppArg,
											   JC_UINT32	uiConnID,
											   JC_RETCODE	rErrCode) ;
/**
 * @brief This function initializes the data connection.
 * @param[out] pvDCHandle Returns the reference to the data connection manager, this will be used with the other API's.
 * @param[in] pvArg Specifies the callback argument which needs to be used when invoking any registered data connection callback.
 * @see jdd_DataConnSetProgressCB
 * @see jdd_DataConnSetDisconnectCB
 * @see jdd_DataConnSetStatusCB
 */
JC_RETCODE jdd_DataConnInitialize (JDD_DATA_CONN_HANDLE	*pvDCHandle,
								   void					*pvArg) ;

/**
 * @brief This function deinitializes the data connection.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 */
JC_RETCODE jdd_DataConnDeInitialize (JDD_DATA_CONN_HANDLE	vDCHandle) ;

/**
 * @brief This function sets the progress callback.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] cbProgressFnc Specifies the long function pointer which is used by the data connection manager
 *							for updating the connection progress.
 */	
JC_RETCODE jdd_DataConnSetProgressCB (JDD_DATA_CONN_HANDLE				vDCHandle,
									  JC_DATA_CONN_PROGRESS_CALLBACK	cbProgressFnc) ;

/**
 * @brief This function deinitializes the data connection.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] cbDisconnectFnc Specifies the long function pointer, which is used by the data connection manager 
 *							  for updating when the specific connection is dropped.
 */
JC_RETCODE jdd_DataConnSetDisconnectCB (JDD_DATA_CONN_HANDLE				vDCHandle,
										JC_DATA_CONN_DISCONNECT_CALLBACK	cbDisconnectFnc) ;

/**
 * @brief This function deinitializes the data connection.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] cbStatusFnc Specifies the long function pointer, which is used by the data connection manager 
 *						  when the data connection could not be established.
 */
JC_RETCODE jdd_DataConnSetStatusCB (JDD_DATA_CONN_HANDLE			vDCHandle,
									JC_DATA_CONN_STATUS_CALLBACK	cbStatusFnc) ;

/**
 * @brief This function establishes the data connection.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] pstDataConn Specifies the data connection information.
 * @param[out] puiConnID Returns the unique connection identifier associated with this specific data connection. 
 * @retval JC_ERR_DATA_CONN_CONTINUE Specifies the data connection function call that is non-blocked. This specifies that data connection layer shall call the callback function once the operation is completed for progress, disconnect and error in making data connection.
 *
 * This function makes the data connection. The handle obtained for the connection is 
 * then used to perform other related operations like send/receive.
 * The unique connection identifier returned from this function is passed to the while creating the socket (jdd_NetSocket)
 * to bound to this specific service provider.
 * 
 */
JC_RETCODE jdd_DataConnMakeConnection (JDD_DATA_CONN_HANDLE		vDCHandle,
									   ST_GENERIC_DATA_CONN		*pstDataConn,
									   JC_UINT32				*puiConnID) ;

/**
 * @brief This function cancel the connection which is in progress.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] uiConnID Specifies the connection identifier, which is returned from jdd_DataConnMakeConnection
 */
JC_RETCODE jdd_DataConnCancelConnection (JDD_DATA_CONN_HANDLE	vDCHandle,
										 JC_UINT32				uiConnID) ;

/**
 * @brief This function disconnects the data connection.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] uiConnID Specifies the connection identifier, which is returned from jdd_DataConnMakeConnection
 */
JC_RETCODE jdd_DataConnDisconnectConnection (JDD_DATA_CONN_HANDLE	vDCHandle,
											 JC_UINT32		uiConnID) ;

/**
 * @brief This function checks whether specified data connection is available.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] uiConnID Specifies the connection identifier, which is returned from jdd_DataConnMakeConnection
 * 
 * Some platform does not provide information when the data connection is dropped and this function is 
 * provided to cross check whether the data connection is available or not. 
 * If the platform can invoke JC_DATA_CONN_DISCONNECT_CALLBACK when the data connection is disconnected, then 
 * this function can return JC_OK without any check.
 */
JC_RETCODE jdd_DataConnIsAvailable (JDD_DATA_CONN_HANDLE vDCHandle, JC_UINT32 uiConnID) ;

/**
 * @brief This function compares whether both the data connection are same.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] pstDataConn1 Specifies the data connection information, which needs to be compared.
 * @param[in] pstDataConn2 Specifies the other data connection information, which needs to be compared.
 * 
 * Returns E_TRUE if both the data connection are matched, otherwise it should return E_FALSE.
 * The application shall use this API to re-use the existing data connection for the new session, if
 * other data connection is already connected. 
 */
JC_BOOLEAN jdd_DataConnCompare (JDD_DATA_CONN_HANDLE		vDCHandle,
								const ST_GENERIC_DATA_CONN	*pstDataConn1,
								const ST_GENERIC_DATA_CONN	*pstDataConn2) ;

/**
 * @brief This function provides the IP address associated with this connection ID.
 * @param[in] vDCHandle Specifies the data connection handle, which is returned from jdd_DataConnInitialize.
 * @param[in] uiConnID Specifies the connection identifier, which is returned from jdd_DataConnMakeConnection
 * @param[out] pcIPAddress Returns the IP Address assigned to the given connection identifier.
 * @Warning This function has been deprecated as most of the environment is not able to 
 * provide the IP Address.
 */
JC_RETCODE jdd_DataConnGetIPAddress (JDD_DATA_CONN_HANDLE	vDCHandle, 
									 JC_UINT32				uiConnID,
									 JC_INT8				*pcIPAddress) ;


#ifdef __cplusplus
}
#endif

#endif

