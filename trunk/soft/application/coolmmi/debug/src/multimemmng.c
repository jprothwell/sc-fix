#ifndef MULTI_MEMMNG_C_
#define MULTI_MEMMNG_C_

#include "multimemmng.h"
#include "cswtype.h"
#include "cos.h"
#include "oslmemory.h"
#ifdef MEM_ULC_3216
#define MULTIMEM_BY_HEAP
#endif

#ifndef MULTIMEM_BY_HEAP
/* The array used for dynamical pool */
unsigned char gv_CommonDynaPool[SYS_DYNAL_POOL_SIZE];
#endif
//for simulator compile
//DebugInit.c
S8 nPrintableStr[1000];

/* The hole link used for dynamical management */
struct hole  hole[NR_HOLES];

/* pointer to first hole */
struct hole *hole_head;             

/* ptr to list of unused table slots */
struct hole *free_slots;            

HANDLE  MULTIMEM_MALLOC_SEM;
#define MULTIMEMMNG_SEM_INIT      do { MULTIMEM_MALLOC_SEM = COS_CreateSemaphore(1); } while(0)
#define MULTIMEMMNG_SEM_CONSUME   do { COS_WaitForSemaphore(MULTIMEM_MALLOC_SEM, COS_WAIT_FOREVER); } while(0)
#define MULTIMEMMNG_SEM_PRODUCE   do { COS_ReleaseSemaphore(MULTIMEM_MALLOC_SEM); } while(0)

#define ALIGN_MEM_SIZE(x)   {x += 3; x &= 0xfffffffc;}

void DelSlot(struct hole *prev_ptr, struct hole *hp);
void Merge(struct hole *hp);
unsigned long PoolLength(void);
unsigned long MaxHole(void);

void AddHole(unsigned long base, unsigned long clicks)
{
    struct hole *hp, *new_ptr, *prev_ptr;

    if(clicks == 0) return;
    if((new_ptr = free_slots) == NIL_HOLE)
    {
    }
    new_ptr->h_base = base;
    new_ptr->h_len = clicks;
    free_slots = new_ptr->h_next;
    hp = hole_head;

    /* If this block's address is numerically less than the lowest hole currently
     * available, or if no holes are currently available, put this hole on the
     * front of the hole list.
     */
    prev_ptr = 0;
    if(hp == NIL_HOLE || base <= hp->h_base)
    {
        /* Block to be freed goes on front of the hole list. */
        new_ptr->h_next = hp;
        hole_head = new_ptr;
        Merge(new_ptr);
        return;
    }

    /* Block to be returned does not go on front of hole list. */
    while (hp != NIL_HOLE && base > hp->h_base)
    {
        prev_ptr = hp;
        hp = hp->h_next;
    }

    /* We found where it goes.  Insert block after 'prev_ptr'. */
    new_ptr->h_next = prev_ptr->h_next;
    prev_ptr->h_next = new_ptr;
    Merge(prev_ptr);
}



void DelSlot(struct hole *prev_ptr, struct hole *hp)
{
    if(hp == hole_head)
    {
        hole_head = hp->h_next;
    }
    else
    {
        prev_ptr->h_next = hp->h_next;
    }

    hp->h_next = free_slots;
    free_slots = hp;
}



void Merge(struct hole *hp)
{
    struct hole *next_ptr;

    /* If 'hp' points to the last hole, no merging is possible.  If it does not,
     * try to absorb its successor into it and free the successor's table entry.
     */
    if( (next_ptr = hp->h_next) == NIL_HOLE) return;
    if (hp->h_base + hp->h_len == next_ptr->h_base)
    {
        hp->h_len += next_ptr->h_len;   /* first one gets second one's mem */
        DelSlot(hp, next_ptr);
    }
    else
    {
        hp = next_ptr;
    }

    /* If 'hp' now points to the last hole, return; otherwise, try to absorb its
     * successor into it.
     */
    if((next_ptr = hp->h_next) == NIL_HOLE) return;
    if(hp->h_base + hp->h_len == next_ptr->h_base)
    {
        hp->h_len += next_ptr->h_len;
        DelSlot(hp, next_ptr);
    }
}


unsigned long PoolLength(void)
{
    struct hole *hp = hole_head;
    unsigned long sum = 0;

    while (hp != NIL_HOLE)
    {
        sum++;
        hp = hp->h_next;
    }
    return (sum);
}

