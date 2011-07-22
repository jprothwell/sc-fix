//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spalp_gdb_stub.h
///
/// Header for the defines used by GDB stub. 
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------


#ifndef  SPALP_GDB_STUB_H
#define  SPALP_GDB_STUB_H


//------------------------------------------------------------------------------
// Defines 
//------------------------------------------------------------------------------

#define SPAL_GDB_REGNUM           38
// all register + command and parameter for command
#define SPAL_GDB_REGNUM_XTD       (SPAL_GDB_REGNUM+3)
#define SPAL_GDB_CONTEXT_SIZE     (SPAL_GDB_REGNUM_XTD*4)

#define SPAL_GDB_CMD_OFF          SPAL_GDB_REGNUM
#define SPAL_GDB_CMDPARAM_OFF     (SPAL_GDB_REGNUM + 1)
#define SPAL_GDB_CMDPARAM2_OFF    (SPAL_GDB_REGNUM + 2)

// command between cpus and debug server
#define SPAL_GDB_CMD_DONE         0x00

#define SPAL_GDB_CMD_FLUSH_CACHE  0x01
#define SPAL_GDB_CMD_STEP         0x02
#define SPAL_GDB_CMD_CONTINUE     0x04
#define SPAL_GDB_SET_BREAKPOINT    0x05
#define SPAL_GDB_UNSET_BREAKPOINT  0x06

#define SPAL_GDB_CMD_BREAKPOINT   0x10
#define SPAL_GDB_CMD_PRINT        0x20

/* Exception codes written in ram and used by jds for diag status */
/* Also used by XCPU to detect BCPU crash cause */
#if 0
// moved to hal :%s/SPAL_\([[:alnum:]_]*\)_EXC_WORD/SPAL_ERROR_\1
#define SPAL_NO_EXC_WORD          0xC0010000

#define SPAL_IT_EXC_WORD          0x1717
#define SPAL_TLB_EXC_WORD         0x71B0
#define SPAL_ASSERT_EXC_WORD      0xA55E
#define SPAL_GDB_EXC_WORD         0x06DB 
#define SPAL_PAGE_EXC_WORD        0xFA9E
#define SPAL_WDOG_EXC_WORD        0xAD09
#define SPAL_DIV0_EXC_WORD        0xD1B0
#define SPAL_RAISE_EXC_WORD       0xA15E
#define SPAL_BCPU_EXC_WORD        0xBBBB
#endif


#endif /* SPAL_GDB_STUB_H */
