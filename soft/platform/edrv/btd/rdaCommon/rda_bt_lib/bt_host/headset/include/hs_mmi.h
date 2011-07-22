/*
 * MODULE NAME:    hs_mmi.h
 * DESCRIPTION:    Headset Profile Headset Application command-line MMI interface 

 * AUTHOR:         Jenny Murphy
 *
 * SOURCE CONTROL: $Id: hs_mmi.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2003 Parthus Technologies.
 *     All rights reserved.
 *
 */
#ifndef PRH_HS_MMI_H
#define PRH_HS_MMI_H

#define ADDRFILENAME "hs_addr.cfg"
#define VOLCTRLENABLED 1

extern int cmdLocallyIntiated;

void MMI_Main(void);
int MMI_RetrieveGatewayAddress(t_bdaddr *address);
int MMI_StoreGatewayAddress(t_bdaddr *address);
int MMI_SelectDevice(u_int32 classOfDeviceMask, t_bdaddr *address);
void MMI_ProcessSleep(void);
void MMI_FatalError(void);
void MMI_Ring(void);
void MMI_SCORelease(void);
void MMI_SCOActive(void);
void MMI_ACLRelease(void);
void MMI_ACLActive(void);

void MMI_SpkrVolSet(int volumeSetting);
void MMI_MicVolSet(int volumeSetting);

void MMI_NoConn(void);

extern enum linkState scoState, aclState;
extern t_bdaddr gatewayBDAddress;

#endif