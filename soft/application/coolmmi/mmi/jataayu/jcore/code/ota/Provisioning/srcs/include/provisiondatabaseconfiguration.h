
/***************************************************************************
 * $Id: ProvisionDatabaseConfiguration.h,v 1.2 2006/01/04 06:50:14 lumitdevpclient Exp $
 * $Revision: 1.2 $
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
 * $Log: ProvisionDatabaseConfiguration.h,v $
 * Revision 1.2  2006/01/04 06:50:14  lumitdevpclient
 * committ after fixing some continuous provisioning related discripancies
 *
 * 
 *
 * Revision 1.1  2005/08/06 10:52:26  arundevpclient
 * Modified it to doxygen standard
 *
 *  
 ***************************************************************************/


#ifndef _PROVISIONING_DATA_CONFIG_H
#define _PROVISIONING_DATA_CONFIG_H

#ifdef JOTA_PROVISION_CORE_LIBRARY
#ifdef ENABLE_PROV_DATABASE



JC_RETCODE PrDbConfInitializeConfData (void) ;


JC_RETCODE PrDbConfSetProvConfdata (ST_PROV_CONFIG_DATA *pstProvConfData) ;

const JC_INT8* PrDbConfGetProvConfPath (void) ;

const JC_INT32 PrDbConfGetProvConfPathLen (void) ;

const JC_INT32 PrDbConfGetProvMaxFileCount (void) ;

void PrDbConfDeinitializeConfData (void) ;



#ifdef USE_EXTERNAL_DATABASE_INTERFACE

PROV_DB_CALLBACK_STORE prDbConfGetCallbackSave (void) ;

PROV_DB_CALLBACK_RETRIEVE prDbConfGetCallbackRetrieve (void) ;

PROV_DB_CALLBACK_DELETE prDbConfGetCallbackDelete (void) ;

#endif

#endif /**< ENABLE_PROV_DATABASE */

#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISIONING_DATA_CONFIG_H */


/*** END OF FILE ***/
