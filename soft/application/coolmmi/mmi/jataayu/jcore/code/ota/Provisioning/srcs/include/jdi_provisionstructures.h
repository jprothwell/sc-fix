/***************************************************************************
 * $Id: jdi_ProvisionStructures.h,v 1.5 2007/01/31 05:39:24 lumitdevpclient Exp $
 * $Revision: 1.5 $
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
 * $Log: jdi_ProvisionStructures.h,v $
 * Revision 1.5  2007/01/31 05:39:24  lumitdevpclient
 * Compiler warning and Lint warning fixed as reported by TTPCom
 *
 * Revision 1.4  2006/11/17 11:28:01  lumitdevpclient
 * Email SMTP settings (From & ReplyTo) have added to Application & modified for GetDataValue
 *
 * Revision 1.3  2006/11/07 04:13:10  lumitdevpclient
 * Containing Fixes for ignoring Invalid PxLogical & rejecting unsupported wap-version context
 *
 * Revision 1.2  2006/01/04 06:50:14  lumitdevpclient
 * committ after fixing some continuous provisioning related discripancies
 *
 * Revision 1.5  2005/08/19 13:45:58  guestpclient
 * Code release on 19th August before changing on the comments on TTPcom
 *
 * Revision 1.7  2005/08/09 15:10:31  guestpclient
 * Modified and changed to suit for the doxygen style and new coding standards.
 *
 *
 * Revision 1.1  2005/08/06 10:52:26  arundevpclient
 * Modified it to doxygen standard
 *
 *  
 ***************************************************************************/

#ifndef _PROVISION_STRUCTURES_H
#define _PROVISION_STRUCTURES_H

#ifdef JOTA_PROVISION_CORE_LIBRARY


#define MIMETYPE_CONNECTIVITY_XML		"text/vnd.wap.connectivity-xml"
#define MIMETYPE_CONNECTIVITY_WBXML		"application/vnd.wap.connectivity-wbxml"
#define CHARACTERISTIC_TAG				"characteristic"
#define PARM_TAG						"parm"
#define BOOTSTRAP_TAG					"BOOTSTRAP"
#define TYPE_TAG						"type"
#define VALUE_TAG						"value"	
#define NAME_TAG						"name"
#define CONDITION_SEPARATOR				"&&"
#define CHARACTERISTIC_SEPARATOR		"/"
#define INDEX_CONDITION					"index"
#define KEY_CONDITION					"key"
#define COUNT_CONDITION					"count"
#define ADD_CONDITION					"add"
#define UPDATE_CONDITION				"update"
#define T_INLINE						"t_inline"						

#define PXLOGICAL_CHAR					"PXLOGICAL"
#define PXPHYSICAL_CHAR					"PXPHYSICAL"
#define PXAUTHINFO_CHAR					"PXAUTHINFO"
#define PORT_CHAR						"PORT"
#define NAPDEF_CHAR						"NAPDEF"
#define NAPAUTHINFO_CHAR				"NAPAUTHINFO"
#define VALIDITY_CHAR					"VALIDITY"
#define BOOTSTRAP_CHAR					"BOOTSTRAP"
#define CLIENTIDENTITY_CHAR				"CLIENTIDENTITY"
#define VENDORCONFIG_CHAR				"VENDORCONFIG"
#define APPLICATION_CHAR				"APPLICATION"
#define APPADDR_CHAR					"APPADDR"
#define APPAUTH_CHAR					"APPAUTH"
#define RESOURCE_CHAR					"RESOURCE"
#define ACCESS_CHAR						"ACCESS"

