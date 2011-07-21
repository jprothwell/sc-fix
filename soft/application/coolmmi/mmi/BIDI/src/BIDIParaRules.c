/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 *    bidipararules.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   bidirectional algorithm engine 
 *
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 *****************************************************************************/

/**************************************************************

	FILENAME	: BIDIParaRules.c

 

	 

 

	DATE		: Aug-23-2004

**************************************************************/
#include "bididef.h"
#include "bidiprot.h"

 
extern U8 input_types[];//source array for storing the direction codes of the input string
extern U8 input_levels[];//This global contains all the embedding levels
extern U8 dest_levels[];//destitnation array for processed levels
extern U8 dest_types[];//Destination array for storing the direction codes of the input string
 
 
#ifdef __BIDI_CONSOLE__
#include "commonprots.h"
#else
#include "unicodexdcl.h"
#include "debuginitdef.h"
#endif
 
/*********************************************************************
   This function will take the unicode character array as input,
   outputs the directional code array and returns the string len.
********************************************************************/
U16 get_direction_codes(U8* inputTypes,U8* inputString,S16 sLen)
{
    S16 iCount = 0 ;
 
	U16 charInput=0;

	pfnEncodingSchemeToUnicode((U16*)&charInput,(U8 *)(&inputString[iCount*BIDI_ENCODING_LENGTH]));
 

 
	if (sLen)
 
//	while( inputString[iCount*BIDI_ENCODING_LENGTH] !=0 && iCount<sLen )
	while( charInput !=0 && iCount<sLen )
 
	{
 	  bidi_get_char_type(&inputString[iCount*BIDI_ENCODING_LENGTH],&inputTypes[iCount]);
      iCount++;
 
		pfnEncodingSchemeToUnicode((U16*)&charInput,(U8 *)(&inputString[iCount*BIDI_ENCODING_LENGTH]));
 
	}
	else
 
 
//	while( inputString[iCount*BIDI_ENCODING_LENGTH] !=0)
	while( charInput !=0)
 
	{
 	  bidi_get_char_type(&inputString[iCount*BIDI_ENCODING_LENGTH],&inputTypes[iCount]);
      iCount++;
 
		pfnEncodingSchemeToUnicode((U16*)&charInput,(U8 *)(&inputString[iCount*BIDI_ENCODING_LENGTH]));
 
	}
	return iCount;
}
/************************************************************************
This routine will take the given stream of charcaters as input and break
them in to diffrent paragraphs by taking only the text between paragraph 
seperator directional code 'B'.it returns the revised character count.
*************************************************************************/
U16 identify_paragraphs(U8* inputTypes,U16 inputLen,pBOOL* bFlag)
{
	S16 iCount ;

 
	//Loop till you get paragraph seperator
	for(iCount = 0;((iCount < inputLen)&&(inputTypes[iCount]!=BIDI_B));iCount++ ) ;
	if (iCount < inputLen && inputTypes[iCount] == BIDI_B)  
	{
		*bFlag=TRUE;
		 
		if (inputTypes[iCount+1] == BIDI_B)
			iCount++;
		 
		//inputTypes[iCount++] = 0;
	}
 
 
    return iCount;
}

/***********************************************************************
This function looks for the first token of any strong type
ie AL , BIDI_L and BIDI_R , then sets the parahraph embedding level depending
upon the strong type. paragraph embedding level is 0  by default ,0
if first strong type is BIDI_L and 1 if first strong type is AL or BIDI_R.
*************************************************************************/
U8 get_para_embedding_level(U8* inputTypes,U16 inputLen)
{

    S16 strongCharType = 0xffff; // This will store the first strong character in the 
	                         //given input string
	S16 iCount;
	for(iCount = 0; iCount < inputLen; ++iCount) 
	{
        U8 token = inputTypes[iCount];
        if (token == AL || token == BIDI_L || token == BIDI_R)
		{
			strongCharType = token;
			break;
		}
    }
    //return the embedding level depending upon the strong character Type

	switch (strongCharType)
    {
	case -1:
	case BIDI_L : 
	default:
		return 0;
		break;
	case AL:
	case BIDI_R:
		return 1;
		break;
	}
}

//Returns the least odd level corresponding to the given input level
U8 getLeastOddEmbeddingLevel( U8 pEmbeddinglevel)
{
	return ODD(pEmbeddinglevel) ? pEmbeddinglevel + 2 : pEmbeddinglevel + 1;
}


//Returns the least odd level corresponding to the given input level
U8 getLeastEvenEmbeddingLevel(U8 pEmbeddinglevel)
{
   return ODD(pEmbeddinglevel) ? pEmbeddinglevel + 1 : pEmbeddinglevel + 2;
}

/* This function is to determine all explicit embedding levels and directions.
   if any directional type of RLE ,LRE,LRO or RLO appears */
 
//Get all explicit embedding level
 
