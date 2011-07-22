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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/src/lcddp.h $
//	$Author: huazeng $
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $
//	$Revision: 1532 $
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file
/// Features to be used only inside the module (protected).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// =============================================================================
// lcdd_ReadReg
// -----------------------------------------------------------------------------
/// Write the address and then read a data from the LCD register
/// 
/// @param addr Address of the register to read.
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PROTECTED VOID lcdd_ReadReg(UINT16 addr, UINT16* pData);



// =============================================================================
// lcdd_ReadData
// -----------------------------------------------------------------------------
/// Read a data to the LCD screen.
///
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PROTECTED VOID lcdd_ReadData(UINT16* pData);


