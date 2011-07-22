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
//
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/cams/include/cams_config.h $
//  $Author: romuald $
//  $Date$
//  $Revision: 16551 $
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _CAMS_CONFIG_H_
#define _CAMS_CONFIG_H_


// =============================================================================
// CAMS_CONFIG_T
// -----------------------------------------------------------------------------
/// Configuration structure for CAMS.
/// Defines parameters such as the sensor orientation, etc.
// =============================================================================
typedef struct
{
    /// Sensor orientation, as the parameter needed by
    /// cams_ResizeVgaToScreen to display the image in the
    /// right direction.
    UINT32 sensorOrientation;
} CAMS_CONFIG_T;


#endif // _CAMS_CONFIG_H_