unsigned long MaxHole(void)
{
    struct hole *hp;
    unsigned long max;

    hp = hole_head;
    max = 0;
    while (hp != NIL_HOLE)
    {
        if (hp->h_len > max) max = hp->h_len;
        hp = hp->h_next;
    }

    return(max);
}

//取得剩余的内存字节数
unsigned long GetFreeMemory(void)
{
    struct hole *hp;
    //unsigned long max;
    unsigned long sum = 0;

    hp = hole_head;
    //max = 0;
    while (hp != NIL_HOLE)
    {
        //if (hp->h_len > max) max = hp->h_len;
        sum += (unsigned long)(hp->h_len);
        hp = hp->h_next;
    }
    
    return sum;
}

void InitAllocMediaMemory(void)
{
#ifndef MULTIMEM_BY_HEAP
    struct hole *hp;

    /* Put all holes on the free list */
    for(hp = &hole[0]; hp < &hole[NR_HOLES]; hp++)
    {
        hp->h_next = hp + 1;
    }
    hole[NR_HOLES-1].h_next = NIL_HOLE;
    hole_head = NIL_HOLE;
    free_slots = &hole[0];

    AddHole((unsigned long)gv_CommonDynaPool, SYS_DYNAL_POOL_SIZE);
    #endif
}
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
extern void CSD_UI_Free_bmpbuf();
#endif
unsigned long AllocMediaMemory(unsigned long clicks)
#ifdef MULTIMEM_BY_HEAP
{
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
    CSD_UI_Free_bmpbuf();
#endif
    return OslMalloc(clicks);
}
#else
{
    struct hole *hp, *prev_ptr = 0;
    struct block_head *pHead;
    unsigned long old_base;

    if(!clicks)
    {
        return(NO_MEM);
    }

    #ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
	CSD_UI_Free_bmpbuf();
    #endif
	
    ALIGN_MEM_SIZE(clicks);
    clicks += sizeof(struct block_head);

    /* Consume semaphore to avoid re-entrance */
    MULTIMEMMNG_SEM_CONSUME;

    hp = hole_head;
    while(hp != NIL_HOLE)
    {
        if(hp->h_len >= clicks)
        {
            /* We found a hole that is big enough.  Use it. */
            old_base = hp->h_base;  /* remember where it started */
            hp->h_base += clicks;   /* bite a piece off */
            hp->h_len -= clicks;    /* ditto */
            pHead = (struct block_head *)old_base;
            pHead->h_len = clicks;

            /* If hole is only partly used, reduce size and return. */
            if (hp->h_len != 0)
            {
                MULTIMEMMNG_SEM_PRODUCE;

                return (old_base + sizeof(struct block_head));
            }
            DelSlot(prev_ptr, hp);  
            /* Produce semaphore */
            MULTIMEMMNG_SEM_PRODUCE;
            return(old_base + sizeof(struct block_head));
        }
        prev_ptr = hp;
        hp = hp->h_next;
    }
    /* Produce semaphore */
    MULTIMEMMNG_SEM_PRODUCE;
    return(NO_MEM);
}
#endif


