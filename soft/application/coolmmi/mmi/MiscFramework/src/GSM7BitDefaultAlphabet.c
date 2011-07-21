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
 *  GSM7BitDefaultAlphabet.c
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
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: GSM7BitDefaultAlphabet.c

  	PURPOSE		: Converts GSM 7-bit Default alphabet to ascii and
					Ascii to GSM 7-bit Default alphabet

 

	AUTHOR		: .

	DATE		: 11-07-03

**************************************************************/

#include "stdc.h"
#include "mmi_data_types.h"
#include "gsm7bitdefaultalphabet.h"
#include "unicodexdcl.h"

//micha1018
#include "wgui_categories.h"


#define SWAP_SHORT(Var)  *Var = *(short*)         SwapEndian((void*)Var, sizeof(short))
#define SWAP_USHORT(Var) *Var = *(unsigned short*)SwapEndian((void*)Var, sizeof(short))
#define SWAP_LONG(Var)   *Var = *(long*)          SwapEndian((void*)Var, sizeof(long))
#define SWAP_ULONG(Var)  *Var = *(unsigned long*) SwapEndian((void*)Var, sizeof(long))
#define SWAP_FLOAT(Var)  *Var = *(float*)         SwapEndian((void*)Var, sizeof(float))
#define SWAP_DOUBLE(Var) *Var = *(double*)        SwapEndian((void*)Var, sizeof(double))


#define UI_GET_NEXT_BYTE(p,c)				\
{	c=(*p);									\
	p+=1;									\
}

#define UI_GET_PREVIOUS_BYTE(p,c)			\
{	p-=1;									\
	c=(*p);									\
}

#define UI_INSERT_BYTE(p,c)					\
{	(*((p))++)=(byte)(c);					\
}

#define UI_SWAP_U16_BYTES(c) (c)=((c)>>8)|((c)<<8)

#define _EURO_CHARACTER_UCS2		(0x20AC)
#define _EURO_CHARACTER				(0xA2)
#define _ESCAPE_CHARACTER			(27)
#define _SPACE_CHARACTER			(32)

extern const U8 AsciiToDefaultArray[];
extern const U8 ExtendedAsciiToDefaultArray[];
extern const U8 DefaultToExtendedAsciiArray[];
extern const U8 DefaultToAsciiArray[];


/* used later

const U8 DefaultToAsciiArray[128] =
{
 64,163, 36, 165,232,233,249,236,242,199,
 10,216,248, 13,197,229, 16, 95, 18, 19,
 20, 21, 22, 23, 24, 25, 26, 27,198,230,
223,201, 32, 33, 34, 35,164, 37, 38, 39,
 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
 60, 61, 62, 63,161, 65, 66, 67, 68, 69, //173 by kevin
 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
 90,196,214,209,220,167,191, 97, 98, 99,
100,101,102,103,104,105,106,107,108,109,
110,111,112,113,114,115,116,117,118,119,
120,121,122,228,246,241,252,224
};

const U8 DefaultToExtendedAsciiArray[128]=
{
 0,0,0,0,0,0,0,0,0,0,
 32,0,0,0,0,0,0,0,0,0,  // Page Break at 10=32 Change Later
 94,0,0,0,0,0,0,27,0,0,
 0,0,0,0,0,0,0,0,0,0,
 123,125,0,0,0,0,0,92,0,0,
 0,0,0,0,0,0,0,0,0,0,
 91,126,93,0,124,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,
 0,101,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0
};

const U8 AsciiToDefaultArray[] =
{
32,32,32,32,32,32,32,32,32,32,
10,32,32,13,32,32,16,32,18,19,
20,21,22,23,24,25,26,27,32,32,
32,32,32,33,34,35,2,37,38,39,
40,41,42,43,44,45,46,47,48,49,
50,51,52,53,54,55,56,57,58,59,
60,61,62,63,0,65,66,67,68,69,
70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,
90,32,32,32,32,17,32,97,98,99,
100,101,102,103,104,105,106,107,108,109,
110,111,112,113,114,115,116,117,118,119,
120,121,122,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,1,36,3,32,95,32,32,
32,32,32,64,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,96,32,32,32,32,91,14,28,9,
32,31,32,32,32,32,32,32,32,93,
32,32,32,32,92,32,11,32,32,32,
94,32,32,30,127,32,32,32,123,15,
29,32,4,5,32,32,7,32,32,32,
32,125,8,32,32,32,124,32,12,6,
32,32,126,32,32,32
};

const U8 ExtendedAsciiToDefaultArray[] =
{
0,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,27,32,32,
32,32,10,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,60,47,62,20,32,32,32,32,32,
32,101,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,40,64,41,61,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32
};
 
 */

const U8 DefaultToAsciiArray[128] =		
{
 64,163, 36, 165,232,233,249,236,242,199,
 10,216,248, 13,197,229, 16, 95, 18, 19,
 20, 21, 22, 23, 24, 25, 26, 27,198,230,
223,200, 32, 33, 34, 35,164, 37, 38, 39,
 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
 60, 61, 62, 63, 161, 65, 66, 67, 68, 69,
 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
 90,196,214,209,220,167,191, 97, 98, 99,
100,101,102,103,104,105,106,107,108,109,
110,111,112,113,114,115,116,117,118,119,
120,121,122,228,246,241,252,224
};

const U8 DefaultToExtendedAsciiArray[128]=
{
 0,0,0,0,0,0,0,0,0,0,
 32,0,0,0,0,0,0,0,0,0,  /* Page Break at 10=32 Change Later*/
 94,0,0,0,0,0,0,32,0,0,
 0,0,0,0,0,0,0,0,0,0,
 123,125,0,0,0,0,0,92,0,0,
 0,0,0,0,0,0,0,0,0,0,
 91,126,93,0,124,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,
 0,162,0,0,0,0,0,0,0,0,	/*	Euro character at 101 changed to 162 */
 0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0
};

