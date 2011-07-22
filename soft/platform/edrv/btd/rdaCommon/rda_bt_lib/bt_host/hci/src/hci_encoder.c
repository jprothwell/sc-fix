/*
 * MODULE NAME:    hci_encoder.c
 * DESCRIPTION:    HCI Command PDU Lookup Tables and Encode Function
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_encoder.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */


#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "hci_primitives.h"
#include "hci_encoder.h"
#include "papi.h"
#include "host_buf.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_hci_fc.h"
#include "hu_config.h"
static last_opcode = 0;
u_int16 hci_return_error = 0;
extern hci_test;
#else
#include "hci_flow_ctrl.h"
#endif


/* 
    prh_hci_command_parm_types[] is a Compound list of possible permutations of 
    parameter types in commands. A command parameter list is identified by an 
    offset and length into this table. (Exceptions are handled individually).
*/
/* const modifier allows placing these tables in ROM for linkers which support this */
const prh_t_hci_Parm_types prh_hci_command_parm_types[] = {
    prh_hci_param_bdaddr, 
    prh_hci_param_uint8, 
    prh_hci_param_uint8, 
    prh_hci_param_uint16, 
    prh_hci_param_uint8, 
    prh_hci_param_uint16, 
    prh_hci_param_uint16, 
    prh_hci_param_uint24, 
    prh_hci_param_uint8, 
    prh_hci_param_uint8, 
    prh_hci_param_bdaddr, 
    prh_hci_param_uint8, 
    prh_hci_param_16bytes, 
    prh_hci_param_bdaddr, 
    prh_hci_param_16bytes, 
    prh_hci_param_bdaddr, 
    prh_hci_param_uint16, 
    prh_hci_param_uint8, 
    prh_hci_param_uint8, 
    prh_hci_param_uint16, 
    prh_hci_param_handle, 
    prh_hci_param_uint8, 
    prh_hci_param_uint8, 
    prh_hci_param_uint32, 
    prh_hci_param_uint32, 
    prh_hci_param_uint32, 
    prh_hci_param_uint32, 
    prh_hci_param_handle, 
    prh_hci_param_uint16, 
    prh_hci_param_uint16, 
    prh_hci_param_uint16, 
    prh_hci_param_uint8, 
    prh_hci_param_string, 
    prh_hci_param_uint8, 
    prh_hci_param_8bytes,
    prh_hci_param_uint8, 
    prh_hci_param_uint24, 
    prh_hci_param_uint8, 
    prh_hci_param_bdaddr, 
    prh_hci_param_string,
    prh_hci_param_uint8, 
    prh_hci_param_uint16, 
    prh_hci_param_8bytes,
    prh_hci_param_bdaddr, 
    prh_hci_param_uint24, 
    prh_hci_param_uint8, 
    prh_hci_param_uint16, 
    prh_hci_param_bdaddr, 
    prh_hci_param_uint8, 
    prh_hci_param_uint8, 
    prh_hci_param_uint16, 
    prh_hci_param_uint8, 
    prh_hci_param_uint8, 
    prh_hci_param_uint32, 
    prh_hci_param_uint32, 
    prh_hci_param_uint32,

/* updated for 1.0b create connection parameters - to be optimised later to save ~6 bytes */
    prh_hci_param_bdaddr, 
    prh_hci_param_uint16, 
    prh_hci_param_uint8, 
    prh_hci_param_uint8, 
    prh_hci_param_uint16, 
    prh_hci_param_uint8,
    prh_hci_param_handle, 

/* bugfix for sniffmode parameter list - to be optimised later to save ~5 bytes */
    prh_hci_param_handle, 
    prh_hci_param_uint16, 
    prh_hci_param_uint16, 
    prh_hci_param_uint16, 
    prh_hci_param_uint16,  /* 67 */

/* updated for 1.2 */
    prh_hci_param_handle, /* 68 */
    prh_hci_param_bdaddr, /* 69 */
    prh_hci_param_bdaddr, /* 70 */
    prh_hci_param_handle, /* 71 */
    prh_hci_param_uint32, /* 72 */
    prh_hci_param_uint32, /* 73 */
    prh_hci_param_uint16, /* 74 */
    prh_hci_param_uint16, /* 75 */
    prh_hci_param_uint8,  /* 76 */
    prh_hci_param_uint16, /* 77 */
    prh_hci_param_handle, /* 78 */
    prh_hci_param_uint8,  /* 79 */
    prh_hci_param_uint8,  /* 80 */
    prh_hci_param_uint8,  /* 81 */
    prh_hci_param_uint32, /* 82 */
    prh_hci_param_uint32, /* 83 */
    prh_hci_param_uint32, /* 84 */
    prh_hci_param_uint32, /* 85 */
    prh_hci_param_10bytes,/* 86 */
    prh_hci_param_bdaddr, /* 87 */
    prh_hci_param_uint32, /* 88 */
    prh_hci_param_uint32, /* 89 */
    prh_hci_param_uint16, /* 90 */
    prh_hci_param_uint16, /* 91 */
    prh_hci_param_uint8,  /* 92 */
    prh_hci_param_uint16, /* 93 */
    prh_hci_param_uint8,    /*94*/  //Zhu Jianguo added for the new feature of bt 2.1. 2008.2.25
    prh_hci_param_240bytes,/*95*/
    prh_hci_param_bdaddr,   /*96*/
    prh_hci_param_uint8,     /*97*/
    prh_hci_param_uint8,    /*98*/
    prh_hci_param_uint8,    /*99*/
	prh_hci_param_bdaddr, /*100*/
    prh_hci_param_16bytes,  /*101*/
    prh_hci_param_16bytes,  /*102*/
};


