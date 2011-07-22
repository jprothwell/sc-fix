/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2003, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*  This source code is the property of Coolsand Technologies and is     */
/*  confidential. Any modification, distribution, reproduction or        */
/*  exploitation of any content of this file is totally forbidden,       */
/*  except with the written permission of Coolsand Technologies.         */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/**
    @file   pal_gsm.c
    
    @brief  This file contains the PAL API & IRQ functions
*/
/*                                                                       */
/*************************************************************************/

// Common includes
#include "cs_types.h"
#include "string.h"

#include "hal_mem_map.h"

// MEMD includes
#include "memd_m.h"

// PAL includes
#include "pal_flsh.h"
#include "pal_flsh_p.h" // private header
#include "palp_cfg.h"

// Target includes, to access the factory settings.
#include "tgt_m.h"

#define   IMEI_LENGTH  9
//--------------------------------------------------------------------//
//                                                                    //
//                    PAL FLASH Internal Structure                    //
//                                                                    //
//--------------------------------------------------------------------//

static char pal_flash_buffer[PAL_FLASHSTACKDATASIZE + PAL_FLASHRESERVEDDATA];
static BOOL pal_flash_written = FALSE;

// (see 3GPP TS 23.003) 
// (see 24.008 section 10.5.1.4)
//static UINT8 g_palDefaultImeiSv[9] =
//    { 0x03, 0x21, 0x43, 0x65, 0x87, 0x09, 0x21, 0x43, 0xf5 };

/// 359759002514931
/// 359738009553407
/// 352740015810083
/// 356261013740590


static UINT8 g_palDefaultImeiSv_4sim[4][9] =
    {
      { 0x33, 0x95, 0x57, 0x09, 0x20, 0x15, 0x94, 0x13, 0xf0 },
      { 0x33, 0x95, 0x37, 0x08, 0x90, 0x55, 0x43, 0x70, 0xf0 },
      { 0x33, 0x25, 0x47, 0x00, 0x51, 0x18, 0x00, 0x38, 0xf0 },
      { 0x33, 0x65, 0x62, 0x01, 0x31, 0x47, 0x50, 0x09, 0xf0 }
    };
//--------------------------------------------------------------------//
//                                                                    //
//                       PAL FLASH API functions                      //
//                                                                    //
//--------------------------------------------------------------------//

VOID* pal_ReadFlash( UINT16 Offset )
{
    PAL_ASSERT(Offset < PAL_FLASHSTACKDATASIZE, "pal_ReadFlash: Offset (= %d) out of boudaries.", Offset);
    return (VOID*) (pal_flash_buffer + Offset + PAL_FLASHRESERVEDDATA);
}

VOID  pal_WriteFlash( VOID* Data, UINT16 Offset, UINT16 Len )
{
    PAL_ASSERT(Offset < PAL_FLASHSTACKDATASIZE, "pal_WriteFlash: Offset out of boudaries.", Offset);
    PAL_ASSERT(Offset + Len < PAL_FLASHSTACKDATASIZE, "pal_WriteFlash: Len out of boudaries.(Offset= %d) ", Offset);
    pal_flash_written = TRUE;
    memcpy(pal_flash_buffer + Offset + PAL_FLASHRESERVEDDATA, Data, Len);
}


//======================================================================
// Phone's IMEI Getter : 
//----------------------------------------------------------------------
/// This function returns the IMEI of the phone. It is usually
/// retrieved from the factory settings of the flash. Check the CRC of
/// the factory settings structure and the "factory step" bit is set,
/// then get the IMEI. Otherwise, return the default IMEI.
///
/// One IMEI is defined
/// per SIM in the case where the phone has several SIMs.
///
/// @param simIndex Index of the SIM whose related IMEI is to be
/// returned. Use 0 when the phone is single-sim.
/// @return Pointer to the IMEI for the sim index passed as a parameter.
//======================================================================
PUBLIC VOID* pal_GetImei(UINT8 simIndex)
{
    CONST TGT_MOBILE_ID_T* mobileId = tgt_GetMobileId();
	
    BOOL    factorySettingsImeiIsWritten;
    UINT8   factorySettingsCrc;
    UINT8   computedCrc;

    // Check IMEI validity: written status and CRC check.
    factorySettingsImeiIsWritten = (mobileId->validMarker & (1 << 15)) == 0;
    factorySettingsCrc = (mobileId->validMarker >> 24) & 0xFF;
    computedCrc = ((mobileId->validMarker & 0x00FFFFFF) % 255) & 0xFF;

	
    if ((factorySettingsCrc == computedCrc) && factorySettingsImeiIsWritten)
    {
        switch(simIndex)
        {
            case 0:
                    return (VOID*)mobileId->imeiSv[0];
            case 1:
                    return (VOID*)mobileId->imeiSv[1];
            case 2:
                    return (VOID*)mobileId->imeiSv[2];
            case 3:
                    return (VOID*)mobileId->imeiSv[3];
            default:
                break;
        }
	}

//    return g_palDefaultImeiSv;
    return g_palDefaultImeiSv_4sim[simIndex];
}



//--------------------------------------------------------------------//
//                                                                    //
//                    PAL FLASH Internal functions                    //
//                                                                    //
//--------------------------------------------------------------------//

VOID palFlashOpen(VOID)
{
    UINT32 rdsize = 0;
    memd_FlashRead((UINT8*)PAL_FLASHSTACKDATALOCATION, PAL_FLASHSTACKDATASIZE + PAL_FLASHRESERVEDDATA, &rdsize, pal_flash_buffer);
}

VOID palFlashClose(VOID)
{
    UINT32 wrsize = 0;
    if (pal_flash_written) {
        memd_FlashErase((UINT8*)PAL_FLASHSTACKDATALOCATION, NULL);
        memd_FlashWrite((UINT8*)PAL_FLASHSTACKDATALOCATION, PAL_FLASHSTACKDATASIZE + PAL_FLASHRESERVEDDATA, &wrsize, pal_flash_buffer);
    }
}

BOOL palReadFlashXtal( UINT32 *pXtal )
{
    if (0xCDACCDAC != *((UINT32*)pal_flash_buffer))
        return FALSE;
    *pXtal = pal_flash_buffer[8];
    return TRUE;
}

VOID  palWriteFlashXtal( UINT32 Xtal )
{
    *((UINT32*)(&(pal_flash_buffer[4]))) = Xtal;
    *((UINT32*)pal_flash_buffer) = 0xCDACCDAC;
    pal_flash_written = TRUE;
}

