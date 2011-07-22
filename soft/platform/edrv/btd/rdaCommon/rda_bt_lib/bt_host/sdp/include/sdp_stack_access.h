/***********************************************************************
 *
 * MODULE NAME:    sdp_stack_access.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery interfaces for the rest of the stack
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_stack_access.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_types.h"
#include "papi.h"

#include "host_ctrl.h"
#include "host_error.h"


#if PRH_HOST_UNIT_TEST==1
#include "stubs/unit_stub_l2cap.h"
#else
#include "l2cap_primitives.h"
#include "l2_utils.h"
#if COMBINED_HOST==0
#include "mgr_secdb.h"
#include "mgr_core.h"
#endif
#endif

#if PRH_L2CAP_AUTO_CONFIG==1
#include "l2_config_helper.h"
#endif

#if COMBINED_HOST==0
#include "mgr_devdb.h"
#endif
#include "host_buf.h"