#define PROXY_ID						"PROXY-ID"
#define PROXY_PW						"PROXY-PW"
#define PPGAUTH_TYPE					"PPGAUTH-TYPE"
#define PROXY_PROVIDER_ID				"PROXY-PROVIDER-ID"
#define NAME							"NAME"
#define PROV_DOMAIN						"DOMAIN"
#define TRUST							"TRUST"
#define MASTER							"MASTER"
#define STARTPAGE						"STARTPAGE"
#define BASAUTH_ID						"BASAUTH-ID"
#define BASAUTH_PW						"BASAUTH-PW"
#define WSP_VERSION						"WSP-VERSION"
#define PUSHENABLED						"PUSHENABLED"
#define PULLENABLED						"PULLENABLED"
#define PXAUTH_TYPE						"PXAUTH-TYPE"
#define PXAUTH_ID						"PXAUTH-ID"
#define PXAUTH_PW						"PXAUTH-PW"
#define PORTNBR							"PORTNBR"
#define SERVICE							"SERVICE"
#define PHYSICAL_PROXY_ID				"PHYSICAL-PROXY-ID"
#define PXADDR							"PXADDR"
#define PXADDRTYPE						"PXADDRTYPE"
#define PXADDR_FQDN						"PXADDR-FQDN"
#define TO_NAPID						"TO-NAPID"
#define NAPID							"NAPID"
#define BEARER							"BEARER"
#define INTERNET						"INTERNET"
#define NAP_ADDRESS						"NAP-ADDRESS"
#define NAP_ADDRTYPE					"NAP-ADDRTYPE"
#define DNS_ADDR						"DNS-ADDR"
#define CALLTYPE						"CALLTYPE"
#define LOCAL_ADDR						"LOCAL-ADDR"
#define LOCAL_ADDRTYPE					"LOCAL-ADDRTYPE"
#define LINKSPEED						"LINKSPEED"
#define DNLINKSPEED						"DNLINKSPEED"
#define LINGER							"LINGER"
#define DELIVERY_ERR_SDU				"DELIVERY-ERR-SDU"
#define DELIVERY_ORDER					"DELIVERY-ORDER"
#define TRAFFIC_CLASS					"TRAFFIC-CLASS"
#define MAX_SDU_SIZE					"MAX-SDU-SIZE"
#define MAX_BITRATE_UPLINK				"MAX-BITRATE-UPLINK"
#define MAX_BITRATE_DNLINK				"MAX-BITRATE-DNLINK"
#define RESIDUAL_BER					"RESIDUAL-BER"
#define SDU_ERROR_RATIO					"SDU-ERROR-RATIO"
#define TRAFFIC_HANDL_PRIO				"TRAFFIC-HANDL-PRIO"
#define TRANSFER_DELAY					"TRANSFER-DELAY"
#define GUARANTEED_BITRATE_UPLINK		"GUARANTEED-BITRATE-UPLINK"
#define GUARANTEED_BITRATE_DNLINK		"GUARANTEED-BITRATE-DNLINK"
#define MAX_NUM_RETRY					"MAX-NUM-RETRY"
#define FIRST_RETRY_TIMEOUT				"FIRST-RETRY-TIMEOUT"
#define REREG_THRESHOLD					"REREG-THRESHOLD"
#define T_BIT							"T-BIT"
#define AUTHTYPE						"AUTHTYPE"
#define AUTHNAME						"AUTHNAME"
#define AUTHSECRET						"AUTHSECRET"
#define AUTH_ENTITY						"AUTH-ENTITY"
#define SPI								"SPI"
#define COUNTRY							"COUNTRY"
#define NETWORK							"NETWORK"
#define SID								"SID"
#define SOC								"SOC"
#define VALIDUNTIL						"VALIDUNTIL"
#define PROVURL							"PROVURL"
#define CONTEXT_ALLOW					"CONTEXT-ALLOW"
#define CLIENT_ID						"CLIENT-ID"
#define APPID							"APPID"
#define PROVIDER_ID						"PROVIDER-ID"
#define AACCEPT							"AACCEPT"
#define APROTOCOL						"APROTOCOL"
#define TO_PROXY						"TO-PROXY"
#define ADDR							"ADDR"
#define ADDRTYPE						"ADDRTYPE"
#define AAUTHLEVEL						"AAUTHLEVEL"
#define AAUTHTYPE						"AAUTHTYPE"
#define AAUTHNAME						"AAUTHNAME"
#define AAUTHSECRET						"AAUTHSECRET"
#define URI								"URI"
#define AAUTHDATA						"AAUTHDATA"
#define RULE							"RULE"

