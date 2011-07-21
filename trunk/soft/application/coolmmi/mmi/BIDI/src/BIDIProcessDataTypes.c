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
 *    bidiprocessdatatypes.c
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

	FILENAME	: BIDIProcessDataTypes.c

 

	 

 

	DATE		: Aug-23-2004

**************************************************************/
#include "bididef.h"
#include "bidiprot.h"
#include "cswtype.h"

 
extern U8 input_types[];//source array for storing the direction codes of the input string
extern U8 input_levels[];//This global contains all the embedding levels
extern U8 dest_levels[];//destitnation array for processed levels
extern U8 dest_types[];//Destination array for storing the direction codes of the input string
 

/****************************************************************************
Returns BIDI_L or BIDI_R depends on the embedding level & take embeddings levels as parametre
Returns BIDI_R if level is odd
Returns BIDI_L if level is Even		
*****************************************************************************/
#define TOKEN_FOR_LEVEL(x) ((x & 1)? BIDI_R:BIDI_L)

/**************************************************************
Rule:X9
Removes the explicit codes & calculates length of the remaining text & assigns it to textLength.
it modifies the global data structure textLength
It also removes any BN(ParaGraph) delimiter
******************************************************************/
U16 delete_explicit_direction_codes(U8* inputTypes,U16 inputLen,U8* all_embeddings_level,U8* destLevels,U8* destTypes)
{
	S16 iCount=0,jCount=0;

	while(iCount <inputLen)
	{
		U16 code;
 
		 
		code=inputTypes[iCount];
		//code=input_types[iCount];
		 
		if(!((code==RLE)||(code==LRE)||(code==RLO)||(code==LRO)||(code==PDF)||(code==BN)))
		{//checking for the presence of RLE,RLO,LRE,LRO,PDF,BN
//			dest_levels[jCount]=dest_levels[iCount];
			destLevels[jCount]=destLevels[iCount];
			all_embeddings_level[jCount]=all_embeddings_level[iCount];
//			dest_types[jCount]=dest_types[iCount];
			destTypes[jCount]=destTypes[iCount];
 
			jCount++;
		}
		iCount++;
	}
	return jCount;//Returns length of the textLength when RLE,RLO,PDF,LRE,BN,LRE are removed
}
/*********************End of Rule X9*****************************************/

/********************************************************************************
Rule:X10
Calculates the sor(start of run),eor(end of the run) & number of the run levels.
it works with one level at a time. All w1-w7 rules are coded here.
**********************************************************************************/
void  get_run_level(U16 inputLen,U8 paragraphEmbeddingLevel,U8* destLevels,U8* destTypes)
{
	U8 lastLevel,calLevel,currentLevel;
	U8 calType,lastType;
	U16 begin=0,range;

	lastLevel=paragraphEmbeddingLevel;//At start we put to equal paraGraphembeddingLevel
	while(begin<inputLen)//checking for length 
	{
 
		//currentLevel=dest_levels[begin];
		currentLevel=destLevels[begin];
		lastType=TOKEN_FOR_LEVEL(GETMAX(currentLevel,lastLevel));//calculation of sor
		range=begin+1;//number of characters at the same level
	//	for(;begin<inputLen && dest_levels[range]==currentLevel;)//calculate tokens having at same level
//		for(;range<inputLen && dest_levels[range]==currentLevel;)//calculate tokens having at same level
		for(;range<inputLen && destLevels[range]==currentLevel;)//calculate tokens having at same level
		   range++;

 
		if(range < inputLen)
//			calLevel=dest_levels[range];
			calLevel=destLevels[range];
		else
			calLevel=paragraphEmbeddingLevel;
		calType=TOKEN_FOR_LEVEL(GETMAX(calLevel,currentLevel));//calcution of eor
//		processWeakTokens(begin,range,currentLevel,lastType,calType,dest_types);//w1-w7 Rules	
//		processNeutralTokens(begin, range, currentLevel, lastType, calType,dest_types);//N1-N2	
//		processImplicitLevels(begin, range, currentLevel, lastType, calType,dest_levels,dest_types);//I1_I2
		processWeakTokens(begin,range,currentLevel,lastType,calType,destTypes);//w1-w7 Rules	
		processNeutralTokens(begin, range, currentLevel, lastType, calType,destTypes);//N1-N2	
		processImplicitLevels(begin, range, currentLevel, lastType, calType,destLevels,destTypes);//I1_I2
 
		lastLevel =currentLevel;
		begin = range;
	}
}		

