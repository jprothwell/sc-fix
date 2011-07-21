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
/*****************************************************************************
 *
 * Filename:
 * ---------
 *	 ScrMemMgr.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Screen-Based Shared Memory Manager
 *
 *  Screen-based shared memory is allocated after entering a screen, and 
 *  released before leaving a screen. 
 *
 *  Screen-based memory must be released before or inside screen exit function;
 *  otherwise, it asserts.
 * 
 *  Screen-based shared memory is deterministic. Because all memory are released
 *  when switching screens, memory fragmentation is restricted inside a screen.
 *
 * Author:
 * -------
 * -------
 * -------
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/***************************************************************************** 
 * Include
 *****************************************************************************/
#include "mmi_features.h"

#include "kal_release.h"
	
#define __NEWSIMULATOR 
	
#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"
#include "debuginitdef.h"
#include "scrmemmgrgprot.h"
#include "scrmemmgr.h"
#include "eventsgprot.h"

#undef __NEWSIMULATOR 

#ifdef MMI_ON_WIN32
#include <stdlib.h>
#include <malloc.h>
#endif /* MMI_ON_WIN32 */

/***************************************************************************** 
 * Define
 *****************************************************************************/

#ifdef MMI_ON_WIN32
#undef MMI_FRM_SCRMEM_USE_ADM
#else
#define MMI_FRM_SCRMEM_USE_ADM
#endif

/* Checksum in header of allocated chunk */
#define MMI_FRM_SCRMEM_HEADER_CHKSUM1  (0xAAAAAAAA)
/* Checksum in header of released chunk */
#define MMI_FRM_SCRMEM_HEADER_CHKSUM2  (0xBBBBBBBB) 
/* Checksum in footer of allocated chunk */
#define MMI_FRM_SCRMEM_FOOTER_CHKSUM1  (0xCCCCCCCC)
/* Checksum in footer of released chunk */
#define MMI_FRM_SCRMEM_FOOTER_CHKSUM2  (0xDDDDDDDD)

/***************************************************************************** 
 * Typedef 
 *****************************************************************************/

/* Context */
typedef struct
{
	/* ADM pool ID */
	KAL_ADM_ID	mem_pool_id;
	/* Count of allocated chunks */
	S32			alloc_count; 
} mmi_frm_scrmem_cntx_struct;


/* Data inserted at the head of a chunk */
typedef struct
{
	U32   scrid;
	U32	chunk_size; /* allocated size round to 4 */
	U32   chksum;
} mmi_frm_scrmem_header_struct;


/* Data inserted at the tail of a chunk */
typedef struct
{
	U32   chksum;
} mmi_frm_scrmem_footer_struct;

/*****************************************************************************              
 * Local Variable                                                                    
 *****************************************************************************/             

/* Context of screen-based memory */
static mmi_frm_scrmem_cntx_struct g_scrmem_cntx;

#if 0
/* memory pool */
static U8 g_mmi_frm_scrmem_memory_pool[MMI_FRM_SCRMEM_POOL_SIZE];
static const U32 g_mmi_frm_scrmem_pool_size[11] = {
  16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 0xffffffff, 0
};
#endif

/*****************************************************************************              
 * Local Function                                                                    
 *****************************************************************************/    

/*****************************************************************************              
 * Extern Global Variable                                                                    
 *****************************************************************************/             

/*****************************************************************************              
 * Extern Global Function                                                                    
 *****************************************************************************/     

