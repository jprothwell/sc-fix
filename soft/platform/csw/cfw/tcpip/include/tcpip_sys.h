/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifdef CFW_TCPIP_SUPPORT
#ifndef __LWIP_SYS_H__
#define __LWIP_SYS_H__
#include "tcpip_opt.h"
#include <csw.h>
//#include <ts.h>

//#include "arch/cc.h"
#include "dbg.h"
extern int sprintf(char *buf, const char *fmt, ...);
#define TCP_ASSERT(expr) \
				do { \
					char assertion_info[256] = {0}; \
					sprintf(assertion_info, "[TCP_ASSERT]FILE:%s FUN:%s LN:%d", __FILE__, __FUNCTION__, __LINE__); \
					DBG_ASSERT(expr, assertion_info); \
				} while (0)




typedef UINT8 sys_sem_t;
typedef UINT8 sys_mbox_t;



#if NO_SYS

/* For a totally minimal and standalone system, we provide null
   definitions of the sys_ functions. */
typedef UINT32 sys_sem_t;
typedef UINT8 sys_mbox_t;
struct sys_timeout
{
	UINT8 dummy;
	UINT8 padding[3];
};
#if 0
#define sys_init() 1
#define sys_timeout(m,h,a)
#define sys_untimeout(m,a)
#define sys_sem_new(c) 1
#define sys_sem_signal(s) 1
#define sys_sem_wait(s) 1
#define sys_sem_free(s) 1
#define sys_mbox_new(s) 1
#define sys_mbox_fetch(m,d) 1
#define sys_mbox_post(m,d) 1
#define sys_mbox_free(m) 1
 
#define sys_thread_new(t,a,p)
#endif
#else /* NO_SYS */

//include "arch/sys_arch.h"

/** Return code for timeouts from sys_arch_mbox_fetch and sys_arch_sem_wait */
#define SYS_ARCH_TIMEOUT 0xffffffff

typedef VOID (*sys_timeout_handler) (VOID *arg);

struct sys_timeout
{
	struct sys_timeout *next;
	UINT32 time;
	sys_timeout_handler h;
	VOID *arg;
};

struct sys_timeouts
{
	struct sys_timeout *next;
};

/* sys_init() must be called before anthing else. */
VOID sys_init(VOID);

/*
 * sys_timeout():
 *
 * Schedule a timeout a specified amount of milliseconds in the
 * future. When the timeout occurs, the specified timeout handler will
 * be called. The handler will be passed the "arg" argument when
 * called.
 *
 */
VOID sys_timeout(UINT32 msecs, sys_timeout_handler h, VOID *arg);
VOID sys_untimeout(sys_timeout_handler h, VOID *arg);
struct sys_timeouts *sys_arch_timeouts(VOID);

/* Semaphore functions. */
sys_sem_t sys_sem_new(UINT8 count);
VOID sys_sem_signal(sys_sem_t sem);
UINT32 sys_arch_sem_wait(sys_sem_t sem, UINT32 timeout);
VOID sys_sem_free(sys_sem_t sem);
VOID sys_sem_wait(sys_sem_t sem);
INT32 sys_sem_wait_timeout(sys_sem_t sem, UINT32 timeout);

/* Time functions. */
#ifndef sys_msleep
VOID sys_msleep(UINT32 ms);	/* only has a (close to) 1 jiffy resolution. */
#endif
#ifndef sys_jiffies
UINT32 sys_jiffies(VOID);	/* since power up. */
#endif

/* Mailbox functions. */
sys_mbox_t sys_mbox_new(VOID);
VOID sys_mbox_post(sys_mbox_t mbox, VOID *msg);
UINT32 sys_arch_mbox_fetch(UINT8 mbox, VOID **msg, UINT32 timeout);
VOID sys_mbox_free(sys_mbox_t mbox);
VOID * sys_mbox_fetch(sys_mbox_t mbox, VOID **msg);

/* Thread functions. */
//sys_thread_t sys_thread_new(VOID (*thread) (VOID *arg), VOID *arg, INT32 prio);

/* The following functions are used only in Unix code, and
   can be omitted when porting the stack. */
/* Returns the current time in microseconds. */
UINT32 sys_now(VOID);

#endif /* NO_SYS */


// #ifndef CII_VERSION
/* Critical Region Protection */
/* These functions must be implemented in the sys_arch.c file.
   In some implementations they can provide a more light-weight protection
   mechanism than using semaphores. Otherwise semaphores can be used for
   implementation */
#ifndef SYS_ARCH_PROTECT
/** SYS_LIGHTWEIGHT_PROT
 * define SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task protection
 * for certain critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
 #define SYS_LIGHTWEIGHT_PROT 0
#if SYS_LIGHTWEIGHT_PROT

/** SYS_ARCH_DECL_PROTECT
 * declare a protection variable. This macro will default to defining a variable of
 * type sys_prot_t. If a particular port needs a different implementation, then
 * this macro may be defined in sys_arch.h.
 */
#define SYS_ARCH_DECL_PROTECT(lev) sys_prot_t lev

/** SYS_ARCH_PROTECT
 * Perform a "fast" protect. This could be implemented by
 * disabling interrupts for an embedded system or by using a semaphore or
 * mutex. The implementation should allow calling SYS_ARCH_PROTECT when
 * already protected. The old protection level is returned in the variable
 * "lev". This macro will default to calling the sys_arch_protect() function
 * which should be implemented in sys_arch.c. If a particular port needs a
 * different implementation, then this macro may be defined in sys_arch.h
 */
#define SYS_ARCH_PROTECT(lev) lev = sys_arch_protect()

/** SYS_ARCH_UNPROTECT
 * Perform a "fast" set of the protection level to "lev". This could be
 * implemented by setting the interrupt level to "lev" within the MACRO or by
 * using a semaphore or mutex.  This macro will default to calling the
 * sys_arch_unprotect() function which should be implemented in
 * sys_arch.c. If a particular port needs a different implementation, then
 * this macro may be defined in sys_arch.h
 */
 
#define SYS_ARCH_UNPROTECT(lev) sys_arch_unprotect(lev)
sys_prot_t sys_arch_protect(VOID);
VOID sys_arch_unprotect(sys_prot_t pval);
#else
#define SYS_ARCH_DECL_PROTECT(lev)    lev
#define SYS_ARCH_PROTECT(lev)    lev
#define SYS_ARCH_UNPROTECT(lev)  lev

#endif /* SYS_LIGHTWEIGHT_PROT */

#endif /* SYS_ARCH_PROTECT */

//#endif //CII_VERSION

#endif /* __LWIP_SYS_H__ */
#endif

