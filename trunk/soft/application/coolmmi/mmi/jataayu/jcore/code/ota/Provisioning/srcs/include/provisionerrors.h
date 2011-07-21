
/***************************************************************************
 * $Id: Provisionerrors.h,v 1.4 2007/01/31 05:39:24 lumitdevpclient Exp $
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
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: Provisionerrors.h,v $
 * Revision 1.4  2007/01/31 05:39:24  lumitdevpclient
 * Compiler warning and Lint warning fixed as reported by TTPCom
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
 * Revision 1.6  2005/08/09 15:10:31  guestpclient
 * Modified and changed to suit for the doxygen style and new coding standards.
 *
 *
 * Revision 1.1  2005/08/06 10:52:26  arundevpclient
 * Modified it to doxygen standard
 *
 *  
 ***************************************************************************/

#ifndef _PROVISION_ERRORS_H
#define _PROVISION_ERRORS_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

#define JC_ERR_PROV_NO_IDENTIFIER					-4500
#define JC_ERR_PROV_DATA_CONNECTION					-4501
#define JC_ERR_PROV_DOCUMENT						-4502
#define JC_ERR_PROV_PXLOGICAL_IDENTIFIER			-4503
#define JC_ERR_PROV_NAPDEF_IDENTIFIER				-4504
#define JC_ERR_PROV_APPLICATION_IDENTIFIER			-4505
#define JC_ERR_PROV_ACCESS_IDENTIFIER				-4506
#define JC_ERR_PROV_INVALID_CONTENT_TYPE			-4507

#define JC_ERR_PROV_CONDITION_QUOTES				-6001
#define JC_ERR_PROV_PARM_NOT_FOUND					-6002
#define JC_ERR_PROV_ACTIVE_DOCUMENT					-6003
#define JC_ERR_PROV_CHARACTERISTIC_PATH				-6004
#define JC_ERR_PROV_CONDITION						-6005
#define JC_ERR_PROV_INVALID_DOCUMENT				-6006
#define JC_ERR_PROV_INVALID_PARM					-6007
#define JC_ERR_PROV_PROVURL							-6008
#define JC_ERR_PROV_CHAR_NOT_FOUND					-6009

#define JC_ERR_PROV_PARM_NOT_FOUND_TO_UPDATE		-6100
#define JC_ERR_PROV_CANNOT_ADD_TO_EXISTING			-6101
#define JC_ERR_PROV_NO_ACTION_TAKEN					-6102
#define JC_ERR_PROV_MORE_DOMAIN_PARM				-6103
#define JC_ERR_PROV_CANNOT_ADD_WITHOUT_BASE			-6104
#define JC_ERR_PROV_NOT_CHARACTERISTIC_TAG			-6105
#define JC_ERR_PROV_STRUCT_TO_XML					-6106
#define JC_ERR_PROV_INVALID_TAG_INPUT				-6107
#define JC_ERR_PROV_VALUE_TAG_NOT_FOUND				-6108

#define JC_ERR_PROV_NOT_PXLOGICAL_CHAR				-6110
#define JC_ERR_PROV_NOT_PXAUTHINFO_CHAR				-6111
#define JC_ERR_PROV_NOT_PORT_CHAR					-6112
#define JC_ERR_PROV_NOT_PXPHYSICAL_CHAR				-6113
#define JC_ERR_PROV_NOT_NAPAUTHINFO_CHAR			-6114
#define JC_ERR_PROV_NOT_VALIDITY_CHAR				-6115
#define JC_ERR_PROV_NOT_APPAUTH_CHAR				-6116
#define JC_ERR_PROV_NOT_NAPDEF_CHAR					-6117
#define JC_ERR_PROV_NOT_BOOTSTRAP_CHAR				-6118
#define JC_ERR_PROV_NOT_VENDORCONFIG_CHAR			-6119
#define JC_ERR_PROV_NOT_CLIENTIDENTITY_CHAR			-6120
#define JC_ERR_PROV_NOT_APPLICATION_CHAR			-6121
#define JC_ERR_PROV_NOT_APPADDR_CHAR				-6122
#define JC_ERR_PROV_NOT_RESOURCE_CHAR				-6123
#define JC_ERR_PROV_NOT_ACCESS_CHAR					-6124

