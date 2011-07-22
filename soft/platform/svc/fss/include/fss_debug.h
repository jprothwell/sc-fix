////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//  $HeadURL$
//  $Author$
//  $Date$
//  $Revision$
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file fss_debug.h
///
/// File System service debug function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _FSS_DEBUG_H_
#define _FSS_DEBUG_H_

#include "cs_types.h"

#include "sxs_io.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================

#define SVC_FSS_TRACE SXS_TRACE

#define DL_ERROR                (0x0)/*MUST be set 0*/
#define DL_WARNING              (DL_ERROR + 1 )
#define DL_BRIEF                (DL_WARNING + 1 )
#define DL_DETAIL               (DL_BRIEF + 1 )
#define DL_MAX                  (DL_DETAIL + 1 )

#define DL_VDSERROR                 (0x0)/*MUST be set 0*/
#define DL_VDSWARNING               (DL_VDSERROR + 1 )
#define DL_VDSBRIEF                 (DL_VDSWARNING + 1 )
#define DL_VDSDETAIL                (DL_VDSBRIEF + 1 )
#define DL_VDSMAX                   (DL_VDSDETAIL + 1 )

#define DL_FATERROR                 (DL_VDSMAX+1)/*MUST be set 0*/
#define DL_FATWARNING               (DL_FATERROR + 1 )
#define DL_FATBRIEF                 (DL_FATWARNING + 1 )
#define DL_FATDETAIL                (DL_FATBRIEF + 1 )
#define DL_FATMAX                   (DL_FATDETAIL + 1 )

#define DL_VFSERROR                 (DL_FATMAX + 1)  
#define DL_VFSWARNING              (DL_VFSERROR + 1) 
#define DL_VFSBRIEF                 (DL_VFSWARNING + 1)
#define DL_VFSDETAIL                (DL_VFSBRIEF + 1)
#define DL_VFSMAX                   (DL_VFSDETAIL + 1) 

#ifndef ERR_SUCCESS
#define ERR_SUCCESS                 0x0
#endif

// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


#endif // _FSS_DEBUG_H_
