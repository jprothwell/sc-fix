/***************************************************************************
 * $Id: ddlutils.h,v 1.8 2007/09/18 06:04:38 kavithadevhtmlbrow Exp $
 * $Revision: 1.8 $
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
 * $Log: ddlutils.h,v $
 * Revision 1.8  2007/09/18 06:04:38  kavithadevhtmlbrow
 * Updated CRT Range fro 'is' functions as per MSDN
 *
 * Revision 1.7  2007/09/11 14:38:35  kumardevhtmlbrow
 * Visual Studio 2005 change update
 *
 * Revision 1.6  2007/01/10 12:00:48  kumardevhtmlbrow
 * Script Integration
 *
 * Revision 1.5  2006/01/30 12:12:44  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2005/09/12 12:17:43  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.3  2005/06/04 07:59:57  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/03/22 10:59:41  kumardevhtmlbrow
 * Update the license agreement
 *
 *  
 ***************************************************************************/

#ifndef _DDLUTILS_H
#define _DDLUTILS_H

#ifdef __cplusplus 
extern "C" {
#endif

/** Specifies the valid range for 'is' functions as per MSDN for CRT Debug **/
#define CRT_VALID_RANGE(c)			((c >= 0x00 && c <= 0xFF) || ('\0' == c))

#define jc_strlen					SUL_Strlen
#define jc_strcpy					SUL_StrCopy
#define jc_strstr					strstr
#define jc_strchr					SUL_StrChr
#define jc_strncpy					SUL_StrNCopy
#define jc_strncmp					SUL_StrNCompare
#define jc_strcasecmp				SUL_StrCaselessCompare // Defined by jataayu
#define jc_strncasecmp				SUL_StrNCaselessCompare
#define jc_strcmp					SUL_StrCompare
#define jc_strcat					SUL_StrCat
#define jc_strrchr					SUL_Strrchr
#define jc_strncat					jdd_strncat
//SUL_StrNCat((x),(y),(n+1))


#define jc_wstrcpy(str1,str2)		SUL_Wcscpy((UINT16*) (str1), (const UINT16*) (str2))
#define jc_wstrlen(x)				pfnUnicodeStrlen((S8*)(x)) //SUL_Wcsnlen??
#define jc_wstrcmp				    SUL_Wcscmp
#define jc_wstrcasecmp				SUL_Wcsicmp
#define jc_wstrncasecmp				SUL_Wcsnicmp
#define jc_wstrncmp					SUL_Wcsncmp 
#define jc_wstrcat					SUL_Wcscat
#define jc_wstrncpy(x,y,z)			pfnUnicodeStrncpy((S8*)(x),(S8*)(y),(z))
#define jc_wstrstr					SUL_Wcsstr
#define jc_wstrncat					SUL_Wcsncat
#define jc_wstrchr					SUL_Wcschr
#define jc_wstrrchr					wcsrchr

#define jdd_isalnum(c)				((c>='A' && c<='Z')||(c>='a' && c<='z')||(c>='0' && c<='9'))

#define jc_isxdigit(c)				(CRT_VALID_RANGE(c) ? isxdigit(c) : 0)
#define jc_isalpha(c)				(CRT_VALID_RANGE(c) ? isalpha(c) : 0)
#define jc_isspace(c)				(CRT_VALID_RANGE(c) ? isspace(c) : 0)
#define jc_isdigit(c)				(CRT_VALID_RANGE(c) ? isdigit(c) : 0)
#define jc_isalnum(c)				(CRT_VALID_RANGE(c) ? (jdd_isalnum(c)) : 0)
#define jc_islower(c)				(CRT_VALID_RANGE(c) ? islower(c) : 0)
#define jc_isupper(c)				(CRT_VALID_RANGE(c) ? isupper(c) : 0)
#define jc_isascii					isascii
#define jc_toupper					__toupper
#define jc_tolower					__tolower

#define jc_iswspace                 SUL_Iswspace  //Dummy function defined by Jataayu

#define jc_itoa						SUL_Itoa
#define jc_ltoa						SUL_Ltoa

#define jc_atoi						SUL_StrAToI
#define jc_atof						atof  //Dummy function defined by Jataayu
#define jc_atol						SUL_StrAToI

#define jc_itow						_itow 
#define jc_strtol					strtol
//#define jc_wsprintf					swprintf 
//#define jc_wsnprintf				_snwprintf
#define jc_sprintf					SUL_StrPrint

#define jc_snprintf					snprintf
//#define jc_wstrtod					wcstod
//#define jc_strtod					strtol

#define jc_memcmp					SUL_MemCompare
#define jc_memcpy					SUL_MemCopy8
#define jc_memset					SUL_MemSet8
#define jc_memmove					SUL_MemMove

#define jc_srand					srand
#define jc_rand						rand
//#define jc_pow						pow
#define jc_ceil						SUL_CeilEX
#define jc_abs						abs


#ifdef __cplusplus
}
#endif

#endif

