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

/*******************************************************************************
 * Filename:
 * ---------
 *  UnicodeStr.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/MiscFramework/Src/UnicodeStr.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:02:28   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:42:50   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:27:54   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 19:06:36   zhoumn
 * 更新了部分代码、文档
 * 
 *    Rev 1.1   Aug 28 2006 17:20:00   zhangxb
 * 更新了PSI和Simon修改的一些东西.
 * Revision 1.1.1.1  2006/06/08 12:43:17  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


/**************************************************************

	FILENAME	: unicode_str.c

  	PURPOSE		: String and encoding conversion functions for Unicode

 

 

	DATE		: Oct 4,02

**************************************************************/

#include "unicode_strgprot.h"
#include "stdc.h"
#include "declare.h"  // zrx add 20060524



#define ARM_LITTLE_ENDIAN



/**************************************************************

	FUNCTION NAME		: UTF8Strlen

  	PURPOSE				: Give the string length of UTF encoded string

	INPUT PARAMETERS	: pointer to an U8  

	OUTPUT PARAMETERS	: length of string

	RETURNS				: void 

 

**************************************************************/




U16 UTF8Strlen(U8 *p)
{

	U16 no_of_characters = 0;
	while( *p != 0)
	{

		p = p + no_of_bytesPerChar(p);
		no_of_characters++;

	}
	return no_of_characters;
}


/**************************************************************

	FUNCTION NAME		: no_of_bytesPerChar

  	PURPOSE				: No of bytes in the multibyte characters

	INPUT PARAMETERS	: pointer to an U8  

	OUTPUT PARAMETERS	: length of string

	RETURNS				: void 

 

**************************************************************/


U8 no_of_bytesPerChar(U8 *p)
{


	U8 count = 0;
	U8 base = 128;
	U8 no_of_bytes = 1;

	for(count = 0 ; count < 8 ; count++)
	{
		
		if(*p & base)
			++no_of_bytes;
		else 
			break;
		base = base/2;

	}
	return no_of_bytes;
}


/**************************************************************

	FUNCTION NAME		: UTF8Increment

  	PURPOSE				: Increment the pointer to next byte

	INPUT PARAMETERS	: pointer to an pU8  

	OUTPUT PARAMETERS	: none

	RETURNS				: U16 

 

**************************************************************/

U16 UTF8Increment(U8 **p)
{

	U16 no_of_bytes = 0;
	no_of_bytes = no_of_bytesPerChar(*p);
	*p = *p + no_of_bytes;
	return no_of_bytes;
}

/*void main()
{

   
	
	U8 q[] = {224,129,130,131,24,128,129 ,0};
	PU8 a = NULL;
	U8 count;
	U16 bytes = 0;
	U32 unicode;
	bytes = unicodeUTF8Encoding(0x7fffffff,&a);	

	for(count = 0 ; count < bytes ; ++count)
	{
		
		printf("a[count] = %x\n",a[count] );
		
	}


	UTF8toUNICODE(a,&unicode);

	printf("\n no of bytes = %x",unicode);
	printf( "no of characters  in first byte = %d\n",no_of_bytesPerChar(p));
	printf( "no of characters  in string = %d\n",UTF8Strlen(p));


}*/


/**************************************************************

	FUNCTION NAME		: unicodeUTF8Encoding

  	PURPOSE				: convert unicode to UTF8 encoding

	INPUT PARAMETERS	: U64

	OUTPUT PARAMETERS	: array of U8

	RETURNS				: U16 

 

**************************************************************/



U16 unicodeUTF8Encoding(U64 unicode,PU8 *arrout)
{
#if 0  // zrx del 20060524
	U16 bytesNeeded = 0;
	
	if(unicode < 0x80 )
	{
		++bytesNeeded;
		*arrout = (PU8)osl_malloc(sizeof(U8) * bytesNeeded);
		*arrout[0] = (U8)unicode;
		return 1;
		
		
	}

	else if(unicode < 0x800 )
	{
	
		bytesNeeded += 2;
		
	}

	else if(unicode < 0x10000 )
	{
	
		bytesNeeded += 3;
		
	}

	else if(unicode < 0x200000 ) 
	{
		bytesNeeded += 4;
		
	}

	else if(unicode < 0x4000000 )
	{
		bytesNeeded += 5;
		
	}
	else
	{

		bytesNeeded += 6;
	}

#if PLUTO_DEBUG
	if(arrout == NULL)
	{
		printf("\n Memory allocation error\n");
		exit(0);
	}
#endif /* PLUTO DEBUG */

	*arrout = (PU8)osl_malloc(sizeof(char) * bytesNeeded);
	UNICODEToUTF8(bytesNeeded ,*arrout,unicode);
	return bytesNeeded;
	#endif
	return 0;
}







