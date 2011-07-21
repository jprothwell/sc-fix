/***************************************************************************
 * $Id: jdi_wspstatus.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
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
 * $Log: jdi_wspstatus.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.1  2005/07/20 14:19:12  kumardevhtmlbrow
 * no message
 *
 *  
 ***************************************************************************/

#ifndef _JDI_WSP_STATUS_H
#define _JDI_WSP_STATUS_H

/*** Macros						***/

#define WSP_OK								0x20
#define WSP_CREATED							0x21
#define WSP_ACCEPTED						0x22
#define WSP_NON_AUTHORITATIVE_INFORMATION	0x23
#define WSP_NO_CONTENT						0x24
#define WSP_RESET_CONTENT					0x25
#define WSP_PARTIAL_CONTENT					0x26

#define WSP_MULTIPLE_CHOICES				0x30
#define WSP_MOVED_PERMANENTLY				0x31
#define WSP_MOVED_TEMPORARILY				0x32 
#define WSP_SEE_OTHER						0x33
#define WSP_NOT_MODIFIED					0x34
#define WSP_USE_PROXY						0x35
#define WSP_TEMPORARY_REDIRECT				0x37


#define WSP_BAD_REQUEST						0x40
#define WSP_UNAUTHORIZED					0x41
#define WSP_PAYMENT							0x42
#define WSP_FORBIDDEN					    0x43
#define WSP_NOT_FOUND						0x44
#define WSP_METHOD_NOT_ALLOWED				0x45
#define WSP_NOT_ACCEPTABLE					0x46
#define WSP_PROXY_AUTHENTICATION_REQD		0x47
#define WSP_REQUEST_TIMEOUT					0x48
#define WSP_CONFLICT						0x49
#define WSP_GONE							0x4A
#define WSP_LENGTH_REQUIRED					0x4B
#define WSP_PRECONDITION_FAILED				0x4C
#define WSP_REQUEST_ENTITY					0x4D
#define WSP_REQUEST_URI						0x4E
#define WSP_UNSUPPORTED_MEDIA_TYPE			0x4F
#define WSP_REQUESTED_RANGE					0x50
#define WSP_EXPECTATION_FAILED				0x51

#define WSP_INTERNAL_SERVER_ERROR			0x60
#define WSP_NOT_IMPLEMENTED					0x61
#define WSP_BAD_GATEWAY						0x62
#define WSP_SERVICE_UNAVAILABLE				0x63
#define WSP_GATEWAY_TIMEOUT					0x64
#define WSP_BAD_VERSION						0x65

#endif
/* End of File */


