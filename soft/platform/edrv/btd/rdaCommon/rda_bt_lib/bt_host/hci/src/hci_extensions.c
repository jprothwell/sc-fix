/*
 * MODULE NAME:    hci_extensions.c
 * DESCRIPTION:    Proprietary HCI extensions 
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_extensions.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"
#include "hci_extensions.h"
#include "hci_primitives.h"
#include "hci_transport.h"
#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_hci_fc.h"
#else
#include "hci_flow_ctrl.h"
#endif

int TCI_Transfer_PDU(unsigned char* pdu_ptr, int pdu_len);

/*************************************************************************************************
 * HCI_GenerateCommand
 *
 * Builds a HCI_AddHardwareSCO() command PDU
 *
 * This command creates a SCO connection similar to HCI_AddSCOConnection, but the
 * source of the data traffic is taken from an on-board source instead of from the host.
 * 
 * This command uses proprietary HCI OGF=0x3F, OCF=0x101
 *
 * Parameters:
 *  Existing ACL Handle         2 bytes
 *  Configuration Specifier     4 bytes (currently reserved, must be 0x00) 
 *
 * Events Generated:
 *  This command generates a ConnectionComplete event in the same manner as HCI_AddSCOConnection.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_AddHardwareSCO(u_int16 aclHandle, u_int32 config) {

    struct host_buf *pdu;
    t_api status;

    status=prh_hci_GetCommandBuffer(3+HCI_cmdADDHARDWARESCO_LEN+HEADERS_FOR_HCITRANSPORT, 0, &pdu);
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_AddHardwareSCO failed to allocate command buffer for %d bytes\n",3+HCI_cmdADDHARDWARESCO_LEN+HEADERS_FOR_HCITRANSPORT));
        return BT_NORESOURCES;
    }

    pdu->data[0]=HCI_cmdADDHARDWARESCO&0xFF;
    pdu->data[1]=(HCI_cmdADDHARDWARESCO>>8)&0xFF;
    pdu->data[2]=HCI_cmdADDHARDWARESCO_LEN;

    pdu->data[3]=aclHandle&0xFF;
    pdu->data[4]=(aclHandle&0xFF00)>>8;

    pdu->data[5]=config&0xFF;
    pdu->data[6]=(config&0xFF00)>>8;
    pdu->data[7]=(config&0xFF0000)>>16;
    pdu->data[8]=(config&0xFF000000)>>24;

    pdu->len=3+HCI_cmdADDHARDWARESCO_LEN;

    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: Sending add_hardware_sco command, aclHandle=0x%04X, config=0x%04X",aclHandle,config));


    /* Send the pdu to the flow control queue */
    status=prh_hci_fc_send_command_pdu(pdu); 

    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_AddHardwareSCO failed send command PDU - prh_hci_fc_send_command_pdu() returned error %d\n",status));
        prh_hci_ReleaseCommandBuffer(pdu);
    }

    return status;
}

int TCI_WriteLocalDeviceAddress(u_int8* bd_addr)
{
    unsigned char pdu[9];
    pdu[0] = (unsigned char)TCI_WRITE_LOCAL_DEVICE_ADDRESS;
    pdu[1] = (unsigned char)(TCI_WRITE_LOCAL_DEVICE_ADDRESS>>8);
    pdu[2] = 6; // Length
    pMemcpy(&(pdu[3]), bd_addr, 6);

    return TCI_Transfer_PDU( pdu, 9);
}

int TCI_ReadPHYRegister(u_int8 flag, u_int32 address)
{
    unsigned char pdu[9];

    pdu[0] = (u_int8)TCI_READ_PHY_REGISTER;
    pdu[1] = TCI_READ_PHY_REGISTER>>8;
    pdu[2] = 6;
    pdu[3] = flag;
    pdu[4] = 1; /* length */
    pdu[5] = (unsigned char)(address & 0xFF);
    pdu[6] = (unsigned char)((address & 0xFF00) >> 8);
    pdu[7] = (unsigned char)((address & 0xFF0000) >> 16);
    pdu[8] = (unsigned char)((address & 0xFF000000) >> 24);

    return TCI_Transfer_PDU(pdu, 9);
}

