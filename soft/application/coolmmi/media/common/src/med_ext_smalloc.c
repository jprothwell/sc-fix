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
#include "cs_types.h"
#include "kal_release.h"
#include "med_global.h"
#include "med_smalloc.h"
#include "ts.h" /* wufasong added 2007.06.14 */

#ifdef  __PRODUCTION_RELEASE__
   #define ASSERT_bypass( st )                    (void)0
   #define ASSERT_reboot( st )                    (void)ASSERT(st)
#else
   #define ASSERT_bypass( st )                    ASSERT( st )
   #define ASSERT_reboot( st )                    ASSERT( st )
#endif

#undef ALIGN


#define EMIT(x) 

#define FIT(x) firstfit(x) /**/

#define PATTERN(p) ((char)((unsigned int)(p) & 0xff))
#ifndef MAX
#define MAX(a,b) ((a)>(b))? (a):(b)
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b))? (a):(b)
#endif

#define ALIGN(p) (((p) + (sizeof(align_t) - 1)) & ~(sizeof(align_t) - 1))

#define ADDR(n) ((address_t)(n) + sizeof(node))
#define AFTER(n) (ADDR(n) + (n)->x.rsize)
#define OVERHEAD (sizeof(node))

#define MINNODE (64 * sizeof(align_t))

#define ADJACENT(n1,n2) \
	((address_t)(n1) + OVERHEAD + (n1)->x.rsize == (address_t)(n2))



typedef double align_t;

union _node {
    struct {
        size_type rsize; /* real size */
        size_type usize; /* user size */
        union _node *next;
    } x;
    align_t align;
};
typedef union _node node;

enum _list {
    AVAILABLE,
    ALLOCATED
};
typedef enum _list list;


static node *available = NULL;
static node *allocated = NULL;
static unsigned long total = 0;
static unsigned long alloc_len = 0;
static unsigned long avail_len = 0;



static void delete(list l, node *n)
{
   node *c, *p, **head;

   head = (l == ALLOCATED) ? &allocated : &available;

   c = p = *head;
   while ((c != NULL) && (ADDR(c) > ADDR(n))) {
	   p = c;
	   c = c->x.next;
   }

   if (l == ALLOCATED) {
	   alloc_len--;
   } else {
	   avail_len--;
   }

   if (c == p) {
	   *head = c->x.next;
   } else {
	   p->x.next = c->x.next;
   }
} /* delete */


static void insert(list l, node *n)
{
   node *c, *p, **head;

   head = (l == ALLOCATED) ? &allocated : &available;

   c = p = *head;
   while ((c != NULL) && (ADDR(c) > ADDR(n))) {
      p = c;
      c = c->x.next;
   }

   if ((l == AVAILABLE) && (*head != NULL) && (c != NULL) && ADJACENT(c,n)) {
      c->x.rsize += OVERHEAD + n->x.rsize;
      if (ADJACENT(c,p)) {
	      delete(AVAILABLE, p);
	      c->x.rsize += OVERHEAD + p->x.rsize;
      }
   } else {
      if (l == ALLOCATED) {
	      alloc_len++;
      } else {
	      avail_len++;
      }
      n->x.next = c;
      if (c == p) {
	      *head = n;
      } else {
	      p->x.next = n;
	      if ((l == AVAILABLE) && (*head != NULL) && ADJACENT(n,p)) {
            delete(AVAILABLE, p);
            n->x.rsize += OVERHEAD + p->x.rsize;
	      }
      }
    }
} /* insert */


static node *find(node *head, address_t ptr)
{
   node *c;

   c = head;
   while((c != NULL) && (ADDR(c) > ptr)) {
	   c = c->x.next;
   }

   if ((c == NULL) || (ADDR(c) != ptr)) {
	   return NULL;
   }

   return c;
} /* find */


static node *firstfit(size_type size)
{
   node *c;
   size_type aligned = ALIGN(size);

   c = available;
   while ((c != NULL) && (c->x.rsize < aligned)) {
      c = c->x.next;
   }

   if (c == NULL) {
      //ASSERT_bypass(c!=NULL);
      return NULL;
   } else {
      delete(AVAILABLE, c);
   }

   if (c->x.rsize >= aligned + OVERHEAD + MINNODE) {
      node *n;
      size_type leftover;
      
      leftover = c->x.rsize - aligned - OVERHEAD;
      c->x.rsize = aligned;
      
      n = (node *)AFTER(c);
      n->x.rsize = leftover;
      
      insert(AVAILABLE, n);

   }

   c->x.usize = size;

   insert(ALLOCATED, c);

   return c;
} /* firstfit */



/*---------------------------------------------------------------------------*/

static address_t _malloc(size_type size)
{
   node *store;

   if (size == 0) {
      ASSERT_bypass(size!=NULL);
      return NULL;
   }

   store = FIT(size);

   if (store == NULL) {
      //ASSERT_bypass(store!=NULL);
      return NULL;
   }

   total += size;
   return ADDR(store);
} /* _malloc */

static void _free(address_t ptr)
{
   node *t;

   if (ptr == NULL) {
      ASSERT_bypass(ptr!=NULL);
      return;
   }

   t = find(allocated, ptr);

   if (t == NULL) {
      ASSERT_bypass(t!=NULL);
      return;
   }

   delete(ALLOCATED, t);
   insert(AVAILABLE, t);

   total -= t->x.usize;
} /* _free */

/*---------------------------------------------------------------------------*/
/*----------------------------- USER INTERFACE: -----------------------------*/
/*---------------------------------------------------------------------------*/

address_t med_ext_smalloc(size_type size)
{
   if(size==0)
      return NULL;
   else   
      return _malloc(size);
} /* malloc */


void med_ext_sfree(address_t ptr)
{
   _free(ptr);
} /* free */


void med_set_ext_memory_pool(address_t memory, size_type size)
{
   node *n = (node *)memory;
 
   n->x.rsize = size - OVERHEAD;
   n->x.next = NULL;
 
   allocated = NULL;
   available = n;
   total = 0;
   alloc_len = 0;
   avail_len = 1;  
}

size_type med_allocated_ext_size(void)
{
	return total;
}

