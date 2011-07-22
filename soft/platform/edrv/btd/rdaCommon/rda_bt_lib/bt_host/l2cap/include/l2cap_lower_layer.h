/******************************************************************
 * MODULE NAME:    l2cap_lower_layer.h
 * PROJECT CODE:
 * DESCRIPTION:    l2cap_lower_layer
 * MAINTAINER:     Gary Fleming
 * DATE:           21 July 1999
 *
 * SOURCE CONTROL: $Id: l2cap_lower_layer.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *    
 ******************************************************************/

#ifndef _PARTHUS_L2CAP_LOWER_LAYERS_
#define _PARTHUS_L2CAP_LOWER_LAYERS_

#if COMBINED_HOST==0
#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_hci.h"
#include "stubs/unit_stub_manager.h"

#else
#include "mgr_core.h"
#include "mgr_secdb.h"
#include "mgr_secdb.h"
#include "hci_primitives.h"
#endif

#include "mgr_utils.h"

#endif

#endif