/* Note data has been received in little endian it is also  converted to big 
   endian  during tornformation to UTF8 format  */
void UNICODEToUTF8(U16 bytesNeeded, PU8 arrout , U64 unicode)
{

	U16 temparr[8];
	int	count = 0;
	U16 index = 0;
	U16 temp1 = 0;
	U16 temp2 = 0;
	U16 bytes = 0;
	bytes = bytesNeeded - 1;


#ifdef ARM_LITTLE_ENDIAN

	for(count = bytes , index = 0; count >= 0 ; --count)
	{
		temparr[index] =  *( ( ( PU8 ) &unicode ) + count);
		++index;
	}/* End of for loop */

#endif

#ifdef ARM_BIG_ENDIAN

	for(count = 0 ; count <= bytes; ++count)
	{
		temparr[count] =  *( ( ( pU8 ) &unicode ) + count);
		
	}/* End of for loop */

#endif

	
	for( count = bytes ; count >= 0  ; --count )
	{

		
		/* Generating last bye */
		if( count == bytes )
		{
			temp1 = temparr[count] >> 6;
			arrout[count] = 0;
			arrout[count] = 128;
			temparr[count] = temparr[count] & 63;
			arrout[count] = arrout[count] | temparr[count];
		}
		/* Generating second last byte */
		else if( count == bytes - 1  && bytes  > 1 )
		{

		 	temp2 = temparr[count] >> 4;
			temparr[count] = temparr[count] << 2;
			temparr[count] = temparr[count] | temp1;
			temparr[count] = temparr[count] & 63;
			temp1 = temp2;
			arrout[count] = 0;
			arrout[count] = 128;
			
			arrout[count] =  arrout[count] | temparr[count];

		}

 
		


		/* For last -4 byte */
		else if( count == bytes - 4 && bytes > 4 )
		{

		 	temp2 = temparr[count];
			temparr[count] = 0;
			temparr[count] = ( temp1 & 63);
			temp1 = temp1 >> 6;
			arrout[count] = 0;
			arrout[count] = 128;
			
			arrout[count] =  arrout[count] | temparr[count];

		}


 

		/* For last -3 byte */
		else if( count == bytes - 3 && bytes > 3 )
		{

		 	temp2 = temparr[count];
			temparr[count] = temparr[count] << 4;
			temparr[count] = temparr[count] | temp1;
			temparr[count] = temparr[count] & 63;
			temp1 = temp2;
			arrout[count] = 0;
			arrout[count] = 128;
			
			arrout[count] =  arrout[count] | temparr[count];

		}


		/* For last - 4 byte */
		else if( count == bytes - 2 && bytes > 2)
		//else if( count == bytes - 1 && bytes > 2 )
		{

		 	temp2 = temparr[count] >> 2;
			temparr[count] = temparr[count] << 4;
			temparr[count] = temparr[count] | temp1;
			temparr[count] = temparr[count] & 63;
			temp1 = temp2;
			arrout[count] = 0;
			arrout[count] = 128;
			
			arrout[count] =  arrout[count] | temparr[count];

		}


		/* Condition nedded only for   U-00010000 - U001FFFFF ie 4 byte long UTF8 encoding */
		/*  5th 6th and 7th  bit is zero in this range  shifted two bit to rt and saved
		    2nd 3rd and 4th bit  to temp2 only bit that are not saved in temparr[count] is 0th 
		    and 1st  and data on 2nd ,3rd ,4th is saved on temp2 and 5th is zero so we can safely
		    shift bit 0 and 1 to 4 bits left ,now bit 0 and 1 takes the position 4 and 5th respectively
		    noe or it with temp1 and finally store it in arrout[count] */
		/*else if(count > 1 && bytesNeeded > 3 )
		{

			
			temp2 = temparr[count] >> 4;
			temparr[count] = temparr[count] << 2;
			temparr[count] = temparr[count] | temp1;

			temparr[count] = temparr[count] & 63;
			temp1 = temp2;
			arrout[count] = 0;
			arrout[count] = 128;
			arrout[count] =  arrout[count] | temparr[count];

		}*/
		
		else if ( count == 0 )
		{

			switch(bytesNeeded)
			{
			
			case 1:
				{
					arrout[count] = (U8)unicode;  
					
				}
			case 2:
				{
					temparr[count] = temparr[count] << 2;
					temparr[count] = temparr[count] | temp1;
					arrout[count] = 0;  
					arrout[count] = 0xC0;  
					temparr[count] = temparr[count] & 31;
					arrout[count] = arrout[count] | temparr[count];
					break;
				}
			case 3:
				{
					//temparr[count] = temparr[count] << 2;
					temparr[count] = temparr[count] | temp1;
					arrout[count] = 0;  
					arrout[count] = 0xE0;  
					temparr[count] = temparr[count] & 15;
					arrout[count] = arrout[count] | temparr[count];
					break;
				}
			case 4:
				{
					//temparr[count] = temparr[count] << 2;
					temparr[count] = temparr[count] | temp1;
					arrout[count] = 0;  
					arrout[count] = 0xF0;  
					temparr[count] = temparr[count] & 7;
					arrout[count] = arrout[count] | temparr[count];
					break;
				}

			case 5:
				{
					
					
					//temparr[count] = temparr[count] << 2;
					temparr[count] = temparr[count] | temp1;
					arrout[count] = 0;  
					arrout[count] = 0xF8;  
					temparr[count] = temparr[count] & 3;
					arrout[count] = arrout[count] | temparr[count];
					break;
				}

			case 6:
				{
					
					
				//	temparr[count] = temparr[count] << 2;
					temparr[count] = temparr[count] | temp1;
					arrout[count] = 0;  
					arrout[count] = 0xFC;  
					temparr[count] = temparr[count] & 3;
					arrout[count] = arrout[count] | temparr[count];
					break;
				}


			}/* End of switch*/

		}/* End of if- els if loop*/


	}/* End of for loop */

	for(count = 0 ; count <= bytes ; ++count)
	{
		
		/*printf("arrout[count] = %x\n",arrout[count] );*/
		
	}/* End of for loop */
}/* End of function */