/*****************************************************************************
* FUNCTION
*  mmi_frm_scrmem_init
* DESCRIPTION
*  Initialize screen-based memory.
* PARAMETERS
*  void
* RETURNS
*  void
*****************************************************************************/
void mmi_frm_scrmem_init(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

#ifdef MMI_FRM_SCRMEM_USE_ADM
	#if 0
	g_scrmem_cntx.mem_pool_id = kal_adm_create(g_mmi_frm_scrmem_memory_pool, 
															 MMI_FRM_SCRMEM_POOL_SIZE, 
															 (kal_uint32*)g_mmi_frm_scrmem_pool_size, 
															 KAL_FALSE);
	#endif
		g_scrmem_cntx.mem_pool_id = 0;
#else /* MMI_FRM_SCRMEM_USE_ADM */
	g_scrmem_cntx.mem_pool_id = 0;
#endif /* MMI_FRM_SCRMEM_USE_ADM */

	g_scrmem_cntx.alloc_count = 0;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_scrmem_alloc
* DESCRIPTION
*  Allocate screen-based shared memory
*	NOTE: it might returns NULL if we fail to allocate memory block of "mem_size".
* PARAMETERS
*	mem_size		IN		memory size
* RETURNS
*  allocated chunk
*****************************************************************************/
void *mmi_frm_scrmem_alloc(U32 mem_size)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

	mmi_frm_scrmem_header_struct *header;
	mmi_frm_scrmem_footer_struct *footer;
	U32 chunk_size;
	void *chunk = NULL;
	U16 scr_id = GetExitScrnID();
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#if 0  
	MMI_ASSERT(scr_id != 0);
	#endif
	
	#if 0
	MMI_ASSERT(g_scrmem_cntx.mem_pool_id != 0);
	#endif

	chunk_size = (((mem_size + 3) >> 2) << 2); /* round to multiple of 4 */
	if (chunk_size == 0)
	{
		/* To avoid return NULL */
		chunk_size = 4;
	}

#ifdef MMI_FRM_SCRMEM_USE_ADM
			#if 0
	chunk = kal_adm_alloc(g_scrmem_cntx.mem_pool_id, 
                         chunk_size + sizeof(mmi_frm_scrmem_header_struct) + 
                                      sizeof(mmi_frm_scrmem_footer_struct));
			#endif
			chunk = OslMalloc(chunk_size + sizeof(mmi_frm_scrmem_header_struct) + sizeof(mmi_frm_scrmem_footer_struct));
#else /* MMI_FRM_SCRMEM_USE_ADM */
	chunk = malloc((size_t) chunk_size + sizeof(mmi_frm_scrmem_header_struct) + 
                                        sizeof(mmi_frm_scrmem_footer_struct));
#endif /* MMI_FRM_SCRMEM_USE_ADM */

	if (chunk == NULL)
	{
		return NULL;
	}

	header = (mmi_frm_scrmem_header_struct *) chunk;
	chunk = header + 1;
	footer = (mmi_frm_scrmem_footer_struct *) (((char*)chunk) + chunk_size);

	header->chksum = MMI_FRM_SCRMEM_HEADER_CHKSUM1;
	header->chunk_size = chunk_size;
	header->scrid = scr_id;
	footer->chksum = MMI_FRM_SCRMEM_FOOTER_CHKSUM1;

	g_scrmem_cntx.alloc_count++;

	return chunk;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_scrmem_free
* DESCRIPTION
*  Free screen-based shared memory
* PARAMETERS
*	mem_ptr		IN		memory block to be free-ed
* RETURNS
*  allocated chunk
*****************************************************************************/
void mmi_frm_scrmem_free(void *mem_ptr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

	mmi_frm_scrmem_header_struct *header;
	mmi_frm_scrmem_footer_struct *footer;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	header = ((mmi_frm_scrmem_header_struct *) mem_ptr) - 1;
	footer = (mmi_frm_scrmem_footer_struct*) (((char*)mem_ptr) + header->chunk_size);
	
	MMI_ASSERT(header->chksum == MMI_FRM_SCRMEM_HEADER_CHKSUM1 && 
	           footer->chksum == MMI_FRM_SCRMEM_FOOTER_CHKSUM1);

	header->chksum = MMI_FRM_SCRMEM_HEADER_CHKSUM2;
	footer->chksum = MMI_FRM_SCRMEM_FOOTER_CHKSUM2;

#ifdef MMI_FRM_SCRMEM_USE_ADM
	#if 0
	kal_adm_free(g_scrmem_cntx.mem_pool_id, header);
	#endif
	OslMfree(header);
#else /* MMI_FRM_SCRMEM_USE_ADM */
	free(header);
#endif /* MMI_FRM_SCRMEM_USE_ADM */

	MMI_ASSERT(g_scrmem_cntx.alloc_count > 0);
	g_scrmem_cntx.alloc_count--;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_scrmem_get_available_size
* DESCRIPTION
*  Get available size of screen-based memory
* PARAMETERS
*	void
* RETURNS
*  Total left space
*****************************************************************************/
U32 mmi_frm_scrmem_get_available_size(void)
{
#ifdef MMI_FRM_SCRMEM_USE_ADM
	#if 0
	return kal_adm_get_total_left_size(g_scrmem_cntx.mem_pool_id) -
			 sizeof(mmi_frm_scrmem_header_struct) - 
			 sizeof(mmi_frm_scrmem_footer_struct);
	#endif
	return 0;
#else /* MMI_FRM_SCRMEM_USE_ADM */
	return (10 * 1024 * 1024); /* Return dummy value */
#endif /* MMI_FRM_SCRMEM_USE_ADM */
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_scrmem_is_all_free
* DESCRIPTION
*  Check if there is allocated screen-based memory 
* PARAMETERS
*	void
* RETURNS
*  Return MMI_TRUE if all memory are released
*****************************************************************************/
MMI_BOOL mmi_frm_scrmem_is_all_free(void)
{
	if (g_scrmem_cntx.alloc_count == 0)
	{
		return MMI_TRUE;
	}
	else
	{
		return MMI_FALSE;
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_scrmem_is_valid_block
* DESCRIPTION
*  Check if a memory block is valid. In other words, it should belong to the 
*  current screen.
* PARAMETERS
*	mem_ptr		IN		memory block to check 
* RETURNS
*  Return MMI_TRUE if all memory are released
*****************************************************************************/
MMI_BOOL mmi_frm_scrmem_is_valid_block(void *mem_ptr)
{
	mmi_frm_scrmem_header_struct *header;
	mmi_frm_scrmem_footer_struct *footer;

	header = ((mmi_frm_scrmem_header_struct *) mem_ptr) - 1;
	footer = (mmi_frm_scrmem_footer_struct*) (((char*)mem_ptr) + header->chunk_size);
	
	if (header->chksum == MMI_FRM_SCRMEM_HEADER_CHKSUM1 &&
	    footer->chksum == MMI_FRM_SCRMEM_FOOTER_CHKSUM1 &&
	    header->scrid == GetExitScrnID())
	{
		return MMI_TRUE;
	}
	else
	{
		return MMI_FALSE;
	}
}