/****************************************************************************
Aplies Rule W1-W7
*****************************************************************************/
void processWeakTokens(U16 begin,U16 range, U16 currentLevel,U8 sor,U8 eor,U8* destTypes)
{
/***************************************************************************
Rule W1
Find all NSM & change them to the type of the previous character
If NSM is at the start of the run level, change it with sor
*******************************************************************************/
	S16 iCount,jCount;
	U8 lastType=sor;
	for(iCount=begin;iCount<range;iCount++)
	{
		U8 tempToken=destTypes[iCount];
		if(tempToken==NSM)
			destTypes[iCount]=lastType;
		else
			lastType=tempToken;//BBB
	}
/***************************************************************************
Rule W2
Find the EN & go back till u find AL,BIDI_R,BIDI_L,sor,if u find AL change EN to AL
*******************************************************************************/
	for (iCount=begin;iCount<range;iCount++)
	{
		U8 tempToken=destTypes[iCount];
		if(tempToken ==EN)
		{
			for(jCount=iCount-1;jCount>=begin;jCount--)
			{
				U16 temp=destTypes[jCount];
				if((temp==BIDI_R)||(temp==BIDI_L)||(temp==AL)||(temp==sor))
				{
					if(temp==AL)
					{
 
						//dest_types[jCount]=AL;
						destTypes[iCount]=BIDI_L;
 
					}
					break;
				}
				
			}
		}
	}
/***************************************************************************
Rule W3
Replace all ALs to BIDI_R.
*******************************************************************************/
	for(iCount=begin; iCount<range;iCount++)
	{
		if(destTypes[iCount]==AL)
			destTypes[iCount]=BIDI_R;
	}
		
/***************************************************************************
Rule W4
Change ES(Eurpoian seperator) to EN between two EN,s.
CS(common seperator) between two numbers of the same type changes to that type.
First & last token will not play part in Rule
*******************************************************************************/		
	for(jCount=begin +1; jCount < range - 1 ;jCount++)
	{
		if(destTypes[jCount]==CS ||destTypes[jCount]==ES)
		{
			U8 lastToken=destTypes[jCount-1];
			U8 nextToken=destTypes[jCount+1];
			if ((destTypes[jCount]==ES ||destTypes[jCount]==CS) && lastToken==EN && nextToken==EN)
				destTypes[jCount]=EN;
			else if (destTypes[jCount]==CS && lastToken==nextToken && lastToken==AN)
				destTypes[jCount]=lastToken;
		}
	}
/***************************************************************************
Rule W5
Change  sequence of ET(Eurpoian Terminator) adjacent to EN to all EN,s.
*******************************************************************************/
	for (iCount = begin; iCount < range; ++iCount)
	{
		if (destTypes[iCount] == ET) //get end of sequence
		{
			U8 temp;
 
//			U8 validToken[]={ET};
			U8 validToken[1]={ET};
			U16 carryStart = iCount;
 
//			U16 varRunRate = get_run_rate(dest_types,carryStart, range,  validToken,4);
			U16 varRunRate = get_run_rate(destTypes,carryStart, range,  validToken,1);
 
			if(carryStart==begin) // check values at ends of sequence
				temp=sor;
			else
				temp=destTypes[carryStart-1];
			if (temp != EN) 
			{
				if(varRunRate == range )
					temp= eor ;
				else
				temp=destTypes[varRunRate];
			}
	
			if (temp == EN) 
	    	{
                    setTokens(destTypes,carryStart,varRunRate, EN);
            }
            iCount = varRunRate;
           
       }

	}
/*********************************************************************************************
RuleW6:change remaining teriminators into neutrals
*********************************************************************************************/
	iCount=begin;
	while(iCount<range)
	{
		U8 temp=destTypes[iCount];
		if(temp==CS||temp==ET||temp==ES)
			destTypes[iCount]=ON;
 
		iCount++;
	}
/***************************************************************************
Rule W7
Find the EN & go back till u find AL,BIDI_R,BIDI_L,sor,if u find BIDI_L change EN to AL
*******************************************************************************/
	for (iCount=begin;iCount<range;iCount++)
	{
		U8 tempToken=destTypes[iCount];
		if(tempToken ==EN)
		{
			for(jCount=iCount-1;jCount>=begin;jCount--)
			{
				U8 temp=destTypes[jCount];
				if((temp==BIDI_R)||(temp==BIDI_L)||(temp==AL)||(temp==sor))
				{
					if(temp==BIDI_L)
					{
					 
						destTypes[iCount]=BIDI_L;  

					}
					break;
				}
				
			}
		}
	}
}

/**********************************************************************************************
used to find a particular sequence & returns immediate next index following token
***********************************************************************************************/
U16 get_run_rate(U8* destTypes,U16 base, U16 rate, U8* correctType,U16 length)//return the base if match is not found
{
	S16 iCount;
	pBOOL bFlag=FALSE;

 
	    
	while (++base <rate)
	{
		U8 temp = destTypes[base];
		bFlag=FALSE;
		for ( iCount = 0; iCount < length; ++iCount) 
		{
			if (temp== correctType[iCount])
			{
				bFlag=TRUE;
				break;
			}
		}
		if (bFlag) continue;
		// can't find a match in correctType
		return base;
	}
	return rate;
}

/****************************************************************************************************
set new Token types
****************************************************************************************************/
void setTokens(U8* destTypes,U16 begin, U16 range,U8 newType)//check
{
	while(begin <range)
	{
		destTypes[begin]=newType;
		begin++;
	}
}

