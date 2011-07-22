/************************************************************************
 * MODULE NAME:    l2_channel.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap layer configuration
 * MAINTAINER:     Gary Fleming
 * DATE:           20-08-2000
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *
 * Description :
 * 
 * 
 **************************************************************************/

#ifndef __L2_LAYER_CONFIG_H
#define __L2_LAYER_CONFIG_H

#define  RELEASE_CHANNEL_ON_TIMEOUT 1
#if (HOST_LOWER_TESTER==1)
#define  PRH_L2CAP_INTERNAL_HOST_TESTER 1
#else
#define  PRH_L2CAP_INTERNAL_HOST_TESTER 0
#endif

#endif
