/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */


#include "javacall_eventqueue.h"
#include "mmi_trace.h"
#include "sxr_sbx.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
 * Creates the event queue lock.
 *
 * @return <tt>JAVACALL_OK</tt> if successully created lock
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
static javacall_uint8 event_quene_lock_sem;
javacall_result javacall_create_event_queue_lock(void){mmi_trace(1,"current function : %s\n",__FUNCTION__);

	event_quene_lock_sem = sxr_NewSemaphore(1);
    return JAVACALL_OK;
}

/**
 * Destroys the event queue lock.
 *
 * @return <tt>JAVACALL_OK</tt> if successully destroyed lock
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_destroy_event_queue_lock(void){mmi_trace(1,"current function : %s\n",__FUNCTION__);
	sxr_FreeSemaphore(event_quene_lock_sem);
    return JAVACALL_OK;
}

/**
 * Waits to get the event queue lock and then locks it.
 *
 * @return <tt>JAVACALL_OK</tt> if successully obtained lock
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_wait_and_lock_event_queue(void){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	sxr_TakeSemaphore(event_quene_lock_sem);
    return JAVACALL_OK;
}

/**
 * Unlocks the event queue.
 *
 * @return <tt>JAVACALL_OK</tt> if successully released lock
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_unlock_event_queue(void){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	sxr_ReleaseSemaphore(event_quene_lock_sem);
    return JAVACALL_OK;
}


#ifdef __cplusplus
} //extern "C"
#endif


