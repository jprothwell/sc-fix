/***************************************************************************
 * $Id: jdi_trprogress.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
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
 * $Log: jdi_trprogress.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.7  2006/01/09 14:45:52  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.5  2005/06/17 08:17:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2005/05/27 07:24:05  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/05/21 11:37:29  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/05/18 15:33:35  kumardevhtmlbrow
 * Update to transport task by adding task information and session id
 *
 * Revision 1.1  2005/05/17 06:25:14  kumardevhtmlbrow
 * Added the transport interface files
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_trprogress.h
 * @ingroup Transport Task.
 * @brief Defines enum and structure used for progress indication.
 *
 * This file defines the enum and the structure that is used to send the progress indication
 * information to the application.
 */

#ifndef _JDI_TRPROGRESS_H
#define _JDI_TRPROGRESS_H


/**
 * @brief The transport used this enum to specify the transport stack status to the application
 */
typedef enum TR_stack_status
{
	E_TR_STACK_NONE, /**< Specifies that the transport stack is in NONE state */
	E_TR_STACK_CONNECTING, /**< Specifies that the transport stack is in connecting state */
	E_TR_STACK_CONNECTED, /**< Specifies that the transport stack is in connected state */
	E_TR_STACK_SENDING, /**< Specifies that the transport stack is in sending state */
	E_TR_STACK_RECEIVING, /**< Specifies that the transport stack is in receiving state */
	E_TR_STACK_RETRYING, /**< Specifies that the transport stack is in retrying state */
	E_TR_STACK_DISCONNECTING, /**< Specifies that the transport stack is in disconnecting state */
	E_TR_STACK_DISCONNECTED /**< Specifies that the transport stack is in disconnected state */
} E_TR_STACK_STATUS ;

/**
 * @brief Once the transport does any lower layer operation (send or receive) it uses this structure
 *		  to send the progress indication event E_TR_PROGRESS_IND_EVENT to the application
 */
typedef struct tr_prog_ind
{
	JC_TASK_INFO				stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32					uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32					uiRequestID ;  /**< Specifies the ID of the request for which the progress indication is sent */
	JC_UINT32					uiBytesRecvd ; /**< Specifies the bytes received in the last receive done at lower layers */
	JC_UINT32					uiBytesSent ; /**< Specifies the bytes received in the last send done at lower layers */
	JC_UINT32					uiTotalBytesToSend ; /**< Specifies the total number of bytes that the transport needs to send to lower layers */
	JC_UINT32					uiTotalBytestoRecv ; /**< Specifies the total number of bytes that the transport needs to receive from lower layers */
	E_DATA_CONN_STATUS			eDataConnectionStatus ; /**< Specifies the data connection status of the transport task session in use */
	E_TR_STACK_STATUS			eTrStackStatus ; /**< Specifies the transport stack status */
} ST_TR_PROGRESS_IND ;

#endif

/* END OF FILE */

