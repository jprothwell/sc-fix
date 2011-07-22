////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/dualsimd/include/dualsim_config.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file dualsim_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the dualsim in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _DUALSIM_CONFIG_H_
#define _DUALSIM_CONFIG_H_

#include "cs_types.h"
#include "hal_spi.h"
#include "hal_gpio.h"




// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// DUALSIM_CFG_T
// -----------------------------------------------------------------------------
/// Configuration structure for the DUALSIM driver CS
// =============================================================================
typedef struct
{    
    HAL_SPI_ID_T            spiBus;
    HAL_SPI_CS_T            spiCs;
    
} DUALSIM_CONFIG_T;



#endif // _DUALSIM_CONFIG_H_