/**< newly added on 17th Nov'06 */
/**< Below mentioned fields are non OMA standards added for Email SMTP support */
#ifdef SUPPORT_EMAIL_SMTP
#define FROM							"FROM"
#define RT_ADDR							"RT-ADDR"
#endif // SUPPORT_EMAIL_SMTP
/**< end of defn...*/

/**< newly added on 5th Oct'05 */
#define DEFAULT							"DEFAULT"
#define DEFAULT_PXADDRTYPE				"IPV4"
#define DEFAULT_LOCALADDRTYPE			"IPV6"
#define DEFAULT_CALLTYPE				"ANALOG-MODEM"
#define DEFAULT_WSPVERSION				"1.2"
#define EQUAL_TOKEN						"="
#define ZERO_VALUE						"0"
#define ONE_VALUE						"1"
#define ONE								1
#define TWO								2
/**< end of defn...*/

#define MAX_XML_DOC_LENGTH				10240

#define MAX_XML_LINE					1024

#define XML_VERSION						"<?xml version=\"1.0\"?>"
#define XML_DOC_TYPE					"<!DOCTYPE wap-provisioningdoc PUBLIC \"-//WAPFORUM//DTD PROV 1.0//EN\" \"http://www.wapforum.org/DTD/prov.dtd\">"

#define	XML_HEADER						"<wap-provisioningdoc version=\"1.1\">"
#define	XML_END							"</wap-provisioningdoc>"

#define PROV_CONTENT_TYPE				"text/vnd.wap.connectivity-xml" 
#define ACTIVE_DOCUMENT									0
#define PRIVILEGED_CONFIGURATION_CONTEXT				1
#define NUMBER_OF_GLOBAL_CHAR_BLOCKS					7


/** definitions added on 16th Oct by Lumit */

#define WAP_PROVISIONINGDOC				"wap-provisioningdoc"
#define VERSION_INFO					"version"

/**
 * @brief Callback defination goes here
 */

/**
 * @brief Callback declaration for HMAC-SHA1 algorithm to get the MAC buffer
 * Here it should take KEY and MESSAGE and generates the MAC using HMAC-SHA1 algo
 */
typedef  JC_RETCODE (*PROV_CALLBACK_HMAC) (JC_UINT8 *pucKey,JC_UINT32 uiKeyLength, 
									  JC_UINT8 *pucMsg, JC_UINT32 uiMsgLength, 
									  JC_UINT8 **ppucData,JC_UINT32 *puiLength);



/**
 * @brief Declaration of all the structure variable goes here
 */


typedef struct VALUES_tag
{
	JC_INT8				*pcValue;
	struct VALUES_tag	*pstNext ;
} ST_PROV_VALUES ;

typedef struct NAME_VALUE_tag
{
	JC_INT8					*pcName ;
	JC_INT8					*pcValue;
	struct NAME_VALUE_tag	*pstNext ;
} ST_PROV_NAME_VALUES ;





typedef struct pxauthinfo_struct
{
	JC_INT8			*pcPxauthType ;		
	JC_INT8			*pcPxauthId ;
	JC_INT8			*pcPxauthPw ;
	struct pxauthinfo_struct	*pstNextPxauthInfo ;
}ST_PROV_PXAUTHINFO ;


typedef struct port_struct
{
	JC_INT8				*pcPortNumber ;
	ST_PROV_VALUES		*pstService ;
	struct port_struct	*pstNextPort ;
} ST_PROV_PORT ;


typedef struct pxphysical_struct
{
	JC_INT8				*pcPhysicalProxyId ;
	JC_INT8				*pcPxaddr ;
	JC_INT8				*pcPxaddrType ;
	JC_INT8				*pcPxaddrFqdn ;
	JC_INT8				*pcWspVersion ;
	JC_INT8				*pcPushEnabled ;
	JC_INT8				*pcPullEnabled ;
	ST_PROV_VALUES		*pstDomain ;
	ST_PROV_VALUES		*pstToNapid ;
	struct port_struct	*pstPxphysicalPort ;
	struct pxphysical_struct	*pstNextPxphysical ;
}ST_PROV_PXPHYSICAL ;


