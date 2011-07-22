/*
 * MODULE NAME:    hci_test.c
 * PROJECT CODE:
 * DESCRIPTION:    HCI Test Program
 * AUTHOR:         Martin Mellody
 * DATE:           20 March 2000
 *
 * SOURCE CONTROL: $Id: hci_test.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    V0.1     20 March 2000 - MM 
 *
 */

#include "host_config.h"
#include "hci_primitives.h"
#include "papi.h"
#include "hci_internals.h"
#include "host_buf.h"
typedef u_int8 BDAddr_t[6];
void TestBasicCommands(void);

u_int16 handle;

/* these are the endpoint BDADDRESSES for null hci bus configuration (HCI_NULL_HCI_BUS) */
t_bd_addr bd_addr_cli, bd_addr_serv;

error_t datacallback(u_int16 handle,struct host_buf *data,u_int8 flags);
error_t ctrlcallback(HCI_Event_t *e);
void TestBasicCommands(void);
void SimpleTests(t_bd_addr addr);

int main(int argc, char **argv) {
    HCI_EventFilter_t  filter;

    struct host_buf *databuf,*databuf2, *databuf3;
    t_bd_addr addr;
    u_int8 bytes[]={ 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA ,0x11, 0x22 ,0xFF};
    u_int8 key[]={ 0x11,0x22,0x33,0x44,0x55,0x66, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA ,0x11, 0x22 ,0xFF,0xCC, 0xBB,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa};
    u_int8 pincode[16]="123456";
    u_int8 addr2[]={ 0x64, 0x20, 0x12, 0x37, 0x80, 0x00};

    databuf=host_buf_alloc(4096);
    databuf->len=4096;



#if HCI_NULL_HCI_BUS
/* Initialise to easily recognisable values */

    memcpy(bd_addr_cli.bytes,bytes,6); /* 0xaabbccddeeff */
    memcpy(bd_addr_serv.bytes,key,6);   /* 0x665544332211 */
#endif


    memcpy(addr.bytes,bytes,6);

    pDebugPrintf((pLOGDEBUG,"\nHCI Test Program... , sizeof(HCI_Event_t)=%d bytes",sizeof(HCI_Event_t)));

#if HCI_NULL_HCI_BUS
    pDebugPrintf((pLOGDEBUG,"HCI_NULL_HCI_BUS defined!!! - faking control commands/events"));
#endif

    pDebugPrintf((pLOGDEBUG,"\nbdaddr=%s",HCI_PrintLargeNumber(addr.bytes,6)));
    pDebugPrintf((pLOGDEBUG,"othernumber=%s\n",HCI_PrintLargeNumber(key,sizeof(key))));

    HCI_Init();

    HCI_RegisterL2capCtrlCallback(ctrlcallback);
    HCI_RegisterL2capACLCallback(datacallback);

    handle=0;

    HCI_Start();

    Sleep(1000);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_ReadBDAddress() ..."));
    HCI_ReadBDAddress();

    pDebugPrintf((pLOGDEBUG,"MGR: setting auto-accept mode in the host controller"));
    filter.FilterType=0x02;
    filter.ConditionType=0x00;
    filter.Condition.AutoAccept=0x02;
    HCI_SetEventFilter(&filter);

    pDebugPrintf((pLOGDEBUG,"MGR: setting host controller to default initial confituration"));
    HCI_WriteScanEnable(0x03);
    HCI_WriteAuthenticationEnable(0x00);
    Sleep(1000);
    HCI_WriteConnectionAcceptTimeout(0xFFFF);
    HCI_WritePageTimeout(0xFFFF);



    if (argv[1]) {

        HCI_WritePageTimeout(0xFFFF);

        memcpy(addr.bytes,addr2,6);


        pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_CreateConnection(addr,0x08,0x00,0x00,0x00,0x00) ..."));
    //  HCI_CreateConnection(addr,0x08,0x00,0x00,0x00,0x00);
        Sleep(1000);
        Sleep(1000);
handle=0xF00;
        pDebugPrintf((pLOGDEBUG,"\n\n"));

        while (handle==0);

        pDebugPrintf((pLOGDEBUG,"\n\nHCI: Got handle 0x%03X!!!!\n\n",handle));

        pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(handle, \"NI! NI! NI!\", 11,HCI_ACL_START_FRAGMENT) ..."));

        memcpy(databuf->data,"NI! NI! NI!",11);
        databuf->len=11;
        databuf2=host_buf_child_alloc(databuf,databuf->data,databuf->len);
        databuf3=host_buf_child_alloc(databuf,databuf->data,databuf->len);
        HCI_SendACL(handle, databuf3,HCI_ACL_START_FRAGMENT);
        HCI_SendACL(handle, databuf2,HCI_ACL_START_FRAGMENT);
        HCI_SendACL(handle, databuf,HCI_ACL_START_FRAGMENT);
        pDebugPrintf((pLOGDEBUG,"\n\n"));
        Sleep(1000);

        pDebugPrintf((pLOGDEBUG,"About to call HCI_Disconnect(handle,0x01) ..."));
        HCI_Disconnect(handle,0x13);    
    }


