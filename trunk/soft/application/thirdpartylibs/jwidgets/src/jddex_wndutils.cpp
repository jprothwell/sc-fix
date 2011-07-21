/***************************************************************************
*
* File Name : jdd_wndutils.cpp
*
* IMPORTANT NOTICE
*
* Please note that any and all title and/or intellectual property rights
* in and to this Software or any part of this (including without limitation
* any images, photographs, animations, video, audio, music, text and/or
* "applets," incorporated into the Software), herein mentioned to as
* "Software", the accompanying printed materials, and any copies of the
* Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu")
* or Jataayu's suppliers as the case may be. The Software is protected by
* copyright, including without limitation by applicable copyright laws,
* international treaty provisions, other intellectual property laws and
* applicable laws in the country in which the Software is being used.
* You shall not modify, adapt or translate the Software, without prior
* express written consent from Jataayu. You shall not reverse engineer,
* decompile, disassemble or otherwise alter the Software, except and
* only to the extent that such activity is expressly permitted by
* applicable law notwithstanding this limitation. Unauthorized reproduction
* or redistribution of this program or any portion of it may result in severe
* civil and criminal penalties and will be prosecuted to the maximum extent
* possible under the law. Jataayu reserves all rights not expressly granted.
*
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
* AND ANY AND ALL REPRESENTATION
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	Implimentation for utility functions
*						
* Created By		:
* Created Date		:
*
*
*
* Current Revision	:
*
***************************************************************************
*
*
* Revision Details
* ----------------
*
* 1. Modified By	:
*    Modified Date	:
*	 Purpose		:
*
*
*
*
***************************************************************************/

#include "ddl.h"
#include "jcal.h"
//#include "jddex_wndutils.h"


#if 0
/**
 * @brief 		overloaded new operator
 * @param[in]	uiSize   size to be allocated
 * @retval		pointer to memory allocated
 *
 */
void* operator jddex_memutils::new( JC_UINT32 uiSize )
{
	return jdd_MemAlloc (1, uiSize);
}

/**
 * @brief 		overloaded delete operator
 * @param[in]	p   pointer to be freed
 * @retval		None
 *
 */
void operator jddex_memutils::delete(void *p)
{
  if( p != NULL ) 
	  jdd_MemFree (p);
}

#endif