typedef struct pxlogical_struct
{
	JC_INT8			*pcProxyId ;
	JC_INT8			*pcProxyPw ;
	JC_INT8			*pcPpgauthType ;		
	JC_INT8			*pcProxyProviderId ;
	JC_INT8			*pcName ;
	JC_INT8			*pcTrust ;
	JC_INT8			*pcMaster ;
	JC_INT8			*pcStartPage ;
	JC_INT8			*pcBasauthId ;
	JC_INT8			*pcBasauthPw ;
	JC_INT8			*pcWspVersion ;
	JC_INT8			*pcPushEnabled ;
	JC_INT8			*pcPullEnabled ;
	ST_PROV_VALUES	*pstDomain ;
	struct pxauthinfo_struct	*pstPxlogicalAuthinfo ;
	struct port_struct			*pstPxlogicalPort ;
	struct pxphysical_struct	*pstPxlogicalPxphysical ;
	struct pxlogical_struct		*pstNextPxlogical ;
} ST_PROV_PXLOGICAL ;


typedef struct napauthinfo_struct
{
	JC_INT8						*pcAuthType ;
	JC_INT8						*pcAuthName ;
	JC_INT8						*pcAuthSecret ;
	JC_INT8						*pcSpi ;
	ST_PROV_VALUES				*pstAuthEntity ;
	struct napauthinfo_struct	*pstNextNapauthinfo ;
}ST_PROV_NAPAUTHINFO ;


typedef struct validity_struct
{
	JC_INT8					*pcCountry ;
	JC_INT8					*pcNetwork ;
	JC_INT8					*pcSid ;
	JC_INT8					*pcSoc ;
	JC_INT8					*pcValidUntil ;
	struct validity_struct	*pstNextValidity ;
}ST_PROV_VALIDITY ;


typedef struct napdef_struct
{
	JC_INT8						*pcNapid ;
	JC_INT8						*pcName ;
	JC_INT8						*pcInternet ;
	JC_INT8						*pcNapAddress ;
	JC_INT8						*pcNapAddrType ;
	JC_INT8						*pcCallType ;
	JC_INT8						*pcLocalAddr ;
	JC_INT8						*pcLocalAddrType ;
	JC_INT8						*pcLinkSpeed ;
	JC_INT8						*pcDnLinkSpeed ;
	JC_INT8						*pcLinger ;
	JC_INT8						*pcDeliveryErrSdu ;
	JC_INT8						*pcDeliveryOrder ;
	JC_INT8						*pcTrafficClass ;
	JC_INT8						*pcMaxSduSize ;
	JC_INT8						*pcMaxBitrateUplink ;
	JC_INT8						*pcMaxBitrateDnlink ;
	JC_INT8						*pcResidualBer ;
	JC_INT8						*pcSduErrorRatio ;
	JC_INT8						*pcTrafficHandlPrio ;
	JC_INT8						*pcTransferDelay ;
	JC_INT8						*pcGuaranteedBitrateUplink ;
	JC_INT8						*pcGuaranteedBitrateDnlink ;
	JC_INT8						*pcMaxNumRetry ;
	JC_INT8						*pcFirstRetryTimeout ;
	JC_INT8						*pcReregThreshold ;
	JC_INT8						*pcTBit ;
	ST_PROV_VALUES				*pstBearer ;
	ST_PROV_VALUES				*pstDnsAddr ;
	struct napauthinfo_struct	*pstNapdefNapAuthInfo ;
	struct validity_struct		*pstNapdefValidity ;
	struct napdef_struct		*pstNextNapdef ;
}ST_PROV_NAPDEF ;