/* these structures need to be byte packed to achieve smallest possible footprint */
const struct prh_st_t_hci_opcode_entry prh_hci_group1_opcodes[]= {
    /*  FORMAT:  paramMapOffset, numParms */
    { 7, 3},        /*  HCI_Inquiry  */
    { 0, 0},        /*  HCI_InquiryCancel  */
    { 5, 5},        /*  HCI_PeriodicInquiryMode  */
    { 0, 0},        /*  HCI_ExitPeriodicInquiryMode  */
    { 56, 6},       /*  HCI_CreateConnection  */ /* the second number is 6 for 1.0b call, 5 for 1.0a */
    { 20, 2},       /*  HCI_Disconnect  */
    { 27, 2},       /*  HCI_AddSCOConnection  */
    {  0, 1},       /*  HCI_CreateConnectionCancel  */
    { 0, 2},        /*  HCI_AcceptConnectionRequest  */
    { 0, 2},        /*  HCI_RejectConnectionReqeust  */
    { 13, 2},       /*  HCI_LinkKeyRequestReply  */
    { 0, 1},        /*  HCI_LinkKeyRequestNegativeReply  */
    { 10, 3},       /*  HCI_PINCodeRequestReply  */
    { 0, 1},        /*  HCI_PINCodeRequestNegativeReply  */
    { 27, 2},       /*  HCI_ChangeConnectionPacketType  */
    { 0, 0},        /*  NA  */
    { 27, 1},       /*  HCI_AuthenticationRequested  */
    { 0, 0},        /*  NA  */
    { 20, 2},       /*  HCI_SetConnectionEncryption  */
    { 0, 0},        /*  NA  */
    { 27, 1},       /*  HCI_ChangeConnectionLinkKey  */
    { 0, 0},        /*  NA  */
    { 21, 1},       /*  HCI_MasterLinkKey  */
    { 0, 0},        /*  NA  */
    { 0, 4},        /*  HCI_RemoteNameRequest  */
    {  0, 1},       /*  HCI_RemoteNameRequestCancel  */
    { 27, 1},       /*  HCI_ReadRemoteSupportedFeatures  */
    { 20, 2},       /*  HCI_ReadRemoteExtendedFeatures  */
    { 27, 1},       /*  HCI_ReadRemoteVersionInformation  */
    { 0, 0},        /*  NA  */
    { 27, 1},       /*  HCI_ReadClockOffset  */
    { 27, 1},       /*  HCI_ReadLMPHandle (0x20) */
    { 27, 1},       /*  HCI_ExchangeFixedInfo */
    { 68, 2},       /*  HCI_ExchangeAliasInfo */
    { 27, 1},       /*  HCI_PrivatePairingRequestReply */
    { 27, 1},       /*  HCI_PrivatePairingRequestNegativeReply */
    { 68, 2},       /*  HCI_GeneratedAlias */
    { 69, 2},       /*  HCI_AliasAddressRequestReply */
    { 69, 1},       /*  HCI_AliasAddressRequestNegativeReply */
    { 71, 7},       /*  HCI_SetupSynchronousConnection */
    { 87, 7},       /*  HCI_AcceptSynchronousConnectionRequest */
    {  0, 2},       /*  HCI_RejectSynchronousConnectionRequest */
    { 96, 4},       /*  HCI_IoCapabilityRequesetReply (0x2B) */   //Zhu Jianguo added for the new feature of bt 2.1. 2008.2.25
    { 96, 1},       /*  HCI_UserConfirmationRequestReply (0x2C) */
    { 96, 1},       /*  HCI_UserConfirmationRequestNegativeReply  (0x2D)*/
    { 87, 2},       /*  HCI_UsePasskeyRequestRply (0x2E) */
    { 96, 1},       /*  HCI_UsePasskeyRequestNegativeRply (0x2F) */
    { 100, 3},       /*  HCI_RemoteOOBDataRequestReply  (0x30) */
    { 0, 0},         /*  N/A (0x31) */
    { 0, 0},         /*  N/A (0x32) */
    { 96, 1},       /*  HCI_RemoteOOBDataRequestNegatgiveReply (0x33) */
    { 96, 2},       /*  HCI_IoCapabilityRequesetNegativeReply (0x34) */       //end of added by Zhu jianguo. 2008.2.25
};
const struct prh_st_t_hci_opcode_entry prh_hci_group2_opcodes[]= {
    /*  FORMAT:  paramMapOffset, numParms */
    { 27, 3},       /*  HCI_HoldMode  */
    { 0, 0},        /*  NA  */
    { 63, 5},       /*  HCI_SniffMode  */
    { 27, 1},       /*  HCI_ExitSniffMode  */
    { 27, 3},       /*  HCI_ParkMode  */
    { 27, 1},       /*  HCI_ExitParkMode  */
    { 20, 7},       /*  HCI_QoSSetup  */
    { 0, 0},        /*  NA  */
    { 27, 1},       /*  HCI_RoleDiscovery  */
    { 0, 0},        /*  NA  */
    { 0, 2},        /*  HCI_SwitchRole  */
    { 27, 1},       /*  HCI_ReadLinkPolicySettings  */
    { 27, 2},       /*  HCI_WriteLinkPolicySettings  */
    {  0, 0},       /*  HCI_ReadDefaultLinkPolicySettings  */
    { 28, 1},       /*  HCI_WriteDefaultLinkPolicySettings  */
    { 78, 8},       /*  HCI_FlowSpecification */
    { 63, 4},       /*  HCI_SniffSubrating */   //Zhu Jianguo added for the new feature of bt 2.1. 2008.2.25
};
const struct prh_st_t_hci_opcode_entry prh_hci_group3_opcodes[]= {
    /*  FORMAT:  paramMapOffset, numParms */
    { 34, 1},       /*  HCI_SetEventMask  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  HCI_Reset  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  HCI_SetEventFilter - not generated by table */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  NA  */ 
    { 27, 1},       /*  HCI_Flush  */
    { 0, 0},        /*  HCI_ReadPINType  */
    { 21, 1},       /*  HCI_WritePINType  */
    { 0, 0},        /*  HCI_CreateNewUnitKey  */
    { 0, 0},        /*  NA  */
    { 0, 2},        /*  HCI_ReadStoredLinkKey  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  HCI_WriteStoredLinkKey  */
    { 0, 2},        /*  HCI_DeleteStoredLinkKey  */
    { 32, 1},       /*  HCI_ChangeLocalName  */
    { 0, 0},        /*  HCI_ReadLocalName  */
    { 0, 0},        /*  HCI_ReadConnectionAcceptTimeout  */
    { 5, 1},        /*  HCI_WriteConnectionAcceptTimeout  */
    { 0, 0},        /*  HCI_ReadPageTimeout  */
    { 5, 1},        /*  HCI_WritePageTimeout  */
    { 0, 0},        /*  HCI_ReadScanEnable  */
    { 21, 1},       /*  HCI_WriteScanEnable  */
    { 0, 0},        /*  HCI_ReadPageScanActivity  */
    { 5, 2},        /*  HCI_WritePageScanActivity  */
    { 0, 0},        /*  HCI_ReadInquiryScanActivity  */
    { 5, 2},        /*  HCI_WriteInquiryScanActivity  */
    { 0, 0},        /*  HCI_ReadAuthenticationEnable  */
    { 21, 1},       /*  HCI_WriteAuthenticationEnable  */
    { 0, 0},        /*  HCI_ReadEncryptionMode  */
    { 21, 1},       /*  HCI_WriteEncryptionMode  */
    { 0, 0},        /*  HCI_ReadClassofDevice  */
    { 7, 1},        /*  HCI_WriteClassofDevice  */
    { 0, 0},        /*  HCI_ReadVoiceSetting  */
    { 5, 1},        /*  HCI_WriteVoiceSetting  */
    { 27, 1},       /*  HCI_ReadAutomaticFlushTimeout  */
    { 27, 2},       /*  HCI_WriteAutomaticFlushTimeout  */
    { 0, 0},        /*  HCI_ReadNumBroadcastRetransmissions  */
    { 21, 1},       /*  HCI_WriteNumBroadcastRetransmissions  */
    { 0, 0},        /*  HCI_ReadHoldModeActivity  */
    { 21, 1},       /*  HCI_WriteHoldModeActivity  */
    { 20, 2},       /*  HCI_ReadTransmitPowerLevel  */
    { 0, 0},        /*  HCI_ReadSCOFlowControlEnable  */
    { 21, 1},       /*  HCI_WriteSCOFlowControlEnable  */
    { 0, 0},        /*  NA  */
    { 21, 1},       /*  HCI_SetHostControllerToHostFlowControl  */
    { 0, 0},        /*  NA  */
    { 3, 4},        /*  HCI_HostBufferSize  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  HCI_HostNumberOfCompletedPackets  */
    { 27, 1},       /*  HCI_ReadLinkSupervisionTimeout  */
    { 27, 2},       /*  HCI_WriteLinkSupervisionTimeout  */
    { 0, 0},        /*  HCI_ReadNumberOfSupportedIAC  */
    { 0, 0},        /*  HCI_ReadCurrentIACLAP  */
    { 0, 0},        /*  HCI_WriteCurrentIACLAP  */
    { 0, 0},        /*  HCI_ReadPageScanPeriodMode  */
    { 21, 1},       /*  HCI_WritePageScanPeriodMode  */
    { 0, 0},        /*  HCI_ReadPageScanMode  */
    { 21, 1},       /*  HCI_WritePageScanMode  */
    { 86, 1},       /*  HCI_SetAFHChannelClassification  */
    {  0, 0},       /*  N/A (0x40) */
    {  0, 0},       /*  N/A (0x41) */
    {  0, 0},       /*  HCI_ReadInquiryScanType  */
    { 21, 1},       /*  HCI_WriteInquiryScanType  */
    {  0, 0},       /*  HCI_ReadInquiryMode  */
    { 21, 1},       /*  HCI_WriteInquiryMode  */
    {  0, 0},       /*  HCI_ReadPageScanType  */
    { 21, 1},       /*  HCI_WritePageScanType  */
    {  0, 0},       /*  HCI_ReadAFHChannelClassificationMode  */
    { 21, 1},       /*  HCI_WriteAFHChannelClassificationMode  */
    {  0, 0},       /*  HCI_ReadAnonymityMode  */
    { 21, 1},       /*  HCI_WriteAnonymityMode  */
    {  0, 0},       /*  HCI_ReadAliasAuthenticationEnable  */
    { 21, 1},       /*  HCI_WriteAliasAuthenticationEnable  */
    {  0, 0},       /*  HCI_ReadAnonymousAddressChangeParameters  */
    { 73, 2},       /*  HCI_WriteAnonymousAddressChangeParameters  */
    { 21, 1},       /*  HCI_ResetFixedAddressAttemptsCounter  */
    {  0, 0},      /* HCI_cmdReadExtendedResponse(0x51)*/     //Zhu Jianguo added for the new feature of bt 2.1. 2008.2.25
    {  94, 2},       /*HCI_cmdWriteExtendedResponse(0x52)*/
    {  27, 1},      /* HCI_cmdREFRESHENCRYPTIONKEY (0x53)*/
    {  0, 0},      /*(0x54)*/
    {  0, 0},      /*(0x55)*/
    {  1, 1},     /*HCI_cmdWriteSimplePairingMode (0x56)*/
    {  0, 0},      /*(0x56)*/
    {  0, 0},      /*(0x57)*/
    {  0, 0},     /*(0x58)*/
    { 0,0},       /*HCI_ReadDefaultErroneousDataReporting(0x5A)*/  
    { 21,1},     /*HCI_WriteDefaultErroneousDataReporting(0x5B)*/   
    { 0,  0},     /*N/A (0x5C)*/
    { 0,  0},     /*N/A (0x5D)*/
    { 0,  0},     /*N/A (0x5E)*/
    { 78,  2},     /*HCI_cmdFLUSHENHANCED (0x5F)*/
    { 0,  2},     /*HCI_SendKeypressNotification (0x60)*/          //end of added by Zhu jianguo. 2008.2.25
};
const struct prh_st_t_hci_opcode_entry prh_hci_group4_opcodes[]= {
    /*  FORMAT:  paramMapOffset, numParms */
    { 0, 0},        /*  HCI_ReadLocalVersionInformation  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  HCI_ReadLocalSupportedFeatures  */
    { 21, 1},        /*  HCI_ReadLocalExtendedFeatures  */
    { 0, 0},        /*  HCI_ReadBufferSize  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  HCI_ReadCountryCode  */
    { 0, 0},        /*  NA  */
    { 0, 0},        /*  HCI_ReadBDADDR  */
};                                              
const struct prh_st_t_hci_opcode_entry prh_hci_group5_opcodes[]= {
    /*  FORMAT:  paramMapOffset, numParms */
    { 27, 1},       /*  HCI_ReadFailedContactCounter  */
    { 27, 1},       /*  HCI_ResetFailedContactCounter  */
    { 27, 1},       /*  HCI_GetLinkQuality  */
    { 0, 0},        /*  NA  */
    { 27, 1},       /*  HCI_ReadRSSI  */
    { 27, 1},       /*  HCI_ReadAFHChannelMap  */
    { 61, 2},       /*  HCI_ReadClock - formerly 78 */
};
const struct prh_st_t_hci_opcode_entry prh_hci_group6_opcodes[]= {
    /*  FORMAT:  paramMapOffset, numParms */
    { 0, 0},        /*  HCI_ReadLoopbackMode  */
    { 21, 1},       /*  HCI_WriteLoopbackMode  */
    { 0, 0},        /*  HCI_EnableDUT  */
    { 1, 1},         /* HCI_WriteSimplePairingDebugMode (0x04)*/   //Zhu Jianguo added for the new feature of bt 2.1. 2008.2.25
};

