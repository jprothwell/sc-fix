/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2010
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
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * NumberLocation.c
 *
 * Project:
 * --------
 * 
 *
 * Description:
 * ------------
 *
 * Author: Wangzl
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef _NUMBERLOCATION_C
#define _NUMBERLOCATION_C

#include "mmi_features.h"
#ifdef __MMI_NUMBER_LOCATION__
#include "mmi_data_types.h"
#include "numberlocationdef.h"
#include "phonebooktypes.h"
#define MIN_NUMBER_LENGTH 11
#define NUMBER_COMPARE_LENGTH 7/*mobile number compare length*/
#define MAX_LOCAL_NUMBER_LENGTH 4/*max length for phone location number*/

U8 unknown_address[] = 
{
	0x2a,0x67,0xe5,0x77,0x30,0x57,0x40,0x57,0x00,0x00
};

extern const MoblieNumberSectData NumberSectMap[];
extern const U8 addressData[];
extern const U16 addressOffsetData[];
extern const PhoneNumberLocationData PhoneLocationMap[];

extern U8 GetNumberSectMapLenght(void);
extern U8 GetPhoneLocationMapLength(void);
/**************************************************************

	FUNCTION NAME		: getLoactionStrFromNumber

  	PURPOSE				: get location address name string from number

	INPUT PARAMETERS	: number string in unicode, now only handle mobile phone number

	OUTPUT PARAMETERS	: nil

	RETURNS				: a pointer to the address name in unicode, and the address name is Chinese only

	REMARKS			: nil

**************************************************************/
PS8 getLoactionStrFromNumber(PS8 number)
{
	U8 number_ASCII[MAX_PB_NUMBER_LENGTH+1+1];
	U8 number_temp[NUMBER_COMPARE_LENGTH + 1];
	U8 number_sect[4];
	U8 number_compare[5];
	U8 number_length;
	U32 number_int;
	U8 numberSect_int;
	U16 location_map_lenght;
	S32 mid, low, high;
	U8 ignoreNumber = 0;
	U8 LocalNumberLen = 2;
	U8 nNumberSectIndex;
	MoblieNumberLocationData *pLocationMap = NULL;

	memset(number_ASCII, 0, sizeof(number_ASCII));
	memset(number_temp, 0, sizeof(number_temp));
	UnicodeToAnsii((PS8)number_ASCII, (PS8)number);
	number_length = strlen(number_ASCII);
	mmi_trace(g_sw_CC, "getLoactionStrFromNumber, number is %s", number_ASCII);
	if(number_length >= MIN_NUMBER_LENGTH)
	{
		memset(number_temp, 0, sizeof(number_temp));
		memcpy(number_temp, number_ASCII + number_length -MIN_NUMBER_LENGTH, NUMBER_COMPARE_LENGTH);
		memset(number_sect, 0, sizeof(number_sect));
		memcpy(number_sect, number_temp, 3);
		memset(number_compare, 0, sizeof(number_compare));
		memcpy(number_compare, number_temp + 3, 4);

		numberSect_int = atoi((const char *)number_sect);
		mmi_trace(g_sw_CC, "getLoactionStrFromNumber, numberSect_int is %d", numberSect_int);
		for(nNumberSectIndex = 0; nNumberSectIndex < GetNumberSectMapLenght(); nNumberSectIndex++)
		{
			if(numberSect_int == NumberSectMap[nNumberSectIndex].sectNumber)
				break;
		}

		if(nNumberSectIndex < GetNumberSectMapLenght())
		{
			number_int = atoi((const char*)number_compare);
			mmi_trace(g_sw_CC, "getLoactionStrFromNumber, number_int is %d", number_int);
			location_map_lenght = NumberSectMap[nNumberSectIndex].nMapLenght;
			pLocationMap = NumberSectMap[nNumberSectIndex].pNumberLocation;
			low = 0;
			high = location_map_lenght -1;
			while(low <= high)
			{
				mid = low + (high - low)/2;
				if(number_int < pLocationMap[mid].minNumber)
				{
					high = mid - 1;
				}
				else if(number_int > pLocationMap[mid].maxNumber)
				{
					low = mid + 1;
				}
				else
				{
					mmi_trace(g_sw_CC, "getLoactionStrFromNumber, address index in pLocationMap is %d", pLocationMap[mid].addressIndex);
					return addressData +addressOffsetData[pLocationMap[mid].addressIndex];
				}
			}
		}

	}

	while(number_ASCII[ignoreNumber])
	{
		if(number_ASCII[ignoreNumber] == '+' || number_ASCII[ignoreNumber] == '0')
		{
			ignoreNumber++;
		}
		else
		{
			break;
		}
	}
	for(LocalNumberLen = 2; LocalNumberLen <= MAX_LOCAL_NUMBER_LENGTH; LocalNumberLen++)
	{
		memset(number_temp, 0, sizeof(number_temp));
		memcpy(number_temp, number_ASCII + ignoreNumber, LocalNumberLen);
		number_int = atoi(number_temp);

		location_map_lenght = GetPhoneLocationMapLength();
		low = 0;
		high = location_map_lenght -1;
		while(low <= high)
		{
			mid = low + (high - low)/2;
			if(number_int < PhoneLocationMap[mid].Number)
			{
				high = mid - 1;
			}
			else if(number_int > PhoneLocationMap[mid].Number)
			{
				low = mid + 1;
			}
			else
			{
				mmi_trace(g_sw_CC, "getLoactionStrFromNumber, address index in PhoneLocationMap  is %d", PhoneLocationMap[mid].addressIndex);
				return addressData +addressOffsetData[PhoneLocationMap[mid].addressIndex];
			}
		}
		
	}
	mmi_trace(g_sw_CC, "getLoactionStrFromNumber, not found");
	return unknown_address;
}
#endif
#endif