typedef struct bootstrap_struct
{
	JC_INT8						*pcName ;
	JC_INT8						*pcCountry ;
	JC_INT8						*pcProvUrl ;
	JC_INT8						*pcContextAllow ;
	ST_PROV_VALUES				*pstProxyId ;
	ST_PROV_VALUES				*pstNetwork ;
	struct bootstrap_struct		*pstNextBootstrap ;
}ST_PROV_BOOTSTRAP ;


typedef struct clientidentity_struct
{
	JC_INT8		*pcClientId ;
}ST_PROV_CLIENTIDENTITY ;



typedef struct vendorconfig_struct
{
	JC_INT8						*pcName ;
	ST_PROV_NAME_VALUES			*pstParmNameValue ;
	struct vendorconfig_struct	*pstNextVendorConfig ;
}ST_PROV_VENDORCONFIG ;


typedef struct appaddr_struct
{
	JC_INT8					*pcAddr ;
	JC_INT8					*pcAddrType ;
	struct port_struct		*pstAppaddrPort ;
	struct appaddr_struct	*pstNextAppaddr ;
}ST_PROV_APPADDR ;


typedef struct appauth_struct
{
	JC_INT8					*pcAauthLevel ;
	JC_INT8					*pcAauthType ;
	JC_INT8					*pcAauthName ;
	JC_INT8					*pcAauthSecret ;
	JC_INT8					*pcAauthData ;
	struct appauth_struct	*pstNextAppauth ;
}ST_PROV_APPAUTH ;


typedef struct resource_struct
{
	JC_INT8					*pcUri ;
	JC_INT8					*pcName ;
	JC_INT8					*pcAaccept ;
	JC_INT8					*pcAauthType ;
	JC_INT8					*pcAauthName ;
	JC_INT8					*pcAauthSecret ;
	JC_INT8					*pcAauthData ;
	JC_INT8					*pcStartPage ;
	struct resource_struct	*pstNextResource ;
}ST_PROV_RESOURCE ;


typedef struct application_struct
{
	JC_INT8						*pcAppid ;
	JC_INT8						*pcProviderId ;
	JC_INT8						*pcName ;
	JC_INT8						*pcAaccept ;
	JC_INT8						*pcAprotocol ;
#ifdef SUPPORT_EMAIL_SMTP
	JC_INT8						*pcFrom ;		// From field in Email
	JC_INT8						*pcReplyTo ;	// Reply to field in Email
#endif // SUPPORT_EMAIL_SMTP
	ST_PROV_VALUES				*pstToProxy ;
	ST_PROV_VALUES				*pstToNapid ;
	ST_PROV_VALUES				*pstAddr ;
	struct appaddr_struct		*pstApplicationAppaddr ;
	struct appauth_struct		*pstApplicationAppauth ;
	struct resource_struct		*pstApplicationResource ;
	struct application_struct	*pstNextApplication ;
}ST_PROV_APPLICATION ;


typedef struct access_struct
{
	ST_PROV_VALUES			*pstRule ;
	ST_PROV_VALUES			*pstAppid ;
	ST_PROV_VALUES			*pstPortNumber ;
	ST_PROV_VALUES			*pstDomain ;
	ST_PROV_VALUES			*pstToNapid ;
	ST_PROV_VALUES			*pstToProxy ;
	struct access_struct	*pstNextAccess ;
}ST_PROV_ACCESS ;

typedef enum _jc_valid_characteristic_names
{
	E_APPLICATION = 0,
	E_ACCESS,
	E_BOOTSTRAP,
	E_PXLOGICAL ,
	E_NAPDEF,
	E_CLIENTIDENTITY,
	E_VENDORCONFIG,
    /** <Internal Characteristic Structures> */
	E_PXAUTHINFO = 7,
	E_PORT,
	E_PXPHYSICAL,
	E_NAPAUTHINFO,
	E_VALIDITY,
	E_APPAUTH,
	E_APPADDR,
	E_RESOURCE,
	E_CHAR_NOT_FOUND
}E_PROV_CHARNAME ;


typedef enum 
{ 
	E_CONTINUOUS_PROVISION_FLAG, 
	E_BOOTSTRAP_FLAG 
} E_PROV_DATA_TYPE;