/**************************************************************

	FUNCTION NAME		: UTF8toUNICODE

  	PURPOSE				: From UTF8 encoding to UNICODE

	INPUT PARAMETERS	: arr of U8

	OUTPUT PARAMETERS	: pointr to U32

	RETURNS				: U8

 

**************************************************************/


U8 UTF8toUNICODE(U8 UTF8[] , PU32 ptr_unicode)
{

	U8 no_of_bytes; /* Variable to keep no of bytes to be read */ 
	U8 test_bit;	/* Variable used to test bit is set to one or not*/
	U8 UTF8_byte;	/* Variable holding the byte of UTF8 encoding*/
	U8 loopCount;	/* Loop counter */
	U32 extracted_bits; /*Variable to hold extrected bits from UTF8 byte*/
	U32 temp_unicode; /*Variable for temporarly created unicode */
	U8 byteCount;

	temp_unicode = 0;
	no_of_bytes = 0;
	loopCount = 0;
	byteCount = 0;
	UTF8_byte = UTF8[0];
	test_bit = 1;

	if(UTF8[0] <= 127)
	{

		*ptr_unicode = UTF8[0];
		return SUCCESS;

	}

	test_bit = test_bit << 7;
	for(loopCount = 0; loopCount < 8 ; ++loopCount)
	{
		if( test_bit & UTF8_byte )
		{
			no_of_bytes++;
			test_bit = test_bit >> 1;
		}
		else
			break;
	}

	/* Check for validity of given UTF8 encoding scheme */
	if(no_of_bytes > 6)
	{
		return FAILURE;
	}

	

	switch(no_of_bytes)
	{

	case 6:

		{
			extracted_bits = 0;
			extracted_bits = UTF8[0] & 1;
			extracted_bits = extracted_bits << 30;
			temp_unicode = temp_unicode | extracted_bits;
			break;
		}
	case 5:

		{
			extracted_bits = 0;
			extracted_bits = UTF8[0] & 3;
			extracted_bits = extracted_bits << 24;
			temp_unicode = temp_unicode | extracted_bits;
			break;
		}

	case 4:

		{
			extracted_bits = 0;
			extracted_bits = UTF8[0] & 7;
			extracted_bits = extracted_bits << 18;
			temp_unicode = temp_unicode | extracted_bits;
			break;
		}
	case 3:

		{
			extracted_bits = 0;
			extracted_bits = UTF8[0] & 15;
			extracted_bits = extracted_bits << 12;
			temp_unicode = temp_unicode | extracted_bits;
			break;
		}
	case 2:

		{
			extracted_bits = 0;
  			extracted_bits = UTF8[0] & 31;
			extracted_bits = extracted_bits << 6;
			temp_unicode = temp_unicode | extracted_bits;
			break;
		}
	
	

	default:
		    break;

	}/* End of switch */

		for(loopCount =  1, byteCount = no_of_bytes - 2; loopCount <  no_of_bytes; loopCount++, byteCount--)
		{
			
			extracted_bits = 0;
			extracted_bits = UTF8[loopCount] &  63;
			extracted_bits = extracted_bits << (6 * (byteCount));
			temp_unicode = temp_unicode | extracted_bits;
		}

		/*	extracted_bits = UTF8[1] & 63;
			extracted_bits = extracted_bits << 12;
			temp_unicode = temp_unicode | extracted_bits;

			extracted_bits = 0;
			extracted_bits = UTF8[2] & 63;
			extracted_bits = extracted_bits << 6;
			temp_unicode = temp_unicode | extracted_bits;

			extracted_bits = 0;
			extracted_bits = UTF8[3] & 63;
			//extracted_bits = extracted_bits << 6;
			temp_unicode = temp_unicode | extracted_bits;*/
			*ptr_unicode = temp_unicode;
  return SUCCESS;
}/*end of function UTF8toUNICODE*/