const U8 AsciiToDefaultArray[] =
{
32,32,32,32,32,32,32,32,32,32,
10,32,32,13,32,32,16,32,18,19,
20,21,22,23,24,25,26,27,32,32,
32,32,32,33,34,35,2,37,38,39,
40,41,42,43,44,45,46,47,48,49,
50,51,52,53,54,55,56,57,58,59,
60,61,62,63,0,65,66,67,68,69,
70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,
90,32,32,32,32,17,32,97,98,99,
100,101,102,103,104,105,106,107,108,109,
110,111,112,113,114,115,116,117,118,119,
120,121,122,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,64,32,1,36,3,32,95,32,32,
32,32,32,64,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,96,32,32,32,32,91,14,28,9,
31,32,32,32,32,32,32,32,32,93,
32,32,32,32,92,32,11,32,32,32,
94,32,32,30,127,32,32,32,123,15,
29,32,4,5,32,32,7,32,32,32,
32,125,8,32,32,32,124,32,12,6,
32,32,126,32,32,32
};

const U8 ExtendedAsciiToDefaultArray[] =
{
0,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,27,32,32,
32,32,10,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,60,47,62,20,32,32,32,32,32,
32,101,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,40,64,41,61,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,101,32,32,32,32,32,32,32,	/*	Changed character at 162 to 101 (old value was 32)	*/
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32
};



/**************************************************************

	FUNCTION NAME		: ConvertDefaultEncodingToAscii

  	PURPOSE				: Converts 7 bit encoding to 8 bit Ascii

	INPUT PARAMETERS	: message: Output Buffer
						  msg:	Input Buffer
						  length: No of bytes to be converted

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

/*void ConvertDefaultEncodingToAscii (U8* message, U8* msg, U16 length, U16 *outLen)
{
   U16 i,j;
   for(i=0,j=0; i<length; i++, j++)
   {
      if(msg[i]!=27) 
		  message[j] = DefaultToAsciiArray[msg[i]];
	  else
	  {
		  i++;
		  if(msg[i]!=27) message[j] = DefaultToExtendedAsciiArray[msg[i]];
	  	  else
		  {
			  message[j] = 32;
			  i++;
		  }
	  }
	}
   if(outLen!=NULL) *outLen=j;
}*/

void ConvertGSM7BitDefaultEncodingToAscii (U8* message, U8* msg, U16 length, U16 *outLen)
{
   U16 i,j=0;
   for(i=0; i<length; i++)
   {
      if(msg[i]!=27)	//escape char ?
      {
         // if(msg[i]!=13)	//CR ?
            message[j++] = DefaultToAsciiArray[msg[i]];
      }
      else
      {
         //treat escape char as a space
         message[j++] = 32;
      }
   }
   if(outLen!=NULL) *outLen=j;
}

/**************************************************************

	FUNCTION NAME		: GSM7BitMapDefaultToASCIIByte

  	PURPOSE				: Converts 7 bit encoding to 8 bit Ascii

	INPUT PARAMETERS	: U8 input (GSM 7 bit encoded value)

	OUTPUT PARAMETERS	: (none)

	RETURNS				: U8 (ASCII value)

 

**************************************************************/
U8 GSM7BitMapDefaultToASCIIByte (U8 input)
{
	if (input == 27) return 32;         // return space for "Escape"
	if (input == 13) return 13;         // Returning same value, but UI should skip CR
	return DefaultToAsciiArray[input];  // use the table for the rest
}

/**************************************************************

	FUNCTION NAME		: GSM7BitMapASCIIToDefaultByte

  	PURPOSE				: Converts  8 bit Ascii to 7 bit encoding

	INPUT PARAMETERS	: U8 input (ASCII value) 

	OUTPUT PARAMETERS	: (none)

	RETURNS				: U8 (GSM 7 bit encoded value)

 

**************************************************************/
U8 GSM7BitMapASCIIToDefaultByte (U8 input)
{
	return AsciiToDefaultArray[input] ;
}

/**************************************************************

	FUNCTION NAME		: ConvertAsciiEncodingToDefault

  	PURPOSE				: Converts  8 bit Ascii to 7 bit encoding

	INPUT PARAMETERS	: message: Output Buffer
						  msg:	Input Buffer
						  length: No of bytes to be converted

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

/*void ConvertAsciiEncodingToDefault (U8* message, U8* msg, U16 length, U16 *outLen)
{
   U16 i,j,temp;
   for(i=0,j=0; i<length; i++,j++)
   {
      if(!msg[i]) break;
	  message[j] = AsciiToDefaultArray[msg[i]];
	  if(message[j]==32) 
	  {
		 temp=ExtendedAsciiToDefaultArray[msg[i]];
		 if(temp!=32) 
		 {
			 message[j++]=27;
			 message[j]=(U8)temp;
		 }
	  }
   }
   if(outLen!=NULL) *outLen=j;
}
*/

void ConvertAsciiEncodingToGSM7BitDefault (U8* message, U8* msg, U16 length, U16 *outLen)
{
   U16 i,j; 
   for(i=0,j=0; i<length; i++,j++)
   {
      message[j] = AsciiToDefaultArray[msg[i]];
   }
   if(outLen!=NULL) *outLen=j;
}

#ifndef OSL_MEMORY_DUMP
/**************************************************************

	FUNCTION NAME		: SATCovertStringForSIM

  	PURPOSE				: Converts data from Platfrorm (ASCII or UCS2) format to one specified

	INPUT PARAMETERS	: data: Pointer to dat to be converted
						  len: No of bytes to be converted
						  format: format to which data is to be converted 7 bit. 8 bit or UCS2

	OUTPUT PARAMETERS	: outlen: Pointer in which output length will be returned

	RETURNS				: Pointer to data containing converted data

 

**************************************************************/