//  SimpleTests(addr);
//  TestBasicCommands(); 
    HCI_Shutdown();

    return 0;
}

error_t ctrlcallback(HCI_Event_t *e) {

    pDebugPrintf((pLOGDEBUG,"\nL2CAP Got Event, type %d",e->Type));

    if (e->Type==HCI_evCONNECTIONCOMPLETE) {
        handle=e->Parms.ConnectionComplete.ConnectionHandle;        
    }

    return 0;
}

error_t datacallback(u_int16 handle,struct host_buf *data,u_int8 flags) {

    pDebugPrintf((pLOGDEBUG,"\nL2CAP Got ACL Data, handle %d, length %d, flags %d: ",handle,data->len,flags));
    if (data->len==15) 
        pDebugPrintf((pLOGDEBUG,"L2CAP ACL Data: \"%s\" (!!!)",data->data+4));


    return 0;
}

#if 0

void SimpleTests(t_bd_addr addr) {

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_CreateConnection(addr,0x08,0x01,0x03,0x1234,0x01) ..."));
    HCI_CreateConnection(addr,0x08,0x01,0x03,0x1234,0x01);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"\", 0,HCI_ACL_START_FRAGMENT) ..."));
    HCI_SendACL(0x2, "", 0,HCI_ACL_START_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"1\", 1,HCI_ACL_START_FRAGMENT) ..."));
    HCI_SendACL(0x2, "1", 1,HCI_ACL_START_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"123456\", 6,HCI_ACL_CONTINUING_FRAGMENT) ..."));
    HCI_SendACL(0x2, "123456", 6,HCI_ACL_CONTINUING_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"1234567\", 7,HCI_ACL_START_FRAGMENT) ..."));
    HCI_SendACL(0x2, "1234567", 7,HCI_ACL_START_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"12345678\", 8,HCI_ACL_START_FRAGMENT) ..."));
    HCI_SendACL(0x2, "12345678", 8,HCI_ACL_START_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"1234567890123456\", 16,HCI_ACL_START_FRAGMENT) ..."));
    HCI_SendACL(0x2, "1234567890123456", 16,HCI_ACL_START_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"12345678901234567\", 17,HCI_ACL_START_FRAGMENT) ..."));
    HCI_SendACL(0x2, "12345678901234567", 17,HCI_ACL_START_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"123456789012345678\", 18,HCI_ACL_CONTINUING_FRAGMENT) ..."));
    HCI_SendACL(0x2, "123456789012345678", 18,HCI_ACL_CONTINUING_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\nAbout to call HCI_SendACL(0x2, \"1234567890123456789012345678901234567890\", 40,HCI_ACL_START_FRAGMENT) ..."));
    HCI_SendACL(0x20, "1234567890123456789012345678901234567890", 40,HCI_ACL_START_FRAGMENT);

    pDebugPrintf((pLOGDEBUG,"\n\n"));

    HCI_ReadBDAddress();
    pDebugPrintf((pLOGDEBUG,"\n\n"));

    HCI_ReadClockOffset(0x1234);
    pDebugPrintf((pLOGDEBUG,"\n\n"));

    HCI_ChangeLocalName("We are the knights who say NI!"); 
    pDebugPrintf((pLOGDEBUG,"\n\n"));

    pDebugPrintf((pLOGDEBUG,"About to call HCI_Disconnect(0x1234,0x01) ..."));
    HCI_Disconnect(0x1234,0x1); 

    pDebugPrintf((pLOGDEBUG,"\n\n"));


}
#endif

