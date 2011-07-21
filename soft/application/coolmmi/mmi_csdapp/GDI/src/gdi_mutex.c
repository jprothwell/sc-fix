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
 *  gdi_mutex.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image Hardware Gif related.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"

#if defined(GDI_MUTEX_DEBUG) && defined(CSD_TARGET)
    //#define GDI_MUTEX_DEBUG_DUMP_STACK
#endif /* GDI_MUTEX_DEBUG */ 

#ifdef MMI_ON_WIN32
#include <windows.h>
#include <process.h>
#endif /* MMI_ON_WIN32 */ 

/* extern void* TCD_Current_Thread; */
extern kal_taskid kal_get_current_thread_ID(void);
extern kal_uint8 INT_Exception_Enter;
extern void INT_ExceptionDumpStack(long *dumpbuf, long *current_thread);
#ifndef MMI_ON_WIN32
#include "drv_comm.h"
#endif 
#if 0
/* global mutex variable */
#if defined(CSD_TARGET)
#pragma arm section rwdata = "INTERNRW" , rodata = "INTERNCONST" , zidata = "INTERNZI"
#endif 
#endif

gdi_mutex_struct gdi_mutex;
int gdi_mutex_serial = 0;
#if 0
#if defined(CSD_TARGET)
#pragma arm section rwdata, rodata , zidata
#endif 
#endif

#ifdef GDI_MUTEX_DEBUG_DUMP_STACK
long gdi_mutex_stack_backup[120];
#endif 
#define GDI_MUTEX_GUARD_PATTERN 0x87654321
/* ///////////////////////////////////////////////////////////////////////////////////////////////////// */

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-11 16:39 */
kal_uint8 INT_Exception_Enter;
kal_taskid   kal_get_current_thread_ID( )
{
	return NULL;
}
void INT_ExceptionDumpStack(long *dumpbuf, long *current_thread)
{

}
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-11 16:39 */
/*****************************************************************************
 * FUNCTION
 *  gdi_mutex_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_mutex_init(gdi_mutex_struct *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_WIN32
    m->mutex = CreateMutex(NULL, FALSE, NULL);
#endif 
    m->cnt = 0;
    m->tid = NULL;
    m->front_guard = GDI_MUTEX_GUARD_PATTERN;
    m->rear_guard = GDI_MUTEX_GUARD_PATTERN;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_mutex_lock
 * DESCRIPTION
 *  
 * PARAMETERS
 *  M       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_mutex_lock(gdi_mutex_struct *M)
{
#ifndef MMI_ON_WIN32
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 save_irq_mask;
    kal_taskid gdi_current_thread_id = kal_get_current_thread_ID();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (INT_Exception_Enter)
    {
        return;
    }
    if (gdi_current_thread_id == NULL)
    {
    #if 0
    #endif /* DEMO_PROJECT */ 
        return;
    }

    if (gdi_current_thread_id != M->tid)
    {
        /* wait gdi mutex free */
        while (1)
        {
            save_irq_mask = SaveAndSetIRQMask();
            if (M->cnt == 0)
            {
                M->cnt = 1;
                RestoreIRQMask(save_irq_mask);
                break;
            }
            RestoreIRQMask(save_irq_mask);
            kal_sleep_task(1);  /* waiting */
        }

        /* get gdi mutex */

        M->tid = gdi_current_thread_id;
        gdi_mutex_serial++;
    #ifdef GDI_MUTEX_DEBUG_DUMP_STACK
        /* Store Stack Status when gdi mutex lock */
        INT_ExceptionDumpStack(gdi_mutex_stack_backup, (long*)gdi_current_thread_id);
    #endif /* GDI_MUTEX_DEBUG */ 
    }
    else
    {
        save_irq_mask = SaveAndSetIRQMask();
        ++(M->cnt);
        RestoreIRQMask(save_irq_mask);
    }
#else /* MMI_ON_WIN32 */ /* if defined(MMI_ON_WIN32) */
    if (M->mutex == NULL)
    {
        return;
    }

    if (GetCurrentThreadId() != (unsigned long)M->tid)
    {
        WaitForSingleObject((void*)M->mutex, INFINITE);
        M->tid = (void*)GetCurrentThreadId();
    }
    ++(M->cnt);

#endif /* MMI_ON_WIN32 */ 

}


/*****************************************************************************
 * FUNCTION
 *  gdi_mutex_unlock
 * DESCRIPTION
 *  
 * PARAMETERS
 *  M       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_mutex_unlock(gdi_mutex_struct *M)
{
#ifndef MMI_ON_WIN32
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 save_irq_mask;
    kal_taskid gdi_current_thread_id = kal_get_current_thread_ID();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (INT_Exception_Enter)
    {
        return;
    }

    save_irq_mask = SaveAndSetIRQMask();
    if (gdi_current_thread_id != NULL)
    {
        GDI_DEBUG_ASSERT(M->tid == gdi_current_thread_id);

        if (--M->cnt == 0)
        {
            M->tid = NULL;
        }
    }
    RestoreIRQMask(save_irq_mask);
#else /* MMI_ON_WIN32 */ /* if defined(MMI_ON_WIN32) */
     
    if (M->mutex == NULL)
    {
        return;
    }

    GDI_ASSERT(M->tid == (void*)GetCurrentThreadId());

    --(M->cnt);

    if (M->cnt == 0)
    {
        M->tid = 0;
        ReleaseMutex((void*)M->mutex);
    }