typedef enum st_sec_type
{
 E_SEC_NOT_DEFINED = -1,
 E_NETWPIN ,
 E_USERPIN,
 E_USERNETWPIN,
 E_USERPINMAC
} E_SEC_TYPE;


typedef enum e_prov_callback_type 
{
	E_NO_CALLBACKS_SUPPORTED, 
	E_AUTHENTICATE_HMAC_SHA_CALLBACK
}E_PROV_CALLBACK_TYPE  ;

typedef union u_prov_callback
{
	PROV_CALLBACK_HMAC pfHmacSha ;
}U_PROV_CALLBACK ;



typedef struct _profile_names
{
 JC_INT8			   *pcAppID; 
 JC_INT8			   *pcAppName; 
 struct _profile_names *pstNext;          
} ST_PROFILE_NAME_LIST ;


typedef struct _profile_detail_info_
{
	ST_PROV_BOOTSTRAP		*pstBootstrap;
	ST_PROV_APPLICATION		*pstApplication;
	ST_PROV_ACCESS			*pstAccess;
	ST_PROV_NAPDEF			*pstNapdef;
	ST_PROV_PXLOGICAL		*pstPxLogical;
	ST_PROV_CLIENTIDENTITY	*pstClientIdentity;
	ST_PROV_VENDORCONFIG	*pstVendorConfig;	
} ST_PROFILE_DETAIL_INFO;

/*** Added on 15th Oct to implement profile detail info api**/

typedef struct _profile_bootstrap
{
	ST_PROV_BOOTSTRAP			*pstBootstrap;
	struct _profile_bootstrap	*pstNextProfileBootstrap;
}ST_PROFILE_BOOTSTRAP ;

typedef struct _profile_application
{
	ST_PROV_APPLICATION			*pstApplication;
	struct _profile_application	*pstNextProfileApplication;
}ST_PROFILE_APPLICATION ;

typedef struct _profile_access
{
	ST_PROV_ACCESS			*pstAccess;
	struct _profile_access	*pstNextProfileAccess;
}ST_PROFILE_ACCESS ;
typedef struct _profile_napdef
{
	ST_PROV_NAPDEF			*pstNapdef;
	struct _profile_napdef	*pstNextProfileNapdef;
}ST_PROFILE_NAPDEF ;

typedef struct _profile_pxlogical
{
	ST_PROV_PXLOGICAL			*pstPxlogical;
	struct _profile_pxlogical	*pstNextProfilePxlogical;
}ST_PROFILE_PXLOGICAL ;

typedef struct _profile_clientid
{
	ST_PROV_CLIENTIDENTITY			*pstClientIdentity;
}ST_PROFILE_CLIENTIDENTITY ;

typedef struct _profile_vendorconf
{
	ST_PROV_VENDORCONFIG		*pstVendorConf;
	struct _profile_vendorconf	*pstNextProfileVendorConf;
}ST_PROFILE_VENDORCONFIG ;

typedef struct _profile_info_
{
	ST_PROFILE_BOOTSTRAP		*pstProfileBootstrap;
	ST_PROFILE_APPLICATION		*pstProfileApplication;
	ST_PROFILE_ACCESS			*pstProfileAccess;
	ST_PROFILE_NAPDEF			*pstProfileNapdef;
	ST_PROFILE_PXLOGICAL		*pstProfilePxLogical;
	ST_PROFILE_CLIENTIDENTITY	*pstProfileClientIdentity;
	ST_PROFILE_VENDORCONFIG		*pstProfileVendorConfig;	

} ST_PROFILE_INFO;

/**< End of adding New structures */

/**< Added by Lumi on 3-9-05 for API development */

typedef struct _prov_condition
{ 
	JC_INT32				iCheck ; 
	JC_INT8					*pcKey ; 
	JC_INT8					*pcValue; 
	struct	_prov_condition	*pstNext; 
	struct	_prov_condition	*pstPrevious;
	 	
}ST_PROV_CONDITION ; 

