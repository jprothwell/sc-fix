/***************************************************************************
 * $Id: jdi_charset.h,v 1.12 2008/12/31 13:53:09 browserdevjtbase Exp $
 * $Revision: 1.12 $
 * $Date: 2008/12/31 13:53:09 $
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
 * $Log: jdi_charset.h,v $
 * Revision 1.12  2008/12/31 13:53:09  browserdevjtbase
 * Core Updates
 *
 * Revision 1.26  2008/12/22 04:40:34  kavithadevhtmlbrow
 * Updated iso-10646-ucs-2
 *
 * Revision 1.25  2008/02/26 10:55:19  kavithadevhtmlbrow
 * UCS2 & UTF16 - LE/BE handling
 *
 * Revision 1.24  2008/01/31 05:25:45  kumardevhtmlbrow
 * Updated the doxygen comments for charset module.
 *
 * Revision 1.23  2008/01/21 14:53:19  kumardevhtmlbrow
 * GB2312 encoder support added.
 *
 * Revision 1.22  2007/10/26 16:05:44  kumardevhtmlbrow
 * euc-kr support added.
 *
 * Revision 1.21  2007/10/15 15:07:52  kumardevhtmlbrow
 * charset module updated for koi8-r russian charset.
 *
 * Revision 1.20  2007/05/03 06:15:30  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.19  2006/11/29 13:08:47  kumardevhtmlbrow
 * charset error handling for the size attribute.
 *
 * Revision 1.18  2006/09/18 06:45:09  kumardevhtmlbrow
 * Updated different charset.
 *
 * Revision 1.17  2006/09/15 12:12:11  kumardevhtmlbrow
 * Updated the charset module with tis-620 charset support.
 *
 * Revision 1.16  2006/05/24 09:34:13  kumardevhtmlbrow
 * Updated the additional charset for rtl
 *  
 ***************************************************************************/

/**
 * @file jdi_charset.h
 * @ingroup Charset
 * @brief Defines the exposed funciton prototypes for Charset Module.
 */


#ifndef JDI_CHARSET_H
#define JDI_CHARSET_H

#include <jdi_commonmodule.h>

/**
 * @brief Specifies the enumeration for the unicode format 
 */
typedef enum unicodeFormat
{
	E_BIG_ENDIAN, /**< Specifies the big endian unicode format. */
	E_LITTLE_ENDIAN /**< Specifies teh little endian unicode format. */
} E_UNICODE_FORMAT ;

/**
 * @brief Defines the US ASCII charset type.
 */
#define CHARSET_US_ASCII				"us-ascii"
/**
 * @brief Defines the UTF-8 charset type.
 */
#define CHARSET_UTF_8					"utf-8"
/**
 * @brief Defines the UCS-2BE charset type.
 */
#define CHARSET_UCS_2BE					"ucs-2BE"
/**
 * @brief Defines the UCS-2LE charset type.
 */
#define CHARSET_UCS_2LE					"ucs-2LE"
/**
 * @brief Defines the UCS-2 charset type.
 */
#define CHARSET_UCS_2					"ucs-2"
/**
 * @brief Defines the ISO-10646-UCS-2 charset type
 */
#define CHARSET_ISO_10646_UCS_2			"iso-10646-ucs-2"
/**
 * @brief Defines the UTF-16BE charset type.
 */
#define CHARSET_UTF_16BE				"utf-16BE"
/**
 * @brief Defines the UTF-16LE charset type.
 */
#define CHARSET_UTF_16LE				"utf-16LE"
/**
 * @brief Defines the UTF-16 charset type.
 */
#define CHARSET_UTF_16					"utf-16"
/**
 * @brief Defines the ISO-8859-1 charset type.
 */
#define CHARSET_ISO_8859_1				"iso-8859-1"
/**
 * @brief Defines the ISO-8859-2 charset type.
 */
#define CHARSET_ISO_8859_2				"iso-8859-2"
/**
 * @brief Defines the ISO-8859-3 charset type.
 */
#define CHARSET_ISO_8859_3				"iso-8859-3"
/**
 * @brief Defines the ISO-8859-4 charset type.
 */
#define CHARSET_ISO_8859_4				"iso-8859-4"
/**
 * @brief Defines the ISO-8859-5 charset type.
 */
#define CHARSET_ISO_8859_5				"iso-8859-5"
/**
 * @brief Defines the ISO-8859-6 charset type.
 */
#define CHARSET_ISO_8859_6				"iso-8859-6"
/**
 * @brief Defines the ISO-8859-7 charset type.
 */
#define CHARSET_ISO_8859_7				"iso-8859-7"
/**
 * @brief Defines the ISO-8859-8 charset type.
 */
#define CHARSET_ISO_8859_8				"iso-8859-8"
/**
 * @brief Defines the ISO-8859-9 charset type.
 */
#define CHARSET_ISO_8859_9				"iso-8859-9"
/**
 * @brief Defines the ISO-8859-10 charset type.
 */