int TCI_WritePHYRegister(u_int8 flag, u_int32 address, u_int8 count, u_int32 *value)
{
    unsigned char pdu[256];
    int i;

    pdu[0] = (u_int8)TCI_WRITE_PHY_REGISTER;
    pdu[1] = TCI_WRITE_PHY_REGISTER>>8;
    pdu[2] = 6 + count * 4;
    pdu[3] = flag;
    pdu[4] = count; /* length */
    pdu[5] = (unsigned char)(address & 0xFF);
    pdu[6] = (unsigned char)((address & 0xFF00) >> 8);
    pdu[7] = (unsigned char)((address & 0xFF0000) >> 16);
    pdu[8] = (unsigned char)((address & 0xFF000000) >> 24);
	for(i=0; i<count; i++)
	{
		pdu[9 + i * 4] = (unsigned char)(value[i] & 0xFF);
		pdu[10+ i * 4] = (unsigned char)((value[i] & 0xFF00) >> 8);
		pdu[11+ i * 4] = (unsigned char)((value[i] & 0xFF0000) >> 16);
		pdu[12+ i * 4] = (unsigned char)((value[i] & 0xFF000000) >> 24);
	}
    
    return TCI_Transfer_PDU(pdu, 9 + count * 4);
}

int TCI_BaseBand_TXTest( u_int8 hop_frq, u_int8 msg_sco, u_int8 msg_burst,
						u_int8 msg_type, u_int8 power_level,u_int16 PRbs_init, u_int16 msg_data,
						u_int8 tx_packet_type, u_int8 EDR_mode, u_int32 tx_len,
						u_int8 am_addr, u_int32 syncword_low, u_int32 syncword_hight)
{
    unsigned char pdu[27];

    pdu[0] = (u_int8)TCI_BASEBAND_TXTEST;
    pdu[1] = TCI_BASEBAND_TXTEST>>8;
    pdu[2] = 24;
	pdu[3] = hop_frq;
	pdu[4] = msg_sco;
	pdu[5] = msg_burst;
	pdu[6] = msg_type;
	pdu[7] = tx_packet_type;
	pdu[8] = EDR_mode;
	pdu[9] = power_level; // pad
	pdu[10] = 0; // pad
    pdu[11] = (unsigned char)(syncword_low & 0xFF);
    pdu[12] = (unsigned char)((syncword_low & 0xFF00) >> 8);
    pdu[13] = (unsigned char)((syncword_low & 0xFF0000) >> 16);
    pdu[14] = (unsigned char)((syncword_low & 0xFF000000) >> 24);
    pdu[15] = (unsigned char)(syncword_hight & 0xFF);
    pdu[16] = (unsigned char)((syncword_hight & 0xFF00) >> 8);
    pdu[17] = (unsigned char)((syncword_hight & 0xFF0000) >> 16);
    pdu[18] = (unsigned char)((syncword_hight & 0xFF000000) >> 24);
    pdu[19] = (unsigned char)(PRbs_init & 0xFF);
    pdu[20] = (unsigned char)((PRbs_init & 0xFF00) >> 8);
    pdu[21] = (unsigned char)(msg_data & 0xFF);
    pdu[22] = (unsigned char)((msg_data & 0xFF00) >> 8);
	pdu[23] = am_addr;
	pdu[24] = 0; // pad
	pdu[25] = (unsigned char)(tx_len & 0xFF);
	pdu[26] = (unsigned char)((tx_len & 0xFF00) >> 8);

    return TCI_Transfer_PDU(pdu, 27);
}

int TCI_BaseBand_RXTest(void)
{
    unsigned char pdu[3];
	
    pdu[0] = (u_int8)TCI_BASEBAND_RXTEST;
    pdu[1] = TCI_BASEBAND_RXTEST>>8;
    pdu[2] = 0;
    return TCI_Transfer_PDU(pdu, 3);
}

int TCI_BaseBand_TestEnd(void)
{
    unsigned char pdu[3];

    pdu[0] = (u_int8)TCI_BASEBAND_TESTEND;
    pdu[1] = TCI_BASEBAND_TESTEND>>8;
    pdu[2] = 0;
    return TCI_Transfer_PDU(pdu, 3);
}

int TCI_Transfer_PDU(unsigned char* pdu_ptr, int pdu_len)
{
    struct host_buf *pdu;
    t_api status;

    status=prh_hci_GetCommandBuffer(pdu_len, 0, &pdu);
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"TCI_Transfer_PDU failed to allocate command buffer for %d bytes\n",3+HCI_cmdADDHARDWARESCO_LEN+HEADERS_FOR_HCITRANSPORT));
        return BT_NORESOURCES;
    }
    pMemcpy(pdu->data, pdu_ptr, pdu_len);

    pdu->len=pdu_len;

    pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: Sending tci command, opcode=0x%02X", pdu_ptr[0]));


    /* Send the pdu to the flow control queue */
    status=prh_hci_fc_send_command_pdu(pdu); 

    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"TCI_Transfer_PDU failed send command PDU - prh_hci_fc_send_command_pdu() returned error %d\n",status));
        prh_hci_ReleaseCommandBuffer(pdu);
    }

    return status;
}