typedef struct _prov_list
{ 
	JC_INT8				*pcValue ; 
	JC_INT8				*pcId ; 
	struct	_prov_list	*pstNext ; 
	struct	_prov_list	*pstPrevious ; 
}ST_PROV_LIST ; 

/**
 * @brief THIS SECTION CONTAINS DATABASE RELATED INFORMATION 
 */


#ifdef ENABLE_PROV_DATABASE 

#define PROF_APP_ID_SIZE	8
#define PROF_APP_NAME_SIZE	64
#define PROF_URL_SIZE		256

/**< This means when application wants its own interface for DB */
#ifdef USE_EXTERNAL_DATABASE_INTERFACE

	/**
	 * @brief Database specific callbacks if external interface req
	 */


	typedef JC_RETCODE (*PROV_DB_CALLBACK_STORE) (JC_INT8* pcKey, JC_UINT8 *pucData, JC_UINT32 uiBufferLen) ;

	typedef JC_RETCODE (*PROV_DB_CALLBACK_RETRIEVE) (JC_INT8* pcKey, JC_UINT8 **ppucData, JC_UINT32 *puiBufferLen) ;

	typedef JC_RETCODE (*PROV_DB_CALLBACK_DELETE) (JC_INT8* pcKey) ;
	
#endif /**< USE_EXTERNAL_DATABASE_INTERFACE */


typedef struct profile_index
{
	JC_INT8					acAppName[PROF_APP_NAME_SIZE];
	JC_INT8					acAppID[PROF_APP_ID_SIZE];
    JC_INT8					acProvURL[PROF_URL_SIZE];
    JC_BOOLEAN				bIsActive;
	JC_BOOLEAN				bIsPrevilegedContext ;
	JC_UINT32				uiIndex;
	JC_UINT32				uiFileIndex;
    struct profile_index	*pstNext;  
} ST_PROFILE_INDEX ;



/**< Arun added for data base support */
 typedef struct profile_details
 {
	 JC_INT8*			pcAppName;            
	 JC_INT8*			pcAppID;            
	 JC_BOOLEAN         bIsActive;
	 JC_BOOLEAN         bIsPrevlgdConfigContext;
	 struct profile_details *pstNext;          
 } ST_PROFILE_DETAILS ;

/**< Arun added for the configuration data */
typedef struct st_prov_config_data
{
	JC_INT8		*pcProvConfigPath ;
	JC_INT32	iPathLengh ;
	JC_INT32	iMaxFileCount ;
#ifdef USE_EXTERNAL_DATABASE_INTERFACE
	/**< Arun added on 26th oct to have Ext DB interface */
	PROV_DB_CALLBACK_STORE	pfSave ;
	PROV_DB_CALLBACK_RETRIEVE	pfRetrieve ;
	PROV_DB_CALLBACK_DELETE		pfDelete ;
#endif
}ST_PROV_CONFIG_DATA ;


#endif /**< ENABLE_PROV_DATABASE */

typedef struct _provision_info_
{
	ST_PROFILE_DETAIL_INFO	*pstProvDetails ;
	JC_INT8				*pcAppID;
	JC_INT8				*pcAppName;
	JC_INT8				*pcProvURL;
	JC_UINT8			*pucProvBuffer;
	JC_UINT32			uiBufferLen;
	JC_INT8				*pcMACBuffer;
	JC_UINT32			uiMACBufferLen; 
	PROV_CALLBACK_HMAC  pfHmacCallback ;

#ifdef ENABLE_PROV_DATABASE
	ST_PROFILE_INDEX	*pstProfileIndexList;
#endif

}ST_PROVISION_INFO;


/**< Arun added on 29/11/2005 */

typedef enum EMessageType 
{ 
	E_MSG_XML, 
	E_MSG_WBXML 
} E_MESSAGE_TYPE ;
#endif /**< JOTA_PROVISION_CORE_LIBRARY */


#endif /**< _PROVISION_STRUCTURES_H */


/*** END OF FILE ***/