#define CHARSET_ISO_8859_10				"iso-8859-10"
/**
 * @brief Defines the ISO-8859-11 charset type.
 */
#define CHARSET_ISO_8859_11				"iso-8859-11"
/**
 * @brief Defines the ISO-8859-13 charset type.
 */
#define CHARSET_ISO_8859_13				"iso-8859-13"
/**
 * @brief Defines the ISO-8859-14 charset type.
 */
#define CHARSET_ISO_8859_14				"iso-8859-14"
/**
 * @brief Defines the ISO-8859-15 charset type.
 */
#define CHARSET_ISO_8859_15				"iso-8859-15"
/**
 * @brief Defines the ISO-8859-16 charset type.
 */
#define CHARSET_ISO_8859_16				"iso-8859-16"
/**
 * @brief Defines the GB2312 charset type.
 */
#define CHARSET_GB2312					"gb2312"
/**
 * @brief Defines the GB2312 charset type in GB-2312 format.
 */
#define CHARSET_GB_2312					"gb-2312"
/**
 * @brief Defines the GB2312 charset type in GBK format.
 */
#define CHARSET_GBK						"gbk"
/**
 * @brief Defines the BIG5 charset type.
 */
#define CHARSET_BIG5					"big5"
/**
 * @brief Defines the SHIFT_JIS charset type.
 */
#define CHARSET_SHIFT_JIS				"shift_jis"
/**
 * @brief Defines the EUC-JP charset type.
 */
#define CHARSET_EUC_JP					"euc-jp"

#ifdef CHARSET_TIS_620_ENABLED
/**
 * @brief Defines the TIS-620 charset type.
 */
#define CHARSET_TIS_620					"tis-620"
/**
 * @brief Defines the WINDOWS-874 charset type.
 */
#define CHARSET_WINDOWS_874				"windows-874"
#endif

/**
 * @brief Defines the WINDOWS-1250 charset type.
 */
#define CHARSET_WINDOWS_1250			"windows-1250"
/**
 * @brief Defines the CP1250 charset type.
 */
#define CHARSET_CP_1250					"cp1250"
/**
 * @brief Defines the WINDOWS-1251 charset type.
 */
#define CHARSET_WINDOWS_1251			"windows-1251"
/**
 * @brief Defines the CP1251 charset type.
 */
#define CHARSET_CP_1251					"cp1251"

/**
 * @brief Defines the WINDOWS-1252 charset type.
 */
#define CHARSET_WINDOWS_1252			"windows-1252"
/**
 * @brief Defines the CP1252 charset type.
 */
#define CHARSET_CP_1252					"cp1252"

#ifdef BIDI_ENABLED
/**
 * @brief Defines the WINDOWS-1255 charset type.
 */
#define CHARSET_WINDOWS_1255			"windows-1255"
/**
 * @brief Defines the CP1255 charset type.
 */
#define CHARSET_CP_1255					"cp1255"

/**
 * @brief Defines the WINDOWS-1256 charset type.
 */
#define CHARSET_WINDOWS_1256			"windows-1256"
/**
 * @brief Defines the CP1256 charset type.
 */
#define CHARSET_CP_1256					"cp1256"
#endif

/**
 * @brief Defines the WINDOWS-1258 charset type.
 */
#define CHARSET_WINDOWS_1258			"windows-1258"
/**
 * @brief Defines the CP1258 charset type.
 */
#define CHARSET_CP_1258					"cp1258"

#ifdef CHARSET_KOI8_R_ENABLED
/**
 * @brief Defines the KOI8-R charset type.
 */
#define CHARSET_KOI8_R					"koi8-r"
#endif

#ifdef CHARSET_EUC_KR_ENABLE
/**
 * @brief Defines the EUC-KR charset type.
 */
#define CHARSET_EUC_KR					"euc-kr"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines the callback function that shall be invoked by the charset
 * module when the entity is found during the encoding. 
 * @param[in] pucData Specifies the entity data, which needs to be converted to unicode value.
 * @param[out] pmEntityValue Returns the unicode value for the entity data.
 * @param[out] piEntityCount Returns the number of bytes that are used by the entity data.
 * 
 * Example: &#32; should return the unicode value as 32.
 * &\#euro; should return the unicode value as 128.
 */
typedef JC_RETCODE (*CB_CHARSET_ENTITY_HANDLER) (
											const JC_UINT8	*pucData, 
											JC_CHAR			*pmEntityValue,
											JC_INT32		*piEntityCount) ;

/**
 * @brief Defines the function pointer that shall be used by the application for 
 * decoding the content to unicode format. 
 * @param[in] pucData Specifies the string that needs to be converted to unicode format.
 * @param[in] cbEntityHandler Specifies the function pointer that shall be used by 
 * charset module for evaluating the entity data.
 * @param[in, out] puiSize Specifies the size of the pmConvertedData and the charset
 * module shall return the number of bytes used for the conversion. 
 * @param[out] Returns the converted unicode string. 
 *
 * @retval JC_ERR_CHARSET_INSUFFICIENT_MEMORY When the size of the pmConvertedData is less 
 * then this error code is returned with the required size for the conversion.
 * @retval JC_ERR_CHARSET_BIDI_OK		When the conversion is success and the converted 
 * unicode string contains any bi-directional data. This is 
 */
