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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/csw/branches/Total_BJ/base/ver/src/csw_ver.c $
//  $Author: mathieu $
//  $Date: 2008/07/21 10:15:54 $
//  $Revision: 19508 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file csw_ver.c
/// Implement the functions returning the 
//
////////////////////////////////////////////////////////////////////////////////


#include "chip_id.h"
#include "cs_types.h"
#include "csw_ver.h"

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
#include "hal_map_engine.h"
#else // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include "lib_version.h"
#include "hal.h"
//#include "target.h"
#include "hal_calib.h"
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)



// =============================================================================
// GLOBAL VARIABLES
// =============================================================================


#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)

// =============================================================================
// g_cswMmiMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of the MMI, when the MMI had filled it with the
/// function csw_VerSetMmiVersion().
// =============================================================================
PROTECTED HAL_MAP_VERSION_T g_cswMmiMapVersion;

#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)



// =============================================================================
// FUNCTIONS
// =============================================================================


// =============================================================================
//csw_VerGetRevision
// -----------------------------------------------------------------------------
/// Return the revision of a given module.
/// 
/// @param  moduleId Id the module.
/// @return The SVN revision of that version of the module.
// =============================================================================
PUBLIC UINT32 csw_VerGetRevision(CSW_VER_MODULE_ID_T moduleId)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
    // HAL function checks the parameter validity.
    HAL_MAP_VERSION_T* version = hal_MapEngineGetVersion(moduleId);
    
    // Check if the module has been registered.
    if (version == NULL)
    {
        return 0;
    }
    return version->revision;
    
#else // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

    return 0;
    
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
}


// =============================================================================
//csw_VerGetNumber
// -----------------------------------------------------------------------------
/// Return the version number of a given module.
/// 
/// @param  moduleId Id the module.
/// @return The version number of that version of the module.
// =============================================================================
PUBLIC UINT32 csw_VerGetNumber(CSW_VER_MODULE_ID_T moduleId)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
    // HAL function checks the parameter validity.
    HAL_MAP_VERSION_T* version = hal_MapEngineGetVersion(moduleId);
    
    // Check if the module has been registered.
    if (version == NULL)
    {
        return 0;
    }
    return version->number;
    
#else // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

    return 0;
    
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
}


// =============================================================================
//csw_VerGetDate
// -----------------------------------------------------------------------------
/// Return the date of build of a given module.
/// The format is 0xYYYYMMDD.
/// 
/// @param  moduleId Id the module.
/// @return The build date of that version of the module.
// =============================================================================
PUBLIC UINT32 csw_VerGetDate(CSW_VER_MODULE_ID_T moduleId)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
    // HAL function checks the parameter validity.
    HAL_MAP_VERSION_T* version = hal_MapEngineGetVersion(moduleId);
    
    // Check if the module has been registered.
    if (version == NULL)
    {
        return 0;
    }
    return version->date;
    
#else // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

    return 0;
    
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
}


// =============================================================================
//csw_VerGetString
// -----------------------------------------------------------------------------
/// Return the version string of a given module.
/// 
/// @param  moduleId Id the module.
/// @return The version string of that version of the module.
// =============================================================================
PUBLIC CHAR* csw_VerGetString(CSW_VER_MODULE_ID_T moduleId)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
    // HAL function checks the parameter validity.
    HAL_MAP_VERSION_T* version = hal_MapEngineGetVersion(moduleId);
    
    // Check if the module has been registered.
    if (version == NULL)
    {
        return "Not registered";
    }
    return version->string;
    
#else // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

    switch (moduleId)
    {
        case CSW_VER_MODULE_ID_MMI:
        return (CHAR*)lib_versions.mmi_version->version;
        break;
        
        case CSW_VER_MODULE_ID_HAL:
        return (CHAR*)lib_versions.phy_version->version;
        break;
        
        case CSW_VER_MODULE_ID_STACK:
        return (CHAR*)lib_versions.stack_version->version;
        break;
        
        case CSW_VER_MODULE_ID_CSW:
        return (CHAR*)lib_versions.platform_version->version;
        break;

        default:
        return "Unknown module";
        break;
    }    
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
}



// =============================================================================
//csw_VerSetMmiVersion
// -----------------------------------------------------------------------------
/// Register the version of the MMI in the CSW version mechanism.
/// 
/// @param  revision Number of the current revision (the SCM revision).
/// @param  string Pointer to the string version.
// =============================================================================
PUBLIC VOID csw_VerSetMmiVersion(UINT32 revision, CONST CHAR* string)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
    g_cswMmiMapVersion.revision = revision;
    g_cswMmiMapVersion.string = (CHAR*)string;

    hal_MapEngineRegisterModule(HAL_MAP_ID_MMI, &g_cswMmiMapVersion, NULL);
    
#else // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)


#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
}