U8 *CovertStringForSIM(U8 *data, U16 len, U8 format, U16 *outLen)
{
	U8 *convertedData = NULL;
	U16 outLength;
	if (len > 0)
    {
		switch (format)
		{
			case MMI_DEFAULT_DCS:
			#ifdef __ASCII
				convertedData = OslMalloc((len*2)+1);
				ConvertAsciiEncodingToGSM7BitDefault(convertedData,data,len,&outLength);
				convertedData[outLength]=0;
				if(outLen!=NULL) *outLen=outLength+1;
			#endif

			#ifdef __UCS2_ENCODING
			{
				U8 *tempData;
				convertedData = OslMalloc((len)+1);
				tempData=OslMalloc(len);
				UnicodeNToAnsii((PS8)tempData,(PS8)data,len);
				ConvertAsciiEncodingToGSM7BitDefault(convertedData,tempData,(U16)(len/ENCODING_LENGTH), &outLength);
				OslMfree(tempData);
				convertedData[outLength]=0;
				if(outLen!=NULL) *outLen=outLength+1;
			}
			#endif
			break;

			case MMI_8BIT_DCS:
			#ifdef __ASCII
				convertedData = OslMalloc((len)+1);
				memcpy(convertedData,data,len);
				memset((void *)&convertedData[len*ENCODING_LENGTH],0,1);
				if(outLen!=NULL) *outLen=len+1;
			#endif
			#ifdef __UCS2_ENCODING
				convertedData = OslMalloc((len/ENCODING_LENGTH)+1);
				UnicodeNToAnsii((PS8)convertedData,(PS8)data,len);
				memset((void *)&convertedData[len/ENCODING_LENGTH],0,1);
				if(outLen!=NULL) *outLen=len/ENCODING_LENGTH+1;
			#endif
			break;

			case MMI_UCS2_DCS:
			#ifdef __ASCII
					convertedData = OslMalloc((len*2)+2);
					AnsiiNToUnicodeString((PS8)convertedData,(PS8)data,len);
					memset((void *)&convertedData[len*2],0,2);
					if(outLen!=NULL) *outLen=len*2+2;
			#endif
			#ifdef __UCS2_ENCODING
					convertedData = OslMalloc((len)+ENCODING_LENGTH);
					memcpy(convertedData,data,len);
					memset((void *)&convertedData[len],0,ENCODING_LENGTH);
					if(outLen!=NULL) *outLen=len+ENCODING_LENGTH;
			#endif
			#ifdef MMI_ON_HARDWARE_P
			{
				U16 *tempData;
				int i;
				tempData=(U16 *)convertedData;
				for(i=0;i<len*2/ENCODING_LENGTH;i+=2)
				{
					SWAP_USHORT(tempData);
					tempData++;
				}
			}
			#endif
			break;
		}

		return convertedData;
	}
	else
	{
		U8 noOfNulls;
		if(format==MMI_UCS2_DCS) noOfNulls=2;
		else noOfNulls=1;
		convertedData = OslMalloc(noOfNulls);
		memset((void *)convertedData,0,noOfNulls);
		if(outLen!=NULL) *outLen=noOfNulls;
	 	return convertedData;
	}
}

/**************************************************************

	FUNCTION NAME		: SATCovertStringForPlatform

  	PURPOSE				: Converts data from specified format to Platform type (UCS2 or ASCII)

	INPUT PARAMETERS	: data: Pointer to data to be converted
						  len: No of bytes to be converted
						  format: format of input data

	OUTPUT PARAMETERS	: outlen: Pointer in which output length will be returned

	RETURNS				: Pointer to data containing converted data

 

**************************************************************/

