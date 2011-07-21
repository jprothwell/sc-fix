/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   UCS2.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   UCS2 utility
 *
 * Author:
 * -------
 * -------
 *  
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 

#include "mmi_data_types.h"
#include "ucs2dcl.h"
#include "ucs2prot.h"
#include "unicodexdcl.h"
#include "string.h"
#include "debuginitdef.h"
#include "stdio.h"

extern S32 mmi_ucs2strlen(const S8 *arrOut);
extern kal_int8 *app_ucs2_strcat(kal_int8 *strDestination, const kal_int8 *strSource);
extern kal_int8 *app_ucs2_strchr(const kal_int8 *string,  kal_wchar ch);
extern kal_int8 *app_ucs2_strcpy(kal_int8 *strDestination, const kal_int8 *strSource);
/**************************************************************

	FUNCTION NAME		: UnicodeToUCS2Encoding

  	PURPOSE				: convert unicode to UCS2 encoding

	INPUT PARAMETERS	: U16 -> unicode value to be encoded 

	OUTPUT PARAMETERS	: array of U8 

	RETURNS				: U8 -> Status 

 

**************************************************************/
U16 AnsiiToUnicode(U8 CharValue)
{

	U16 Unicode = 0;
	U8* pUnicode = NULL;

	pUnicode = (U8*)&Unicode;
	*pUnicode++ = CharValue;
	*pUnicode=0;
	
	return Unicode;			
}

U8 UnicodeToUCS2Encoding(U16 unicode,U8 *charLength,U8 *arrOut)
{

	U8 status = ST_SUCCESS;
	U8 index = 0;

	if(arrOut != 0)
	{
		
		if( unicode < 256 )
		{
			arrOut[index++] = *((U8*)(&unicode));
			arrOut[index] = 0;
			
		}
		else
		{
			arrOut[index++] = *((U8*)(&unicode));
			arrOut[index] =   *(((U8*)(&unicode)) + 1);

		}
		*charLength = 2;
	}	
	else
	{
		
		status = ST_FAILURE;
	}

#ifdef __FOR_TESTING  //MMI_ON_HARDWARE_P
	if(arrOut != 0)
	{
		
		if( unicode < 256 )
		{
			arrOut[index++] = 0; /* *((U8*)(&unicode));*/
			arrOut[index] = *((U8*)(&unicode));
			
		}
		else
		{
			arrOut[index++] = *(((U8*)(&unicode)) + 1);/* *((U8*)(&unicode));*/
			arrOut[index] =  *((U8*)(&unicode)); /* *(((U8*)(&unicode)) + 1);*/

		}
		*charLength = 2;
	}	
	else
	{
		
		status = ST_FAILURE;
	}
#endif



	return status;
}




/**************************************************************

	FUNCTION NAME		: UCS2EncodingToUnicode

  	PURPOSE				: convert UCS2 encoded scheme to unicode

	INPUT PARAMETERS	: PS8 -> array containing  UCS2 encoded characters

	OUTPUT PARAMETERS	: U16 -> unicode equivalent

	RETURNS				: U8 -> Status

 

**************************************************************/


