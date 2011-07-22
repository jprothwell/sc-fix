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

/**********************************************************************

	Filename:	KeyMap.h

 

	Date:		Aug 6, 2002

	Contains:	Skin key mapping: Actual windows coordinates
***********************************************************************/

#ifdef WIN32
#include <windows.h>
#endif
#include "mmi_data_types.h"

S16	HEIGHT_OFFSET=42;   
S16	WIDTH_OFFSET=3;    

S16 Leftsoftkey[] 		=	{79,279,105,302};
S16 Rightsoftkey[]		=	{169,279,195,302};
S16 UpArrow[]			=	{116,276,156,290};
S16 DownArrow[]			=	{116,324,160,338};
S16 LeftArrow[]			=	{108,283,125,327};
S16 RightArrow[]		=	{153,284,169,327};
S16 KeyMenu[]			=	{0,0,0,0};				// HW not support  
S16 KeyClear[]			=	{0,0,0,0};				// HW not support  
S16 KeyCall[]			=	{78,310,104,336};
S16 KeyEnd[]			=	{170,311,323,336};
S16 KeyPowerDown[]		=	{0,0,0,0};				// HW not support  
S16 VolumeUp[]			=	{16,270,48,303};
S16 VolumeDown[]		=	{16,318,47,349};
S16 KeyOne[]			=	{70,340,108,364};
S16	KeyTwo[]			=	{115,342,156,364};
S16	KeyThree[]			=	{165,340,204,364};
S16	KeyFour[]			=	{68,368,108,394};
S16	KeyFive[]			=	{115,370,156,394};
S16	KeySix[]			=	{165,369,204,394};
S16	KeySeven[]			=	{68,396,108,424};
S16	KeyEight[]			=	{114,401,156,424};
S16	KeyNine[]			=	{165,399,204,424};
S16	KeyZero[]			=	{115,430,156,454};
S16	KeyStar[]			=	{67,427,108,454};
S16	KeyHash[]			=	{163,428,204,454};
S16 KeyEnter[]			=	{0,0,0,0};				// HW not support  
S16 KeyReserved1[]		=	{1,1,1,1};
S16 KeyReserved2[]		=	{1,1,1,1};