void TestBasicCommands(void) {

    t_bd_addr addr;
    u_int8 bytes[]={ 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA ,0x11, 0x22 ,0xFF};
    char tempbuf[1024];
    int i;

    HCI_EventFilter_t filter;

    memcpy(addr.bytes,bytes,6);

    HCI_Inquiry(0x123456,0x78,0x9a);
    HCI_InquiryCancel();
    HCI_PeriodicInquiryMode(0x1234,0x5678,0x9abcde,0xf1,0x23);
    HCI_ExitPeriodicInquiryMode();
    HCI_CreateConnection(addr,0x1234,0x56,0x78,0x9abc,0xde);
    HCI_Disconnect(0x1234,0x56);
    HCI_AddSCOConnection(0x1234,0x5678);
    HCI_AcceptConnectionRequest(addr,0x12);
    HCI_RejectConnectionReqeust(addr,0x12);

    HCI_ChangeConnectionPacketType(0x1234,0x5678);

    HCI_RemoteNameRequest(addr,0x12,0x34,0x5678);
    HCI_ReadRemoteSupportedFeatures(0x1234);
    HCI_ReadRemoteVersionInformation(0x1234);
    HCI_ReadClockOffset(0x1234);
    
    HCI_QoSSetup(0x1234,0x56,0x78,0x9abc,0xdef0,0x1234,0x5678);
    HCI_SwitchRole(addr,0x12);

    HCI_SetEventMask(bytes);
    HCI_Reset();    

    filter.Condition.BDAddress=addr; 
    filter.Condition.DeviceClass=0x123456;
    filter.Condition.DeviceClassMask=0x778899;
    filter.Condition.AutoAccept=0xaa;

    filter.FilterType=0x00;
    filter.ConditionType=0x00;
    HCI_SetEventFilter(&filter);
    
    filter.FilterType=0x01;
    filter.ConditionType=0x00;
    HCI_SetEventFilter(&filter);

    filter.FilterType=0x01;
    filter.ConditionType=0x01;
    HCI_SetEventFilter(&filter);

    filter.FilterType=0x01;
    filter.ConditionType=0x02;
    HCI_SetEventFilter(&filter);

    filter.FilterType=0x02;
    filter.ConditionType=0x00;
    HCI_SetEventFilter(&filter);

    filter.FilterType=0x02;
    filter.ConditionType=0x01;
    HCI_SetEventFilter(&filter);
    filter.FilterType=0x02;

    filter.ConditionType=0x02;
    HCI_SetEventFilter(&filter);

    HCI_Flush(0x1234);

    HCI_ChangeLocalName("!"); 
    HCI_ChangeLocalName("NI!"); 
    HCI_ChangeLocalName("We are the knights who say NI!"); 

    for (i=0; i<250; i++)
        tempbuf[i]=(i%26)+65;

    tempbuf[249]='\0';
    HCI_ChangeLocalName(tempbuf);
    tempbuf[248]='\0';
    HCI_ChangeLocalName(tempbuf);
    tempbuf[247]='\0';
    HCI_ChangeLocalName(tempbuf);
    tempbuf[246]='\0';
    HCI_ChangeLocalName(tempbuf);
    tempbuf[245]='\0';
    HCI_ChangeLocalName(tempbuf);

    HCI_ReadLocalName();

    HCI_ReadPageTimeout();
    HCI_WritePageTimeout(0x1234);

    HCI_ReadClassOfDevice();

    HCI_ReadLocalVersionInformation();
    HCI_ReadLocalSupportedFeatures();
    HCI_ReadBufferSize();
    HCI_ReadCountryCode();
    HCI_ReadBDAddress();

    HCI_GetLinkQuality(0x1234);
    HCI_ReadRSSI(0x1234);

    HCI_ReadLoopbackMode();
    HCI_WriteLoopbackMode(0x12);
    HCI_EnableDUT();

}

/* 
    Final test code to be used when complete   -MM
*/

#if 0