const struct prh_st_t_hci_group_entry prh_hci_command_groups[prh_hci_max_command_group] = {

    { prh_hci_group1_opcodes, sizeof(prh_hci_group1_opcodes)/sizeof(prh_hci_group1_opcodes[0]) },
    { prh_hci_group2_opcodes, sizeof(prh_hci_group2_opcodes)/sizeof(prh_hci_group2_opcodes[0]) },
    { prh_hci_group3_opcodes, sizeof(prh_hci_group3_opcodes)/sizeof(prh_hci_group3_opcodes[0]) },
    { prh_hci_group4_opcodes, sizeof(prh_hci_group4_opcodes)/sizeof(prh_hci_group4_opcodes[0]) },
    { prh_hci_group5_opcodes, sizeof(prh_hci_group5_opcodes)/sizeof(prh_hci_group5_opcodes[0]) },
    { prh_hci_group6_opcodes, sizeof(prh_hci_group6_opcodes)/sizeof(prh_hci_group6_opcodes[0]) }
};


/*************************************************************************************************
 * HCI_GenerateCommand
 *
 * Builds a command PDU from an opcode and a varargs list of parameters.
 *
 * NOTE: This assumes that "int" is at least 16 bits. va_start requires an int, not u_int16, etc. 
 *
 ************************************************************************************************/