U8 *CovertStringForPlatform(U8 *data, U16 len, U8 format, U16 *outLen)
{
//CSD JL added for init
	U8 *convertedData=NULL;
//CSD end
	U16 outLength;
	if (len > 0)
    {
	  switch (format)
      {
      	 case (MMI_DEFAULT_DCS):
			#ifdef __UCS2_ENCODING
			 {
				U8 *tempData;
				convertedData = OslMalloc((len*ENCODING_LENGTH)+ENCODING_LENGTH);
				tempData=OslMalloc(len);
				ConvertGSM7BitDefaultEncodingToAscii(tempData,data,len,&outLength);
				AnsiiNToUnicodeString((PS8)convertedData,(PS8)tempData,outLength);
				memset((void *)&convertedData[outLength*ENCODING_LENGTH],0,ENCODING_LENGTH);
				OslMfree(tempData);
			 }
			#endif
			#ifdef __ASCII
				convertedData = OslMalloc((len)+ENCODING_LENGTH);
				ConvertGSM7BitDefaultEncodingToAscii(convertedData,data,len, &outLength);
				memset((void *)&convertedData[outLength],0,ENCODING_LENGTH);
			#endif
			if(outLen!=NULL) *outLen=outLength*ENCODING_LENGTH+ENCODING_LENGTH;
		 break;

		 case (MMI_8BIT_DCS):
            #ifdef __UCS2_ENCODING
				convertedData = OslMalloc((len*ENCODING_LENGTH)+ENCODING_LENGTH);
				AnsiiNToUnicodeString((PS8)convertedData,(PS8)data,len);
				memset((void *)&convertedData[len*ENCODING_LENGTH],0,ENCODING_LENGTH);
			#endif
			#ifdef __ASCII
				convertedData = OslMalloc(len+ENCODING_LENGTH);
				memcpy(convertedData,data,len);
				memset((void *)&convertedData[len*ENCODING_LENGTH],0,ENCODING_LENGTH);
			#endif
			if(outLen!=NULL) *outLen=len*ENCODING_LENGTH+ENCODING_LENGTH;
        break;

		case (MMI_UCS2_DCS):
			
//micha1217, Solve #873, Henry China FT issue begin
#if (0)
			#ifdef __UCS2_ENCODING
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
			#endif
			#ifdef __ASCII
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
			#endif
#endif
			#ifdef __UCS2_ENCODING
				convertedData = OslMalloc(len+1+ENCODING_LENGTH);		//prevent odd len number
				memset((void *)convertedData,0,len+1+ENCODING_LENGTH);			
				memcpy(convertedData,data,len);
				if(outLen!=NULL) *outLen=len+ENCODING_LENGTH;
			#endif
			#ifdef __ASCII
				convertedData = OslMalloc((len/ENCODING_LENGTH)+1+ENCODING_LENGTH);
				memset((void *)convertedData,0,(len/ENCODING_LENGTH)+1+ENCODING_LENGTH);						
				UnicodeNToAnsii((PS8)convertedData,(PS8)data,len);
				if(outLen!=NULL) *outLen=len/ENCODING_LENGTH+ENCODING_LENGTH;
			#endif
//micha1217, Solve Henry China FT issue end

			#ifdef MMI_ON_HARDWARE_P
			{
				U16 *tempData;
				int i;
//micha0902, #124
				tempData=(U16 *)convertedData;
				for(i=0;i<len;i+=2)
				{
					SWAP_USHORT(tempData);
					tempData++;
				}
			}
			#endif
		break;

//micha1008 begin
		case (MMI_UCS2_81):
				convertedData = OslMalloc((len+1)*2+ENCODING_LENGTH);		//prevent odd len number
				memset((void *)convertedData,0,(len+1)*2+ENCODING_LENGTH);			

				InputboxConvertGSMToDeviceEncoding( data, 
																   convertedData, 
																   (U8)len, 
																   (len+1)*2+ENCODING_LENGTH,
																   MMI_UCS2_81, 
																   0, 
																   1);
		break;


		case (MMI_UCS2_82):
				convertedData = OslMalloc((len+1)*2+ENCODING_LENGTH);		//prevent odd len number
				memset((void *)convertedData,0,(len+1)*2+ENCODING_LENGTH);			

				InputboxConvertGSMToDeviceEncoding( data, 
																   convertedData, 
																   (U8)len, 
																   (len+1)*2+ENCODING_LENGTH,
																   MMI_UCS2_82, 
																   0, 
																   1);
		break;
//micha1008 end

      }
	  return convertedData;
	}
	else
	{
	 	convertedData = OslMalloc(ENCODING_LENGTH);
		memset((void *)convertedData,0,ENCODING_LENGTH);
		if(outLen!=NULL) *outLen=ENCODING_LENGTH;
	 	return convertedData;
	}
}


/**************************************************************

	FUNCTION NAME		: SATCovertStringForPlatform

  	PURPOSE				: Converts data from specified format to Platform type (UCS2 or ASCII)

	INPUT PARAMETERS	: data: Pointer to data to be converted
						  len: No of bytes to be converted
						  format: format of input data

	OUTPUT PARAMETERS	: outlen: Pointer in which output length will be returned

	RETURNS				: Pointer to data containing converted data

 

**************************************************************/
#define SPACE_CODE (0x20)
U8 *CovertStringForGetSMS(U8 *data, U16 len, U8 format, U16 *outLen)
{
	U8 *convertedData=NULL;

	U16 outLength;
	if (len > 0)
    {
	  switch (format)
      {
      	 case (MMI_DEFAULT_DCS):
		{
			convertedData = OslMalloc((len)+ENCODING_LENGTH);
			ConvertGSM7BitDefaultEncodingToAscii(convertedData,data,len, &outLength);
			memset((void *)&convertedData[outLength],0,ENCODING_LENGTH);
			if(outLen!=NULL)
			{
				int i;
				*outLen=outLength;
				for(i = 0; i < len; i ++)
				{
					if(*(convertedData + i) == 0)
					{
						*(convertedData + i) = SPACE_CODE;
					}
				}
			}
			break;
		}
		 case (MMI_8BIT_DCS):
		{
			/*Guojian Add Start For 10999  Ver: GreenStone  on 2008-12-23 17:26 */
			if(len % 2 != 0)
			{
				len += 1;  ////prevent odd len number
			}
			/*Guojian Add End  For 10999  Ver: GreenStone  on 2008-12-23 17:26 */
			convertedData = OslMalloc(len+ENCODING_LENGTH);
			memcpy(convertedData,data,len);
			memset((void *)&convertedData[len],0,ENCODING_LENGTH);

			if(outLen!=NULL)
			{
				int i;
				*outLen=len;
				for(i = 0; i < len; i ++)
				{
					if(*(convertedData + i) == 0)
					{
						*(convertedData + i) = SPACE_CODE;
					}
				}
				
			}
			break;
		}

		case (MMI_UCS2_DCS):
			convertedData = OslMalloc(len+ENCODING_LENGTH);		
			memset((void *)convertedData,0,len+ENCODING_LENGTH);			
			memcpy(convertedData,data,len);
			//if(outLen!=NULL) *outLen=len;
			if(outLen!=NULL) 
			{
				int i;
				*outLen=len;
				for(i = 0; i < len; i += 2)
				{
					if(*(convertedData + i) == 0 && *(convertedData + i + 1) == 0)
					{
						*(convertedData + i + 1) = SPACE_CODE;
					}
				}
			}

			#ifdef MMI_ON_HARDWARE_P
			{
				U16 *tempData;
				int i;
				tempData=(U16 *)convertedData;
				for(i=0;i<len;i+=2)
				{
					SWAP_USHORT(tempData);
					tempData++;
				}
			}
			#endif
		break;


      }
	  return convertedData;
	}
	else
	{
	 	convertedData = NULL;
		if(outLen!=NULL) *outLen=0;
	 	return convertedData;
	}
}

