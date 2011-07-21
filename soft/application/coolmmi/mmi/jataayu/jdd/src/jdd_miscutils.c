#define MODULE_NAME "JDD_MISCUTILS"
#define FILE_NAME "jdd_miscutils.c"

#ifdef JATAAYU_SUPPORT
/***************************************************************************
 *
 * File Name jdd_miscutils.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
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
 ***************************************************************************/
 
 /** 
 * @file	jdd_miscutils.c
 * @ingroup	jdd_miscutils.c
 * @brief	Defines common utility functions.
 **/
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
//#include "jddex_listbox.h"



/*
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "wgui_status_icons.h"
#include "mmi_trace.h"
#include "gui.h"
#include "unicodexdcl.h"
*/
#include <ddl.h>
#include <jcal.h>
#include <jdi_cutils.h>
#include <jwap_str_id.h>
#include <jwap_interface.h>
#include <jdi_browserengine.h>



#ifdef BROWSER_JUDGMENT_OF_CONTENT

#define MISC_IMAGE_JPEG				"image/jpeg"
#define MISC_IMAGE_PNG				"image/png"
static const ST_CONTENT_JUDGMENT stContentjudgment [] = 
{
	{"!",							"text/html"},
	{"h",							"text/html"},
	{"H",							"text/html"},
	{"b",							"text/html"},
	{"B",							"text/html"},
	{"?xml version=",				"application/xhtml+xml"},
	{"GIF87a",						"image/gif"},
	{"GIF89a",						"image/gif"},
	{"MThd",						"audio/midi"},
	{"melo",						JC_NULL},
	{"ftyp",						"video/3gpp"},
	{"FWS",							"application/x-shockwave-flash"},
	{"ID3",							"audio/aac"},
	{JC_NULL,						JC_NULL}
} ;

#endif




//void* jWap_Get_Brw_Handle();

#if 0


/***************************************************************************
 * Macros
 **************************************************************************/
 
#if defined(JDD_LOG_ENABLED)
#define __MODULE_ID__					OTHER_MODULE
#else
#define __MODULE_ID__					0
#endif

#define JAT_TOLOWER(x)				((x >= 'A' && x <= 'Z') ? x + 32 : x)
#define MAX_ITOA_LEN				(50)

/***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
 
 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
 
 
 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/

/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup		jdd_miscutils
 * @brief		Calculates the length of the string
 * @param[in]	pmInputString contains string whose length to be calculated
 * @retval		returns the length of string
 * @retval		if no input string is there returns 0
 **/