U8 UCS2EncodingToUnicode(PU16 pUnicode ,PU8 arr)
{

	U8 index = 0;
	U8 status = ST_SUCCESS;
	
	
	if( (arr != NULL) && (pUnicode != NULL) )
	{
				
			*((U8*)(pUnicode)) = arr[index++];
			*(((U8*)(pUnicode))+ 1) = arr[index];
	}	
	else
	{
		
		status = ST_FAILURE;
	}


#ifdef __FOR_TESTING //MMI_ON_HARDWARE_P	
	if( (arr != NULL) && (pUnicode != NULL) )
	{
				
			*((U8*)(pUnicode)) = arr[index];/*arr[index++];*/
			*(((U8*)(pUnicode))+ 1) = arr[index++];/* arr[index];*/
	}	
	else
	{
		
		status = ST_FAILURE;
	}
#endif
	return status;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2trim_left
 * DESCRIPTION
 *  skip over the TrimChars
 * PARAMETERS
 *  strSrc          [IN]        
 *  TrimChars       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
S8 *mmi_ucs2trim_left(const S8 *strSrc, const S8 *TrimChars)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 *chr = (U16*) strSrc;
    S32 trim_length = mmi_ucs2strlen(TrimChars);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (*chr)
    {
        S32 i = 0;

        for (; i < trim_length; i++)
        {
            if (*chr == ((U16*) TrimChars)[i])
            {
                chr++;
                break;
            }
        }
        if (i == trim_length)
        {
            return (S8*) chr;
        }
    }
    return NULL;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2cat
 * DESCRIPTION
 *  
 *  
 *  User has to ensure that enough space is
 *  available in destination
 * PARAMETERS
 *  strDestination      [OUT]         
 *  strSource           [IN]        
 * RETURNS
 *  PS8
 *****************************************************************************/
S8 *mmi_ucs2cat(S8 *strDestination, const S8 *strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* On the Target */
    return (S8*)app_ucs2_strcat((kal_int8*)strDestination, (const kal_int8*)strSource);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2chr
 * DESCRIPTION
 *  Searches a UCS2 encoded string for a given wide-character,
 *  which may be the null character L'\0'.
 * PARAMETERS
 *  strSrc        [IN]  UCS2 encoded string to search in.       
 *  c             [IN]  UCS2 encoded wide-character to search for.      
 * RETURNS
 *  returns pointer to the first occurrence of ch in string
 *  returns NULL if ch does not occur in string
 *****************************************************************************/
S8 *mmi_ucs2chr(const S8 *strSrc, U16 c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (S8 *)app_ucs2_strchr((const kal_int8 *)strSrc,  (kal_wchar)c);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2cpy
 * DESCRIPTION
 *  copies the one UCS2 encoded string to other
 * PARAMETERS
 *  strDestination      [OUT]       StrDest-> Destination array
 *  strSource           [IN]        
 * RETURNS
 *  PS8 -> pointer to destination string or NULL
 *****************************************************************************/
S8 *mmi_ucs2cpy(S8 *strDestination, const S8 *strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* On the Target */
    return (S8*)app_ucs2_strcpy((kal_int8*)strDestination, (const kal_int8*)strSource);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2toi
 * DESCRIPTION
 *  to parse over a int number, including positive and negative signs (+,-).
 * PARAMETERS
 *  strSrc          [IN]        
 *  out_num         [?]         Is the parsed int number
 *  read_length     [?]         
 * RETURNS
 *  1: parse successfully ; -1: parse number fail.
 *****************************************************************************/
S32 mmi_ucs2toi(const S8 *strSrc, S32 *out_num, S32 *read_length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#define     GETNUM(wchar_p) (*wchar_p - AnsiiToUnicode('0'))

    U16 *src = (U16*) strSrc;
    MMI_BOOL is_negative = FALSE;
    S32 num = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (*src == AnsiiToUnicode('-'))
    {
        is_negative = TRUE;
        src++;
    }
    if (*src ==AnsiiToUnicode('+'))
    {
        src++;
    }
    if (GETNUM(src) < 0 || GETNUM(src) > 9)
    {
        return -1;
    }
    while (*src)
    {
        if (GETNUM(src) < 0 || GETNUM(src) > 9)
        {
            break;
        }
        num = num * 10 + GETNUM(src);
        src++;
    }

    *read_length = src - (U16*) strSrc;

    if (is_negative)
    {
        *out_num = -num;
    }
    else
    {
        *out_num = num;
    }
    return 1;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2find
 * DESCRIPTION
 *  search for the break_chars and store how many chars have been read over to reach
 *  the desired break_char in read_length
 * PARAMETERS
 *  strSrc          [IN]        
 *  break_chars     [IN]        
 *  read_length     [?]         
 *  direction       [IN]        1: sesarch forward ; -1: search backward.
 * RETURNS
 *  1: found the break_chars ;  -1: searched over the whole strSrc and do not fing the break_chars
 *****************************************************************************/
S32 mmi_ucs2find(const S8 *strSrc, const S8 *break_chars, S32 *read_length, S16 direction)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 *src = (U16*) strSrc;
    S32 x = 0;
    S32 break_chars_length = mmi_ucs2strlen(break_chars);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (*src)
    {
        for (x = 0; x < break_chars_length; x++)
        {
            if (*src == ((U16*) break_chars)[x])
            {
                if (direction > 0)
                {
                    *read_length = src - (U16*) strSrc;
                }
                else
                {
                    *read_length = (U16*) strSrc - src;
                }
                return 1;
            }
        }
        src += direction;
    }
    return -1;

}


/**************************************************************

	FUNCTION NAME		: UCS2Strlen

  	PURPOSE				: Gives the length of UCS2 encoded string

	INPUT PARAMETERS	: PS8 -> array containing  UCS2 encoded characters

	OUTPUT PARAMETERS	: none

	RETURNS				: U16 -> Status

 

**************************************************************/


S32 UCS2Strlen(const S8 *arrOut)
{

	S32 nCount = 0;
	S32	nLength	=	0;
	/*Check for NULL character only at the odd no. of bytes 
	  assuming forst byte start from zero */	
	if(arrOut)
	{
		while(arrOut[nCount] != 0 || arrOut[nCount+1] != 0)
		{
			++nLength;
			nCount+=2;
		}
	}
	return nLength;/*One is added to count 0th byte*/
}


/**************************************************************

	FUNCTION NAME		: UCS2Strcpy

  	PURPOSE				: copies the one UCS2 encoded string to other

	INPUT PARAMETERS	: PS8  strSrc -> Source Array	

	OUTPUT PARAMETERS	: PS8  strDest-> Destination array 

	RETURNS				: PS8 -> pointer to destination string or NULL

 

**************************************************************/


S8 *UCS2Strcpy(S8 *strDestination, const S8 *strSource )
{
	
	U16 count =1;
	S8 *temp = strDestination;

	if( strSource == NULL )
	{
            if(strDestination)
            {
		*(strDestination + count -1 ) = '\0';
		*(strDestination + count  ) = '\0';
	    }
    	    return temp;
	    
	}
		
	if( strDestination == NULL ||strSource == NULL  )
		return NULL;
	while(  !((*(strSource + count ) == 0 ) &&  (*(strSource + count -1 ) == 0 )))
	{
	
		
		
		*(strDestination + count -1 ) = *(strSource + count -1 );
		*(strDestination + count  ) = *(strSource + count );
		count += 2;
	}

	*(strDestination + count -1 ) = '\0';
	*(strDestination + count  ) = '\0';
		

	return temp;
}

/**************************************************************

	FUNCTION NAME		: UCS2Strcmp

  	PURPOSE				: compares two strings

	INPUT PARAMETERS	: PU8  string1 -> first String 

	OUTPUT PARAMETERS	: PU8  string2 -> Second String

	RETURNS				: 

 

**************************************************************/

S32 UCS2Strcmp( const S8 *string1, const S8 *string2 )
{

	if(NULL == string1 && NULL == string2)
	{
		return 0;
	}
	else if(NULL == string1)
	{
		return *string2;
	}
	else if(NULL == string2)
	{
		return *string1;
	}
	
	while( (*string1 == *string2) && (*(string1+1) == *(string2+1)))
	{
			
		if((*string1 == 0) && (*(string1+1) == 0)) 
		{
				return 0;
		}
			
		string1+=2; string2+=2;	
		
	}/*End of while */

	/*The return value indicates the lexicographic relation of string1 to string2*/
	/* ie  < 0 , > 0 and 0*/
	if ( *string1 == *string2 && *(string1 + 1) != *(string2+1) )
	{
		return ( *(string1+1) - *(string2+1) );
	}
	else 
	{
		return (*string1 - *string2);
		
	}
	

}



/**************************************************************

	FUNCTION NAME		: UCS2Strncmp

  	PURPOSE				: compares two strings

	INPUT PARAMETERS	: PU8  string1 -> first String 

	OUTPUT PARAMETERS	: PU8  string2 -> Second String

	RETURNS				: 

 

**************************************************************/

 
S32 UCS2Strncmp( const S8 *string1, const S8 *string2 , U32 size )
{
	U32 count = 0;
	U16 nStr1;
	U16 nStr2;
	size = size << 1; /* User is passing no of charcters not bytes*/
	
	if(NULL == string1 && NULL == string2)
	{
		return 0;
	}
	else if(NULL == string1)
	{
		return *string2;
	}
	else if(NULL == string2)
	{
		return *string1;
	}
	
	while( count < size )
	{
		nStr1 = (string1[1] << 8) | string1[0];
		nStr2 = (string2[1] << 8) | string2[0];
		if( nStr1 == 0 || nStr2 == 0 || nStr1 != nStr2 )
			return nStr1 - nStr2;
		string1 +=2;
		string2 +=2;
		count += 2;
	}
	return 0;
}
 
/*
S32 UCS2Strncmp( const S8 *string1, const S8 *string2 , U32 size)
{

	U32 count = 0;
	U16 nStr1 = 0;
	U16 nStr2 = 0;
	size *=2; // User is passing no of charcters no bytes


	while(count< size)
	{
		
		if((string1[count] != string2[count]) || (string1[count+1] != string2[count+1]) )
		{
			memcpy(&nStr1,string1+count,2);
			memcpy(&nStr2,string2+count,2);


			return nStr1  - nStr2;
		}
		count = count + 2;
	}
	return 0;

}
*/

/**************************************************************

	FUNCTION NAME		: UCS2Strncpy

  	PURPOSE				: copies the one UCS2 encoded string to other

	INPUT PARAMETERS	: PS8  strSrc -> Source Array	

	OUTPUT PARAMETERS	: PS8  strDest-> Destination array 

	RETURNS				: PS8 -> pointer to destination string or NULL

 

**************************************************************/


S8 *UCS2Strncpy(S8 *strDestination, const S8 *strSource , U32 size )
{
	
	U16 count =1;
	U32 count1 = 0;
	S8 *temp = strDestination;
	size = size * 2;

	MMI_ASSERT(!(strDestination == NULL));
	
	while(  !((*(strSource + count ) == 0 ) &&  (*(strSource + count -1 ) == 0 ))	
						 &&
				 ( count1 < size ))
	{
	
		*(strDestination + count -1 ) = *(strSource + count -1 );
		*(strDestination + count  ) = *(strSource + count );
		count += 2; count1 += 2; 
	}

	*(strDestination + count -1 ) = '\0';
	*(strDestination + count  ) = '\0';
		

	return temp;
}



/**************************************************************

	FUNCTION NAME		: UCS2Strcat

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 
						  available in destination
**************************************************************/


S8 *UCS2Strcat( S8 *strDestination, const S8 *strSource )
{

	S8  *dest = strDestination;

	
	dest = dest + UCS2Strlen(strDestination)* ENCODING_LENGTH;

	UCS2Strcpy(dest,strSource);
	return strDestination;


}

/**************************************************************

	FUNCTION NAME		: UCS2Strncat

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 
						  available in destination
**************************************************************/


S8 *UCS2Strncat( S8 *strDestination, const S8 *strSource , U32 size )
{

	S8  *dest = strDestination;

	
	dest = dest + UCS2Strlen(strDestination)* ENCODING_LENGTH;

	UCS2Strncpy(dest,strSource, size);
	return strDestination;


}


/**************************************************************

	FUNCTION NAME		: UCS2StrAppendChar

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 
						  available in destination
**************************************************************/
S8 *UCS2StrAppendChar( S8 *strDestination, U16 ch)
{
	S8  *dest = strDestination;
	U16 buffer[2];
	
	dest = dest + UCS2Strlen(strDestination) * ENCODING_LENGTH;
	buffer[0] = ch;
	buffer[1] = 0;

	UCS2Strcpy(dest, (const S8 *) buffer);
	return strDestination;
}


/**************************************************************

	FUNCTION NAME		: UCS2StrNAppendChar

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 
						  available in destination
**************************************************************/


S8 *UCS2StrNAppendChar( S8 *strDestination, U16 ch, U32 size )
{
	S8  *dest = strDestination;
	U16 buffer[2];
	
	dest = dest + UCS2Strlen(strDestination)* ENCODING_LENGTH;
	buffer[0] = ch;
	buffer[1] = 0;

	UCS2Strncpy(dest, (const S8 *) buffer, size);
	return strDestination;
}



/**************************************************************

	FUNCTION NAME		: AnsiiToUnicodeString

  	PURPOSE				: Converts Ansii encode string to unicode

	INPUT PARAMETERS	: S8*

	OUTPUT PARAMETERS	: S8*

	RETURNS				: U16

 
						  should be as large 	
						  	

**************************************************************/

U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer )
{
	
	S16 count = -1;
	U8 charLen = 0;
	U8  arrOut[2];

	while( *pInBuffer != '\0')
	{
		
		UnicodeToUCS2Encoding(((U16)*pInBuffer)&0x00ff,&charLen,arrOut);


//#ifdef MMI_ON_WIN32
		pOutBuffer[++count] = arrOut[0];
		pOutBuffer[++count] = arrOut[1];
		pInBuffer++;
//#endif

#ifdef __FOR_TESTING //MMI_ON_HARDWARE_P
		pOutBuffer[++count] = arrOut[1]; /*arrOut[0];*/
		pOutBuffer[++count] = arrOut[0]; /* arrOut[1];*/
		pInBuffer++;
#endif

	}

	pOutBuffer[++count] = '\0';
    pOutBuffer[++count] = '\0';
	return count + 1;
}


/**************************************************************

	FUNCTION NAME		: AnsiiNToUnicodeString

  	PURPOSE				: Converts N character Ansii encode string to unicode

	INPUT PARAMETERS	: S8*

	OUTPUT PARAMETERS	: S8*

	RETURNS				: U16

 
						  should be as large 	
						  	

**************************************************************/


U16 AnsiiNToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer, U32 len  )
{
	
	S16 count = -1;
	U8 charLen = 0;
	U8  arrOut[2];

	while(len)
	{
		
		UnicodeToUCS2Encoding(((U16)*pInBuffer)&0x00ff,&charLen,arrOut);


// #ifdef MMI_ON_WIN32
		pOutBuffer[++count] = arrOut[0];
		pOutBuffer[++count] = arrOut[1];
		
		if ( *pInBuffer == '\0')
		{
		   break;
		}
		else
		{
		   pInBuffer++;
		}
//#endif

#ifdef __FOR_TESTING //MMI_ON_HARDWARE_P
		pOutBuffer[++count] = arrOut[1]; /*arrOut[0];*/
		pOutBuffer[++count] = arrOut[0]; /* arrOut[1];*/
		pInBuffer++;
#endif

		len--;

	}

	
	return count + 1;
}

/**************************************************************

	FUNCTION NAME		: UnicodeToAnsii

  	PURPOSE				: Converts Unicode encode string to Ascii

	INPUT PARAMETERS	: S8*

	OUTPUT PARAMETERS	: S8*

	RETURNS				: U16

 
						  should be  large enough	
						  	

**************************************************************/



U16 UnicodeToAnsii(S8 *pOutBuffer, S8 *pInBuffer )
{

	U16 count = 0;
	
	while(  !((*pInBuffer == 0) && (*(pInBuffer + 1) == 0))  )
	{
		*pOutBuffer = *(pInBuffer) ; 

#ifdef __FOR_TESTING //MMI_ON_HARDWARE_P
		*pOutBuffer = *(pInBuffer + 1); 
#endif
		pInBuffer += 2; 
		pOutBuffer++;
		count++;
	}

		*pOutBuffer = 0;
	return count;			
}


/**************************************************************

	FUNCTION NAME		: UnicodeNToAnsii

  	PURPOSE				: Converts N character Unicode encode string to Ascii

	INPUT PARAMETERS	: S8*

	OUTPUT PARAMETERS	: S8*

	RETURNS				: U16

 
						  should be  large enough	
						  	

**************************************************************/

U16 UnicodeNToAnsii(S8 *pOutBuffer, S8 *pInBuffer, U32 len )
{

	U16 count = 0;
	while( (len) && ( !((*pInBuffer == 0) && (*(pInBuffer + 1) == 0)) ))
	{
		*pOutBuffer = *(pInBuffer) ; 

#ifdef __FOR_TESTING //MMI_ON_HARDWARE_P
		*pOutBuffer = *(pInBuffer + 1); 
#endif
		pInBuffer += 2; 
		pOutBuffer++;
		count++;
		len-=2;
	}

		
	return count;			
}




void csd_wsprintf(unsigned short *outstr, char *fmt,...)
{

	U8  CoverBuf[160+1];
	va_list ap;

	MMI_ASSERT( ( outstr != NULL) && (fmt != NULL) );
	
	CoverBuf[0] = 0;
	//sprintf(CoverBuf, fmt);
	va_start(ap, fmt);
	vsprintf((char *)(CoverBuf), fmt, ap);
	va_end(ap);	

	CoverBuf[160 ] = 0;
	AnsiiToUnicodeString((S8*)outstr,(S8*)CoverBuf);


}


WCHAR *kal_wstrcat(WCHAR *s1, const WCHAR *s2)
{
       return 	(WCHAR *)UCS2Strcat((s8*)s1, (s8*)s2);
}

WCHAR *kal_wstrcpy(WCHAR *to, const WCHAR *from)
{
	return (WCHAR *)UCS2Strcpy((s8*)to, (s8*)from);
}
	




/**************************************************************

	FUNCTION NAME		: UCS2Strchr

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 
						  available in destination
**************************************************************/


S8 *UCS2Strchr(  const S8 *strSource , U16 ch ) 
{
	S8 *tps = (S8*)strSource;
	while(*tps !=ch)
	{
		if((tps-strSource)>=UCS2Strlen(strSource)*2)
		{
				mmi_trace(1,"UCS2Strchr,return NULL,len is %d",UCS2Strlen(strSource)*2);

		 return NULL;
		}

		tps+=2;

	}

	tps+=2;
	return (S8 *)tps;


}

/*****************************************************************************
 * FUNCTION
 *  mmi_asc_to_ucs2
 * DESCRIPTION
 *  Converts Ansii encode string to unicode
 *
 *  Caller has to ensure that pOutBuffer
 *  should be as large
 * PARAMETERS
 *  pOutBuffer      [OUT]
 *  pInBuffer       [IN]
 * RETURNS
 *  U16
 *****************************************************************************/
U16 mmi_asc_to_ucs2(S8 *pOutBuffer, S8 *pInBuffer)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* On the Target */
	return (U16)AnsiiToUnicodeString((kal_int8*)pOutBuffer, (kal_int8*)pInBuffer);
}



/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2strlen
 * DESCRIPTION
 *  Gives the length of UCS2 encoded string
 * PARAMETERS
 *  arrOut      [IN]        array containing  UCS2 encoded characters
 * RETURNS
 *  U16 -> Status
 *****************************************************************************/
S32 mmi_ucs2strlen(const S8 *arrOut)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* On the Target */
	return (S32)UCS2Strlen((const kal_int8 *)arrOut);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2ncat
 * DESCRIPTION
 *
 *
 *  User has to ensure that enough space is
 *  available in destination
 * PARAMETERS
 *  strDestination      [OUT]
 *  strSource           [IN]
 *  size                [IN]
 * RETURNS
 *  PS8
 *****************************************************************************/
S8 *mmi_ucs2ncat(S8 *strDestination, const S8 *strSource, U32 size)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* On the Target */
	return (S8*)UCS2Strncat(
			   (kal_int8*)strDestination,
			   (const kal_int8*)strSource,
			   (kal_uint32)size);
}