/**************************************************************

	FUNCTION NAME		: SATCovertStringForSIM

  	PURPOSE				: Converts data from Platfrorm (ASCII or UCS2) format to one specified

	INPUT PARAMETERS	: data: Pointer to dat to be converted
						  len: No of bytes to be converted
						  format: format to which data is to be converted 7 bit. 8 bit or UCS2

	OUTPUT PARAMETERS	: outlen: Pointer in which output length will be returned

	RETURNS				: Pointer to data containing converted data

 

**************************************************************/

U8 *CovertStringForSetSMS(U8 *data, U16 len, U8 format, U16 *outLen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *convertedData = NULL;
    U16 outLength;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (len > 0)
    {
        switch (format)
        {
            case MMI_DEFAULT_DCS:
                {

                    convertedData = OslMalloc((len) + 1);
                    ConvertAsciiEncodingToGSM7BitDefault(
                        convertedData,
                        data,
                        len ,
                        &outLength);
                    convertedData[outLength] = 0;
                    if (outLen != NULL)
                    {
                        *outLen = outLength + 1;
                    }
                }
                break;


            case MMI_UCS2_DCS:
                convertedData = OslMalloc((len) + ENCODING_LENGTH);
                memcpy(convertedData, data, len);
                memset((void*)&convertedData[len], 0, ENCODING_LENGTH);
                if (outLen != NULL)
                {
                    *outLen = len + ENCODING_LENGTH;
                }
            #ifdef MMI_ON_HARDWARE_P
                {
                    U16 *tempData;
                    int i;

                    tempData = (U16*) convertedData;
                    for (i = 0; i < len * 2 / ENCODING_LENGTH; i += 2)
                    {
                        SWAP_USHORT(tempData);
                        tempData++;
                    }
                }
            #endif /* MMI_ON_HARDWARE_P */ 
                break;
        }

        return convertedData;
    }
    else
    {
        U8 noOfNulls;

        if (format == MMI_UCS2_DCS)
        {
            noOfNulls = 2;
        }
        else
        {
            noOfNulls = 1;
        }
        convertedData = OslMalloc(noOfNulls);
        memset((void*)convertedData, 0, noOfNulls);
        if (outLen != NULL)
        {
            *outLen = noOfNulls;
        }
        return convertedData;
    }
}

#else
/**************************************************************

	FUNCTION NAME		: SATCovertStringForSIM

  	PURPOSE				: Converts data from Platfrorm (ASCII or UCS2) format to one specified

	INPUT PARAMETERS	: data: Pointer to dat to be converted
						  len: No of bytes to be converted
						  format: format to which data is to be converted 7 bit. 8 bit or UCS2

	OUTPUT PARAMETERS	: outlen: Pointer in which output length will be returned

	RETURNS				: Pointer to data containing converted data

 

**************************************************************/

U8 *CovertStringForSIMInt(U8 *data, U16 len, U8 format, U16 *outLen, S8 *fileName, S32 lineNumber)
{
	U8 *convertedData;
	U16 outLength;
	if (len > 0)
    {
		switch (format)
		{
			case MMI_DEFAULT_DCS:
			#ifdef __ASCII
				convertedData = OslMallocCHK((len*2)+1);
				ConvertAsciiEncodingToGSM7BitDefault(convertedData,data,len,&outLength);
				convertedData[outLength]=0;
				if(outLen!=NULL) *outLen=outLength+1;
			#endif

			#ifdef __UCS2_ENCODING
			{
				U8 *tempData;
				convertedData = OslMallocCHK((len)+1,fileName,lineNumber);
				tempData=OslMallocCHK(len,fileName,lineNumber);
				UnicodeNToAnsii((PS8)tempData,(PS8)data,len);
				ConvertAsciiEncodingToGSM7BitDefault(convertedData,tempData,(U16)(len/ENCODING_LENGTH), &outLength);
				OslMfree(tempData);
				convertedData[outLength]=0;
				if(outLen!=NULL) *outLen=outLength+1;
			}
			#endif
			break;

			case MMI_8BIT_DCS:
			#ifdef __ASCII
				convertedData = OslMallocCHK((len)+1);
				memcpy(convertedData,data,len);
				memset((void *)&convertedData[len*ENCODING_LENGTH],0,1);
				if(outLen!=NULL) *outLen=len+1;
			#endif
			#ifdef __UCS2_ENCODING
				convertedData = OslMallocCHK((len/ENCODING_LENGTH)+1,fileName,lineNumber);
				UnicodeNToAnsii((PS8)convertedData,(PS8)data,len);
				memset((void *)&convertedData[len/ENCODING_LENGTH],0,1);
				if(outLen!=NULL) *outLen=len/ENCODING_LENGTH+1;
			#endif
			break;

			case MMI_UCS2_DCS:
			#ifdef __ASCII
					convertedData = OslMallocCHK((len*2)+2);
					AnsiiNToUnicodeString((PS8)convertedData,(PS8)data,len);
					memset((void *)&convertedData[len*2],0,2);
					if(outLen!=NULL) *outLen=len*2+2;
			#endif
			#ifdef __UCS2_ENCODING
					convertedData = OslMallocCHK((len)+ENCODING_LENGTH,fileName,lineNumber);
					memcpy(convertedData,data,len);
					memset((void *)&convertedData[len],0,ENCODING_LENGTH);
					if(outLen!=NULL) *outLen=len+ENCODING_LENGTH;
			#endif
			#ifdef MMI_ON_HARDWARE_P
			{
				U16 *tempData;
				int i;
				tempData=(U16 *)convertedData;
				for(i=0;i<len*2/ENCODING_LENGTH;i+=2)
				{
					SWAP_USHORT(tempData);
					tempData++;
				}
			}
			#endif
			break;
		}

		return convertedData;
	}
	else
	{
		U8 noOfNulls;
		if(format==MMI_UCS2_DCS) noOfNulls=2;
		else noOfNulls=1;
		convertedData = OslMallocCHK(noOfNulls,fileName,lineNumber);
		memset((void *)convertedData,0,noOfNulls);
		if(outLen!=NULL) *outLen=noOfNulls;
	 	return convertedData;
	}
}