#endif /* MMI_ON_WIN32 */ 

}

/* ////////////////////////////////////////////////////////////////////////////////////////////////////////// */


/*****************************************************************************
 * FUNCTION
 *  gdi_fast_mutex_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_fast_mutex_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_WIN32
    gdi_mutex.mutex = CreateMutex(NULL, FALSE, NULL);
#endif 
    gdi_mutex.cnt = 0;
    gdi_mutex.tid = NULL;
    gdi_mutex.front_guard = GDI_MUTEX_GUARD_PATTERN;
    gdi_mutex.rear_guard = GDI_MUTEX_GUARD_PATTERN;
}
#if 0
#if defined(CSD_TARGET)
#pragma arm section code = "INTERNCODE"
#endif 
#endif


/*****************************************************************************
 * FUNCTION
 *  gdi_fast_mutex_lock
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_fast_mutex_lock(void)
{
#ifndef MMI_ON_WIN32
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 save_irq_mask;

    /* kal_taskid gdi_current_thread_id = kal_get_current_thread_ID(); */
    kal_taskid gdi_current_thread_id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef DEMO_PROJECT
    GDI_ASSERT(gdi_mutex.front_guard == GDI_MUTEX_GUARD_PATTERN);
    GDI_ASSERT(gdi_mutex.rear_guard == GDI_MUTEX_GUARD_PATTERN);
#endif /* DEMO_PROJECT */ 

    if (INT_Exception_Enter)
    {
        return;
    }
#if 0
		#if 0
		#endif
		#ifdef GDI_MUTEX_DEBUG_DUMP_STACK
		#endif // GDI_MUTEX_DEBUG
#endif /* 0 */ 
    save_irq_mask = SaveAndSetIRQMask();
    if ((gdi_current_thread_id = kal_get_current_thread_ID()) != NULL)
    {
        if (gdi_current_thread_id != gdi_mutex.tid)
        {
            /* wait gdi mutex free */
            while (gdi_mutex.cnt)
            {
                RestoreIRQMask(save_irq_mask);
                kal_sleep_task(1);  /* waiting */
                save_irq_mask = SaveAndSetIRQMask();
            }
            gdi_mutex.cnt = 1;
            gdi_mutex.tid = gdi_current_thread_id;
            gdi_mutex_serial++;
    		#ifdef GDI_MUTEX_DEBUG_DUMP_STACK
    		//Store Stack Status when gdi mutex lock
    		INT_ExceptionDumpStack(gdi_mutex_stack_backup,(long*)gdi_current_thread_id);
    		#endif // GDI_MUTEX_DEBUG
        }
        else
        {
            ++(gdi_mutex.cnt);
        }
    }
    RestoreIRQMask(save_irq_mask);

#else /* MMI_ON_WIN32 */ /* if defined(MMI_ON_WIN32) */
    if (gdi_mutex.mutex == NULL)
    {
        return;
    }

    if (GetCurrentThreadId() != (unsigned long)gdi_mutex.tid)
    {
        WaitForSingleObject((void*)gdi_mutex.mutex, INFINITE);
        gdi_mutex.tid = (void*)GetCurrentThreadId();
    }
    ++(gdi_mutex.cnt);

#endif /* MMI_ON_WIN32 */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_fast_mutex_unlock
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_fast_mutex_unlock(void)
{
#ifndef MMI_ON_WIN32
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 save_irq_mask;

    /* kal_taskid gdi_current_thread_id = kal_get_current_thread_ID(); */
    kal_taskid gdi_current_thread_id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (INT_Exception_Enter)
    {
        return;
    }

    save_irq_mask = SaveAndSetIRQMask();
    gdi_current_thread_id = kal_get_current_thread_ID();
    if (gdi_current_thread_id != NULL)
    {
        GDI_DEBUG_ASSERT(gdi_mutex.tid == gdi_current_thread_id);

        if (--gdi_mutex.cnt == 0)
        {
            gdi_mutex.tid = NULL;
        }
    }
    RestoreIRQMask(save_irq_mask);
#else /* MMI_ON_WIN32 */ /* if defined(MMI_ON_WIN32) */
     
    if (gdi_mutex.mutex == NULL)
    {
        return;
    }

    GDI_ASSERT(gdi_mutex.tid == (void*)GetCurrentThreadId());

    --(gdi_mutex.cnt);

    if (gdi_mutex.cnt == 0)
    {
        gdi_mutex.tid = 0;
        ReleaseMutex((void*)gdi_mutex.mutex);
    }
#endif /* MMI_ON_WIN32 */ 
}
#if 0
#ifdef CSD_TARGET
#pragma arm section code
#endif 
#endif


