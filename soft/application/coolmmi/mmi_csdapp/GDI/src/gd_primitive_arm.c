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
 *	 gd_primitive_arm.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Primitive ARM related.
 * Author:
 * -------
 * -------
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"
#if 0   //remove by elisa for not used in our platform on 2008_10_9
#if defined(CSD_TARGET)
#pragma arm section code
#endif 
#endif

#if 0//defined(CSD_TARGET)
#ifdef __RVCT__
#endif
#ifdef __RVCT__
#endif
#else
int GDI_GRAY16SET(int add_end, int add)
{
    do
    {
        U32 color_of_two_pixels = ((U32*)add)[0]; //BBBBBGGGGGGRRRRRBBBBBGGGGGGRRRRR
        U32 r, g, b;

        b = (color_of_two_pixels & 0xF000F000) >> 11; // B : 00000000000BBBB0|00000000000BBBB0
        g = (color_of_two_pixels & 0x07800780) >> 5;  //2G : 0000000000GGGG00|0000000000GGGG00
        r = color_of_two_pixels & 0x001E001E;         // R : 00000000000RRRR0|00000000000RRRR0
        

        r = (r + g + b) >> 2; //(R+2G+B)/4
        r = r & 0x1F001F;     // To clear unneeded bits
        
        ((U32*)add)[0] = (r << 11) | (r << 6) | (r); //RRRRRRRRRR0RRRRR|RRRRRRRRRR0RRRRR
        
        add += 4;
        
    }while (add < add_end);
	return add;
}
#endif
