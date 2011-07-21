#ifndef MULTI_MEMMNG_H_
#define MULTI_MEMMNT_H_

#include "mmi_features.h"//wug add 20071009
struct hole
{
      unsigned long h_base;           /* where does the hole begin? */
      unsigned long h_len;            /* how big is the hole? */
      struct hole *h_next;          /* pointer to next entry on the list */
};

struct block_head
{
    unsigned long h_base;
    unsigned long h_len;
};

//#define SYS_DYNAL_POOL_SIZE  (181 * 1024)
#if defined (__MMI_MAINLCD_240X320__)
#define SYS_DYNAL_POOL_SIZE  (310 * 1024)  //modified by WeiD
#elif defined ( __MMI_MAINLCD_176X220__)
#define SYS_DYNAL_POOL_SIZE  (270 * 1024)  //modified by WeiD
#elif defined ( __MMI_MAINLCD_220X176__)
#define SYS_DYNAL_POOL_SIZE  (270 * 1024)  //modified by WeiD
#else
#define SYS_DYNAL_POOL_SIZE  (200 * 1024)  //modified by WeiD
#endif


#define NR_HOLES   32
#define NIL_HOLE                (struct hole *) 0
#define NO_MEM                  0

void FreeMediaMemory(unsigned long base);
unsigned long AllocMediaMemory(unsigned long clicks);

#endif
