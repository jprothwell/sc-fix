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

/*******************************************************************************
 * Filename:
 * ---------
 *  DebugInit.c
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
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: DebugInit.c

  	PURPOSE		: To put all trace related functionality in common place

 

 

	DATE		: Sep' 4, 2002

**************************************************************/

#include "debuginitdef.h"

#ifdef MMI_ON_HARDWARE_P
#endif


#ifdef DEBUG_ASSERT
BOOL g_mmi_frm_enable_debug_assert = MMI_FALSE;
#endif /* DEBUG_ASSERT */

#ifdef DEBUG_FILE
extern FILE *fpDebugTrace;
#endif
#ifdef MMI_ON_WIN32
#ifdef DEBUG_WINDOW
extern void DebugWindowAlloc();
extern void DisplayToDebugwindow(S8 *);
#endif
#endif



/**************************************************************

	FUNCTION NAME		: DebugInit

  	PURPOSE				: Initialize trace mechanism  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/

void DebugInit()
{

#ifdef MMI_ON_WIN32
#ifdef DEBUG_FILE
	
	if(!(fpDebugTrace = fopen("DebugTraceFile","w")))
	{
		exit(1);
	}
#endif
#ifdef MMI_ON_WIN32
#ifdef DEBUG_WINDOW
	DebugWindowAlloc();
#endif
#endif
#endif
}

/**************************************************************

	FUNCTION NAME		: Trace2

  	PURPOSE				: To display/print to the debug trace window/file   

	INPUT PARAMETERS	: Variable arguments with format being the first one

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/


/*
void DebugClose()
{
	fclose(fpDebugTrace);
}
*/


