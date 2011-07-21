/***************************************************************************
 * $Id: jdi_transportmodule.h,v 1.5 2006/11/16 11:32:26 browserdevjtbase Exp $
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
 * $Log: jdi_transportmodule.h,v $
 * Revision 1.5  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.4  2006/06/16 07:10:35  browserdevjtbase
 * Updated with e-mail protocol
 *
 * Revision 1.3  2006/06/16 06:58:58  browserdevjtbase
 * Updated with email protocol
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.8  2006/01/09 14:45:52  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2005/12/19 06:05:32  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.5  2005/09/17 11:58:39  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.3  2005/08/30 05:59:54  kumardevhtmlbrow
 * Added the SIA Module
 *
 * Revision 1.2  2005/05/18 15:33:35  kumardevhtmlbrow
 * Update to transport task by adding task information and session id
 *
 * Revision 1.1  2005/05/10 07:56:42  kumardevhtmlbrow
 * Added HTTP Utils function
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_transportmodule.h
 * @ingroup Transport Task.
 * @brief Defines the modules in the transport task.
 *
 */

#ifndef _JDI_TRANSPORT_MODULE_H
#define _JDI_TRANSPORT_MODULE_H

/*** Macros				***/

#define TRANSPORT_MODULE			TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0100)
#define WSP_MODULE					TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0200)
#define HTTP_MODULE					TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0300)
#define HTTP_UTILS_MODULE			TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0400)
#define SIA_MODULE					TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0500)
#define HTTPPUSH_MODULE				TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0600)
#define WSP_UTILS_MODULE			TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0700)
#define SMTP_MODULE					TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0800)
#define POP3_MODULE					TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0900)
#define IMAP4_MODULE				TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0A00)
#define SSL_MODULE					TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0B00)
#define WTLS_MODULE					TERMINAL_MODULE_CODE (TRANSPORT_APPLICATION, 0x0C00)

#endif

