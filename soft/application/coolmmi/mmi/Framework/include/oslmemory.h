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
 *	OslMemory.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for OSL memory interface with system service.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef OSL_MEMORY_H
#define OSL_MEMORY_H
#include		"mmi_data_types.h"
#include "cos.h"    //added by elisa on 2008_10_9
 /***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef struct tsize{
	long s;
}TSIZE;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

extern void *lastMemory;
extern TSIZE *lastSize;
extern void *MMI_Malloc(char *file,char *function,int line,UINT32 size);
extern void MMI_Free(void *addr);
	
#ifdef OSL_MEMORY_DUMP
	extern void OslIntMemoryStart();
	extern long OslIntMemCounter(void);
	extern void OslIntDumpDataInFile(void);
	extern void *OslMallocCHK (long nob, char* fileName, int lineNumber, ...);
	extern void OslMfreeCHK(void* frp);

	#define OslMemoryStart(var) NULL
	#define OslMemCounter() OslIntMemCounter()
	#define OslDumpDataInFile() OslIntDumpDataInFile()
	#define OslMalloc(nob) 	OslMallocCHK((nob), __FILE__, __LINE__)
	#define OslMfree(frp)		if(frp!=NULL)\
								{\
									OslMfreeCHK((frp));\
									frp=NULL;\
								}

#else
	#define OslMemoryStart(var) NULL
	#define OslMemCounter() 0
	#define OslDumpDataInFile() 

	#ifdef MMI_ON_WIN32
		#define OslMalloc(nob) 	malloc(nob)
		#define OslMfree(frp)	if(frp!=NULL)\
								{\
									free(frp);\
								}
	#else
		#define OslMalloc(nob) 	MMI_Malloc((char *)__FILE__,(char *)__FUNCTION__,(int )__LINE__,nob)
		#define OslMfree(frp)	MMI_Free(frp)
	#endif


#endif

#ifdef MMI_ON_HARDWARE_P
	#define OslMemcpy LCD_memcpy
#else
	#define OslMemcpy memcpy
#endif


#endif /* OSL_MEMORY_H */

