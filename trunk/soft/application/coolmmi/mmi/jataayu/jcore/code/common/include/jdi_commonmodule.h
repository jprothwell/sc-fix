/***************************************************************************
 * $Id: jdi_commonmodule.h,v 1.10 2009/04/13 06:38:21 browserdevjtbase Exp $
 * $Revision: 1.10 $
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
 * $Log: jdi_commonmodule.h,v $
 * Revision 1.10  2009/04/13 06:38:21  browserdevjtbase
 * Core Update
 *
 * Revision 1.15  2009/03/18 12:47:15  shajudevhtmlbrow
 * New error code jdi_ERR_DOM_CONTINUE for resuming eventflow and
 * new ESMP event object.
 *
 * Revision 1.14  2007/09/03 11:44:10  kumardevhtmlbrow
 * Extended profile updated.
 *
 * Revision 1.13  2007/06/06 07:26:14  shreyasdevhtmlbrow
 * Changes based on the code review comments
 *
 * Revision 1.12  2007/01/02 13:08:26  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.11  2006/11/24 07:29:20  kumardevhtmlbrow
 * Update
 *
 * Revision 1.10  2006/11/16 12:03:26  kavithadevhtmlbrow
 * CSS Module ID
 *
 * Revision 1.9  2006/03/24 06:20:05  shajudevhtmlbrow
 * BIDI_MODULE macro added
 *
 * Revision 1.8  2006/01/24 16:13:34  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2006/01/17 15:33:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/12/23 11:42:11  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.4  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/08/23 16:34:37  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 *  
 ***************************************************************************/

/**
 * @mainpage
 *
 * @version 0.9(Draft) 
 * Interface Description
 *
 * @section Introduction
 * 
 * 
 * @defgroup charset Charset
 * @defgroup cmem Chunk Memory
 * @defgroup communicator Communicator
 * @defgroup css CSS Parser
 * @defgroup Utils Common Utils
 * @defgroup dmem Dynamic Memory 
 * @defgroup imem Internal Memory Management
 * @defgroup hash Hash Algorithm
 * @defgroup url URL Formation
 * @defgroup wapgsm WAP GSM 
 * @defgroup wbxml WBXML Decoder
 * @defgroup xml XML Parser 
 * @defgroup dxml XML Data Type
 */

#ifndef _JDI_COMMONMODULE_H
#define _JDI_COMMONMODULE_H

/** @cond */

#define COMMON_UTILS_MODULE			TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0100)
#define XML_MODULE					TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0200)
#define CHARSET_MODULE				TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0300)
#define WBXML_MODULE				TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0400)
#define MEMUTILS_MODULE				TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0500)
#define URLFORMATION_MODULE			TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0600)
#define WAPGSM_MODULE				TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0700)
#define COMMUNICATOR_MODULE			TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0800)
#define BIDI_MODULE					TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0900)
#define CSS_MODULE					TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0A00)
#define PUNYCODE_MODULE				TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0B00)
#define DEFLATE_MODULE				TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0C00)
#define CHUNKSTORAGE_MODULE			TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0D00)
#define DOM_MODULE					TERMINAL_MODULE_CODE(COMMON_APPLICATION, 0x0E00)
/** @endcond */

#endif

/* End of File */