#define JC_ERR_PROV_UNKNOWN_EVENT					-6125 
#define JC_ERR_PROV_NO_INDEX_KEY					-6126 
#define JC_ERR_PROV_INVALID_CHARACTERISTIC			-6127
#define JC_ERR_PROV_NO_COUNT_VALUE					-6128
#define JC_ERR_PROV_INDEX_OUT_OF_RANGE				-6129
#define	JC_ERR_PROV_ALREADY_PRESENT					-6130
#define JC_ERR_PROV_NOT_PRESENT						-6131
#define JC_ERR_PROV_STORE_NEW_DOC					-6132 
#define JC_ERR_PROV_IGNORE_NEW_DOC					-6132 
#define JC_ERR_PROV_LESS_THAN_MAX_DOC				-6133 
#define JC_ERR_PROV_MAX_DOC_EXCEED					-6134
#define JC_ERR_PROV_MORE_CONFIG_FILES_ERROR			-6135
#define JC_ERR_PROV_MAX_DOC_LENGTH_EXCEED			-6136

#define JC_ERR_PROV_CHAR_PXPHYSICAL					-6140

#define JC_ERR_PROV_NO								-7000
#define JC_ERR_ACTIVE_PROFILE                       -7001
#define JC_ERR_INVALID_HANDLE                       -7002
#define JC_ERR_PROFILENAME_EXISTS                   -7003
//#define JC_ERR_PROVURL_EXISTS                     -7004
//#define JC_ERR_URL_AND_NAME_EXISTS                -7005
#define JC_ERR_AUTHENTICATION                       -7006
#define JC_ERR_ALREADY_ACTIVE						-7007
#define JC_ERR_NAME_VAL_ABSENT						-7008
#define JC_ERR_PROVURL_ABSENT_IN_DOC				-7009

/**< added on 03-10-05 by Lumit as to handle the document validity */
#define JC_ERR_PXPHYSICAL_NOT_FOUND					-8001	// now internally handled
#define JC_ERR_TONAPID_NOT_FOUND					-8002
#define JC_ERR_ACCESS_RULE_NOT_FOUND				-8003
#define JC_ERR_MUST_FIELD_NOT_FOUND					-8004
#define JC_ERR_REDUNDANT_NAP_ID						-8005
#define JC_ERR_NOT_GLOBAL_CHAR						-8006
#define JC_ERR_NOT_SUB_CHARACTERISTIC				-8007
#define JC_ERR_PROFILE_NOT_EXIST					-8008
#define JC_ERR_PROV_NAPDEF_ABSENT					-8009
#define JC_ERR_PROV_NO_DEFULT_CONNECTION			-8010

#define JC_ERR_PROXY_NO_MATCH						-9001
#define JC_ERR_NAPID_NO_MATCH						-9002
#define JC_ERR_CONDITION_NO_MATCH					-9003
#define JC_ERR_INVALID_PARM_INPUT					-9004
#define JC_ERR_INVALID_SUBCHAR_INPUT				-9005
#define JC_ERR_BLOCK_MANIPULATION					-9006
#define JC_ERR_SUBCHAR_EXIST						-9007

/**< Arun added on 21/12/05 for SIM card provisioning */
#define JC_ERR_PROV_SIM_NO_BUF_FOR_UPDATE			-9008
#define JC_ERR_PROV_NULL_CALLBACK					-9009

/**< Lumit added on 16th Oct'06 for version issue raised */
#define JC_ERR_WAP_VERSION_NOT_SUPPORTED			-9100

/**< Lumit added on 18th Oct'06 for the partial data returning */
#define JC_OK_PARTIAL_CONTEXT_RETRIEVED				-9200



#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISION_ERRORS_H */


/*** END OF FILE ***/
