/******************************************************************************/
/*              Copyright (C) 2003, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/*                                                                            */
/**
    @file   pal_flsh_p.h
    
    @brief This file defines the private functions to complete the stack 
    access to flash memory
*/
/*                                                                            */
/******************************************************************************/


#ifndef __PAL_FLSH_P_H__
#define __PAL_FLSH_P_H__

#include "sxs_type.h"

#define PAL_FLASH

#define PAL_FLASHRESERVEDDATA 8

/**
    Read the Sector to the memory buffer, this must be done before the 
    flash task is started
*/
void palFlashOpen(void);

/** 
    Write the memory buffer to the Sector, this must be done at the very end, 
    when the flash task is stopped
*/
void palFlashClose(void);

/**
    Read the Xtal value from the flash sector
    - if the tag is valid *pXtal is updated and the function returns \c TRUE
    - if the tag is not valid *pXtal is unchanged and the function returns
    \c FALSE
*/
BOOL palReadFlashXtal( UINT32 *pXtal );

/**
    Set the Xtal value to by programmed at palFlashClose
*/
void  palWriteFlashXtal( UINT32 Xtal );

#endif
