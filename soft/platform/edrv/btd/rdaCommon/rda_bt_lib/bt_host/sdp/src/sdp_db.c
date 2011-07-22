/***********************************************************************
 *
 * MODULE NAME:    sdp_db.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Database code
 * MAINTAINER:     Mark Donnelly
 * CREATION DATE:  08 March 2000
 *
 * SOURCE CONTROL: $Id: sdp_db.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_config.h"

#if SDP_SERVER
#include "sdp_stack_access.h"
#include "sdp_constants.h"
#include "sdp_db.h"
#include "sdp_le.h"

#if SDP_RECORD_GEN
t_SDP_serviceRecord* prh_sdp_database = 0x00;
#elif SDP_ROM_SUPPORT
static const u_int8 sdp_db[]=  //oop, ftp,hfp AG,hsp AG
{
/*
0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06,
0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0xCD, 0xCD,
0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02,
0xCD, 0xCD, 0x02, 0x01, 0xCC, 0xCC, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00,
0x00, 0x01, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x64, 0x01, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0xB0, 0x00,
0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x1F, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x12, 0x03, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD,
0x05, 0x06, 0xCC, 0xCC, 0x0C, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0xDC, 0x00, 0x00, 0x00, 0x05, 0x06,
0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00,
0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x08, 0x00, 0x00, 0x00, 0x14, 0x01,
0x00, 0x00, 0x38, 0x01, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x1F, 0x00, 0x00,
0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x04, 0x00, 0x00, 0x00, 0x4C, 0x01, 0x00, 0x00, 0x50, 0x01,
0x00, 0x00, 0x48, 0x46, 0x50, 0x20, 0x11, 0x03, 0xCD, 0xCD, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD,
0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x7C, 0x01, 0x00, 0x00, 0x44, 0x02, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00,
0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0xA8, 0x01, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x12, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00,
0x12, 0x03, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x0C, 0x00, 0x00, 0x00, 0xBC, 0x01, 0x00, 0x00, 0xF8, 0x01, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00,
0x00, 0x00, 0xD4, 0x01, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0xCD, 0xCD, 0x05, 0x06,
0xCC, 0xCC, 0x08, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x18, 0x02, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC,
0x02, 0x00, 0x00, 0x00, 0x11, 0x12, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x03, 0xCD, 0xCD, 0x00, 0x05, 0xCC, 0xCC, 0x01, 0x00,
0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x5C, 0x02, 0x00, 0x00,
0x00, 0x03, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00, 0x7C, 0x02, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00,
0x00, 0x00, 0x11, 0x06, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x11, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xE4, 0x02, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC,
0x03, 0x00, 0x00, 0x00, 0xB4, 0x02, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0xC0, 0x02, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0xD8, 0x02,
0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC,
0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x08, 0x00,
0x00, 0x00, 0xF8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x42, 0x45, 0x58, 0x20, 0x46, 0x54, 0x50, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00,
0x00, 0x01, 0x00, 0x03, 0x18, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x2C, 0x03, 0x00, 0x00, 0x38, 0x03,
0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x05, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x11, 0x00, 0x00, 0x00, 0x4C, 0x03, 0x00, 0x00,
0xA0, 0x03, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x70, 0x03, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0x7C, 0x03, 0x00, 0x00, 0x05, 0x06,
0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x94, 0x03, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00,
0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x10, 0x00, 0x00, 0x00, 0xB4, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x42, 0x45, 0x58, 0x20, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x20,
0x50, 0x75, 0x73, 0x68
*/
0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00,
0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00,
0x4C, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0xCD, 0xCD, 0x02, 0x01, 0xCC, 0xCC, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x78, 0x01, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD,
0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x1F, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00,
0x00, 0x00, 0x12, 0x03, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x0C, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00,
0xDC, 0x00, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00,
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x08, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00, 0x00,
0x38, 0x01, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x1F, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00,
0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x18, 0x00, 0x00, 0x00, 0x4C, 0x01, 0x00, 0x00, 0x64, 0x01, 0x00, 0x00, 0x48, 0x61, 0x6E, 0x64, 0x73, 0x2D, 0x46, 0x72,
0x65, 0x65, 0x20, 0x41, 0x75, 0x64, 0x69, 0x6F, 0x20, 0x47, 0x61, 0x74, 0x65, 0x77, 0x61, 0x79, 0x11, 0x03, 0xCD, 0xCD, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x90, 0x01, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC,
0x06, 0x00, 0x00, 0x00, 0xA4, 0x01, 0x00, 0x00, 0xBC, 0x01, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x12, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x12, 0x03,
0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x0C, 0x00, 0x00, 0x00, 0xD0, 0x01, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0xE8, 0x01, 0x00, 0x00,
0x05, 0x06, 0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03,
0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x08, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x44, 0x02, 0x00, 0x00,
0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x2C, 0x02, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x12, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
0x00, 0x00, 0x00, 0x01, 0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x14, 0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x6C, 0x02, 0x00, 0x00, 0x48, 0x65, 0x61, 0x64, 0x73, 0x65, 0x74, 0x20, 0x41, 0x75, 0x64, 0x69,
0x6F, 0x47, 0x61, 0x74, 0x65, 0x77, 0x61, 0x79, 0x02, 0x03, 0xCD, 0xCD, 0x00, 0x05, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01,
0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x98, 0x02, 0x00, 0x00, 0x3C, 0x03, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0xAC, 0x02, 0x00, 0x00,
0xB8, 0x02, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x06, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x11, 0x00, 0x00, 0x00, 0xCC, 0x02, 0x00, 0x00, 0x20, 0x03,
0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0xFC, 0x02, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00,
0x14, 0x03, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00,
0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x08, 0x00, 0x00, 0x00, 0x34, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x4F, 0x42, 0x45, 0x58, 0x20, 0x46, 0x54, 0x50, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x54, 0x03, 0x00, 0x00, 0x20, 0x04,
0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x68, 0x03, 0x00, 0x00, 0x74, 0x03, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x05, 0x00, 0x00,
0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x11, 0x00, 0x00, 0x00, 0x88, 0x03, 0x00, 0x00, 0xDC, 0x03, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0xAC, 0x03, 0x00, 0x00, 0x05, 0x06,
0xCC, 0xCC, 0x05, 0x00, 0x00, 0x00, 0xB8, 0x03, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0xD0, 0x03, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08,
0x00, 0x00, 0x00, 0x01, 0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x10, 0x00, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x4F, 0x42, 0x45, 0x58, 0x20, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x20,
0x50, 0x75, 0x73, 0x68, 0x03, 0x03, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x14, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xCC, 0xCC, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00,
0x00, 0x00, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x38, 0x04, 0x00, 0x00, 0x10, 0x05, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC,
0x03, 0x00, 0x00, 0x00, 0x4C, 0x04, 0x00, 0x00, 0x58, 0x04, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x0A, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x10, 0x00,
0x00, 0x00, 0x6C, 0x04, 0x00, 0x00, 0xB4, 0x04, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x84, 0x04, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x9C, 0x04, 0x00, 0x00,
0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x19,
0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x08, 0x00, 0x00, 0x00, 0xC8, 0x04, 0x00, 0x00, 0xEC, 0x04, 0x00, 0x00,
0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0xD4, 0x04, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x0D, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
0x00, 0x00, 0x00, 0x01, 0xCD, 0xCD, 0x05, 0x04, 0xCC, 0xCC, 0x10, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x41, 0x64, 0x76, 0x61, 0x6E, 0x63, 0x65, 0x64, 0x20, 0x41, 0x75,
0x64, 0x69, 0x6F, 0x20, 0x00, 0x00, 0xCD, 0xCD, 0x02, 0x01, 0xCD, 0xCD, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x05, 0x28, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xCD, 0xCD, 0x05, 0x06,
0xCC, 0xCC, 0x03, 0x00, 0x00, 0x00, 0x3C, 0x05, 0x00, 0x00, 0x48, 0x05, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x0C, 0x00, 0x00, 0x04, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC,
0x10, 0x00, 0x00, 0x00, 0x5C, 0x05, 0x00, 0x00, 0xA4, 0x05, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x74, 0x05, 0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x8C, 0x05,
0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00,
0x00, 0x17, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0xCD, 0xCD, 0x05, 0x06, 0xCC, 0xCC, 0x08, 0x00, 0x00, 0x00, 0xB8, 0x05, 0x00, 0x00, 0xDC, 0x05,
0x00, 0x00, 0x05, 0x06, 0xCC, 0xCC, 0x06, 0x00, 0x00, 0x00, 0xC4, 0x05, 0x00, 0x00, 0x01, 0x03, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x11, 0x0C, 0x00, 0x00, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00,
0x01, 0x00, 0x00, 0x00, 0x11, 0x03, 0xCD, 0xCD, 0x01, 0x01, 0xCC, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
t_SDP_serviceRecord * const prh_sdp_database = (t_SDP_serviceRecord*)sdp_db;
#endif

