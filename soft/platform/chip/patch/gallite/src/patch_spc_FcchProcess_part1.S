//------------------------------------------------------------------------------
//              Copyright (C) 2010, Coolsand Technologies, Inc.               
//                            All Rights Reserved                             
//------------------------------------------------------------------------------
/// @file patch_spc_FcchProcess_part1.S
///
/// This file contains FCCH process related functions.
///
//  $HeadURL: http://10.10.100.14/svn/developing1/Sources/chip/trunk/patch/gallite/src/patch_spp_fcchDetect.S $ //
//  $Author: huazeng $                                                        // 
//  $Date: 2010-10-28 22:05:00 +0800 (星期四, 28 十月 2010) $                     //   
//  $Revision: 3230 $                                                        //   
//------------------------------------------------------------------------------

#ifdef ENABLE_PATCH_SPAL_IFCDISABLERF

#include "regdef.h"

.section .sramtext, "awx", @progbits

.set mips16
.globl	patch_spc_FcchProcess_part1
.ent	patch_spc_FcchProcess_part1
patch_spc_FcchProcess_part1:

li	a1,1
sb	a1,997(s0)
nop
cmpi 	s1,2
bteqz	addr_tmp
lw	v0,addr_b
jr	v0
addr_tmp:
lw	v0,addr_bteqz
jr v0

addr_bteqz: .word 0x81e1078b
addr_b: .word 0x81e116a3
.end	patch_spc_FcchProcess_part1 
.set nomips16

#endif // ENABLE_PATCH_SPAL_IFCDISABLERF
