/*
 * MODULE NAME:    hci_encoder.h
 * DESCRIPTION:    HCI PDU Lookup Tables Header File
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_encoder.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef prh_HCI_TABLES_H
#    define prh_HCI_TABLES_H

#include "host_config.h"
#include "host_types.h"
#include "hci_constants.h"

t_api HCI_GenerateCommand(int opcode, ... );


#define prh_hci_max_command_group		0x06	
#define prh_hci_max_event_number		0x20		


struct prh_st_t_hci_opcode_entry {	
	u_int8 parmOffset;
	u_int8 numArgs;
};

struct prh_st_t_hci_group_entry {
	const struct prh_st_t_hci_opcode_entry *opcodeArray;
	u_int8 numOpcodes;
};


typedef u_int8 prh_t_hci_Parm_types; 


/* these values are lengths in bytes, except for prh_hci_param_handle */
#define prh_hci_param_uint8			1
#define prh_hci_param_uint16		2
#define prh_hci_param_uint24		3	/* used for LAP parameters */
#define prh_hci_param_uint32		4
#define prh_hci_param_bdaddr		6	
										
#define prh_hci_param_8bytes		8	/* used for PIN code */
#define prh_hci_param_10bytes		10
#define prh_hci_param_16bytes		16	/* used for link key */
	
#define prh_hci_param_handle		50	/* real length=2 bytes  */
#define prh_hci_param_240bytes		240	
#define prh_hci_param_string		248	


#endif /* prh_HCI_TABLES_H */