//void resolve_explicit_embedding_levels(U8* inputTypes,U16 inputLen,U8 paraEmbeddingLevel,U8* all_embeddings_level)
//void resolve_explicit_embedding_levels(U16 inputLen,U8 paraEmbeddingLevel/*,U8* all_embeddings_level*/)
 
void resolve_explicit_embedding_levels(U8* inputTypes,U16 inputLen,U8 paraEmbeddingLevel)//,U8* all_embeddings_level)
 
{

	//all the local variables used for stack operations
	//keeps track of stack full/empty status 
	 U16  stackFullIndicator     = 0;

	//This is to keep track for the invalid embedding level in the case 
	//when RLE ,LRE , RLO or LRO happens at level 60
	 U16  stackAboutToFullIndicator = 0;

	//used as an index to the embedding levels stack 
	 U16  stackIndex   = 0 ;

	 //For storing the updated value of embedding level
	 U8  newEmbeddingLevel ;
    
	 //This array is used to store all the embedding levels encountered
   	 U8 pushedEmbeddingValues[MAX_LEVEL_EMBEDDING] ;

	 U8 token;
 	 U8 presentEmbeddingLevel  =  paraEmbeddingLevel;
	 U8 presentEmbeddingValue =  paraEmbeddingLevel;
     S16 iCount = 0; //Loop Counter 

     while (iCount < inputLen )
	 {
         input_levels[iCount] = presentEmbeddingValue ;
 
		  
		 token = inputTypes[iCount];
		 //token = input_types[iCount];
		  
 
		 //process if RLE ,LRE ,RLO ,LRO are there
		if( token == RLE || token == LRE || token == LRO || token == RLO )
		{
			//proceed only when current level is valid
			if( stackFullIndicator == 0)
			{
				if ( token == RLE || token == RLO )
				{
					//Get the least odd embedding level
					newEmbeddingLevel = getLeastOddEmbeddingLevel(presentEmbeddingLevel);
				}
				else
				{
                    //Get the least even embedding level
					newEmbeddingLevel = getLeastEvenEmbeddingLevel(presentEmbeddingLevel);
				}

				/*
				If this updated embedding level does not cross the boundry of
                MAX_LEVEL_EMBEDDING remember this embedding level by pushing in to the stack
				*/
				if( newEmbeddingLevel < MAX_LEVEL_EMBEDDING )
				{
					pushedEmbeddingValues[stackIndex] = newEmbeddingLevel;
					stackIndex++ ;
					presentEmbeddingLevel = newEmbeddingLevel;

					if( token == LRO || token == RLO )
					{
					  presentEmbeddingValue = (presentEmbeddingLevel | 0x80) ;
					}
					else
					{
                      presentEmbeddingValue = presentEmbeddingLevel;
					}

					//Now push this embedding in to the stack
					pushedEmbeddingValues[stackIndex] = presentEmbeddingValue;
				}
				
				if( presentEmbeddingValue == MAX_LEVEL_EMBEDDING - 1)
				{
					stackAboutToFullIndicator++;
				}
			}//if (stackFullIndicator == 0)
            stackFullIndicator++; 
		}//if RLE ,LRE , LRO , RLO
        else
		{
			if( token == PDF )
			{
                /*
				This is just like an end brace '}' and we have to find start brace'{' 
				corresponding to each PDF stack is already full , decrement it 
                since we are going to pop one embedding level
				*/
                if((stackAboutToFullIndicator > 0)&&(presentEmbeddingLevel !=MAX_LEVEL_EMBEDDING ))
                    stackAboutToFullIndicator--;
				else
					stackIndex-- ;

                //pop up the embedding value from the stack
				presentEmbeddingValue  = pushedEmbeddingValues[stackIndex];
				//get the overriding status
				presentEmbeddingLevel  = presentEmbeddingValue & 0x7f;
			}//if( token == PDF )
			else
			{
                if(token==BIDI_B)
				{
					//Reset all the variables as 'BIDI_B' is the last token processed
					presentEmbeddingLevel  =  paraEmbeddingLevel;
					presentEmbeddingValue =  paraEmbeddingLevel;
					stackIndex = 0;
					stackAboutToFullIndicator = 0;
					pushedEmbeddingValues[iCount] = paraEmbeddingLevel;
				}//if(token==BIDI_B)				
			}//if( token == PDF )
		}
		iCount++ ; 
	 }//End of while loop
}

void set_dest_levels(U16 nSCount,U16 nECount,U8 paraEmbedLevel,U8* pDestLevels)
{
//	while (startCount<=endCount)
//		dest_levels[startCount++] = paraEmbedLevel;
	while (nSCount<nECount)
		pDestLevels[nSCount++] = paraEmbedLevel;
}

void copy_direction_codes(U8* dTypes,U8* sTypes,U16 nCount)
{
	S16 iCount;
	for(iCount=0;iCount<nCount;iCount++) dTypes[iCount]=sTypes[iCount];
}

