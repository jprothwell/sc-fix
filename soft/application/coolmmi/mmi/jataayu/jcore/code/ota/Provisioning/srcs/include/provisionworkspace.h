/***************************************************************************
 * $Id: ProvisionWorkspace.h,v 1.3 2007/01/31 05:39:24 lumitdevpclient Exp $
 * $Revision: 1.3 $
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



#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_


typedef struct st__provision_workspace
{
	E_MESSAGE_TYPE	eMessageType ;	/**< encoding type whether wbxml or xml */
	JC_UINT32		uiMaxMsgSize ;	/**< Maximum Message Size, pBuffer is allocated that much */
	JC_UINT8		*pucBuffer ;	/**< The buffer to be generated */
	JC_UINT8		*pucCurrPos ;	/**< Current position where we are writing to the buffer */
	JC_UINT32		uiBytesWritten ;/**< Number of bytes written */
}ST_PROVISION_WORKSPACE ;

#define PROVISION_GENERATOR "SyncML_Generator" 

JC_RETCODE InitWorkSpace( E_MESSAGE_TYPE eMessageType, 
						 JC_UINT32 uiMaxMsgSize,
						 ST_PROVISION_WORKSPACE *pSyncMLWorkSpace ) ;

void DeInitWorkSpace( ST_PROVISION_WORKSPACE *pstSyncMLWorkSpace ) ;

JC_UINT32 GetBytesWrittenInWorkSpace( ST_PROVISION_WORKSPACE *pstSyncMLWorkSpace ) ;

void GetBufferFromWorkSpace( ST_PROVISION_WORKSPACE *pstSyncMLWorkSpace , 
							JC_UINT8 **ppuiBuf,
							JC_UINT32 *puiBufLen ) ;

void AppendBufferInWorkSpace( ST_PROVISION_WORKSPACE *pstSyncMLWorkSpace,
								   JC_UINT8	*pucBuffer,
								   JC_UINT32 uiBufLen,
								   JC_BOOLEAN *pbWorkSpaceOverFlow ) ;

void AppendByteInWorkSpace( ST_PROVISION_WORKSPACE *pstSyncMLWorkSpace,
								   JC_UINT8	ucSingleByte,
								   JC_BOOLEAN *pbWorkSpaceOverFlow ) ;




#endif /**< _WORKSPACE_H_ */



/*** END OF FILE ***/
