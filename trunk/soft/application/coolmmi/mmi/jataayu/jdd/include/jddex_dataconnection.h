/***************************************************************************
 * $Id: jddex_dataconnection.h,v 1.6 2008/08/14 08:25:27 browserdevjtbase Exp $
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
 * $Log: jddex_dataconnection.h,v $
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

#ifndef _JDDEX_DATACONNECTION_H
#define _JDDEX_DATACONNECTION_H


#ifdef __ADP_MULTI_SIM__
#include "cfw_multi_sim.h"
CFW_SIM_ID wapmmsgetsimid (UINT32 uiConnID) ;
#else
typedef  unsigned char CFW_SIM_ID ;
#endif

typedef enum e_dataconn_state
{
	E_DATACONN_ATTACHING,
	E_DATACONN_ACTIVATING,
	E_DATACONN_ACTIVATED,
	E_DATACONN_DEACTIVATING
} E_DATACONN_STATE ;

typedef struct jc_user_info_connid
{
	 CFW_SIM_ID			nSimID ;
	 UINT8				nCid ;
	 UINT16				nUTI ;
	 JC_INT8			*ApnAddr ;
	 JC_INT8 				acUserName [MAX_DATA_CONN_USERNAME_SIZE] ; /**< Specifies the user name for the data connection */
	 JC_INT8					acPassword [MAX_DATA_CONN_PASSWORD_SIZE] ; /**< Specifies the password for the data connection */
	 E_DATACONN_STATE	eConState ;
	 JC_BOOLEAN			bIsCancelRequested ;
	 struct jc_user_info_connid *pstNext;
} JC_USER_INFO_CONNID ;
#endif
