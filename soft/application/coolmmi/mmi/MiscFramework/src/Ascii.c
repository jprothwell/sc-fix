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
 *  Ascii.c
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
/**********************************************************************************
	Filename:		Ascii.c
 
	Date Created:	Feb 5-2003
	Contains:		
**********************************************************************************/


#include "mmi_data_types.h"
#include "asciiprot.h"
#include "string.h"
#include "unicodexdcl.h"



/**************************************************************

	FUNCTION NAME		: AsciiStrlen

  	PURPOSE				: Gives the length of Ascii encoded string

	INPUT PARAMETERS	: PS8 -> array containing  Ascii encoded characters

	OUTPUT PARAMETERS	: none

	RETURNS				: U16 -> Status

 

**************************************************************/


S32 AsciiStrlen(const S8 *arrOut)
{

	S32 count = 0;
		
	

	while(*arrOut)
	{
		++count;
		++arrOut;
	}

	return count;

}


/**************************************************************

	FUNCTION NAME		: AsciiStrcpy

  	PURPOSE				: 

	INPUT PARAMETERS	: PS8 -> array containing  Ascii encoded characters

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 

**************************************************************/


PS8 AsciiStrcpy(S8 *strDestination, const S8 *strSource )
{

		//CSD added by JL 040130 for copy error issue
		while ( !( (*strDestination==0)  && (*strSource==0) ) )
		{
			++strDestination;
			++strSource; 
		}

		strDestination = '\0';
		return strDestination;


}



/**************************************************************

	FUNCTION NAME		: AsciiStrcmp

  	PURPOSE				: Compares the ascii encoded strings

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: S16 -> Status

 

**************************************************************/

S32 AsciiStrcmp( const S8 *string1, const S8 *string2 )
{
	
	/*while( (*string1) && (*string2) )
	{
		if((*string1 - *string2) != 0)
		{
			break;
		}
		else
		{
			string1++;
			string2++;

		}

	}
	return 	(*string1 - *string2);*/



	while( *string1 == *string2 )
	{
		if(*string1 == '\0')
		{
			return 0;
		}
		
			string1++;
			string2++;

		

	}
	return 	(*string1 - *string2);
	
}


/**************************************************************

	FUNCTION NAME		: AsciiStrncmp

  	PURPOSE				: Compares the ascii encoded strings of specified length

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: S16 -> Status

 

**************************************************************/

S32 AsciiStrncmp( const S8 *string1, const S8 *string2, U32 size )
{
 
 U32 count = 0;
 
	while(count< size)
	{
		
		if(string1[count] != string2[count])
		{
			return string1[count]- string2[count];
		}
		++count;
	}
	return 0;
}
 
 



/**************************************************************

	FUNCTION NAME		: AsciiStrncpy

  	PURPOSE				: 

	INPUT PARAMETERS	: PS8 -> array containing  Ascii encoded characters

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 

**************************************************************/


PS8 AsciiStrncpy(S8 *strDestination, const S8 *strSource , U32 size )
{

		U32 count = 0;
		
		while( (*strDestination  = *strSource ) && ( count < size ) )
		{
			++strDestination;
			++strSource; 
			++count;
		}

		strDestination = '\0';
		return strDestination;


}



/**************************************************************

	FUNCTION NAME		: AsciiStrcat

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: none

	RETURNS				: PS8

 

**************************************************************/


PS8 AsciiStrcat( S8 *strDestination, const S8 *strSource )
{

	S8  *dest = strDestination;

	
	dest = dest + AsciiStrlen(strDestination);
	AsciiStrcpy(dest,strSource);
	return strDestination;


}





/* Dummy funtion to be removed later on */

char *AsciiStrtok( char *strToken, const char *strDelimit )
{
	return "";
}


