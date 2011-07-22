/***********************************************************************
 *
 * MODULE NAME:    tci.h
 * DESCRIPTION:    Bluetooth Host Stack TCI API Header
 * AUTHOR:         Jenny Murphy
 *
 * SOURCE CONTROL: $Id: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 *
 ***********************************************************************/


#ifndef BLUETOOTH_TCI_API_DECLARED
#define BLUETOOTH_TCI_API_DECLARED

int TCI_WriteLocalDeviceAddress(u_int8* bd_addr);

int TCI_ReadPHYRegister(u_int8 flag, u_int32 address);
int TCI_WritePHYRegister(u_int8 flag, u_int32 address, u_int8 count, u_int32 *value);

int TCI_BaseBand_TXTest( u_int8 hop_frq, u_int8 msg_sco, u_int8 msg_burst,
						u_int8 msg_type, u_int8 power_level,u_int16 PRbs_init, u_int16 msg_data,
						u_int8 tx_packet_type, u_int8 EDR_mode, u_int32 tx_len,
						u_int8 am_addr, u_int32 syncword_low, u_int32 syncword_hight);
int TCI_BaseBand_RXTest(void);
int TCI_BaseBand_TestEnd(void);


#endif /* BLUETOOTH_TCI_API_DECLARED */