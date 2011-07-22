/*
 * MODULE NAME:    rfcomm_lower_layer.h
 * DESCRIPTION:    RFCOMM Lower Layer Include File
 * AUTHOR:         Dave Airlie
 * DATE:           05 March 2001
 *
 * SOURCE CONTROL: $Id: rfcomm_lower_layer.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __RFCOMM_LOWER_LAYER_H
#define __RFCOMM_LOWER_LAYER_H

#include "host_config.h"

#include "host_types.h"
#include "host_buf.h"
#include "host_error.h"
#include "host_ctrl.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_papi.h"
#include "stubs/unit_stub_l2cap.h"

#undef PRH_RFCOMM_INITIAL_CREDITS
extern u_int8 prh_ut_rftest_initial_credits;
#define PRH_RFCOMM_INITIAL_CREDITS prh_ut_rftest_initial_credits

#else
#include "papi.h"
#include "l2cap_primitives.h"
#endif

#endif
