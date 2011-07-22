/*
 * MODULE NAME:    hci_extensions.h
 * DESCRIPTION:    Proprietry HCI extensions header
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_extensions.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef prh_HCI_EXTENSIONS_H
#	define prh_HCI_EXTENSIONS_H

#include "host_config.h"
#include "host_buf.h"
#include "host_error.h"
#include "papi.h"

#define PROP_OGF                        0x3f

#define HCI_cmdADDHARDWARESCO_LEN       0x6
#define HCI_cmdADDHARDWARESCO			((0x3f<<10) | 0x101)		/* OGF=0x3F, OCF=0x101 */

#define TCI_WRITE_LOCAL_DEVICE_ADDRESS   ((0x3f<<10) | 0x1a)

#define TCI_READ_PHY_REGISTER			 ((0x3f<<10) |0x101)
#define TCI_WRITE_PHY_REGISTER			 ((0x3f<<10) |0x102)

#define TCI_BASEBAND_TXTEST				 ((0x3f<<10) |0x110)
#define TCI_BASEBAND_RXTEST				 ((0x3f<<10) |0x111)
#define TCI_BASEBAND_TESTEND			 ((0x3f<<10) |0x112)


APIDECL1 t_api APIDECL2 HCI_AddHardwareSCO(u_int16 aclHandle, u_int32 config);

int TCI_WriteLocalDeviceAddress(u_int8* bd_addr);

int TCI_ReadPHYRegister(u_int8 flag, u_int32 address);
int TCI_WritePHYRegister(u_int8 flag, u_int32 address, u_int8 count, u_int32 *value);

int TCI_BaseBand_TXTest( u_int8 hop_frq, u_int8 msg_sco, u_int8 msg_burst,
						u_int8 msg_type, u_int8 power_level,u_int16 PRbs_init, u_int16 msg_data,
						u_int8 tx_packet_type, u_int8 EDR_mode, u_int32 tx_len,
						u_int8 am_addr, u_int32 syncword_low, u_int32 syncword_hight);
int TCI_BaseBand_RXTest(void);
int TCI_BaseBand_TestEnd(void);


#endif /* prh_HCI_EXTENSIONS_H */