/**************************************************************

	FUNCTION NAME		: SATCovertStringForPlatform

  	PURPOSE				: Converts data from specified format to Platform type (UCS2 or ASCII)

	INPUT PARAMETERS	: data: Pointer to data to be converted
						  len: No of bytes to be converted
						  format: format of input data

	OUTPUT PARAMETERS	: outlen: Pointer in which output length will be returned

	RETURNS				: Pointer to data containing converted data

 

**************************************************************/

U8 *CovertStringForPlatformInt(U8 *data, U16 len, U8 format, U16 *outLen,S8 *fileName,S32 lineNumber)
{
	U8 *convertedData;
	U16 outLength;
	if (len > 0)
    {
	  switch (format)
      {
      	 case (MMI_DEFAULT_DCS):
			#ifdef __UCS2_ENCODING
			 {
				U8 *tempData;
				convertedData = OslMallocCHK((len*ENCODING_LENGTH)+ENCODING_LENGTH,fileName,lineNumber);
				tempData=OslMallocCHK(len,fileName,lineNumber);
				ConvertGSM7BitDefaultEncodingToAscii(tempData,data,len,&outLength);
				AnsiiNToUnicodeString((PS8)convertedData,(PS8)tempData,outLength);
				memset((void *)&convertedData[outLength*ENCODING_LENGTH],0,ENCODING_LENGTH);
				OslMfree(tempData);
			 }
			#endif
			#ifdef __ASCII
				convertedData = OslMallocCHK((len)+ENCODING_LENGTH);
				ConvertGSM7BitDefaultEncodingToAscii(convertedData,data,len, &outLength);
				memset((void *)&convertedData[outLength],0,ENCODING_LENGTH);
			#endif
			if(outLen!=NULL) *outLen=outLength*ENCODING_LENGTH+ENCODING_LENGTH;
		 break;

		 case (MMI_8BIT_DCS):
            #ifdef __UCS2_ENCODING
				convertedData = OslMallocCHK((len*ENCODING_LENGTH)+ENCODING_LENGTH,fileName,lineNumber);
				AnsiiNToUnicodeString((PS8)convertedData,(PS8)data,len);
				memset((void *)&convertedData[len*ENCODING_LENGTH],0,ENCODING_LENGTH);
			#endif
			#ifdef __ASCII
				convertedData = OslMallocCHK(len+ENCODING_LENGTH);
				memcpy(convertedData,data,len);
				memset((void *)&convertedData[len*ENCODING_LENGTH],0,ENCODING_LENGTH);
			#endif
			if(outLen!=NULL) *outLen=len*ENCODING_LENGTH+ENCODING_LENGTH;
        break;

		case (MMI_UCS2_DCS):
			#ifdef __UCS2_ENCODING
				convertedData = OslMallocCHK(len+ENCODING_LENGTH,fileName,lineNumber);
				memcpy(convertedData,data,len);
				memset((void *)&convertedData[len],0,ENCODING_LENGTH);
				if(outLen!=NULL) *outLen=len+ENCODING_LENGTH;
			#endif
			#ifdef __ASCII
				convertedData = OslMallocCHK((len/ENCODING_LENGTH)+ENCODING_LENGTH);
				UnicodeNToAnsii((PS8)convertedData,(PS8)data,len);
				memset((void *)&convertedData[len/ENCODING_LENGTH],0,ENCODING_LENGTH);
				if(outLen!=NULL) *outLen=len/ENCODING_LENGTH+ENCODING_LENGTH;
			#endif
			#ifdef MMI_ON_HARDWARE_P
			{
				U16 *tempData;
				int i;
//micha0902, #124
				tempData=(U16 *)convertedData;
				for(i=0;i<len;i+=2)
				{
					SWAP_USHORT(tempData);
					tempData++;
				}
			}
			#endif

		break;

      }
	  return convertedData;
	}
	else
	{
	 	convertedData = OslMallocCHK(ENCODING_LENGTH,fileName,lineNumber);
		memset((void *)convertedData,0,ENCODING_LENGTH);
		if(outLen!=NULL) *outLen=ENCODING_LENGTH;
	 	return convertedData;
	}
}
#endif

/**************************************************************

	FUNCTION NAME		: SATConvertBCDToString

  	PURPOSE				: Converts  BCD Digits to Character String

	INPUT PARAMETERS	: message: Output Buffer
						  msg:	Input Buffer


	OUTPUT PARAMETERS	: outlen: Pointer to variable which will have output length
							of Character String  will be returned

	RETURNS				: Character String of BCD Characters

 

**************************************************************/

U8 *ConvertBCDToString(U8 *BCDNumber, U16 len, U16 *outLen)
{
	U8 *outString;
	U8 i = 0, j = 0;
	outString=OslMalloc(2*len+1);

//micha1104, #211
	if (BCDNumber[0] == 0x91)
	{
		outString[j++] = '+';
		i++;
	}
//micha1104, #211
	for(; i < len; i++)
	{
		outString[j++]=ConvertDigitToChar((U8)(BCDNumber[i] & 0x0F));
		outString[j]=ConvertDigitToChar((U8)((BCDNumber[i]>>4) & 0x0F));
		if(outString[j++]==0)
		{
			if(outLen) *outLen=j;
			return outString;
		}
	}
	outString[j++]=0;
	if(outLen) *outLen=j;
	return outString;
}

/**************************************************************

	FUNCTION NAME		: SATConvertDigitToChar

  	PURPOSE				: Converts  BCD Digit to Char

	INPUT PARAMETERS	: digit: BCD Digit


	OUTPUT PARAMETERS	: nil

	RETURNS				: Character format of BCD Digit

 

**************************************************************/