/*************************************************************************************************
Rule N1 & N2
Processes the Neutral Types
**************************************************************************************************/
void processNeutralTokens(U16 begin, U16 range, U16 currentLevel, U8 sor, U8 eor,U8* destTypes)
{
	U16 i;
	// on entry, only these tokens are checked in destTokenTypes
 
	//U16 validToken[]= {BIDI_L, BIDI_R, EN, AN, BIDI_B, S, WS, ON};
	U8 validToken[]= {BIDI_B, S, WS, ON};
	for ( i = begin; i < range; ++i)
	{
		U16 temp = destTypes[i];
		if (temp == WS || temp == ON || temp == BIDI_B || temp == S) 
		{
			// find bounds of run of neutrals
			U8 leadingToken;
			U8 trailingToken;
			U8 validatedToken;
			U16 runBegin = i;
 
			U16 runRange = get_run_rate(destTypes,runBegin, range, validToken,4);

			// determine effective types at ends of run
	        
			if (runBegin == begin) 
			{
				leadingToken = sor;
			} 
			else 
			{
				leadingToken = destTypes[runBegin - 1];
				if (leadingToken == BIDI_L || leadingToken == BIDI_R) 
				{
					// match found for the strong type
				} 
				else if (leadingToken == AN) 
				{
					leadingToken = BIDI_R; 
				} 
				else if (leadingToken == EN) 
				{
					// last clause
	                
					leadingToken = BIDI_R;
				}
			}

			if (runRange == range)
			{
				trailingToken = eor;
			}
			else 
			{
				trailingToken= destTypes[runRange];
				if (trailingToken == BIDI_L || trailingToken == BIDI_R)
				{
					// Match found here
				}
				else if (trailingToken == AN)
				{
					trailingToken = BIDI_R;
				} 
				else if (trailingToken == EN)
				{
					trailingToken = BIDI_R;
				}
			}

	        
			if (leadingToken == trailingToken) 
			{
				// Rule N1.
				validatedToken = leadingToken;
			} 
			else 
			{
				// Rule N2.
	            
				validatedToken = TOKEN_FOR_LEVEL(currentLevel);
			}

			setTokens(destTypes,runBegin, runRange, validatedToken);

			// skip over run of (former) neutrals
			i = runRange;
		}                    
	}
}

/**************************************************************************************************************
Rule:I1 & I2
Resolves the implicit levels
**************************************************************************************************************/
void processImplicitLevels(U16 begin, U16 range, U16 currentLevel, U8 sor, U8 eor,U8* destLevels,U8* destTypes)
{
    S16 iCount;
    if ((currentLevel & 1) == 0) 
	{ // even level
        for ( iCount = begin; iCount < range; ++iCount)
		{
            U8 temp = destTypes[iCount];
            if (temp == BIDI_L ) 
			{
                // no change
            } 
			else if (temp == BIDI_R)
			{
                destLevels[iCount] += 1;
            } 
			else if( temp == AN || temp == EN)
			{
                destLevels[iCount] += 2;
            }
        }
    } 
	else 
	{ // odd level
        for ( iCount = begin; iCount < range; ++iCount) 
		{
            U8 temp = destTypes[iCount];
            if (temp == BIDI_R)
			{
                // no change
            } 
			else if(  temp == BIDI_L || temp == AN || temp == EN)
                destLevels[iCount] += 1;
            
        }
    }
}

/**************************************************************************************************************
Resinserting the explicit levels
returns the textLength
***************************************************************************************************************/
U16 insert_explicit_direction_codes(U8* inputTypes,U16 inputLen,U8 paragraphEmbeddingLevel,U8* all_embeddings_level,U8* destLevels,U8* destTypes) 
{
    S16 iCount;

    for ( iCount = inputLen; --iCount >= 0;) 
    {
        S16 kCount;
		U8 temp = inputTypes[iCount];
		kCount=inputLen;
        if (temp == LRE || temp == RLE || temp == LRO || temp == RLO || temp == PDF || temp == BN) 
		{
            all_embeddings_level[iCount] = 0;
            destTypes[iCount] = temp;
            destLevels[iCount] = 0xff;
        } 
		else 
		{
            --kCount;
            all_embeddings_level[iCount] = all_embeddings_level[kCount];
            destTypes[iCount] = destTypes[kCount];
            destLevels[iCount] = destLevels[kCount];
        }
    }

    // now propagate forward the levels information (could have 
    // propagated backward, the main thing is not to introduce a level
    // break where one doesn't already exist).
#if 0 //destLevels[0] == -1 is never true.
    if (destLevels[0] == -1) 
	 {
        destLevels[0] = paragraphEmbeddingLevel;
    }

    for ( iCount = 1; iCount < inputLen; ++iCount) 
	 {
        if (destLevels[iCount] == -1) 
		  {
            destLevels[iCount] = destLevels[iCount-1];
        }
    }
#endif
    // Embedding information is for informational purposes only
    // so need not be adjusted.

    return inputLen;
}