#if SDP_RECORD_GEN

static void prh_sdp_db_init(void);
static void prh_sdp_db_modify_state(void);
static void prh_sdp_db_create_record_state(t_SDP_serviceRecord* pRecord);
static void prh_sdp_modify_record_state(t_SDP_serviceRecord* pRecord);
static void prh_sdp_db_free_record(t_SDP_serviceRecord* pRecord);

u_int32 prh_sdp_primitive_id = 0x0000ffff;

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_add
 * DESCRIPTION:
 
 This function will create a new attribute assign place the data in its
 value attribute. The attribute will be placed at the end of the 
 service records attribute list. If an entry exists with the same aid then
 the request will be ignored. Any memory that was allocated will be freed.

 * KNOWN ISSUES:
 
 none.
 *
 *
 ***********************************************************************/
u_int8 prh_sdp_add(t_SDP_serviceRecord* pRecord,u_int16 id,t_SDP_dataElement* pData)
{
	u_int8 ret_value;
    
	t_SDP_attribute* pAttribute;
	t_SDP_attribute* pTemp;
    
	pAttribute = &(pRecord->attribute);
    
	ret_value = BT_NOERROR;
    
#if COMBINED_HOST==0
	pTemp = (t_SDP_attribute*)pMalloc(sizeof(t_SDP_attribute));
#endif	
	if(pTemp)
    {
		
		if(id == SDP_AID_RECORD_HANDLE)
		{ /* not allowed */
			ret_value = BT_INVALIDPARAM;      
			
#if COMBINED_HOST==0
			pFree(pTemp);
#endif
		} else {
			while(pAttribute->p_next_attribute != 0x00)
			{
				if(pAttribute->id == id)
					break;
				
				pAttribute = pAttribute->p_next_attribute;      
			}
			
			if(pAttribute->id !=id)
			{
				pAttribute->p_next_attribute = pTemp;
				pTemp->id = id;
				pTemp->value = *pData;
				pTemp->p_next_attribute = 0x00;
				
				prh_sdp_db_modify_record_state(pRecord);
			} else {
				ret_value = BT_INVALIDPARAM;
#if COMBINED_HOST==0
				pFree(pTemp);
#endif
			}
		}   
    } else {
		ret_value = BT_NORESOURCES;
    }
	
	return ret_value;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_add_record
 * DESCRIPTION:
 
 This function will add a service record to the list of service records.
 It will initialise that service record with the appropiate handle. If
 the service list is empty then it will insert its entry at the head.
 Else it will attatch itself at the end. The entry will not be inserted if
 the record handle already exists.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8 prh_sdp_add_record(u_int32 handle)
{
	u_int8 ret_value;
	t_SDP_serviceRecord* pRecord;
	t_SDP_serviceRecord* pTemp;
	u_int8* pValue;
	u_int32 value=0;
	u_int8* pData;
    
	pRecord = prh_sdp_database;
    
#if COMBINED_HOST==0
	pTemp = (t_SDP_serviceRecord*)pMalloc(sizeof(t_SDP_serviceRecord));
#endif	
	ret_value = BT_NOERROR;
	
	if(pTemp)
    {
		
		
		pData = (u_int8*)&pTemp->attribute.value.data.local;
		
		prh_sdp_write32(handle,pData);
		
		pTemp->attribute.id = SDP_AID_RECORD_HANDLE;
		pTemp->attribute.value.size = 2;
		pTemp->attribute.value.type = SDP_UINT;
		pTemp->attribute.value.data_length = 4;
		pTemp->attribute.p_next_attribute = 0x00;
		
		prh_sdp_db_create_record_state(pTemp);
		
		pTemp->p_next_record = 0x00;	      
		
		if(pRecord == 0x00)
		{
			prh_sdp_db_init(); /* create the servers record */
			
			prh_sdp_database->p_next_record = pTemp;
		} else {
			while(pRecord->p_next_record != 0x00)
			{
				pValue = (u_int8*)&pRecord->attribute.value.data.local;
				value = prh_sdp_read32(pValue);
				
				if(value == handle)
					break;
				
				pRecord = pRecord->p_next_record;
			}
			
			if(value != handle)
			{
				prh_sdp_db_modify_state(); /* update the state of the database. */
				
				pRecord->p_next_record = pTemp;
			} else { 
#if COMBINED_HOST==0
				pFree(pTemp);
#endif
				ret_value = BT_UNKNOWNERROR;
			}
		}
    } else {
		ret_value = BT_NORESOURCES;
    }
	
	return ret_value;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_remove_record
 * DESCRIPTION:
 
 This function will remove and entry from the sdp service record list.
 When the entry is removed the database state will be updated also.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8 prh_sdp_remove_record(u_int32 handle)
{
	t_SDP_serviceRecord* pRecord;
	t_SDP_serviceRecord* pPrevious;
	u_int32 value;
	
	pRecord = pPrevious = prh_sdp_database;
	
	if(!handle)
		return BT_UNSUPPORTED;
	
	while(pRecord)
    {
		value = prh_sdp_read32((u_int8*)&pRecord->attribute.value.data.local);
		
		if(value == handle)
		{
			if(pPrevious == pRecord)
			{
				/* i.e the first one */
				prh_sdp_database = pRecord->p_next_record;    
			} else {
				pPrevious->p_next_record = pRecord->p_next_record;
			}
			/* free that record */
			
			prh_sdp_db_free_record(pRecord);			
			prh_sdp_db_modify_state();
			return BT_NOERROR;
		}
		
		pPrevious = pRecord;
		pRecord = pRecord->p_next_record;
    }
	
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_flush_database
 * DESCRIPTION:
 
 This function will remove and entry from the sdp service record list.
 When the entry is removed the database state will be updated also.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8  prh_sdp_flush_database(void)
{
	t_SDP_serviceRecord* pRecord;
	t_SDP_serviceRecord* pNext;
	
	pRecord = prh_sdp_database;
	
	/*	pDebugCheck(); */
	
	while(pRecord)
    {
		pNext = pRecord->p_next_record;
		
		prh_sdp_db_free_record(pRecord);
		
		/*	pDebugCheck(); */
		
		pRecord = pNext;
    }
	
	prh_sdp_database = 0x00;
	
	/*	pDebugCheck(); */
	
	
	prh_sdp_primitive_id = 0x0000ffff;
	
	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_encode_data
 * DESCRIPTION:
 
 This function will take raw data and copy it into a buffer that is 
 malloc'd to store it. The type and size paramters are passed in.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8 prh_sdp_encode_data(t_SDP_dataElement* data,u_int8* raw_data)
{
	u_int8* p_buffer;
	u_int8 ret_value;
#if BTHOST_BIGENDIAN==0
	u_int8 counter;
#endif
	
	ret_value = BT_NOERROR;
	
	data->data.local = 0;
	
	if(data->size < 5)
    {   
		if(data->size > 2)
		{
#if COMBINED_HOST==0
			data->data.pointer = (u_int8*)pMalloc(data->data_length);
#endif			
			p_buffer = data->data.pointer;
		} else {
			p_buffer = (u_int8*)&data->data.local;
		}
		
		if(p_buffer)
		{
#if BTHOST_BIGENDIAN
			pMemcpy(p_buffer,raw_data,data->data_length);
#else
			counter = data->data_length -1 ;
			do {
				p_buffer[data->data_length - 1 - counter] = raw_data[counter];
			} while( counter--);
#endif
		} else {
			ret_value = BT_NORESOURCES;
		}      
		
    } else {
#if COMBINED_HOST==0
		p_buffer = (u_int8*)pMalloc(data->data_length);
#endif		
		if(p_buffer)
		{
			pMemcpy(p_buffer,raw_data,data->data_length);
			data->data.pointer = p_buffer;
		} else { 
			ret_value = BT_NORESOURCES;
		}
    }
	
	return ret_value;
}




/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_encode_list
 * DESCRIPTION:
 
 This function will take a list of t_SDP_dataElement pointers and will
 place them in a list of at the end of the data. I have created space for
 the list of pointers as well as the pointers themselves.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8 prh_sdp_encode_list(t_SDP_dataElement* value, u_int32 num_elements, t_SDP_dataElement** elements)
{
	u_int32 counter;
	u_int32 size;
	u_int8 ret_value;
	
	t_SDP_dataElement* storage;
	
	ret_value = BT_NOERROR;
	size = 0;
	
#if COMBINED_HOST==0
	storage = (t_SDP_dataElement*)pMalloc(sizeof(t_SDP_dataElement) * num_elements);
#endif	
	if(storage)
	{
		
		value->type = SDP_SEQUENCE;
		value->data.pointer = (u_int8*)storage;
		
		for(counter = 0; counter < num_elements; counter++)
		{
			storage[counter] = *elements[counter];
			
			/* next I need to determine the size */
			switch(elements[counter]->size)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:	  
				size += elements[counter]->data_length +1;
				break;	  
			case 5:
				size += elements[counter]->data_length + SDP_SMALL_HEADER;
				break;
			case 6:	  
				size += elements[counter]->data_length + SDP_MEDIUM_HEADER;
				break;
			case 7:
				size += elements[counter]->data_length + SDP_LARGE_HEADER;
				break;	
			}
		}
		
		/*
		now to set up the size field of the list itself.
		*/
		
		value->data_length = size;
		
		
		if(size < 1<<8)
		{      
			value->size = 5;
		} else if(size < 1<<16) {
			value->size = 6;
		} else if(size <= 0xffffffff ) {
			value->size = 7;      
		}			
		
	} else {
		ret_value = BT_NORESOURCES;
	}
	
	return ret_value;
}

#endif /* SDP_RECORD_GEN */


#if SDP_ROM_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_flush_database
 * DESCRIPTION:
 
 This function will remove and entry from the sdp service record list.
 When the entry is removed the database state will be updated also.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8  prh_sdp_flush_database(void)
{
#if COMBINED_HOST==0
	if(prh_sdp_database)
		pFree(prh_sdp_database);
	
	//prh_sdp_database = 0x00;
#endif
	return BT_NOERROR;
}

#endif /* SDP_ROM_SUPPORT */

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_query
 * DESCRIPTION:
 
 This function will search a record for an attribute.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8 prh_sdp_query(t_SDP_serviceRecord* pRecord, u_int16 attribute_id,t_SDP_dataElement** pElement)
{
	u_int8 ret_value;
    
	t_SDP_attribute* p_attribute;
    
	ret_value  = 0;
    
	p_attribute = &(pRecord->attribute);
    
	do {
		if(p_attribute->id == attribute_id)
			break;
#if SDP_ROM_SUPPORT
		p_attribute = (t_SDP_attribute*)(p_attribute->p_next_attribute ? (u_int8*)prh_sdp_database + (u_int32)p_attribute->p_next_attribute : 0x00);
#else
		p_attribute = p_attribute->p_next_attribute;
#endif /* SDP_ROM_SUPPORT */
		
	} while (p_attribute != 0x00);
    
    
	if(p_attribute != 0x00)
    {
		*pElement = &(p_attribute->value);
		ret_value = 1;
    } else {
		*pElement = 0x00;
    }
    
	return ret_value;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_valid_record
 * DESCRIPTION:
 
 This function will search through the list of service records and see
 if there is an entry that contains the service_record_handle if there
 is then it will return 1 else it will return 0. It will also change
 pRec to point to that record if it is successfull else it should be
 ignored.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8 prh_sdp_valid_record(u_int32 service_record_handle, t_SDP_serviceRecord** pRec)
{
	t_SDP_serviceRecord* pRecord;
	u_int8* pValue;
	u_int32 value;
	u_int8 ret_value;
    
	pRecord = prh_sdp_database;
    
	ret_value = 0;
	
	do {      
		pValue = (u_int8*)&pRecord->attribute.value.data.local; /* first one always the service record handle */
        
		value = prh_sdp_read32(pValue);
        
		if( value == service_record_handle)
		{
			break;
		}
#if SDP_ROM_SUPPORT
		pRecord = (t_SDP_serviceRecord*)(pRecord->p_next_record ? (u_int8*)prh_sdp_database + (u_int32)pRecord->p_next_record : 0x00);
#else
		pRecord = pRecord->p_next_record;
#endif /* SDP_ROM_SUPPORT */
	} while( pRecord != 0x00);
    
	if(pRecord != 0x00)
    {
		ret_value = 1;
		*pRec = pRecord;
    } else {
		*pRec = 0x00;
	}
    
	return ret_value;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_service_search
 * DESCRIPTION:
 
 This function will look through the database a service record at at time
 for each service record it will find all the uuids and store them in a 
 list. It will then compair the uuids against the serach pattern and if a
 pattern matches it will add the record handel for that service record to 
 the list of service records and it will increment the number of handles.

 * KNOWN ISSUES:
  
 The ammount of uuids that it finds in the database is limited at the
 moment to 20.

 *
 *
 ***********************************************************************/
u_int32 prh_sdp_service_search(u_int32* pSearchPattern,u_int32 number_uuids, u_int16* number_handles,u_int32** pHandles)
{
	u_int32 ret_value;
	t_SDP_serviceRecord* pRecord;
	u_int32 lcv;
	u_int32 lcv_2;
	u_int32 counter;
	u_int32 matches;
	t_SDP_attribute* pAttribute;
	u_int8* pValue;
#if COMBINED_HOST==0
	u_int32* handles = NULL;
	t_SDP_dataElement** pUUIDs;
#else
	static u_int32 handles[5];
	t_SDP_dataElement* pUUIDs[8];
#endif
	pRecord = prh_sdp_database;
	lcv = 0;
	*number_handles = 0;
	counter = 0;
    
	if(!pRecord)
	{
		number_handles = 0x00;
		*pHandles = 0x00;
		return 0;
	}
	
	ret_value = 0;
    
	do { /* find out the number of service_records */
		lcv++; 
#if SDP_ROM_SUPPORT
		pRecord = (t_SDP_serviceRecord*)(pRecord->p_next_record ? (u_int8*)prh_sdp_database + (u_int32)pRecord->p_next_record : 0x00);
#else
		pRecord = pRecord->p_next_record;
#endif /* SDP_ROM_SUPPORT */
		
	} while( pRecord != 0x00);
    
#if COMBINED_HOST==0
	handles = (u_int32*)pMalloc( lcv << 2 );
    
	pUUIDs = (t_SDP_dataElement**)pMalloc( sizeof(t_SDP_dataElement*) * 20);
#endif
    
	if(!(handles && pUUIDs))
		return BT_NORESOURCES;

	pRecord = prh_sdp_database;
	
#if SDP_ROM_SUPPORT
	pAttribute = (t_SDP_attribute*)(pRecord->attribute.p_next_attribute ? (u_int8*)prh_sdp_database + (u_int32)pRecord->attribute.p_next_attribute : 0x00);
#else
	pAttribute = pRecord->attribute.p_next_attribute;
#endif /* SDP_ROM_SUPPORT */
	
	if(pAttribute != 0x00)
    {
            do 
            {
            
			do 
                    {
				if(pAttribute == 0x00)
					break;
                /* now check to see if it is a uuid or a list types */
				switch(pAttribute->value.type)
				{
				case SDP_UUID:
					pUUIDs[counter] = &(pAttribute->value);
					counter++;
					/* add it to the list */
					break;
				case SDP_SEQUENCE:
				case SDP_UNION:
					prh_sdp_traverse_list(&(pAttribute->value),pUUIDs,&counter);
					break;
				}
                
#if SDP_ROM_SUPPORT
				pAttribute = (t_SDP_attribute*)(pAttribute->p_next_attribute ? (u_int8*)prh_sdp_database + (u_int32)pAttribute->p_next_attribute : 0x00);
#else
				pAttribute = pAttribute->p_next_attribute;
#endif /* SDP_ROM_SUPPORT */      
			} while (pAttribute != 0x00);
            
			/* check and see if the uuid match the pattern */
            
			for(lcv=0,matches=0;lcv<number_uuids;lcv++)
			{
				for(lcv_2 = 0;lcv_2< counter; lcv_2++)
				{
					if(prh_sdp_uuid_compair(pSearchPattern[lcv],pUUIDs[lcv_2]))
					{
						matches++;
						break;
					}
				}
			}
			
			if(matches == number_uuids)
			{
				pValue = (u_int8*)&pRecord->attribute.value.data.local;
				
				handles[*number_handles] = prh_sdp_read32(pValue);
				(*number_handles)++;
                /* copy in a handle value */
			}
            
			counter = 0;
            
            
#if SDP_ROM_SUPPORT
			
			pRecord = (t_SDP_serviceRecord*)(pRecord->p_next_record ? (u_int8*)prh_sdp_database + (u_int32)pRecord->p_next_record : 0x00);
#else
			pRecord = pRecord->p_next_record;
#endif /* SDP_ROM_SUPPORT */
			
			if(pRecord!=0x00)
#if SDP_ROM_SUPPORT
				pAttribute = (t_SDP_attribute*)(pRecord->attribute.p_next_attribute ? (u_int8*)prh_sdp_database + (u_int32)pRecord->attribute.p_next_attribute : 0x00);
#else 
			pAttribute = pRecord->attribute.p_next_attribute;
#endif /* SDP_ROM_SUPPORT */
			
		} while( pRecord != 0x00);
    }
    
#if COMBINED_HOST==0
	pFree(pUUIDs);
#endif
    
	*pHandles = handles;  
	
	return ret_value;
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_traverse_list
 * DESCRIPTION:
 
 This function will move though a list searching for uuids of each one 
 it finds it will place it in a list that will be passed back though the
 call chain.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int16 prh_sdp_traverse_list(t_SDP_dataElement* pElement,t_SDP_dataElement** pUUIDs,u_int32* uuid_counter)
{
	u_int16 ret_value;
	
	/* first determine the ammount of data in the list */
	
	u_int16 data_len;
	u_int16 lcv;
	u_int16 lcv_2;
	u_int16 counter;
	
	u_int16 temp;
	
	t_SDP_dataElement* pTemp;
	ret_value = 1;
	
	data_len = pElement->data_length;
	
	for(counter = 0,lcv_2 = 0 ; lcv_2 < data_len; counter++)
	{
#if SDP_ROM_SUPPORT
		pTemp = ((t_SDP_dataElement*)  ((u_int8*)prh_sdp_database +  (u_int32)pElement->data.pointer) ) + counter;
#else
		pTemp = ((t_SDP_dataElement*)pElement->data.pointer) + counter;
#endif /* SDP_ROM_SUPPORT */	 
		
		if(pTemp->size < 5)
		{
			lcv = 1 << pTemp->size;
			ret_value += 1 << pTemp->size;
		} else {
			lcv = 1 << (5 - pTemp->size);
			ret_value += lcv;
		}	 
		
		lcv_2 += lcv;
		
		switch(pTemp->type)
		{
		case SDP_UUID:
			/* add it to the list */
			pUUIDs[*uuid_counter] = pTemp;
			(*uuid_counter)++;
			
			lcv_2++; /* add header info for uuid */
			ret_value++;
			break;
		case SDP_SEQUENCE:
		case SDP_UNION:
			temp = prh_sdp_traverse_list(pTemp,pUUIDs,uuid_counter);
			lcv_2 += temp;
			ret_value += temp;
			break;
		default:
			lcv_2++;
			ret_value++;
		}
	}
	
	return ret_value;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_uuid_compair
 * DESCRIPTION:
 
 This function will compare one uuid against the other it will not handle
 the comparison of full 128 bit as it will just check the 4 bytes at the 
 top of those values. This function is called in the service search it is
 always passed a 16 or 32 bit uuid.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8 prh_sdp_uuid_compair(u_int32 Pattern,t_SDP_dataElement* pElement)
{
	u_int32 temp_value=0;
	
	switch(pElement->size)
	{
	case 1:
		temp_value = prh_sdp_read16((u_int8*)&pElement->data.local);
		break;
	case 2:
		temp_value = prh_sdp_read32((u_int8*)&pElement->data.local);
		break;
	case 4:
#if SDP_ROM_SUPPORT
		temp_value = prh_sdp_read32((u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer);
#else 
		temp_value = prh_sdp_read32(pElement->data.pointer);
#endif
		break;
		
	}
	
	return (temp_value == Pattern) ? 0x01 : 0x00;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_list_handles
 * DESCRIPTION:
 
 This function will return a pointer to a list of record handles.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32* prh_sdp_list_handles(u_int16 *p_num_handles)
{
	u_int32* pHandles;
	u_int8* pValue;
	t_SDP_serviceRecord* pRecord;
	
	u_int32 count;
	
	if(!p_num_handles)
		return 0x00;
	
#if SDP_ROM_SUPPORT
	for(count = 0,pRecord = prh_sdp_database; pRecord; pRecord = (t_SDP_serviceRecord*)(pRecord->p_next_record ? (u_int8*)prh_sdp_database + (u_int32)pRecord->p_next_record : 0x00))
#else
		for(count = 0,pRecord = prh_sdp_database; pRecord; pRecord = pRecord->p_next_record)
#endif /* SDP_ROM_SUPPORT */
		{
			count++;
		}
		
#if COMBINED_HOST==0
		pHandles = pMalloc(sizeof(u_int32) * count);
		if(!pHandles)
			return 0x00;
#endif

#if SDP_ROM_SUPPORT
		for(count = 0,pRecord = prh_sdp_database;pRecord;pRecord = (t_SDP_serviceRecord*)(pRecord->p_next_record ? (u_int8*)prh_sdp_database + (u_int32)pRecord->p_next_record : 0x00),count++)
#else
			for(count = 0,pRecord = prh_sdp_database;pRecord;pRecord = pRecord->p_next_record,count++)
#endif /* SDP_ROM_SUPPORT */
			{
				pValue = (u_int8*)&pRecord->attribute.value.data.local;
				pHandles[count] = prh_sdp_read32(pValue);
			}   
			
			*p_num_handles = count;
			
			return pHandles;
}

#if SDP_RECORD_GEN

/***********************************************************************
 *
 * FUNCTION NAME:   _sdp_db_init
 * DESCRIPTION:
 
 This function will create the sdp service record for the sdp server 
 itself. This will contain the version number and the database state 
 attributes.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
static void prh_sdp_db_init(void)
{
	t_SDP_serviceRecord* pRecord;
	u_int8* pData;
	
	t_SDP_dataElement* list[10];
	
	t_SDP_dataElement service_class,class_list,sdp_version,version_list,sdp_database_state;
	
	u_int16 _service_class,_sdp_version;
	u_int32 _state;
	
	
	pRecord = pMalloc(sizeof(t_SDP_serviceRecord));

	if(!pRecord)
	{
#if pDEBUG
		pDebugPrintfEX((pLOGFATAL,pLOGSDP,"Unable to malloc main database record..."));
#else
               SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,LOG_SDP_SDP_DB,NULL,0,0);
#endif
		return;
	}

	pData = (u_int8*)&pRecord->attribute.value.data.local;
	
	pData[0] = 0x00;
	pData[1] = 0x00;
	pData[2] = 0x00;
	pData[3] = 0x00;
	pRecord->attribute.id = SDP_AID_RECORD_HANDLE;
	pRecord->attribute.value.size = 2;
	pRecord->attribute.value.type = SDP_UINT;
	pRecord->attribute.value.data_length = 4;
	pRecord->attribute.p_next_attribute = 0x00;
	
	pRecord->p_next_record = 0x00;
	
	/* now to create some of the attributes */
	
	/* service_class_list */
	service_class.type = SDP_UUID;
	service_class.size = 1;
	service_class.data_length = 2;
	_service_class = SDP_SCLASS_UUID_SDS;
	prh_sdp_encode_data(&service_class,(u_int8*)&_service_class);
    
	list[0] = &service_class;
	
	prh_sdp_encode_list(&class_list,1,list);
	
	/* Version Number List */
	sdp_version.type = SDP_UINT;
	sdp_version.size = 1;
	sdp_version.data_length = 2;
	_sdp_version = SDP_VERSION; /* version 1.0 */
	prh_sdp_encode_data(&sdp_version,(u_int8*)&_sdp_version);
	
	list[0] = &sdp_version;
	
	prh_sdp_encode_list(&version_list,1,list);
	
	/* database state */
	
	sdp_database_state.type = SDP_UINT;
	sdp_database_state.size = 2;
	sdp_database_state.data_length = 4;
	_state = 0x00000001;
	prh_sdp_encode_data(&sdp_database_state,(u_int8*)&_state);
	
	/* add the attributes to the record */
	
	prh_sdp_add(pRecord,SDP_AID_SERVICE_CLASS,&class_list);
	prh_sdp_add(pRecord,SDP_AID_VERSION_LIST,&version_list);
	prh_sdp_add(pRecord,SDP_AID_DATABASE_STATE,&sdp_database_state);
	
	prh_sdp_database = pRecord;
}


/***********************************************************************
 *
 * FUNCTION NAME:   _sdp_db_modify_state
 * DESCRIPTION:
 
 This function will change the ServiceDatabaseState attribute.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
static void prh_sdp_db_modify_state(void)
{
	u_int32 value;
	
	u_int8* pData;
	
	pData = (u_int8*)&prh_sdp_database->attribute.p_next_attribute->p_next_attribute->p_next_attribute->value.data.local;
	
	value = prh_sdp_read32(pData);
	value++;  
	prh_sdp_write32(value,pData); 
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_db_create_record_state
 * DESCRIPTION:
 
 This function will create a state attribute for a service record.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
static void prh_sdp_db_create_record_state(t_SDP_serviceRecord* pRecord)
{
	u_int32 _record_state;
	t_SDP_dataElement record_state;
	
	record_state.type = SDP_UINT;
	record_state.size = 2;  
	record_state.data_length = 4;
	_record_state = 0x00000001;  /* init state */
	prh_sdp_encode_data(&record_state,(u_int8 *)&_record_state);
	
	prh_sdp_add(pRecord,SDP_AID_RECORD_STATE,&record_state);
}


/***********************************************************************
 *
 * FUNCTION NAME:   _sdp_db_mofidy_record_state
 * DESCRIPTION:
 
 This function will change the record state variable of a service
record.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
static void prh_sdp_modify_record_state(t_SDP_serviceRecord* pRecord)
{
	u_int32 value;
	
	u_int8* pData;
	
	pData = (u_int8*)&pRecord->attribute.value.data.local;
	if(pData[0] || pData[1] || pData[2] || pData[3]) /* skip the servers record */
    {
		pData = (u_int8*)&pRecord->attribute.p_next_attribute->value.data.local;
		
		if(pRecord->attribute.p_next_attribute->id == 0x0002)
		{
			value = prh_sdp_read32(pData);
			value++;
			prh_sdp_write32(value,pData);
		}
    }
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_db_mofidy_record_state
 * DESCRIPTION:
 
 This function will change the record state variable of a service
record. by calling the internal function.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_db_modify_record_state(t_SDP_serviceRecord* pRecord)
{
	prh_sdp_modify_record_state(pRecord);
}




/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_allocate_record_handle
 * DESCRIPTION:
 
 This function will allocate the record handles starting from 0x00010000
 
 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_sdp_allocate_record_handle(void)
{
	return ++prh_sdp_primitive_id;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_free_nested_element
 * DESCRIPTION:
 
 This function will recursively deallocate a sdp_dataElement it will not 
deallocate the top element.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_sdp_free_nested_element(t_SDP_dataElement* pElement,t_SDP_dataElement* stop)
{
	t_SDP_dataElement* pTemp;
	u_int32 ret_value = 0;
	u_int32 counter= 0;  
	
	if(pElement->type == SDP_SEQUENCE || pElement->type == SDP_UNION)
	{
		
		for(; ret_value < pElement->data_length; counter++)
		{
			pTemp = ((t_SDP_dataElement*)pElement->data.pointer) + counter;
			
			ret_value += 1 + ((pTemp->size <5) ? (1 << pTemp->size) : (1 << (pTemp->size -5)) );
			
			if(pTemp->type == SDP_SEQUENCE || pTemp->type == SDP_UNION)
			{
				ret_value += prh_sdp_free_nested_element(pTemp,stop);
			} else {
				if(pTemp->size > 2)
				{
					pFree(pTemp->data.pointer);
				}
				ret_value += pTemp->data_length;
			}
		}
		/* free the storage */
		pFree(pElement->data.pointer);
    } else {	
		if(pElement->size > 2)
		{
			pFree(pElement->data.pointer);
		}
	}
	/* not needed I am not so sure so I will leave it in */
	if(pElement->type != SDP_SEQUENCE && pElement->type != SDP_UNION)
    {
		if(pElement !=stop)
			pFree(pElement);
    }
	
	return ret_value;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_free_attribute
 * DESCRIPTION:
 
 This function will use recursion to free a list of attributes.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_free_attribute(t_SDP_attribute* p_attribute)
{
	if(p_attribute)
    {
		prh_sdp_free_attribute(p_attribute->p_next_attribute);      
		prh_sdp_free_nested_element(&p_attribute->value,&p_attribute->value);
		pFree(p_attribute);
    }
}


/***********************************************************************
 *
 * FUNCTION NAME:   _sdp_db_free_record
 * DESCRIPTION:
 
 This function will free a complete record.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
static void prh_sdp_db_free_record(t_SDP_serviceRecord* pRecord)
{
	t_SDP_attribute* p_attribute;
	
	p_attribute = pRecord->attribute.p_next_attribute;  
	prh_sdp_free_attribute(p_attribute);
	
	prh_sdp_free_nested_element(&pRecord->attribute.value,&pRecord->attribute.value);
	
	pFree(pRecord);
}

#if SDP_ROM_CREATION

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_determine_value_size
 * DESCRIPTION:
 
 This function will return the number of bytes that a value will take up 
 in memory.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_sdp_determine_value_size(t_SDP_dataElement* input)
{
	u_int32 val_size;
	u_int32 nested;
	t_SDP_dataElement *finger;
	u_int8 headers;
	
	val_size = 0;
	
	val_size += sizeof(t_SDP_dataElement);
	
	if(input->size > 2)
	{
		if(input->type == SDP_SEQUENCE || input->type == SDP_UNION)
		{
			nested = input->data_length;
			finger = (t_SDP_dataElement*)input->data.pointer;
			
			while(nested)
			{
				val_size += prh_sdp_determine_value_size(finger);
				
				if(finger->size > 4)
				{
					headers = 1 + (1<<(finger->size-5));
				} else {
					headers = 1;
				}
				
				nested -= headers + finger->data_length;
				finger++;
			}
			
		} else {
			val_size += input->data_length;
		}
	}
	
	return val_size;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_determine_attributes_size
 * DESCRIPTION:
 
 This function will return the size in bytes for the attributes of a 
 service record.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_sdp_determine_attributes_size(t_SDP_attribute* input)
{
	u_int32 attrib_size;
	t_SDP_attribute* pAttrib;
	
	attrib_size = 0;
	
	pAttrib = input;
	
	while(pAttrib)
	{
		attrib_size += sizeof(t_SDP_attribute);
		attrib_size -= sizeof(t_SDP_dataElement);
		
		attrib_size += prh_sdp_determine_value_size(&pAttrib->value);
		
		pAttrib = pAttrib->p_next_attribute;
	}
	
	return attrib_size;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_determine_database_size
 * DESCRIPTION:
 
 This function will return the size in bytes for the entire database
 in memory.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_sdp_determine_database_size(void)
{
	u_int32 database_size;
	t_SDP_serviceRecord* pRecord;
	
	database_size = 0;
	
	pRecord = prh_sdp_database;
	
	while(pRecord)
	{
		database_size += sizeof(t_SDP_serviceRecord);
		
		database_size += prh_sdp_determine_attributes_size(pRecord->attribute.p_next_attribute);
		
		pRecord = pRecord->p_next_record;
	}
	
	return database_size;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_value_to_static
 * DESCRIPTION:
 
 This function will convert a value from a dynamic format to a flattened
 format in a buffer.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_value_to_static(t_SDP_dataElement* input, u_int8* base, u_int32* finger)
{
	t_SDP_dataElement* pData;
	t_SDP_dataElement* pTemp;
	u_int32 nested;
	u_int32 starter;
	u_int8 headers;
	
	
	starter = *finger;
	
	if(input->size > 2)
	{
		if(input->type == SDP_SEQUENCE || input->type == SDP_UNION)
		{
			nested = input->data_length;
			pData = (t_SDP_dataElement*)input->data.pointer;
			
			/*
			first of all I need to determine how many elements are in the list
			as all the structs are placed down first followed by the data.
			*/
			
			while(nested)
			{
				pTemp = (t_SDP_dataElement*)(base + *finger);
				*pTemp = *pData;
				*finger += sizeof(t_SDP_dataElement);
				
				if(pData->size > 4)
				{
					headers = 1 + (1<<(pData->size-5));
				} else {
					headers = 1;
				}
				
				nested -= headers + pData->data_length;
				
				pData++;
			}
			
			/* now to fill in the missing data */
			
			nested = input->data_length;
			pData = (t_SDP_dataElement*)input->data.pointer;
			
			while(nested)
			{
				pTemp = (t_SDP_dataElement*)(base + starter);
				starter += sizeof(t_SDP_dataElement);
				/* check do I need to set the pointer */
				
				if(pTemp->size > 2)
					pTemp->data.pointer = (unsigned char*)*finger;
				
				prh_sdp_value_to_static(pData,base,finger);
				
				if(pData->size > 4)
				{
					headers = 1 + (1<<(pData->size-5));
				} else {
					headers = 1;
				}
				
				nested -= headers + pData->data_length;
				
				pData++;
			}
			
			
		} else {
			pMemcpy(base + *finger,input->data.pointer,input->data_length);
			*finger += input->data_length;
		}
	}
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_attribute_to_static
 * DESCRIPTION:
 
 This function will convert a list of attributes from a dynamic format 
 to a flattened format in a buffer.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_attribute_to_static(t_SDP_attribute* input,u_int8* base, u_int32* finger)
{
	t_SDP_attribute* pAttrib;
	t_SDP_attribute* pTemp;
	
	pAttrib = input;
	
	while(pAttrib)
	{
		pTemp = (t_SDP_attribute*)(base + *finger);
		*pTemp = *pAttrib;
		*finger += sizeof(t_SDP_attribute);
		
		if(pTemp->value.size > 2)
			pTemp->value.data.pointer = (u_int8*)*finger;
		
		prh_sdp_value_to_static(&pAttrib->value,base,finger);
		
		if(pTemp->p_next_attribute)
		{
			pTemp->p_next_attribute = (t_SDP_attribute*)*finger;
		}
		
		pAttrib = pAttrib->p_next_attribute;
	}
	
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_db_to_static
 * DESCRIPTION:
 
 This function will convert an entire database from a dynamic format to 
 a flattened format in a buffer.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8* prh_sdp_db_to_static(u_int32 length)
{
	u_int8* static_db;
	u_int32 finger;
	t_SDP_serviceRecord *pRecord;
	t_SDP_serviceRecord* pTemp;
	
	static_db = pMalloc(length);
	
	
	finger = 0;
	
	if(static_db)
	{
		pMemset(static_db,0,length);

		pRecord = prh_sdp_database;
		
		while(pRecord)
		{
			pTemp = (t_SDP_serviceRecord*)(static_db + finger);
			*pTemp = *pRecord;
			finger += sizeof(t_SDP_serviceRecord);
			
			if(pTemp->attribute.p_next_attribute)
			{
				pTemp->attribute.p_next_attribute = (t_SDP_attribute*)finger;
				prh_sdp_attribute_to_static(pRecord->attribute.p_next_attribute,static_db,&finger);
			}
			if(pTemp->p_next_record)
				pTemp->p_next_record = (t_SDP_serviceRecord*)finger;
			
			pRecord = pRecord->p_next_record;
		}
	}
	
	return static_db;
}

#endif /* SDP_ROM_CREATION */


#endif /* SDP_RECORD_GEN */


#endif /* SDP_SERVER */