/**************************************************************

	FUNCTION NAME		: UTF8strcpy

  	PURPOSE				: copy one string to other

	INPUT PARAMETERS	: pointr to S8

	OUTPUT PARAMETERS	: pointer to S8

	RETURNS				: U8

 

**************************************************************/



S8 *UTF8strcpy( S8 *strDestination, const S8 *strSource )
{
	
	return strcpy( strDestination,strSource );

}



/**************************************************************

	FUNCTION NAME		: UTF8strcmp

  	PURPOSE				: compare two strings

	INPUT PARAMETERS	: pointr to S8

	OUTPUT PARAMETERS	: pointer to S8

	RETURNS				: U32

 

**************************************************************/



S32 UTF8strcmp( const S8 *string1, const S8 *string2 )
{

	return strcmp(string1,string2 );

}


/**************************************************************

	FUNCTION NAME		: UTF8strncmp

  	PURPOSE				: compare n bytes of  strings

	INPUT PARAMETERS	: pointr to S8

	OUTPUT PARAMETERS	: pointer to S8

	RETURNS				: U32

 

**************************************************************/


S32 UTF8strncmp( const S8 *string1, const S8 *string2, U16 count )
{

 	return strncmp(string1, string2, count);
}

	
/*
U16 unicodeUTF8Encoding(U64  unicode ,unsigned char arrout[] )
{

	U16 bytesNeeded = 0;
	unsigned count = 0;
	U16 byte = 0;
	unsigned char temparr[8];
	if(unicode < 0x80 )
		++bytesNeeded;

	else if(unicode < 0x800 )
		bytesNeeded += 2;

	else if(unicode < 0x10000 )
		bytesNeeded += 3;

	else 
		bytesNeeded += 4;


	arrout = (char*)malloc(sizeof(char) * bytesNeeded);

#if PLUTO_DEBUG
	if(arrout == NULL)
	{
		printf("\n Memory allocation error\n");
		exit(0);
	}
#endif 

	if(unicode < 0x80 )
		arrout[0] = (char)unicode;

	else if(unicode < 0x800 )
	{
		 
		unsigned char temp = 0;

#ifdef BIG_ENDIAN

		temparr[0] =  *( ( char* ) &unicode );
		temparr[1] =  *( ( ( char* ) &unicode ) + 1);

		printf(" temparr[0] = %x\n",temparr[0]);
		printf(" temparr[1] = %x\n",temparr[1]);
		
		
		temp = temparr[0] & 192;
		temp = temp >> 6;
		arrout[0] = 0;
		arrout[0] = 128;
		temparr[0] = temparr[0] & 63;
		arrout[0] = arrout[0] | temparr[0];


		temparr[1] = temparr[1] << 2;
		temparr[1] = temparr[1] | temp;
		arrout[1] = 0;  
		arrout[1] = 0xC0;  
		temparr[1] = temparr[1] & 31;
		arrout[1] = arrout[1] | temparr[1];
				
		printf(" arrout[0] = %x\n",arrout[0]);
		printf(" arrout[1] = %x\n",arrout[1]);

#else  

		temparr[1] =  *( ( unsigned char* ) &unicode );
		temparr[0] =  *( ( ( unsigned char* ) &unicode ) + 1);

		printf(" temparr[0] = %x\n",temparr[0]);
		printf(" temparr[1] = %x\n",temparr[1]);
		
	
		temp = temparr[1] >> 6;
		arrout[1] = 0;
		arrout[1] = 128;
		temparr[1] = temparr[1] & 63;
		arrout[1] = arrout[1] | temparr[1];


		temparr[0] = temparr[0] << 2;
		temparr[0] = temparr[0] | temp;
		arrout[0] = 0;  
		arrout[0] = 0xC0;  
		temparr[0] = temparr[0] & 31;
		arrout[0] = arrout[0] | temparr[0];
				
		printf(" arrout[0] = %x\n",arrout[0]);
		printf(" arrout[1] = %x\n",arrout[1]);
#endif
	}

    return bytesNeeded;
}
*/

