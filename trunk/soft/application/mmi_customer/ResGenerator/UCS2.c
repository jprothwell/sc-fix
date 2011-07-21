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
 

#include "cs_types.h"
#include "mmi_data_types.h"
#include "ucs2dcl.h"
#include "ucs2prot.h"
#include "unicodexdcl.h"
#include "string.h"
#include "debuginitdef.h"



/**************************************************************

	FUNCTION NAME		: UnicodeToUCS2Encoding

  	PURPOSE				: convert unicode to UCS2 encoding

	INPUT PARAMETERS	: U16 -> unicode value to be encoded 

	OUTPUT PARAMETERS	: array of U8 

	RETURNS				: U8 -> Status 

	REMARKS				: 

**************************************************************/


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

	REMARKS				: 

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



/**************************************************************

	FUNCTION NAME		: UCS2Strlen

  	PURPOSE				: Gives the length of UCS2 encoded string

	INPUT PARAMETERS	: PS8 -> array containing  UCS2 encoded characters

	OUTPUT PARAMETERS	: none

	RETURNS				: U16 -> Status

	REMARKS				: 

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

	REMARKS				: 

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

	REMARKS				: 

**************************************************************/

S32 UCS2Strcmp( const S8 *string1, const S8 *string2 )
{

	
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

	REMARKS				: In size pass no of characters not bytes

**************************************************************/

// CSD Added by Tim for solve a potential wrong answer when string1 and string2 are the same and less than "size"
S32 UCS2Strncmp( const S8 *string1, const S8 *string2 , U32 size )
{
	U32 count = 0;
	U16 nStr1;
	U16 nStr2;
	size = size << 1; /* User is passing no of charcters not bytes*/  

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
// End CSD: Tim
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

	REMARKS				: In size pass no of characters not bytes

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

	REMARKS				: User has to ensure that enough space is 
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

	REMARKS				: User has to ensure that enough space is 
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

	REMARKS				: User has to ensure that enough space is 
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

	REMARKS				: User has to ensure that enough space is 
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

	REMARKS				: Caller has to ensure that pOutBuffer
						  should be as large 	
						  	

**************************************************************/

U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer )
{
	
	S16 count = -1;
	U8 charLen = 0;
	U8  arrOut[2];

	while( *pInBuffer != '\0')
	{
		
		UnicodeToUCS2Encoding((U16)*pInBuffer,&charLen,arrOut);


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

	REMARKS				: Caller has to ensure that pOutBuffer
						  should be as large 	
						  	

**************************************************************/


U16 AnsiiNToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer, U32 len  )
{
	
	S16 count = -1;
	U8 charLen = 0;
	U8  arrOut[2];

	while(len)
	{
		
		UnicodeToUCS2Encoding((U16)*pInBuffer,&charLen,arrOut);


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

	REMARKS				: Caller has to ensure that pOutBuffer
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

	REMARKS				: Caller has to ensure that pOutBuffer
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










