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
 *  DebugInitDef.h
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

/**************************************************************

	FILENAME	: DebugInitDef.h

  	PURPOSE		: To implement debug print levels and their conditional configurations.

 

	AUTHOR		: coolsand Engineers

	DATE		: 

**************************************************************/

#ifndef _DEBUG_INT_H
#define _DEBUG_INT_H

#include "cswtype.h"
#include "ds.h"
#include "ts.h"
#include "mmi_data_types.h"//simon add
#if defined(MMI_ON_HARDWARE_P)
#include "hal_debug.h"
#include "kal_release.h" /* target or mnt */
#elif !defined(__GNUC__)
#include "assert.h" /* pc simulator. not resource generator. */
#endif
#include "mmi_trace.h"

#define DEBUG_INFORMATION
#define DEBUG_ASSERT

#ifndef MMI_ON_HARDWARE_P
	typedef enum
   {
	  MMI_TRACE_FUNC=0,
	  MMI_TRACE_STATE,
	  MMI_TRACE_INFO,
	  MMI_TRACE_WARNING,
	  MMI_TRACE_ERROR,
	  
	  MMI_TRACE_G1_FRM,
	  MMI_TRACE_G2_GUI,
	  MMI_TRACE_G3_BOOTUP,
	  MMI_TRACE_G4_PHB,
	  MMI_TRACE_G5_CM,
	  MMI_TRACE_G6_SMS,
	  MMI_TRACE_G7_MISC,
	  MMI_TRACE_G8_MEDIA,
	  MMI_TRACE_G9_EMAIL,
	  MMI_TRACE_G10_RESERVED,
	  MAX_MMI_TRACE_LOG_TYPE
	} mmi_log_type_enum;
#endif


#ifdef DEBUG_INFORMATION
	#ifdef MMI_ON_HARDWARE_P
	#define PRINT_INFORMATION(args) Trace args
	#define PRINT_INFORMATION_2(args) Trace2 args
	#define PRINT_INFORMATION_3(args)  Trace3 args

//	#define MMI_TRACE(args) kal_lib_trace args
	#define MMI_TRACE(args)
	#else
	#define PRINT_INFORMATION(args) 
	#define PRINT_INFORMATION_2(args) 
	#define PRINT_INFORMATION_3(args) 
	#define MMI_TRACE(args)
	#endif
#else
	#ifdef MMI_ON_HARDWARE_P
	#define PRINT_INFORMATION(...)
	 
	//#define PRINT_INFORMATION_2(args) NULL
	#define PRINT_INFORMATION_2(args)
	#define PRINT_INFORMATION_3(args)
	#define MMI_TRACE(...)
	#else
	#define PRINT_INFORMATION(args)
	//#define PRINT_INFORMATION_2(args) NULL
	#define PRINT_INFORMATION_2(args)
	#define PRINT_INFORMATION_3(args)
	#define MMI_TRACE(args)
#endif
#endif

/*
 * = MMI_ASSERT = 
 *
 * Equivalent to ASSERT(). We are sure that this should not happen, and we don't know how to recover from it.
 * 
 * Note: MMI_ASSERT might be REMOVED for __PRODUCTION_RELEASE__ in the future!!
 *
 * = MMI_DBG_ASSERT =
 *
 * MMI_DBG_ASSERT is ignored unless g_mmi_frm_enable_debug_assert is set in Engineer Mode.
 * It is used to test if something strange occurs, typically non-fatal errors.
 * However, we should prevent failure of MMI_DBG_ASSERT in most cases.
 * MMI_DBG_ASSERT is completely removed in produce release to save space and time.
 *
 * = MMI_EXT_ASSERT =
 *
 * In production release, it reset the mobile phone on failure.
 * In non-production release, it print error message on failure.
 */

//DBG_ASSERT((expr), "%d", __LINE__)

#ifdef DEBUG_ASSERT
	extern BOOL g_mmi_frm_enable_debug_assert;
	
	#ifdef MMI_ON_HARDWARE_P
		/////////////////////////////
		// Define MMI_ASSERT. Do not use MMI_DBG_ASSERT again in MMI code.
		#ifdef __PRODUCTION_RELEASE__
			#define MMI_ASSERT(expr)
		#else
#if 1
			#define MMI_ASSERT(expr) \
				if(!(expr)){ \
					char assertion_info[256] = {0}; \
					sprintf(assertion_info, "[MMI_ASSERT]FILE:%s FUN:%s LN:%d", __FILE__, __FUNCTION__, __LINE__); \
					DBG_ASSERT(expr, assertion_info); \
				}
#else        
			#define MMI_ASSERT(expr) \
				do { \
                    if (!(expr)) {\
    					hal_DbgAssert("[MMI_ASSERT]FILE:%s FUN:%s LN:%d\n", __FILE__, __FUNCTION__, __LINE__); }\
				} while (0)
			#define MMI_ASSERT_MSG(expr, format, ...) \
				do { \
                    if (!(expr)) \
                    {\
    					char assertion_info[256] = {0}; \
    					sprintf(assertion_info , format, ##__VA_ARGS__);\
    					hal_DbgAssert("[MMI_ASSERT]FILE:%s FUN:%s LN:%d\n %s", __FILE__, __FUNCTION__, __LINE__, assertion_info); \
                    }\
				} while (0)
#endif
				
		#endif
		#ifdef __PRODUCTION_RELEASE__
			#define MMI_DBG_ASSERT(expr)
		#else
			#define MMI_DBG_ASSERT(expr)	do {if (g_mmi_frm_enable_debug_assert) ASSERT(expr);} while(0)
		#endif
		/////////////////////////////////////////////////////
		// Un-define CSW defined ASSERT, because it's just an dummy, includes DS_ASSERT.
		#ifdef ASSERT
		#undef ASSERT
		#define ASSERT MMI_ASSERT
		#endif

		#ifdef DS_ASSERT
		#undef DS_ASSERT
		#endif	
	#elif !defined(__GNUC__) 
		/* PC Simulator */
		#define MMI_ASSERT(expr)		assert(expr)
		#define MMI_DBG_ASSERT(expr)	do {if (g_mmi_frm_enable_debug_assert) assert(expr);} while(0)
	#else
		/* Resource generation tool */
		#define MMI_ASSERT(expr)
		#define MMI_DBG_ASSERT(expr)
	#endif
#else
		#define MMI_ASSERT(expr)
		#define MMI_DBG_ASSERT(expr)
#endif /* DEBUG_ASSERT */

#endif //_DEBUG_INT_H

