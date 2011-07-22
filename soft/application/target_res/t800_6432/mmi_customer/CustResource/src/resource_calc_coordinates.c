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
#include "mmi_data_types.h"
#include "mmi_features.h"
#ifdef __MMI_MAINLCD_176X220__
const U8 gCalcImgCoor[][2] = 
{
#ifdef __MMI_SLIM_CALCULATOR__
	{0, 0},
	{0, 47},
	{0, 0},
	{63, 0},
	{0, 0},   
#else
	{55, 21},	/* Add */
	{55, 49},	/* Subtract */
	{55, 21},	/* Multiply */
	{105, 21},	/* Divide */
	{74, 34},	/* Equal */	
#endif
	{29, 2},	/* MP */
	{29, 69},	/* MM */
	{29, 2},	/* MC	*/
	{124, 2}	/* MR */
};
#elif defined(__MMI_MAINLCD_220X176__)
const U8 gCalcImgCoor[][2] = 
{
#ifdef __MMI_SLIM_CALCULATOR__
	{0, 0},
	{0, 47},
	{0, 0},
	{63, 0},
	{0, 0},   
#else
	{65, 19},
	{60,49},
	{54, 27},
	{101, 29},
	{74, 33},
#endif
	{46, 2},
	{43, 60},
	{31, 16},
	{114, 18}
};
#elif defined(__MMI_MAINLCD_128X128__)
const U8 gCalcImgCoor[][2] = {
#ifdef __MMI_SLIM_CALCULATOR__
	{0, 0},
	{0, 32},
	{0, 0},
	{46, 0},
	{0, 0},   
#else
//modify by lin for cs368 @20070829 start
	{52, 15},
	{52, 44},
	{32, 29},
	{76, 29},
	{52, 29},  
#endif	
	{40, 0},
	{40, 57},
	{0,  13},
	{102, 13}
};
#else  //128x160
const U8 gCalcImgCoor[][2] = {
#ifdef __MMI_SLIM_CALCULATOR__
	{0, 0},
	{0, 32},
	{0, 0},
	{46, 0},
	{0, 0},   
#else		////guoyt modify for f658@090407
	{37, 56},	/* Add */
	{34, 85},	/* Subtract */
	{34, 59},	/* Multiply */
	{71, 57},	/* Divide */
	{54, 70},  	/* Equal */
#endif	
	{18, 42},	/* MP */
	{19, 102},	/* MM */
	{17, 45},	/* MC	*/
	{90, 43}	/* MR */	
};
#endif