void FreeMediaMemory(unsigned long base)
#ifdef MULTIMEM_BY_HEAP
{
    OslMfree(base);
}
#else
{
    struct hole *hp, *new_ptr, *prev_ptr;
    struct block_head *pHead;
    unsigned long clicks;

    if(base < sizeof(struct block_head))
    {
        return;
    }
    /* Add this just for check memory exception */
    if(base & 0x03)
    {
        return;
    }
    if((base >= (unsigned long)(gv_CommonDynaPool + SYS_DYNAL_POOL_SIZE)) || (base < (unsigned long)gv_CommonDynaPool))
    {
        return;
    }

    base -= sizeof(struct block_head);
    pHead = (struct block_head *)base;
    clicks = pHead->h_len;
    if(clicks == 0) return;

    prev_ptr = 0;
    if((new_ptr = free_slots) == NIL_HOLE)
    {
        return;
    }
    /* Consume semaphore to avoid re-entrance */
    MULTIMEMMNG_SEM_CONSUME;

    new_ptr->h_base = base;
    new_ptr->h_len = clicks;
    free_slots = new_ptr->h_next;
    hp = hole_head;

    /* If this block's address is numerically less than the lowest hole currently
     * available, or if no holes are currently available, put this hole on the
     * front of the hole list.
     */
    if(hp == NIL_HOLE || base <= hp->h_base)
    {
        /* Block to be freed goes on front of the hole list. */
        new_ptr->h_next = hp;
        hole_head = new_ptr;
        Merge(new_ptr);
        /* Produce semaphore */
        MULTIMEMMNG_SEM_PRODUCE;
        return;
    }

    /* Block to be returned does not go on front of hole list. */
    while (hp != NIL_HOLE && base > hp->h_base)
    {
        prev_ptr = hp;
        hp = hp->h_next;
    }

    /* We found where it goes.  Insert block after 'prev_ptr'. */
    new_ptr->h_next = prev_ptr->h_next;
    prev_ptr->h_next = new_ptr;
    Merge(prev_ptr);
    /* Produce semaphore */
    MULTIMEMMNG_SEM_PRODUCE;
}
#endif

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
unsigned long CSD_AllocMediaMemory(unsigned long clicks)
#ifdef MULTIMEM_BY_HEAP
{
return OslMalloc(clicks);
}
#else
{
    struct hole *hp, *prev_ptr = 0;
    struct block_head *pHead;
    unsigned long old_base;

    if(!clicks)
    {
        return(NO_MEM);
    }

    ALIGN_MEM_SIZE(clicks);
    clicks += sizeof(struct block_head);

    /* Consume semaphore to avoid re-entrance */
    MULTIMEMMNG_SEM_CONSUME;

    hp = hole_head;
    while(hp != NIL_HOLE)
    {
        if(hp->h_len >= clicks)
        {
            /* We found a hole that is big enough.  Use it. */
            old_base = hp->h_base;  /* remember where it started */
            hp->h_base += clicks;   /* bite a piece off */
            hp->h_len -= clicks;    /* ditto */
            pHead = (struct block_head *)old_base;
            pHead->h_len = clicks;

            /* If hole is only partly used, reduce size and return. */
            if (hp->h_len != 0)
            {
                MULTIMEMMNG_SEM_PRODUCE;

                return (old_base + sizeof(struct block_head));
            }
            DelSlot(prev_ptr, hp);  
            /* Produce semaphore */
            MULTIMEMMNG_SEM_PRODUCE;
            return(old_base + sizeof(struct block_head));
        }
        prev_ptr = hp;
        hp = hp->h_next;
    }
    /* Produce semaphore */
    MULTIMEMMNG_SEM_PRODUCE;
    return(NO_MEM);
}
#endif



void CSD_FreeMediaMemory(unsigned long base)
#ifdef MULTIMEM_BY_HEAP
{
    OslMfree(base);
}
#else
{
    struct hole *hp, *new_ptr, *prev_ptr;
    struct block_head *pHead;
    unsigned long clicks;

    if(base < sizeof(struct block_head))
    {
        return;
    }
    /* Add this just for check memory exception */
    if(base & 0x03)
    {
        return;
    }
    if((base >= (unsigned long)(gv_CommonDynaPool + SYS_DYNAL_POOL_SIZE)) || (base < (unsigned long)gv_CommonDynaPool))
    {
        return;
    }

    base -= sizeof(struct block_head);
    pHead = (struct block_head *)base;
    clicks = pHead->h_len;
    if(clicks == 0) return;

    prev_ptr = 0;
    if((new_ptr = free_slots) == NIL_HOLE)
    {
        return;
    }
    /* Consume semaphore to avoid re-entrance */
    MULTIMEMMNG_SEM_CONSUME;

    new_ptr->h_base = base;
    new_ptr->h_len = clicks;
    free_slots = new_ptr->h_next;
    hp = hole_head;

    /* If this block's address is numerically less than the lowest hole currently
     * available, or if no holes are currently available, put this hole on the
     * front of the hole list.
     */
    if(hp == NIL_HOLE || base <= hp->h_base)
    {
        /* Block to be freed goes on front of the hole list. */
        new_ptr->h_next = hp;
        hole_head = new_ptr;
        Merge(new_ptr);
        /* Produce semaphore */
        MULTIMEMMNG_SEM_PRODUCE;
        return;
    }

    /* Block to be returned does not go on front of hole list. */
    while (hp != NIL_HOLE && base > hp->h_base)
    {
        prev_ptr = hp;
        hp = hp->h_next;
    }

    /* We found where it goes.  Insert block after 'prev_ptr'. */
    new_ptr->h_next = prev_ptr->h_next;
    prev_ptr->h_next = new_ptr;
    Merge(prev_ptr);
    /* Produce semaphore */
    MULTIMEMMNG_SEM_PRODUCE;
}
#endif
#endif

#endif  /* MULTIMEMMNG_C */