U8 ConvertDigitToChar(U8 digit)
{
 
	if(digit<10)
		return 48+digit;
	if(digit==15)
		return 0;
	if(digit==10)
		return 42; /* '*' */
	if(digit==11)
		return 35; /* '#' */
	if(digit==12)
		return 43; /* '+' */
	if(digit>=13)
		return 53+digit;
	return 0;
}

/**************************************************************

	FUNCTION NAME		: SATSwapEndian

  	PURPOSE				: Converts  Little Endian to Big Endian & vice versa

	INPUT PARAMETERS	: *Addr: Pointer to data to be swapped
						  Nb: No. of bytes to be swapped

	OUTPUT PARAMETERS	: nil

	RETURNS				: Pointer to Swapped Digits

 

**************************************************************/

void *SwapEndian(void* Addr, const int Nb)
{
//micha1014 begin	
	static char Swapped[16];
	static U32 Result_align[4];
	int i;
	
	switch (Nb) {
		case 2:	Swapped[0]=*((char*)Addr+1);
				Swapped[1]=*((char*)Addr  );
				break;
		case 4:	Swapped[0]=*((char*)Addr+3);
				Swapped[1]=*((char*)Addr+2);
				Swapped[2]=*((char*)Addr+1);
				Swapped[3]=*((char*)Addr  );
				break;
		case 8:	Swapped[0]=*((char*)Addr+7);
				Swapped[1]=*((char*)Addr+6);
				Swapped[2]=*((char*)Addr+5);
				Swapped[3]=*((char*)Addr+4);
				Swapped[4]=*((char*)Addr+3);
				Swapped[5]=*((char*)Addr+2);
				Swapped[6]=*((char*)Addr+1);
				Swapped[7]=*((char*)Addr  );
				break;
		case 16:Swapped[0]=*((char*)Addr+15);
				Swapped[1]=*((char*)Addr+14);
				Swapped[2]=*((char*)Addr+13);
				Swapped[3]=*((char*)Addr+12);
				Swapped[4]=*((char*)Addr+11);
				Swapped[5]=*((char*)Addr+10);
				Swapped[6]=*((char*)Addr+9);
				Swapped[7]=*((char*)Addr+8);
				Swapped[8]=*((char*)Addr+7);
				Swapped[9]=*((char*)Addr+6);
				Swapped[10]=*((char*)Addr+5);
				Swapped[11]=*((char*)Addr+4);
				Swapped[12]=*((char*)Addr+3);
				Swapped[13]=*((char*)Addr+2);
				Swapped[14]=*((char*)Addr+1);
				Swapped[15]=*((char*)Addr  );
				break;
	}
	
	for ( i = 0; i < 16; i++)
		*((char*)Result_align +i) = Swapped[i];
		
	return (void*)Result_align;
//micha1014 end	
}


/*	flag=1: Handle extended characters. flag=0: Ignore extended characters	*/
/*	UCS2_count=0 for Default DCS, non zero for UCS2 DCS						*/
S32 InputboxConvertGSMToDeviceEncoding(U8 *inBuffer,U8 *outBuffer, U8 inBufferLength,S32 outBufferSize,S32 UCS2_count,S32 GSM_ext_count,byte flag)
{	byte				done=0;
	UI_buffer_type		p1=(UI_buffer_type)inBuffer;
	UI_buffer_type		p2=(UI_buffer_type)outBuffer;
	UI_character_type	c,cc;
	S32					out_count=0;
	U8					in_count=0;

	if(flag)
	{
		if(UCS2_count==0)	/*ASCII*/
		{
			while(!done)
			{
				UI_GET_NEXT_BYTE(p1,c);
				in_count++;
				if(UI_STRING_END_OF_STRING_CHARACTER(c) && (in_count > inBufferLength)) /*Also check in_conut to make sure all chracter converted*/
				{
					done=1;
					UI_STRING_INSERT_CHARACTER(p2,c);	/*Insert NULL terminator*/
					out_count+=ENCODING_LENGTH;
				}
				else if(UI_TEST_ESC_CHARACTER(c))
				{
					cc=c;
					UI_GET_NEXT_BYTE(p1,c);
					in_count++;
					c=DefaultToExtendedAsciiArray[c];
					if(UI_TEST_GSM_EXTENDED(c))
					{	
						UI_STRING_INSERT_CHARACTER(p2,c);
						out_count+=ENCODING_LENGTH;
					}
					else
					{
						UI_GET_PREVIOUS_BYTE(p1,c);
						in_count--;
						c=DefaultToAsciiArray[cc];
						UI_STRING_INSERT_CHARACTER(p2,c);
						out_count+=ENCODING_LENGTH;
					}
				}
				else
				{
					c=DefaultToAsciiArray[c];
					UI_STRING_INSERT_CHARACTER(p2,c);
					out_count+=ENCODING_LENGTH;
				}
				if(out_count >= (outBufferSize -2)) /*To ensure null terminator at the end when exceeds buffer.*/
				{
					done=1;
					UI_STRING_INSERT_CHARACTER(p2,0);
					out_count+=ENCODING_LENGTH;
				}
			}
		}
		else /*MMI_PHB_UCS2*/
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 and 0x82 UCS2*/
		if(UCS2_count == 0x08)	
#endif
		{
			while(!done)
			{
				UI_STRING_GET_NEXT_CHARACTER(p1,c);
				UI_SWAP_U16_BYTES(c);
				if(UI_STRING_END_OF_STRING_CHARACTER(c)) 
					done=1;
				//if(c==_EURO_CHARACTER_UCS2) 
				//	c=_EURO_CHARACTER;
				UI_STRING_INSERT_CHARACTER(p2,c); /*Append character here, including null terminator*/
				out_count+=ENCODING_LENGTH;

				if(out_count>=(outBufferSize-2))	/*To ensure null terminator at the end when exceeds buffer.*/
				{
					done=1;
					UI_STRING_INSERT_CHARACTER(p2,0);
					out_count+=ENCODING_LENGTH;
				}
			}
		}
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 and 0x82 UCS2*/
		else if(UCS2_count == 0x09)	/*MMI_PHB_UCS2_81*/
		{	
			U16 base_ptr;
			U8 i,length;
			
			UI_GET_NEXT_BYTE(p1,c);
			UI_GET_NEXT_BYTE(p1,c);
			length = (U8)c;
			base_ptr = (*p1) << 7;
			UI_GET_NEXT_BYTE(p1,c);

			for(i=0 ; i < length; i++)
			{
				if(out_count>= (outBufferSize-2))
					break;
			
				UI_GET_NEXT_BYTE(p1,c);

				if (c < 0x80)	/*Default GSM character*/
				{
					*(p2)++ = (U8)c;
					*(p2)++ = 0x0;
				}
				else	/*UCS2 character*/
				{
					c &= 0x7F;
					c += base_ptr;

					*(p2)++ = c & 0xFF;
					*(p2)++ = c >> 8 & 0xFF;
				}
			}
			
			UI_STRING_INSERT_CHARACTER(p2,0); /*Add null terminator at the end*/
			out_count+=ENCODING_LENGTH;
		}
		else if(UCS2_count == 0x10)	/*MMI_PHB_UCS2_82*/
		{
			U16 base_ptr;
			U8 i,length;
			
			UI_GET_NEXT_BYTE(p1,c);
			UI_GET_NEXT_BYTE(p1,c);
			length = (U8)c;
			base_ptr = (*p1) << 8;
			UI_GET_NEXT_BYTE(p1,c);
			base_ptr |= (*p1);
			UI_GET_NEXT_BYTE(p1,c);
			
			for(i=0 ; i < length; i++)
			{
				if(out_count>= (outBufferSize-2))
					break;
					
				UI_GET_NEXT_BYTE(p1,c);

				if (c < 0x80)	/*Default GSM character*/
				{
					*(p2)++ = (U8)c;
					*(p2)++ = 0x0;
				}
				else	/*UCS2 character*/
				{
					c &= 0x7F;
					c += base_ptr;

					*(p2)++ = c & 0xFF;
					*(p2)++ = c >> 8 & 0xFF;
				}
			
				out_count+=ENCODING_LENGTH;
			}

			UI_STRING_INSERT_CHARACTER(p2,0); /*Add null terminator at the end*/
			out_count+=ENCODING_LENGTH;
		}		
#endif
	}
#if 0	/*This Part is not used currently, Always handle extended characters*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	return out_count;
}