/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2_n_to_asc
 * DESCRIPTION
 *  Converts N character Unicode encode string to Ascii
 *
 *  Caller has to ensure that pOutBuffer
 *  should be  large enough
 * PARAMETERS
 *  pOutBuffer      [OUT]
 *  pInBuffer       [IN]
 *  len             [IN]
 * RETURNS
 *  U16
 *****************************************************************************/
U16 mmi_ucs2_n_to_asc(S8 *pOutBuffer, S8 *pInBuffer, U32 len)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* On the Target */
	return (U16)UnicodeNToAnsii(
			   (kal_int8*)pOutBuffer,
			   (kal_int8*)pInBuffer,
			   (kal_uint32)len);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_asc_n_to_ucs2
 * DESCRIPTION
 *  Converts N character Ansii encode string to unicode
 *
 *  Caller has to ensure that pOutBuffer
 *  should be as large
 * PARAMETERS
 *  pOutBuffer      [OUT]
 *  pInBuffer       [IN]
 *  len             [IN]
 * RETURNS
 *  U16
 *****************************************************************************/
U16 mmi_asc_n_to_ucs2(S8 *pOutBuffer, S8 *pInBuffer, U32 len)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* On the Target */
	return (U16)AnsiiNToUnicodeString(
			   (kal_int8*)pOutBuffer,
			   (kal_int8*)pInBuffer,
			   (kal_uint32)len);
}
U16 mmi_ucs2_to_asc(S8 *pOutBuffer, S8 *pInBuffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* On the Target */
    return (U16)app_ucs2_str_to_asc_str((kal_int8*)pOutBuffer,(kal_int8*)pInBuffer);
}

