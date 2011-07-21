
/***************************************************************************
 * $Id: ProvisionUpdateContext.h,v 1.2 2006/01/04 06:50:14 lumitdevpclient Exp $
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
 *
 *  
 ***************************************************************************/


#ifndef _PROVISION_UPDATE_CONTEXT_H
#define _PROVISION_UPDATE_CONTEXT_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

/**< Functions related to Continuous Provisioning */

JC_RETCODE prUpdateConfigurationContext (ST_PROFILE_DETAIL_INFO *pstToUpdateDetailInfo, 
										 ST_PROFILE_DETAIL_INFO *pstDetailInfo) ;

JC_RETCODE prCheckAndUpdateBootstrapBlock (ST_PROFILE_DETAIL_INFO *pstProvDetailInfo, 
										   ST_PROV_BOOTSTRAP *pstBootstrap) ;

JC_RETCODE prCheckAndUpdateApplicationBlock (ST_PROFILE_DETAIL_INFO *pstProvDetailInfo, 
											 ST_PROV_APPLICATION *pstApplication) ;

JC_RETCODE prCheckAndUpdateNapdefBlock (ST_PROFILE_DETAIL_INFO *pstProvDetailInfo, 
										ST_PROV_NAPDEF *pstNapdef) ;

JC_RETCODE prCheckAndUpdatePxlogicalBlock (ST_PROFILE_DETAIL_INFO *pstProvDetailInfo, 
										   ST_PROV_PXLOGICAL *pstPxlogical) ;

JC_RETCODE prCheckAndUpdateAccessBlock (ST_PROFILE_DETAIL_INFO *pstProvDetailInfo, 
										ST_PROV_ACCESS *pstAccess) ;

JC_RETCODE prCheckAndUpdateClientIdentityBlock (ST_PROFILE_DETAIL_INFO *pstProvDetailInfo, 
												ST_PROV_CLIENTIDENTITY *pstClientIdentity) ;

JC_RETCODE prCheckAndUpdateVendorConfigBlock (ST_PROFILE_DETAIL_INFO *pstProvDetailInfo, 
											  ST_PROV_VENDORCONFIG *pstVendorConfig) ;

#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISION_UPDATE_CONTEXT_H */


/*** END OF FILE ***/