/*	flag=1: Handle extended characters. flag=0: Ignore extended characters	*/
/*	UCS2_count=0 for Default DCS, non zero for UCS2 DCS						*/
S32 InputboxConvertDeviceEncodingToGSM(U8 *inBuffer,U8 *outBuffer,S32 outBufferSize,S32 UCS2_count,S32 GSM_ext_count,byte flag)
{	byte				done=0;
	UI_buffer_type		p1=(UI_buffer_type)inBuffer;
	UI_buffer_type		p2=(UI_buffer_type)outBuffer;
	UI_character_type	c;
	S32					out_count=0;

	if(flag)
	{	if(UCS2_count==0)
		{	while(!done)
			{	UI_STRING_GET_NEXT_CHARACTER(p1,c);
				if(UI_STRING_END_OF_STRING_CHARACTER(c))
				{	done=1;
					UI_INSERT_BYTE(p2,c);
					out_count+=1;
				}
				else if(UI_TEST_GSM_EXTENDED(c))
				{	UI_INSERT_BYTE(p2,_ESCAPE_CHARACTER);
					c=ExtendedAsciiToDefaultArray[c];
					UI_INSERT_BYTE(p2,c);
					out_count+=2; //CSD 2003-12-29
				}
				else
				{	c=AsciiToDefaultArray[c];
					UI_INSERT_BYTE(p2,c);
					out_count+=1;
				}
				if(out_count>=outBufferSize) done=1;
			}
		}
		else
		{	while(!done)
			{	UI_STRING_GET_NEXT_CHARACTER(p1,c);
				if(UI_STRING_END_OF_STRING_CHARACTER(c)) done=1;
				//if(c==_EURO_CHARACTER) c=_EURO_CHARACTER_UCS2;
				UI_SWAP_U16_BYTES(c);
				UI_STRING_INSERT_CHARACTER(p2,c);
				out_count+=ENCODING_LENGTH;
				if(out_count>=outBufferSize) done=1;
			}
		}
	}
	else
	{	if(UCS2_count==0)
		{	while(!done)
			{	UI_STRING_GET_NEXT_CHARACTER(p1,c);
				if(UI_STRING_END_OF_STRING_CHARACTER(c))
				{	done=1;
					UI_INSERT_BYTE(p2,c);
				}
				else
				{	c=AsciiToDefaultArray[c];
					UI_INSERT_BYTE(p2,c);
				}
				out_count+=1;
				if(out_count>=outBufferSize) done=1;
			}
		}
		else
		{	while(!done)
			{	UI_STRING_GET_NEXT_CHARACTER(p1,c);
				if(UI_STRING_END_OF_STRING_CHARACTER(c)) done=1;
				//if(c==_EURO_CHARACTER) c=_EURO_CHARACTER_UCS2;
				UI_SWAP_U16_BYTES(c);
				UI_STRING_INSERT_CHARACTER(p2,c);
				out_count+=ENCODING_LENGTH;
				if(out_count>=outBufferSize) done=1;
			}
		}
	}
	return(out_count);
}