void TestAllCommands(void) {

    error_t status;
    t_bd_addr bdaddress;
    u_int8 bytes[] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    memcpy(bdaddress.bytes,bytes,6);

    
    pDebugPrintf((pLOGDEBUG,"Starting Tests...."));



    /*
        HCI link control commands - Opcode Group Field = 0x01
    */


    pDebugPrintf((pLOGDEBUG,"HCI Link Control Commands, OGF=0x04"));


    if (status=HCI_Inquiry(0x123456, 0xab, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_Inquiry(0x123456, 0xab, 0xab) returned error code %d",status));

    if (status=HCI_InquiryCancel() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_InquiryCancel() returned error code %d",status));

    if (status=HCI_PeriodicInquiryMode(0x7890, 0x7890, 0x123456, 0xab, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_PeriodicInquiryMode(0x7890, 0x7890, 0x123456, 0xab, 0xab) returned error code %d",status));

    if (status=HCI_ExitPeriodicInquiryMode() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ExitPeriodicInquiryMode() returned error code %d",status));

    if (status=HCI_CreateConnection(bdaddress, 0x7890, 0xab, 0xab, 0x7890, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_CreateConnection(0xffeeddccbbaa, 0x7890, 0xab, 0xab, 0x7890, 0xab) returned error code %d",status));

    if (status=HCI_Disconnect(0x7890, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_Disconnect(0x7890, 0xab) returned error code %d",status));

    if (status=HCI_AddSCOConnection(0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_AddSCOConnection(0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_AcceptConnectionRequest(bdaddress, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_AcceptConnectionRequest(0xffeeddccbbaa, 0xab) returned error code %d",status));

    if (status=HCI_RejectConnectionReqeust(bdaddress, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_RejectConnectionReqeust(0xffeeddccbbaa, 0xab) returned error code %d",status));

    if (status=HCI_LinkKeyRequestReply(bdaddress, u_int8 *linkKey) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_LinkKeyRequestReply(0xffeeddccbbaa, u_int8 *linkKey) returned error code %d",status));

    if (status=HCI_LinkKeyRequestNegativeReply(bdaddress) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_LinkKeyRequestNegativeReply(0xffeeddccbbaa) returned error code %d",status));

    if (status=HCI_PINCodeRequestReply(bdaddress, 0xab, u_int8 *pincode) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_PINCodeRequestReply(0xffeeddccbbaa, 0xab, u_int8 *pincode) returned error code %d",status));

    if (status=HCI_PINCodeRequestNegativeReply(bdaddress) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_PINCodeRequestNegativeReply(0xffeeddccbbaa) returned error code %d",status));

    if (status=HCI_ChangeConnectionPacketType(0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ChangeConnectionPacketType(0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_AuthenticationRequested(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_AuthenticationRequested(0x7890) returned error code %d",status));

    if (status=HCI_SetConnectionEncryption(0x7890, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_SetConnectionEncryption(0x7890, 0xab) returned error code %d",status));

    if (status=HCI_ChangeConnectionLinkKey(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ChangeConnectionLinkKey(0x7890) returned error code %d",status));

    if (status=HCI_MasterLinkKey(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_MasterLinkKey(0xab) returned error code %d",status));

    if (status=HCI_RemoteNameRequest(bdaddress, 0xab, 0xab, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_RemoteNameRequest(0xffeeddccbbaa, 0xab, 0xab, 0x7890) returned error code %d",status));

    if (status=HCI_ReadRemoteSupportedFeatures(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadRemoteSupportedFeatures(0x7890) returned error code %d",status));

    if (status=HCI_ReadRemoteVersionInformation(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadRemoteVersionInformation(0x7890) returned error code %d",status));

    if (status=HCI_ReadClockOffset(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadClockOffset(0x7890) returned error code %d",status));


    /*
        HCI link policy commands - Opcode Group Field = 0x02
    */

    pDebugPrintf((pLOGDEBUG,"HCI Link Policy Commands, OGF=0x02"));


    if (status=HCI_HoldMode(0x7890, 0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_HoldMode(0x7890, 0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_SniffMode(0x7890, 0x7890, 0x7890, 0x7890) != E_NO_ERROR)     
        pDebugPrintf((pLOGERROR,"HCI_SniffMode(0x7890, 0x7890, 0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_ExitSniffMode(0x7890) != E_NO_ERROR)     
        pDebugPrintf((pLOGERROR,"HCI_ExitSniffMode(0x7890) returned error code %d",status));

    if (status=HCI_ParkMode(0x7890, 0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ParkMode(0x7890, 0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_ExitParkMode(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ExitParkMode(0x7890) returned error code %d",status));

    if (status=HCI_QoSSetup(0x7890, 0xab, 0xab, 0x123456, 0x123456, 0x123456, 0x123456) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_QoSSetup(0x7890, 0xab, 0xab, 0x123456, 0x123456, 0x123456, 0x123456) returned error code %d",status));

    if (status=HCI_RoleDiscovery(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_RoleDiscovery(0x7890) returned error code %d",status));

    if (status=HCI_SwitchRole(bdaddress, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_SwitchRole(0xffeeddccbbaa, 0xab) returned error code %d",status));


    /*
        HCI host controller and baseband commands - Opcode Group Field = 0x03
    */

    pDebugPrintf((pLOGDEBUG,"HCI Host Controller and Baseband Commands, OGF=0x03"));


    if (status=HCI_SetEventMask(u_int8 *eventMask) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_SetEventMask(u_int8 *eventMask) returned error code %d",status));

    if (status=HCI_Reset() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_Reset() returned error code %d",status));

    if (status=HCI_SetEventFilter(HCI_EventFilter_t filter) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_SetEventFilter(HCI_EventFilter_t filter) returned error code %d",status));

    if (status=HCI_Flush(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_Flush(0x7890) returned error code %d",status));

    if (status=HCI_ReadPINType() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadPINType() returned error code %d",status));

    if (status=HCI_WritePINType(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WritePINType(0xab) returned error code %d",status));

    if (status=HCI_CreateNewUnitKey() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_CreateNewUnitKey() returned error code %d",status));

    if (status=HCI_ReadStoredLinkKey(bdaddress, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadStoredLinkKey(0xffeeddccbbaa, 0xab) returned error code %d",status));

    if (status=HCI_WriteStoredLinkKey(0xab, HCI_WriteLinkKey_t *links) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteStoredLinkKey(0xab, HCI_WriteLinkKey_t *links) returned error code %d",status));

    if (status=HCI_DeleteStoredLinkKey(bdaddress, 0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_DeleteStoredLinkKey(0xffeeddccbbaa, 0xab) returned error code %d",status));

    if (status=HCI_ChangeLocalName(u_int8 *name) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ChangeLocalName(u_int8 *name) returned error code %d",status)); 

    if (status=HCI_ReadLocalName() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadLocalName() returned error code %d",status));

    if (status=HCI_ReadConnectionAcceptTimeout() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadConnectionAcceptTimeout() returned error code %d",status));

    if (status=HCI_WriteConnectionAcceptTimeout(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteConnectionAcceptTimeout(0x7890) returned error code %d",status));

    if (status=HCI_ReadPageTimeout() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadPageTimeout() returned error code %d",status));

    if (status=HCI_WritePageTimeout(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WritePageTimeout(0x7890) returned error code %d",status));

    if (status=HCI_ReadScanEnable() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadScanEnable() returned error code %d",status));

    if (status=HCI_WriteScanEnable(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteScanEnable(0xab) returned error code %d",status));

    if (status=HCI_ReadPageScanActivity() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadPageScanActivity() returned error code %d",status));

    if (status=HCI_WritePageScanActivity(0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WritePageScanActivity(0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_ReadInquiryScanActivity() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadInquiryScanActivity() returned error code %d",status));

    if (status=HCI_WriteInquiryScanActivity(0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteInquiryScanActivity(0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_ReadAuthenticationEnable() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadAuthenticationEnable() returned error code %d",status));

    if (status=HCI_WriteAuthenticationEnable(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteAuthenticationEnable(0xab) returned error code %d",status));

    if (status=HCI_ReadEncryptionMode() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadEncryptionMode() returned error code %d",status));

    if (status=HCI_WriteEncryptionMode(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteEncryptionMode(0xab) returned error code %d",status));

    if (status=HCI_ReadClassOfDevice() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadClassOfDevice() returned error code %d",status));

    if (status=HCI_WriteClassOfDevice(0x123456) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteClassOfDevice(0x123456) returned error code %d",status));

    if (status=HCI_ReadVoiceSetting() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadVoiceSetting() returned error code %d",status));

    if (status=HCI_WriteVoiceSetting(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteVoiceSetting(0x7890) returned error code %d",status));

    if (status=HCI_ReadAutomaticFlushTimeout() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadAutomaticFlushTimeout() returned error code %d",status));

    if (status=HCI_WriteAutomaticFlushTimeout(0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteAutomaticFlushTimeout(0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_ReadNumBroadcastRetransmissions() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadNumBroadcastRetransmissions() returned error code %d",status));

    if (status=HCI_WriteNumBroadcastRetransmissions(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteNumBroadcastRetransmissions(0xab) returned error code %d",status));

    if (status=HCI_ReadHoldModeActivity() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadHoldModeActivity() returned error code %d",status));

    if (status=HCI_WriteHoldModeActivity(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteHoldModeActivity(0xab) returned error code %d",status));

    if (status=HCI_ReadTransmitPowerLevel() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadTransmitPowerLevel() returned error code %d",status));

    if (status=HCI_ReadSCOFlowControlEnable() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadSCOFlowControlEnable() returned error code %d",status));

    if (status=HCI_WriteSCOFlowControlEnable(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteSCOFlowControlEnable(0xab) returned error code %d",status));

    if (status=HCI_SetHostControllerToHostFlowControl(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_SetHostControllerToHostFlowControl(0xab) returned error code %d",status));

    if (status=HCI_HostBufferSize(0x7890, 0xab, 0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_HostBufferSize(0x7890, 0xab, 0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_HostNumberOfCompletedPackets(0xab, HCI_NumPackets_t *numPackets) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_HostNumberOfCompletedPackets(0xab, HCI_NumPackets_t *numPackets) returned error code %d",status));

    if (status=HCI_ReadLinkSupervisionTimeout() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadLinkSupervisionTimeout() returned error code %d",status));

    if (status=HCI_WriteLinkSupervisionTimeout(0x7890, 0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteLinkSupervisionTimeout(0x7890, 0x7890) returned error code %d",status));

    if (status=HCI_ReadNumberOfSupportedIAC() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadNumberOfSupportedIAC() returned error code %d",status));

    if (status=HCI_ReadCurrentIACLAP() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadCurrentIACLAP() returned error code %d",status));

    if (status=HCI_WriteCurrentIACLAP(0xab, u_int32 *lap) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteCurrentIACLAP(0xab, u_int32 *lap) returned error code %d",status));

    if (status=HCI_ReadPageScanPeriodMode() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadPageScanPeriodMode() returned error code %d",status));

    if (status=HCI_WritePageScanPeriodMode(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WritePageScanPeriodMode(0xab) returned error code %d",status));

    if (status=HCI_ReadPageScanMode() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadPageScanMode() returned error code %d",status));

    if (status=HCI_WritePageScanMode(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WritePageScanMode(0xab) returned error code %d",status));


    /*
        HCI informational commands - Opcode Group Field = 0x04
    */

    pDebugPrintf((pLOGDEBUG,"HCI Informational Commands, OGF=0x04"));


    if (status=HCI_ReadFailedContactCounter(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadFailedContactCounter(0x7890) returned error code %d",status));

    if (status=HCI_ResetFailedContactCounter(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ResetFailedContactCounter(0x7890) returned error code %d",status));

    if (status=HCI_GetLinkQuality(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_GetLinkQuality(0x7890) returned error code %d",status));

    if (status=HCI_ReadRSSI(0x7890) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadRSSI(0x7890) returned error code %d",status));


    /*
        HCI status commands - Opcode Group Field = 0x05
    */

    pDebugPrintf((pLOGDEBUG,"HCI Status Commands, OGF=0x05"));


    if (status=HCI_ReadLocalVersionInformation() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadLocalVersionInformation() returned error code %d",status));

    if (status=HCI_ReadLocalSupportedFeatures() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadLocalSupportedFeatures() returned error code %d",status));

    if (status=HCI_ReadBufferSize() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadBufferSize() returned error code %d",status));

    if (status=HCI_ReadCountryCode() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadCountryCode() returned error code %d",status));

    if (status=HCI_ReadBDAddress() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_Read0xffeeddccbbaa() returned error code %d",status));


    /*
        HCI test commands - Opcode Group Field = 0x06
    */

    pDebugPrintf((pLOGDEBUG,"HCI Test Commands, OGF=0x06"));


    if (status=HCI_ReadLoopbackMode() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_ReadLoopbackMode() returned error code %d",status));

    if (status=HCI_WriteLoopbackMode(0xab) != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_WriteLoopbackMode(0xab) returned error code %d",status));

    if (status=HCI_EnableDUT() != E_NO_ERROR) 
        pDebugPrintf((pLOGERROR,"HCI_EnableDUT() returned error code %d",status));


    return;
}

#endif /* 0 */