typedef JC_RETCODE (*CB_CHARSET_UNICODE_CONVERTER) (
							JC_UINT8 					*pucData, 
							CB_CHARSET_ENTITY_HANDLER	cbEntityHandler,
							JC_UINT32 					*puiSize,
							JC_CHAR 					*pmConvertedData) ;

/**
 * @brief Defines the function pointer that shall be used by the application for 
 * encoding the unicode data to the specific format. 
 * @param[in] pmData Specifies the string, which needs to be converted to the given format.
 * @param[in] cbEntityHandler Specifies the function pointer that shall be used by the
 * charset module for evaluating the entity data. This is not required in this case, and
 * available as the placeholder for having the same prototype like decoder (converter).
 * @param[in, out] uiSize Specifies the size of the pucConvertedData and the charset
 * module shall return the number of bytes used for the conversion. 
 * @param[out] Returns the converted string in the given format. 
 *
 * @retval JC_ERR_CHARSET_INSUFFICIENT_MEMORY When the size of the pucConvertedData is less 
 * then this error code is returned with the required size for the conversion.
 */
typedef JC_RETCODE (*CB_CHARSET_UNICODE_ENCODER) (
							JC_CHAR 					*pmData, 
							CB_CHARSET_ENTITY_HANDLER	cbEntityHandler,
							JC_UINT32 					*puiSize,
							JC_UINT8 					*pucConvertedData) ;

/**
 * @brief Provides the function pointer for converting given encoding scheme to UNICODE
 * @param[in] pcSrcEncoding Specifies the encoding scheme
 * @param[out] pcbConverterFnc Provides the function pointer which needs to be used for 
 *								converting the data.
 * @retval JC_ERR_CHARSET_NOT_AVAILABLE	Specifies no conversion routine available for the
 *										encoding scheme specified
 * @retval JC_OK						Specifies the encoding scheme is available.
 *
 */
JC_RETCODE jdi_CharsetGetUnicodeConverter (
						const JC_INT8 					*pcSrcEncoding,
						CB_CHARSET_UNICODE_CONVERTER 	*pcbConverterFnc) ;

/**
 * @brief Provides the function pointer for converting given UNICODE to the given encoding scheme.
 * @param[in] pcSrcEncoding Specifies the encoding scheme to which it needs to be converted.
 * @param[out] pcbConverterFnc Provides the function pointer which needs to be used for 
 *								converting the data.
 * @retval JC_ERR_CHARSET_NOT_AVAILABLE	Specifies no conversion routine available for the
 *										encoding scheme specified
 * @retval JC_OK						Specifies the encoding scheme is available.
 *
 */
JC_RETCODE jdi_CharsetGetUnicodeEncoder (
						const JC_INT8 				*pcSrcEncoding,
						CB_CHARSET_UNICODE_ENCODER 	*pcbConverterFnc) ;

/**
 * @brief Converts the content from UCS2 to UTF-8 format.
 * @param[in] pucSrcData Specifies the data in UCS2 format 
 * @param[in] uiSrcLen Specifies the size of the UCS2 data.
 * @param[in] eUnicodeFormat Specifies whether the data is in Big Indian or Little Indian format.
 * @param[in,out] puiDestSize Specifies the size of the pucDestData and returns the actual
 *							  size copied to the data/actual size required to copy the data
 *							  if the pucDestData is NULL.
 * @param[out] pucDestData Provides the converted data in UTF-8 format
 * @retval JC_OK						Specifies the conversion is successful.
 * @retval JC_ERR_CHARSET_INSUFFICIENT_MEMORY	Specifies the memory is not sufficient for 
 *							the charset operation.
 *
 */
JC_RETCODE jdi_CharsetUCS2ToUTF8 (JC_UINT8			*pucSrcData,
								  JC_UINT32			uiSrcLen,
								  E_UNICODE_FORMAT	eUnicodeFormat,
								  JC_UINT32			*puiDestSize,
								  JC_UINT8			*pucDestData) ;

/** 
 * @brief Defines the insufficient memory for the conversion.
 */
#define JC_ERR_CHARSET_INSUFFICIENT_MEMORY  TERMINAL_ERR_CODE(CHARSET_MODULE, 0x01)

/**
 * @brief Defines the unavailablity of the charset conversion. 
 */
#define JC_ERR_CHARSET_NOT_AVAILABLE		TERMINAL_ERR_CODE(CHARSET_MODULE, 0x02)

#ifdef BIDI_ENABLED
/**
 * @brief Defines the successful conversion of the charset conversion, but specifies 
 * that the converted data contains the bi-directional content as well.
 */
#define JC_ERR_CHARSET_BIDI_OK				TERMINAL_ERR_CODE(CHARSET_MODULE, 0x03)
#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

