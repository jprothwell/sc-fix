#ifndef _PAL_MEM_H
#define _PAL_MEM_H

#include "sxs_type.h"

#define PAL_RAM_BASE            0x80000000
#define PAL_INTERNAL_RAM_BASE   0x80C00000

#define PAL_CACHED(addr)    ((addr)&0xdfffffff)
#define PAL_UNCACHED(addr)  ((addr)|0x20000000)

#define PAL_FUNC_INTERNAL   __attribute__((section(".sramtext")))
#define PAL_DATA_INTERNAL   __attribute__((section(".sramdata")))


#endif /* _PAL_MEM_H */

