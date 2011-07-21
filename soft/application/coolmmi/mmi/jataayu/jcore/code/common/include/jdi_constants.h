/***************************************************************************
 * $Id: jdi_constants.h,v 1.5 2008/06/16 05:52:17 browserdevjtbase Exp $
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
 * $Log: jdi_constants.h,v $
 * Revision 1.5  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.21  2007/01/02 13:08:26  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.20  2006/12/01 13:45:02  kumardevhtmlbrow
 * Update
 *
 * Revision 1.19  2006/01/17 15:33:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.18  2006/01/16 06:29:28  kavithadevhtmlbrow
 * Added Plus Token
 *
 * Revision 1.17  2005/12/29 07:41:15  kavithadevhtmlbrow
 * UTF8 Characters added
 *
 * Revision 1.16  2005/12/09 15:15:16  kumardevhtmlbrow
 * Added the suspend and resume request. Providing additional interface for starting confirmed & http push. Changes as per new JDD LOG abstraction
 *
 * Revision 1.15  2005/10/18 09:40:25  kavithadevhtmlbrow
 * Backslash added
 *
 * Revision 1.14  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.13  2005/09/12 06:17:23  kavithadevhtmlbrow
 * Added asterisk constant
 *
 * Revision 1.12  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.11  2005/07/18 12:50:49  kavithadevhtmlbrow
 * Added Apos and Quote token
 *
 * Revision 1.10  2005/06/24 11:34:16  kavithadevhtmlbrow
 * Added Hyphen
 *
 * Revision 1.9  2005/06/20 15:25:55  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/06/20 10:44:26  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/06/16 10:58:46  kumardevhtmlbrow
 * added additional constants for $ and pict scheme.
 *
 * Revision 1.6  2005/05/18 10:26:50  kumardevhtmlbrow
 * Updated the additional constants
 *
 * Revision 1.5  2005/05/16 08:19:09  kumardevhtmlbrow
 * Added URI Parse function.
 *
 * Revision 1.4  2005/05/13 14:05:48  kumardevhtmlbrow
 * Added new tokens.
 *
 * Revision 1.3  2005/05/13 12:23:26  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/05/13 06:11:51  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/05/13 06:08:29  kumardevhtmlbrow
 * Added the regularly used constants and common module header file
 *
 *  
 ***************************************************************************/

 /**
 * @file jdi_constants.h
 * @ingroup Common
 * @brief It contains macro definitions for different constants.
 *
 * This file provides macro definitions for different constants that are 
 * used commomly.
 */

#ifndef _JDI_CONSTANTS_H
#define _JDI_CONSTANTS_H

#define EOS					'\0'
#define EOS_TOKEN			'\0'


#define TAB_TOKEN			'\t'
#define CARRIAGE_TOKEN		'\r'
#define NEW_LINE_TOKEN		'\n'
#define SPACE_TOKEN			' '
#define SEMICOLON_TOKEN		';'
#define COLON_TOKEN			':'
#define EQUAL_TOKEN			'='
#define COMMA_TOKEN			','
#define AMPERSAND_TOKEN		'&'
#define SLASH_TOKEN			'/'
#define BACKSLASH_TOKEN		'\\'
#define AT_RATE_TOKEN		'@'
#define QUESTION_MARK_TOKEN	'?'
#define DOLLAR_TOKEN		'$'
#define HASH_TOKEN			'#'
#define ASTERISK_TOKEN		'*'
#define HYPHEN_TOKEN		'-'
#define APOS_TOKEN			'\''
#define QUOTE_TOKEN			'"'
#define DOT_TOKEN			'.'
#define PERCENT_TOKEN		'%'
#define UNDERSCORE_TOKEN	'_'
#define PLUS_TOKEN			'+' 

#define SMALL_BUF_VALUE		127
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

#define HTTP_SCHEME				"http" 
#define HTTPS_SCHEME			"https" 
#define PICT_SCHEME				"pict"
#define FILE_SCHEME				"file"
#define MAIL_TO_SCHEME			"mailto:"
#define TEL_SCHEME				"tel:"

#define HTTP_PORT_NUMBER		80
#define HTTPS_PORT_NUMBER		443

#define	UTF8_SIGN_FIRST_BYTE		0xef
#define	UTF8_SIGN_SECOND_BYTE		0xbb
#define	UTF8_SIGN_THIRD_BYTE		0xbf

#endif

/* End of File */