t_api HCI_GenerateCommand(int opcode, ... ) 
{

    u_int8 ogf;
    u_int16 ocf;
    const struct prh_st_t_hci_opcode_entry *commandinfo;
    u_int16 length, i;
    va_list args;
    u_int8 numArgs;
    const prh_t_hci_Parm_types *parmList;
    struct host_buf *pdu;
    u_int16 PDUoffset;
    prh_t_hci_Parm_types argLen=0;
    u_int32 nextarg;
    u_int8 *argPtr;
    u_int8 j;
    t_api status;
    int ret;
    u_int8 pinCodeLen=0;

#if(HU_MGR_TESTS ==1)
    if(!hci_test)
      {
	last_opcode = opcode;
	return hci_return_error;
      }
#endif

    va_start(args, opcode);

    ocf=((unsigned int)opcode)&0x3FF;
    ogf=(((unsigned int)opcode)>>10)&0x3F;


    if(ogf>prh_hci_max_command_group || ogf<1) 
    {
        /* Invalid group code */        
        return BT_INVALIDPARAM;
    }
    
    if (ocf>prh_hci_command_groups[ogf-1].numOpcodes || ogf<1)
    {
        /* Invalid opcode */
        return BT_INVALIDPARAM;
    }
    
    commandinfo=prh_hci_command_groups[ogf-1].opcodeArray+ocf-1;
    numArgs=commandinfo->numArgs;
    parmList=prh_hci_command_parm_types+commandinfo->parmOffset;
    
    /* Possible optimization: if ogf==4, there are no parameters, so remove lookup table */

    /* 
        Now we need to get the length of the PDU we are generating: 
        Two possible approaches here - malloc 257 bytes (max. command size)
        in all cases, or spend time adding up the total length of the 
        parameters first to save RAM at the expense of some ROM code.
          -> Here we add up calculate the total length first.        
    */

    length=0; 
    /* 
        parameter identifiers are defined to match the sizes they 
        represent, except the handle and string types.
    */
    for (i=0; i<numArgs; i++) 
    {
        if (parmList[i]==prh_hci_param_handle)
            length+=2; /* handle is 2 bytes */
        else
            length+=parmList[i];
    }
    length+=3; /* Add the length of the header */

    ret=prh_hci_GetCommandBuffer(length, 0, &pdu);
    if (ret!=BT_NOERROR) 
    {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_GenerateCommand failed to allocate host_buf for %d bytes\n",length));
        return BT_NORESOURCES;
    }
    
    pdu->len=length;

    /* write opcode field */
    pdu->data[0]=opcode&0xFF;
    pdu->data[1]=(opcode>>8)&0xFF;
    pdu->data[2]=length-3;

    /* start encoding the parameters */  
    PDUoffset=3;  

    for(i=0; i<numArgs; i++)
    {
        argLen=parmList[i];

        if (argLen==prh_hci_param_handle)
            argLen=2; /* handle is 2 bytes */
        
        if (argLen>prh_hci_param_uint32) {
            /* this parameter is a pointer (to a bdaddress, pincode, linkkey, or string) */
            argPtr=va_arg(args,void *);
        } else {
            /* this parameter is a value */
            nextarg=va_arg(args,u_int32);
            argPtr=(u_int8 *)(&nextarg);
#if BTHOST_BIGENDIAN
            /* since this parameter is a value, compensate for endianess (pdu is little-endian) */
            if (argLen<=prh_hci_param_uint32) {
                u_int32 temp;
                temp = *((u_int32*)argPtr);
                temp=TOGGLE_ENDIANESS_UINT32(temp);
                *((u_int32*)argPtr) = temp;
            }
#endif
        }

        /* 
            Cope with special case where API pointer may not contain a full 16 byte PIN code.
            The previous argument always contains the correct length of the supplied PIN code.
        */
        if (opcode==HCI_cmdPINCODEREQUESTREPLY) {            
            if (i==1) {
#if BTHOST_BIGENDIAN
                pinCodeLen=TOGGLE_ENDIANESS_UINT32(nextarg);
#else
		pinCodeLen=nextarg;
#endif
            } else if (i==2) {
                argLen=pinCodeLen;
                pMemset(pdu->data+PDUoffset+pinCodeLen,0,16-pinCodeLen); /* add zero-fill to pdu */
            }
        }            

        /* write the bytes of the parameter individually to the pdu */
        for (j=0; j<argLen; j++) {
            /* the string is NUL-terminated in the stack, but not always in the pdu */
            if (argLen==prh_hci_param_string && *argPtr=='\0') {                
                break;
            }
            pdu->data[PDUoffset++] = *argPtr++;             
        }
    }

    va_end(args);

    /* zero-fill the local name parameter - this is the only parameter for this command */
    if (opcode==HCI_cmdCHANGELOCALNAME) {               
        pMemset(pdu->data+PDUoffset,0,HCI_cmdCHANGELOCALNAME_LEN-PDUoffset); 
    }
   
    /* Send the pdu to the flow control queue */
    status=prh_hci_fc_send_command_pdu(pdu); 
 
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_GenerateCommand failed send command PDU - prh_hci_fc_send_command_pdu() returned error %d\n",status));
        prh_hci_ReleaseCommandBuffer(pdu);
    }

    return status;
}