JC_INT32 jdd_Miscwstrlen (const JC_CHAR *pmInputString)
{
	JC_INT32 iValue ;
	register const JC_CHAR *pmGetString ;
	
	if (JC_NULL == pmInputString)
	{
		iValue = 0 ;
	}
	else
	{
		for (pmGetString = pmInputString ; *pmGetString ; pmGetString++) ;

		iValue = pmGetString - pmInputString ;
	}
	return (iValue) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function compares the two given string
 * @param[in]	pmFristString First input string
 * @param[in]	pmSecondString Second input string
 * @retval		Returns 0 on sucess
 * @retval		Returns -1 on if its small
 * @retval		Returns 1 on if its greater
 **/
JC_INT32 jdd_MiscWstrcmp (register const JC_CHAR *pmFirstString, \
					register const JC_CHAR *pmSecondString)
{

	JC_INT32 iRetval = -1 ;
	if (JC_NULL == pmFirstString || JC_NULL == pmSecondString)
	{
		iRetval = -1 ;
	}
	else
	{
		while (*((JC_CHAR *)pmFirstString) == *((JC_CHAR *)pmSecondString))
		{
			if (!(*pmFirstString++)) 
			{
				iRetval = 0 ;
			}
			++pmSecondString ;
		}
	}

	if(0 != iRetval)
	{
		iRetval = (*((JC_CHAR *)pmFirstString) < \
						*((JC_CHAR *)pmSecondString)) ? -1 : 1 ;
	}

	return (iRetval) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function compares the two given string irrespective
 *				of case of the two string.
 * @param[in]	pmFristString First input string
 * @param[in]	pmSecondString Second input string
 * @retval		Returns 0 on sucess
 * @retval		Returns -1 on if its small
 * @retval		Returns 1 on if its greater
 **/
JC_INT32 jdd_MiscWstrcasecmp (const JC_CHAR *pmFirstString, \
						const JC_CHAR *pmSecondString)
{
	JC_CHAR *pmTempStr1 ;
	JC_CHAR *pmTempStr2 ;
	JC_INT32 iVal1 ;
	JC_INT32 iVal2 ;
	JC_INT32 iRetVal ;

	if (JC_NULL != pmFirstString || JC_NULL != pmSecondString)
	{
		pmTempStr1 = (JC_CHAR *) pmFirstString ;
		pmTempStr2 = (JC_CHAR *) pmSecondString ;
		
		while ('\0' != *pmTempStr1 && '\0' != *pmTempStr2)
		{
			iVal1 = JAT_TOLOWER (*pmTempStr1) ;
			iVal2 = JAT_TOLOWER (*pmTempStr2) ;
			if (iVal1 > iVal2)
			{
				iRetVal =  1 ;
				break ;
			}
			else if (iVal1 < iVal2)
			{
				iRetVal = -1 ;
				break ;
			}
			else
			{
				pmTempStr1++ ;
				pmTempStr2++ ;
			}
		}
	}
	if ('\0' == *pmTempStr1 && '\0' == *pmTempStr2)
	{
		iRetVal = 0 ;
	}
	else if ('\0' == *pmTempStr1)
	{
		iRetVal = -1 ;
	}
	else 
	{
		iRetVal = 1 ;
	}
	return (iRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function compares the two given string irrespective
 *				of case of the two string upto mentiond no of characters.
 * @param[in]	pmFristString First input string
 * @param[in]	pmSecondString Second input string
 * @param[in]	iValue Specifies the length till the string is to be compared.
 * @retval		Returns 0 on sucess
 * @retval		Returns -1 on if its small
 * @retval		Returns 1 on if its greater
 **/
JC_INT32 jdd_MiscWstrncasecmp (const JC_CHAR *pmFirstString, \
						const JC_CHAR *pmSecondString, JC_INT32 iValue)
{
	JC_INT32 iRetVal ;
	
    while (JAT_TOLOWER(*pmFirstString) == JAT_TOLOWER(*pmSecondString))
    {
        if ('\0' == *pmFirstString)           
		{
			iRetVal = 0 ;
			break ;
		}
		
        if (0 == iValue)
		{
            iRetVal = 0 ;
			break ;
		}
		
        ++pmFirstString ;
        ++pmSecondString ;
        --iValue ;
    }
	
    if (0 != iValue)
	{
		iRetVal = (*pmFirstString > *pmSecondString ? 1 : -1);
	}
	
    return (iRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function returns the first Instance of cFind in Input String .
 * @param[in]	pmInputString Input string
 * @param[in]	cFind Input Char to be searched in string .
 * @retval		Returns pointer to first Instance of cFind on success
 * @retval		Returns NULL on failure
 **/

JC_CHAR * jdd_MiscWstrchr (register const JC_CHAR *pmInputString, JC_INT32 cFind)
{
	if (JC_NULL != pmInputString)	
	{
		do 
		{
			if (*pmInputString == ((JC_CHAR)cFind)) 
			{
				break ;
			}
		} while (*pmInputString++) ;
	}

	return ((JC_CHAR *) pmInputString) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function returns the last Instance of cFind in Input String .
 * @param[in]	pmInputString Input string
 * @param[in]	cFind Input Char to be searched in string .
 * @retval		Returns pointer to last Instance of cFind on success
 * @retval		Returns NULL on failure
 **/


JC_CHAR * jdd_MiscWstrrchr (register const JC_CHAR *pmInputString, JC_INT32 cFind)
{
	JC_CHAR *pmTempString = JC_NULL ;

	if (JC_NULL != pmInputString)
	{
		do 
		{
			if (*pmInputString == ((JC_CHAR)cFind)) 
			{
				pmTempString = (JC_CHAR *)pmInputString ;
			}
		} while (*pmInputString++) ;
	}

	return (pmTempString) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function returns the first Instance of sub string in Input String .
 * @param[in]	pmFirstString Input string
 * @param[in]	pmSecondString Input sub string .
 * @retval		Returns pointer to first Instance of sub string on success
 * @retval		Returns NULL on failure
 **/

JC_CHAR * jdd_MiscWstrstr (const JC_CHAR *pmFirstString, \
					const JC_CHAR *pmSecondString)
{
	register const JC_CHAR *pmTempString1 = pmFirstString ;
	register const JC_CHAR *pmTempString2 = pmSecondString ;
	
	if (JC_NULL != pmFirstString || JC_NULL !=  pmSecondString)
	{
		do 
		{
			if (!(pmTempString2))
			{
				pmTempString1 = pmFirstString ;
				break ;
			}

			if (*pmTempString2 == *pmTempString1) 
			{
				++pmTempString2 ;
				++pmTempString1 ;
			} 
			else 
			{
				pmTempString2 = pmSecondString ;
				if (!(*pmTempString1)) 
				{
					pmTempString1 = JC_NULL ; 
					break ;
				}
				pmTempString1 = ++pmFirstString ;
			}
		} while (1) ;
	}
	return ((JC_CHAR *) pmTempString1) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function converts integer to wide char String .
 * @param[in]	lNum Input Number .
 * @param[out]	pmItoa_arr resultant string .
 * @param[in]	iRadix Input Radix .
 * @retval		Returns pointer to first Instance of sub string on success
 * @retval		Returns NULL on failure
 **/

JC_CHAR * jdd_MiscItow (JC_INT64 lNum, JC_CHAR *pmItoa_arr, JC_INT32 iRadix)
{
	JC_INT64	lValue, lLength, lCount ;
	JC_CHAR cTemp_arr [255] ;
	JC_INT32 iIs_negative_val = 0 ;

    if (JC_NULL == pmItoa_arr)
	{
        pmItoa_arr = JC_NULL ;
	}
	else
	{
		lValue = 0 ;
		if (0 >lNum )
		{
			iIs_negative_val = 1 ;
			lNum *= -1 ;
		}
		do
		{
			cTemp_arr [lValue++] = (JC_CHAR) ((lNum % iRadix) + '0') ;
			if (255 == lValue)
			{
				break ;
			}
		} while (0 < (lNum /= iRadix)) ;
		cTemp_arr [lValue] = '\0' ;
		lLength = jc_tcslen ((JC_INT8*)cTemp_arr) ;
		
		lCount = 0 ;
		if (1 == iIs_negative_val)
		{
			pmItoa_arr [lCount++] = '-' ;
		}
		for (lValue = lLength - 1 ; lValue >= 0; lValue--, lCount++)
		{
			pmItoa_arr [lCount] = cTemp_arr [lValue] ;
		}
		pmItoa_arr [lCount] = '\0' ;
	}
	return (pmItoa_arr) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief 		This function locates the last occurance of a character in 
 *				string.
 * @param[in] 	pcStr1 Input string in which the last occurance of 
 *				cChar has to be found.
 * @param[in] 	cChar character to match with.
 * @retval		pointer to last occurance of cChar in pcStr1.
 * @retval		NULL if cChar is not found.
 **/
JC_INT8* jdd_MiscStrrchr (const JC_INT8 *pcStr1, JC_INT8 cChar)
{
	JC_INT8* pcRetVal = JC_NULL ;
	
	JDD_LOG_INFO0 ("jdd_MiscStrrchr : Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pcStr1), 
					"jdd_MiscStrrchr: pcStr1 is NULL ") ;
	
	while ('\0' != *pcStr1)
	{
		if(cChar == *pcStr1)
		{
			pcRetVal = pcStr1 ;
		}
		pcStr1++ ;
	}

	JDD_LOG_INFO0 ("jdd_MiscStrrchr : Exit") ;
	return (pcRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief 		This function locates a substring in a string.
 * @param[in] 	pcStr1 String from which substring has to be located. 
 * @param[in] 	pcStr2 substring.
 * @return 		pcRetval Pointer.
 **/
JC_INT8* jdd_Miscstrstr(const JC_INT8 *pcStr1, const JC_INT8 *pcStr2)
{
	JC_INT8* pcRetVal = JC_NULL ;
	JC_INT8* pcRet = JC_NULL ;
	JC_INT8* pcTemp = JC_NULL ; 
	
	JDD_LOG_INFO0 ("jdd_MiscStrstr : Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pcStr1) && (JC_NULL != pcStr2)
				"jdd_MiscStrstr: Invalid parameter") ;
	if (!*pcStr2)
	{
		pcRetVal = pcStr1 ;
	}
	else
	{	
		while (*pcStr1)
	    {
			pcRet = pcStr1 ;
			pcTemp = pcStr2 ;
			do
			{
				if (!*pcTemp)
				{
					pcRetVal = pcStr1 ;
					break ;
				}
			} while (*pcRet++ == *pcTemp++) ;
			pcStr1 ++ ;
	    }
	}
	JDD_LOG_INFO0 ("jdd_MiscStrstr : Exit") ;
	return ( pcRetVal ) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief 		This function compare two strings ignoring case for n bytes. 
 * @param[in] 	pcStr1 	String to be compared. 
 * @param[in] 	pcStr2 String to be compared.
 * @param[in]	ilen Number of characters to be compared.
 * @return 		iRetval 0 on success else -1
 */
JC_INT32 jdd_MiscStrncasecmp (const JC_INT8 *pcStr2, const JC_INT8 *pcStr1, \
							JC_INT32 ilen)
{
	JC_INT32 iRetVal = 0 ;
	JC_INT32 istr1_len ;
	JC_INT32 istr2_len ;
	JC_INT32 iNum = 0 ;

	JDD_LOG_INFO0 ("jdd_MiscStrncasecmp : Entry") ;

	if (pcStr2 != JC_NULL || pcStr1 != JC_NULL)
	{
		istr1_len = (JC_INT32)(jdd_MiscStrlen (pcStr1)) ;
		istr2_len = (JC_INT32)(jdd_MiscStrlen (pcStr2)) ;

		if (istr1_len < ilen || istr2_len < ilen)
		{
			iRetVal =  -1 ;
		}
		else
		{
			for (iNum=0; iNum < istr1_len; iNum++)
			{
				if (jdd_MiscTolower((JC_INT8)pcStr2 [iNum]) \
								!= jdd_MiscTolower(pcStr1 [iNum]))
				{
					iRetVal = -1 ;
					break;
				}
			}
		}
	}

	JDD_LOG_INFO0 ("jdd_MiscStrncasecmp : Exit") ;
	return ( iRetVal ) ;
}


/**
 * @ingroup		jdd_charutils
 * @brief		This function translate characters to uppercase
 * @param[in]	iVal Value to be converted as uppercase character
 * @retval		returns the uppercase equivalent of iVal
 **/
JC_INT32 jdd_MiscToupper (JC_INT32 iVal)
{
	return ((iVal >= 'a' && iVal <= 'z') ? iVal - 32: iVal) ;
}

/**
 * @ingroup		jdd_charutils
 * @brief		This function translate characters to lowercase
 * @param[in]	iVal Value to be converted as lowercase character
 * @retval		returns the lowercase equivalent of iVal
 **/
JC_INT32 jdd_MiscTolower (JC_INT32 iVal)
{	
	return ((iVal >= 'A' && iVal <= 'Z') ? iVal + 32: iVal) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function compares the two given string.
 * @param[in]	pmFirstString Specifies the First Input string.
 * @param[in]	pmSecondString Specifies the Second Input string.
 * @retval		Returns 0 if both are same.
 * @retval		Returns -1 if not same.
 **/
JC_INT32 jdd_MiscStrcmp (const JC_INT8 *pcFirstString, \
						const JC_INT8 *pcSecondString)
{
	JC_INT32 iRetVal ;
	while ((*pcFirstString) == (*pcSecondString))
	{
		if (('\0' == *pcFirstString) && ('\0' == *pcSecondString))
		{
			iRetVal = 0 ;
			break ;
		}
		pcFirstString++ ;
		pcSecondString++ ;
	}
	
	if(0 != iRetVal)
	{
		iRetVal = (*(pcFirstString) < *((pcSecondString)) ? -1 : 1) ;
	}
	
	return (iRetVal) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function compares the two given string.
 * @param[in]	pmFirstString Specifies the First Input string.
 * @param[in]	pmSecondString Specifies the Second Input string.
 * @param[in]	ivalue Specifies the number till the string is compared.
 * @retval		Returns 0 if both are same.
 * @retval		Returns 1 or -1 if not same.
 **/
JC_INT32 jdd_MiscStrncmp (const JC_INT8 *pcFirstString, \
					const JC_INT8 *pcSecondString, JC_INT32 iValue)
{
	JC_INT32 iRetval = 0 ;
	while ((*pcFirstString) == (*pcSecondString))
	{
		if (iRetval == iValue)
		{
			iRetval = 0 ;
			break ;
		}
		iRetval++ ;
		pcFirstString++ ;
		pcSecondString++ ;
	}

	if(0 != iRetval)
	{
		iRetval = (*(pcFirstString) < *((pcSecondString)) ? -1 : 1) ;
	}
	return (iRetval);
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function copies the second string to first.
 * @param[in]	pmFirstString Specifies the Destination Input string.
 * @param[in]	pmSecondString Specifies the Source Input string.
 * @retval		Returns pointer of Destination string.
 **/
 JC_INT8* jdd_MiscStrcpy (JC_INT8 *pcFirstString, \
						const JC_INT8 *pcSecondString)
 {
	if (JC_NULL != pcSecondString)
	{
		do
		{
			*pcFirstString++ = *pcSecondString ;
		} while ('\0' != pcSecondString++ ) ;
		*pcFirstString = '\0' ;
	}
	return (pcFirstString) ;
}

 /**
 * @ingroup		jdd_miscutils
 * @brief		This function copies the second string to first.
 * @param[in]	pmFirstString Specifies the Destination Input string.
 * @param[in]	pmSecondString Specifies the Source Input string.
 * @param[in]	ivalue Specifies the number till the string is copied.
 * @retval		Returns pointer of Destination string.
 **/
JC_CHAR *jdd_MiscWstrncpy (JC_CHAR * pcFirstString, \
					register const JC_CHAR * pcSecondString, JC_INT32 iValue)		
{
	JC_INT32 iCount = 0 ;
	if (JC_NULL != pcSecondString)
	{
		do
		{
			*pcFirstString++ = *pcSecondString ;
			iCount++ ;
		} while (iCount != iValue) ;
		*pcFirstString = '\0' ;
	}
	return (pcFirstString) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function ascii string to integer.
 * @param[in]	pcBuffer Input string.
 * @retval		Returns integer equivalant to ascii string .
 **/
JC_INT32 jdd_MiscAtoI (JC_INT8 *pcBuffer)
{
    JC_INT32 iValue = 0, iDigit;
    JC_INT8 cTemp ;

    while ((cTemp = *pcBuffer++) != '\0') 
	{

        if (cTemp >= '0' && cTemp <= '9')
		{
            iDigit = (JC_INT32) (cTemp - '0') ;
		}
        else
		{
            break;
		}
        iValue = (iValue * 10) + iDigit;
    }
    return (iValue);
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function returns integer pointer to ascii string.
 * @param[in]	lValue Input number to be converted to ascii string .
 * @param[out]	pcLtoa_arr Output string.
 * @param[in]	iRadix Input Radix.
 * @retval		Returns integer pointer to ascii string .
 * @retval		Returns NULL on error
 **/
 
JC_INT8 *jdd_MiscLtoA (JC_INT64 lValue, JC_INT8 *pcLtoa_arr, JC_INT32 iRadix)
{
	JC_INT64 	lCount, lLength, lValue ;
	JC_INT8 cTemp_arr [MAX_ITOA_lLength] ;
	JC_INT32 iIs_negative_val = 0 ;

	lCount = 0 ;
	if (0 > lValue)
		iIs_negative_val = 1 ;
	do
	{
		cTemp_arr [lCount++] = (JC_INT8) ((lValue % iRadix) + '0') ;
   	} while (0 < (lValue /= iRadix)) ;
	cTemp_arr [lCount] = '\0' ;
	lLength = jc_strlLength (cTemp_arr) ;
	lValue = 0 ;
	if (iIs_negative_val == 1)
		pcLtoa_arr [lValue++] = '-' ;
	for (lCount = lLength - 1 ; lCount >= 0; lCount--, lValue++)
	{
		pcLtoa_arr [lValue] = cTemp_arr [lCount] ;
	}
	pcLtoa_arr [lValue] = '\0' ;
	return (pcLtoa_arr) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function concatenate two strings
 * @param[in]	pcstr1 String which is going to be appended
 * @param[in]	pcstr2 String to be copied
 * @retval		This function returns the initial value of pcstr1
 *
 * jdd_strcat appends a copy of the string pointed to by pcstr2 (including the
 * terminating null character) to the end of the string pointed to by pcstr1.
 * The  initial  character of pcstr2 overwrites the null character at the end
 * of pcstr1.
 **/
JC_INT8 * jdd_MiscStrcat (JC_INT8 *pcstr1, const JC_INT8 *pcstr2)
{
	JC_INT8 *pcRetVal = JC_NULL ;
	
	DD_LOG_INFO0 ("jdd_strcat: Entry") ;
	
	/*
	 * Concatenate pcstr2 on the end of pcstr1. pcstr1 must be large enough.
	 * Return s1.
	 */
	pcRetVal = pcstr1;

	while(*pcstr1++)
		;

	--pcstr1 ;
	
	while(*pcstr1++ = *pcstr2++) ;

	JDD_LOG_INFO0 ("jdd_strcat: Exit") ;
	
	return (pcRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function concatenate two strings
 * @param[in]	pcstr1 String which is going to be appended
 * @param[in]	pcstr2 String to be copied
 * @param[in]	iN length of charecters to be copied
 * @retval		This function returns the initial value of str1
 *
 * jdd_strncat appends not more than iN(length) characters from the string
 * pointed to by pcstr2 (including the terminating null character)  to  the
 * end of the string pointed to by pcstr1. The initial character of pcstr2
 * overwrites the null character at the end of pcstr1. A terminating null
 * character is always appended to the result
 **/
JC_INT8* jdd_MiscStrncat (JC_INT8* pcstr1, const JC_INT8* pcstr2, JC_INT32 iN)
{
	JC_INT8 *pcRetVal = JC_NULL ;
	
	DD_LOG_INFO0 ("jdd_strncat: Entry") ;
	
	/*
	 * Concatenate pcstr2 on the end of pcstr1. pcstr1 must be large enough.
	 * At most iN characters are moved.
	 */
	pcRetVal = pcstr1;
	while(*pcstr1++) ;
		
	--pcstr1 ;
	
	while(*pcstr1++ = *pcstr2++)
	{
		if(iN == 0)
		{
			*--pcstr1 = '\0' ;
			break ;
		}
		iN-- ;
	}
  	
	JDD_LOG_INFO0 ("jdd_strncat: Exit") ;
	
	return (pcRetVal) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		whitespace wide character test
 * @param[in]	pmValue Value to be tested
 * @retval		non-zero for value is whitespace wide characters.
 **/
JC_BOOLEAN jdd_MiscIswspace (JC_CHAR pmValue )
{
	JC_BOOLEAN	bRetVal = E_FALSE ;
	
	JDD_LOG_INFO0 ("jdd_iswspace: Entry") ;	
    /*
	 * returns a non-zero value if c is a white-space character 
	 * (0x09 ¡V 0x0D or 0x20)
	 */
    JC_CHAR wsMin = 0x0009 ;
    JC_CHAR wsMax = 0x000d ;
    JC_CHAR wsSpace = 0x0020 ;

    if (pmValue >= wsMin && pmValue <= wsMax)
        bRetVal = E_TRUE ;

    if (pmValue == wsSpace)
        bRetVal = E_TRUE ;
		
	JDD_LOG_INFO0 ("jdd_iswspace: Exit") ;

    return (bRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		whitespace character predicate
 * @param[in]	iVal Value to be tested
 * @retval		It is a predicate returning non-zero for whitespace  
 *				characters, and 0 for other characters. 
 **/
JC_BOOLEAN jdd_MiscIsspace (JC_INT32 iVal)
{
	JC_BOOLEAN	bRetVal = E_FALSE ;
	
	JDD_LOG_INFO0 ("jdd_isspace: Entry") ;

	if (c == ' ' || c == '\t' || c == '\n')
	{
		bRetVal = E_TRUE ;
	}
	
	JDD_LOG_INFO0 ("jdd_isspace: Exit") ;
	
	return (bRetVal) ;
}


/**
 * @ingroup		jdd_miscutils
 * @brief		This function specifies whether the input is 
 *				an uppercase alphabet (A-Z).
 * @retval		E_TRUE	If input is an Uppercase Alphabet
 * @retval		E_FALSE	If input is not an Uppercase Alphabet 
 **/
JC_BOOLEAN jdd_MiscIsupper (JC_INT32 i)
{
	JC_BOOLEAN bRetVal = E_FALSE ;

	if ('A' <= i && 'Z' >= i)
	{
		bRetVal = E_TRUE ;
	}

	return (bRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function specifies whether the input is 
 *				a lowercase alphabet (a-z).
 * @retval		E_TRUE	If input is a Lowercase Alphabet
 * @retval		E_FALSE	If input is not a Lowercase Alphabet 
 **/
JC_BOOLEAN jdd_MiscIslower (JC_INT32 i)
{
	JC_BOOLEAN bRetVal = E_FALSE ;

	if ('a' <= i && 'z' >= i)
	{
		bRetVal = E_TRUE ;
	}

	return (bRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function specifies whether the input is 
 *				a digit(0-9).
 * @retval		E_TRUE	If input is a digit
 * @retval		E_FALSE	If input is not a digit 
 **/
JC_BOOLEAN jdd_MiscIsdigit (JC_INT32 i)
{
	JC_BOOLEAN bRetVal = E_FALSE ;

	if ('0' <= i && '9' >= i)
	{
		bRetVal = E_TRUE ;
	}

	return (bRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function specifies whether the input is 
 *				an alphabet/number (a-z , A-Z, 0-9) .
 * @retval		E_TRUE	If input is an alphabet/number
 * @retval		E_FALSE	If input is not an alphabet/number 
 **/
JC_BOOLEAN jdd_MiscIsalnum(JC_INT32 i)
{
	JC_BOOLEAN bRetVal = E_FALSE ;

	if ( ('A' <= i && 'Z' >= i) || ('a' <= i && 'z' >= i) || \
		('0' <= i && '9' >= i) )
	{
		bRetVal = E_TRUE ;
	}

	return (bRetVal) ;

}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function specifies whether the input is 
 *				an alphabet (a-z , A-Z).
 * @retval		E_TRUE	If input is an Alphabet
 * @retval		E_FALSE	If input is not an Alphabet 
 **/
JC_BOOLEAN jdd_MiscIsalpha(JC_INT32 i)
{
	JC_BOOLEAN bRetVal = E_FALSE ;

	if ( ('A' <= i && 'Z' >= i) || ('a' <= i && 'z' >= i) )
	{
		bRetVal = E_TRUE ;
	}

	return (bRetVal) ;

}

/**
 * @ingroup		jdd_miscutils
 * @brief		This function specifies whether the input is 
 *				an ascii value (0-128).
 * @retval		E_TRUE	If input is an ascii value
 * @retval		E_FALSE	If input is not an ascii value 
 **/
JC_BOOLEAN jdd_MiscIsascii(JC_INT32 i)
{
	JC_BOOLEAN bRetVal = E_FALSE ;

	if (0 <= i && 128 > i) 
	{
		bRetVal = E_TRUE ;
	}

	return (bRetVal) ;

}

/**
 * @ingroup		jdd_miscutils
 * @brief 		This function checks whether given number is a 
 *				hexadecimal (has value 0-9 or a-f or A-F).
 * @param[in]	iCharVal ASCII value of the character to be validated.
 * @retval 		E_TRUE if iCharVal is a hexadecimal
 * @retval		E_FALSE otherwise.
 **/
JC_BOOLEAN jdd_MiscIsxdigit (JC_INT32 iCharVal)
{
	JC_BOOLEAN bRetVal;
	
	if ((0 <= iCharVal && 9 >= iCharVal) || \
		('a' <= iCharVal && 'f' >= iCharVal) || \
		('A' <= iCharVal && 'F' >= iCharVal) )
	{
		bRetVal = E_TRUE ;
	}
	else
	{
		bRetVal = E_FALSE ;
	}
	
	return (bRetVal) ;
}

/**
 * @ingroup		jdd_miscutils
 * @brief 		This function searches for the first occurance of 
 *				given character in the specified string.
 * @param[in]	pcSrcStr String in which cChar has to be searched.
 * @param[in]	cChar ASCII value of the character to be searched.
 * @retval 		JC_INT8 pointer pointing to first occurance of cChar.
 **/
 JC_INT8 *jdd_MiscStrchr (const JC_INT8 *pcSrcStr, const JC_INT8 cChar)
 {
	JC_INT8* pcRetval = JC_NULL ;
	
	jdd_AssertOnFail ((JC_NULL != pcSrcStr), \
		"jdd_miscStrchr: pcSrcStr is NULL ") ;
		
	while ('\0' != *pcSrcStr)
	{
		if (*pcSrcStr == cChar)
		{
			pcRetVal = pcSrcStr ;
			break ;
		}
		pcSrcStr++ ;
	}
	
	return (pcRetval) ;
 }
#endif

JC_INT8 * jdd_strncat(JC_INT8 * pStr1, const JC_INT8 *pStr2, JC_UINT32 Count)
{
    JC_INT8 * r = pStr1;
    JC_INT8 c;

    while (*pStr1 != '\0')
        pStr1++;

    while ((Count-- > 0) && ((c = *pStr2++) != '\0'))
        *pStr1++ = c;
    *pStr1++ = '\0';

    return r;
}


JC_UINT64 jdd_MiscGetUniqueValue ()
{
	return (jdd_TimerSystemGetTickCount ());
	//return (1) ;
	//return GetTickCount () ;

}


JC_DOUBLE jdd_MiscPower (JC_DOUBLE x , JC_DOUBLE y)
{
//	return pow (x, y) ;

	JC_UINT32	ix, iy ;
	JC_UINT32	iRet ;
	
	ix = x;
	iy = y;

	if (0 == ix)
		iRet = 0 ;
	else if ((0 == iy) || (ix == 1))
		iRet = 1 ;
	else
	{
		for (iRet = 1; iy; iy--)
		{
			iRet *= ix ;
		}
	}

	mmi_trace (1, "@@@ jdd_miscpower: %f ^ %f = %d", x, y, iRet) ;
	return ((JC_DOUBLE)iRet) ;
}



static const ST_MIME_EXTENSION stMimeExtension [] =
{
	{".mrt",			"x-multipart/related"},
	{".html",			"text/html"},
	{".htm",			"text/html"},
	{".xhtml",			"text/html"}, 
	{".txt",			"text/plain"},
	{".vcs",			"text/x-vcalendar"}, 
	{".vcf",			"text/x-vcard"},
	{".wml",			"text/vnd.wap.wml"},
	{".wmls",			"text/vnd.wap.wmlscript"},
	{".wmlc",			"application/vnd.wap.wmlc"},
	{".wmlsc",			"application/vnd.wap.wmlscriptc"},
	{".bmp",			"image/bmp"},
	{".bmp",			"image/x-bmp"},
	{".gif",			"image/gif"},
	{".jpeg",			"image/jpeg"},
	{".jpg",			"image/jpeg"},
	{".tiff",			"image/tiff"},
	{".tif",			"image/tiff"},
	{".png",			"image/png"},
	{".wbmp",			"image/vnd.wap.wbmp"},
	{".wbmp",			"image/wbmp"},
	{".si",				"text/vnd.wap.si"},
	{".sic",			"application/vnd.wap.sic"},
	{".sl",				"text/vnd.wap.sl"},
	{".slc",			"application/vnd.wap.slc"},
	{".co",				"text/vnd.wap.co"},
	{".coc",			"application/vnd.wap.coc"},
	{".css",			"text/css"},
	{".amm",			"application/vnd.wap.multipart.mixed"},
	{".wav",			"audio/wav"},
	{".wav",			"audio/x-wav"},
	{".mp3",			"audio/mp3"},
	{".mp3",			"audio/mpeg"},
	{".svg",			"image/svg+xml"},
	{".svg+xml",		"image/svg+xml"},
	{".js",				"application/x-javascript"},
	{".es",				"application/x-ecmascript"},
	{".js",				"application/javascript"},
	{".es",				"application/ecmascript"},
	{".js",				"text/javascript"},
	{".es",				"text/ecmascript"},
	{".out",			"multipart/related; boundary=\"=jataayuProxyAccelerator\""},
	{".3gp",			"video/3gpp"},
	{".3g2",			"video/3gpp2"},
	{".aac",			"audio/aac"},
	{".mid",			"audio/mid"},
	{".mid",			"audio/midi"},
	{".mid",			"audio/sp-midi"},
	{".mpeg",			"video/mpeg"},
	{".mpg",			"video/mpeg"},	
	{".avi",			"video/avi"},
	{".mp4",			"video/mp4"},
	{".amr"	,			"audio/amr"},
	{".xml"	,			"text/xml"},
	{".qcp"	,			"audio/vnd.qcelp"},
	{".mmf"	,			"application/vnd.smaf"},
	{".mmf"	,			"audio/mmf"},
	{".imy"	,			"audio/imy"},
	{".imy"	,			"text/x-imelody"},
	{".doc"	,			"application/msword"},
	{".xls"	,			"application/vnd.ms-excel"},
	{".ppt"	,			"application/vnd.ms-powerpoint"},
	{".pdf"	,			"application/pdf"},
	{JC_NULL,			JC_NULL}
} ;


/**
 * @brief This function gets the appropriate content type for 
 *		  given filename extension.
 * @param[in] pmFileName, File name for which to get the content type
 * @param[out] ppcContentType The content type for the filename
 * @return void
 */
void	jdd_MiscGetContentTypeForExt (const JC_CHAR		*pmFileName, 
									  const JC_INT8		**ppcContentType)
{
	JC_INT32	iIndex = 0 ; 
	JC_BOOLEAN	bFound = E_FALSE ;

	if (JC_NULL != pmFileName)
	{
		while ((E_FALSE == bFound) && 
			   (JC_NULL != stMimeExtension[iIndex].pcExtension))
		{
			if (E_TRUE == jdi_CUtilsTcsStrEndsWith (pmFileName, 
												stMimeExtension[iIndex].pcExtension))
			{
				bFound = E_TRUE ;
				*ppcContentType = stMimeExtension[iIndex].pcContentType ;
			}
			iIndex++ ;
		}
	}
	if (E_TRUE == bFound)
	{
		mmi_trace (1, "JDD_LOG: jdd_MiscGetContentTypeForExt >> Content Type: %s", *ppcContentType) ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jdd_MiscGetContentTypeForExt >> Content Type: NULL") ;
	}
	return ;
}


#ifdef BROWSER_JUDGMENT_OF_CONTENT

/**
 * @brief Checks the few starting bytes of the given content and gets the content type
 * @param[in]	pucContent		Specifies the content to be judged
 * @param[out]	ppcContentType	Holds the output content type
 * @return void
 */
void jdd_MiscGetContentTypeForData (const JC_UINT8		*pucContent,
									const JC_INT8		**ppcContentType)
{
	JC_INT32		iIndex ;
	JC_UINT8		aucPNG[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00} ;

	if (0xFF == *pucContent && 0xD8 == *(pucContent + 1))
	{
		*ppcContentType = MISC_IMAGE_JPEG ;
	}
	else if (E_TRUE == jdi_CUtilsStrStartsWith ((JC_INT8 *)pucContent, (JC_INT8 *)aucPNG))
	{
		*ppcContentType = MISC_IMAGE_PNG ;
	}
	else
	{
		*ppcContentType = JC_NULL ;
		iIndex = 0 ;
		while ((JC_NULL == *ppcContentType) && 
			   (JC_NULL != stContentjudgment[iIndex].pcStartBytes))
		{
			if (E_TRUE == jdi_CUtilsStrStartsWith ((JC_INT8 *)pucContent, 
								stContentjudgment[iIndex].pcStartBytes))
			{
				*ppcContentType = stContentjudgment[iIndex].pcContentType ;
			}
			iIndex++ ;
		}
	}
}

#endif


/**
 * @brief This function gets the extension to apply to the filename
 *        for storing the data of given content type.
 * @param[in] pcContentType, Type of the given content
 * @param[out] ppcExtension, Extension to apply for the content
 * @return void
 */
void jdd_MiscGetExtForContentType	(const JC_INT8		*pcContentType, 
									 const JC_INT8		**ppcExtension)
{
	JC_INT32		iIndex ;
	JC_BOOLEAN		bFound ;

	iIndex = 0 ;
	bFound = E_FALSE ;
	if (JC_NULL != pcContentType)
	{
		while ((E_FALSE == bFound) && 
			   (JC_NULL != stMimeExtension[iIndex].pcContentType))
		{
			if (0 != jc_strstr (pcContentType, stMimeExtension[iIndex].pcContentType))
			{
				bFound = E_TRUE ;
				*ppcExtension = stMimeExtension[iIndex].pcExtension ;
			}
			iIndex++ ;
		}
	}
	if (E_TRUE == bFound)
	{
		mmi_trace (1, "JDD_LOG: jdd_MiscGetExtForContentType >> Extention is %s for %s", *ppcExtension, pcContentType) ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jdd_MiscGetExtForContentType >> Extention is NULL for %s", pcContentType) ;
	}
	return ;
}





/**
 * @ingroup		jdd_miscutils
 * @brief 		This function compare two strings ignoring case.
 * @param[in] 	pcStr1 String to be compared. 
 * @param[in] 	pcStr2 String to be compared.
 * @return 		iRetval 0 on success else -1
 **/
JC_INT32 strcasecmp (const JC_INT8 *pcStr2, const JC_INT8 *pcStr1)
{
	
	JC_INT32 iRetVal = 0 ;
	JC_INT32 istr1_len ;
	JC_INT32 istr2_len ;
	JC_INT32 iNum = 0 ;

	JDD_LOG_INFO0 ("jdd_MiscStrcasecmp : Entry") ;

	if (pcStr2 != JC_NULL || pcStr1 != JC_NULL)
	{
		istr1_len = (JC_INT32)(strlen (pcStr1)) ;
		istr2_len = (JC_INT32)(strlen (pcStr2)) ;
		if (istr1_len != istr2_len)
		{
			iRetVal =  -1 ;
		}
		else
		{
			for (iNum=0; iNum < istr1_len; iNum++)
			{
				if (tolower((JC_INT8)pcStr2 [iNum]) \
								!= tolower((JC_INT8)pcStr1 [iNum]))
				{
					iRetVal = -1 ;
					break;
				}
			}
		}
	}
	JDD_LOG_INFO0 ("jdd_MiscStrcasecmp : Exit") ;
	return ( iRetVal ) ;
}



/**
 * @ingroup		jdd_miscutils
 * @brief 		This function converts from ascii to float.
 * @param[in] 	pcStr1 Input ascii value which has to be converted to float. 
 * @return 		float value corresponding to pcStr1.
 **/
 JC_DOUBLE atof(const JC_INT8 *pcStr1)
{
	JC_DOUBLE dVal, dPower, dRetVal = 0.0 ;
	JC_INT8 cSign;
	JC_INT32	cNum ;
	
	JDD_LOG_INFO0 ("jdd_MiscAtof : Entry") ;

	for (cNum = 0; isspace(pcStr1[cNum]); cNum++) /* skip white space */
		;
	cSign = (pcStr1[cNum] == '-') ? -1 : 1 ;
	if (pcStr1[cNum] == '+' || pcStr1[cNum] == '-')
		cNum++ ;
	for (dVal = 0.0; isdigit(pcStr1[cNum]); cNum++)
		dVal = 10.0 * dVal + (pcStr1[cNum] - '0') ;
	if (pcStr1[cNum] == '.')
		cNum++ ;
	for (dPower = 1.0; isdigit(pcStr1[cNum]); cNum++) 
	{
		dVal = 10.0 * dVal + (pcStr1[cNum] - '0') ;
		dPower *= 10 ;
	}
	dRetVal = cSign * dVal / dPower ;

	JDD_LOG_INFO0 ("jdd_Miscatof : Exit") ;
	return (dRetVal) ;
}



/** 
 * @brief This function returns floor of the double value.
 * @param[in] dValue	Input value
 */
JC_DOUBLE jdd_MiscFloor (JC_DOUBLE dValue)
{
	return ((JC_DOUBLE)SUL_FloorEX (dValue)) ;
}

/**
 * @brief This function return the ceil of the double value.
 * @param[in] dValue	Input value
 */
JC_DOUBLE jdd_MiscCeil (JC_DOUBLE dValue)
{
	return ((JC_DOUBLE)SUL_CeilEX (dValue)) ;
}

#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-28 */
/** 
 * @brief This function returns floor of the double value.
 * @param[in] dValue	Input value
 */
JC_DOUBLE ceil (JC_DOUBLE dValue)
{
	return dValue ;
}

/**
 * @brief This function return the ceil of the double value.
 * @param[in] dValue	Input value
 */
JC_DOUBLE floor (JC_DOUBLE dValue)
{
	return dValue ;
}

#endif /* #ifdef MMI_ON_HARDWARE_P */

/**
 * @ingroup		jdd_miscutils
 * @brief		This function converts integer to wide char String .
 * @param[in]	lNum Input Number .
 * @param[out]	pmItoa_arr resultant string .
 * @param[in]	iRadix Input Radix .
 * @retval		Returns pointer to first Instance of sub string on success
 * @retval		Returns NULL on failure
 **/

JC_CHAR * jdd_MiscItow (JC_INT64 lNum, JC_CHAR *pmItoa_arr, JC_INT32 iRadix)
{
	JC_INT64	lValue, lLength, lCount ;
	JC_CHAR cTemp_arr [255] ;
	JC_INT32 iIs_negative_val = 0 ;

    if (JC_NULL == pmItoa_arr)
	{
        pmItoa_arr = JC_NULL ;
	}
	else
	{
		lValue = 0 ;
		if (0 >lNum )
		{
			iIs_negative_val = 1 ;
			lNum *= -1 ;
		}
		do
		{
			cTemp_arr [lValue++] = (JC_CHAR) ((lNum % iRadix) + '0') ;
			if (255 == lValue)
			{
				break ;
			}
		} while (0 < (lNum /= iRadix)) ;
		cTemp_arr [lValue] = '\0' ;
		lLength = jc_tcslen ((JC_INT8*)cTemp_arr) ;
		
		lCount = 0 ;
		if (1 == iIs_negative_val)
		{
			pmItoa_arr [lCount++] = '-' ;
		}
		for (lValue = lLength - 1 ; lValue >= 0; lValue--, lCount++)
		{
			pmItoa_arr [lCount] = cTemp_arr [lValue] ;
		}
		pmItoa_arr [lCount] = '\0' ;
	}
	return (pmItoa_arr) ;
}

JC_CHAR *_itow (JC_INT32	i)
{
	return 0 ;
}

#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-28 */

JC_CHAR wcschr (JC_CHAR *src, JC_INT8 c)
{
	return 0 ;
}

JC_CHAR wcsrchr (JC_CHAR *src, JC_INT8 c)
{
	return 0 ;
}

JC_CHAR *wcsstr (JC_CHAR *src, JC_CHAR *sub)
{
	return 0 ;
}

JC_BOOLEAN iswspace (JC_CHAR c)
{
	return 0 ;
}

JC_BOOLEAN wcsnicmp (JC_CHAR *str1, JC_CHAR *str2, JC_INT32 iCount)
{
	JC_UINT16 *uiStr1, *uiStr2 ;

	if (!str1 || !str2)
	{
		return (E_TRUE) ;
	}

	uiStr1 = str1 ;
	uiStr2 = str2 ;
	
	while (iCount && *uiStr1 && *uiStr2)
	{
		if (*uiStr1 != *uiStr2)
		{
			return (E_TRUE) ;
		}

		uiStr1++, uiStr2++, iCount-- ;
	}

	if (!iCount || (!uiStr1 && !uiStr2))
	{
		return E_FALSE ;
	}
	else
	{
		return E_TRUE ;
	}
	
}

JC_BOOLEAN wcsicmp (JC_CHAR *str1, JC_CHAR *str2)
{
	JC_UINT16 *uiStr1, *uiStr2 ;

	if (!str1 || !str2)
	{
		return (E_TRUE) ;
	}
	
	uiStr1 = str1 ;
	uiStr2 = str2 ;
	
	while (*uiStr1 && *uiStr2)
	{
		if (*uiStr1 != *uiStr2)
		{
			return (E_TRUE) ;
		}

		uiStr1++, uiStr2++ ;
	}

	if (!uiStr1 && !uiStr2)
	{
		return E_FALSE ;
	}
	else
	{
		return E_TRUE ;
	}
}

#endif /* #ifdef MMI_ON_HARDWARE_P */

JC_INT8* ltoa (JC_UINT64 lVal, JC_INT8* pcOut, JC_UINT32 uiRad)
{
//	return (JC_INT8*)(jc_itoa((S32)lVal,(UI_string_type) pcOut, uiRad)) ;
	return (NULL) ;
}

void misc_print (int size)
{
}

JC_INT8* jmisc_itoa (JC_UINT32 lVal, JC_INT8* pcOut, JC_UINT32 uiRad)
{
	jc_sprintf(pcOut,"%d",lVal) ;
	return pcOut ;
}

void jdd_MiscWatchDogReset (void)
{
	return ;
}


/*END OF FILE*/
#endif
