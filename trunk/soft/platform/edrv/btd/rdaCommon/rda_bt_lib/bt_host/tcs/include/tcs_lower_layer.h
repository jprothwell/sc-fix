/*
 * MODULE NAME:    tcs_lower_layer.h
 * DESCRIPTION:    TCS Lower Layer Include File
 * AUTHOR:         Dave Airlie
 * DATE:           05 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_lower_layer.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_LOWER_LAYER_H
#define __TCS_LOWER_LAYER_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "host_types.h"
#include "host_buf.h"
#include "host_error.h"
#include "papi.h"
#include "hci_constants.h"
#include "l2_const.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_l2cap.h"
#include "stubs/unit_stub_manager.h"
#else
#include "l2cap_primitives.h"
#endif



#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif
