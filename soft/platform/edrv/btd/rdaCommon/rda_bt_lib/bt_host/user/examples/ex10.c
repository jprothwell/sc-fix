/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 10, v1.0
    - Adding an RFCOMM server SDP record 
*/


#include <stdio.h>

#include "bt.h"

int main(int argc, char *argv[]) {
    
    int status;

    u_int8 channel;
    u_int32 handle;
    u_int16 serial_uuid,l2cap_uuid,rfcomm_uuid,language_id,language_encoding,language_base;
    sdp_service_record *pRecord;
    sdp_data_element SerialPortUUID, ServiceClassList, L2CAP_UUID, Protocol0;
    sdp_data_element RFCOMM_UUID, RFCOMM_CHANNEL, Protocol1, ProtocolDescriptorList;
    sdp_data_element Serial_Name, Lang_id, Lang_Enc, Lang_base, Lang_list_1;
    sdp_data_element Lang_base_list, *list[10];

    portid_t portId;
  
    char *Service_name = "Serial_service_a";

    handle = 16;


    printf("Parthus Technologies Bluetooth Host Software\n");
    printf("Example Program 10 - Adding an RFCOMM server SDP record\n\n");


    /************  Initialise the Host Stack */

    status = MGR_Init(0);

    if (status == BT_NOERROR) {


        /************  Start the RFCOMM server port entity */ 

        channel = 0x02; /* setting the RFCOMM channel number */

        PE_RegisterPort("COM6:", &channel, &portId);


        /************ Create a Service Record */ 

        SDP_CreateRecord(&handle,&pRecord);

        /************ Service class list */

        serial_uuid = 0x1101; /* serial port uuid */
        SDP_EncodeAttribute(SDP_UUID,2,&serial_uuid,&SerialPortUUID);

        /************ Place the uuid in a list */

        list[0] = &SerialPortUUID;
        SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

        /************ Add this as an attribute to the service record */

        SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);



        /************ Protocol descriptor list:: */



        /************ Construct the l2cap protocol entry */

        l2cap_uuid = 0x0100;
        SDP_EncodeAttribute(SDP_UUID,2,&l2cap_uuid,&L2CAP_UUID);

        /************ Place l2cap uuid in a list */

        list[0] = &L2CAP_UUID;
        SDP_CreateList(SDP_SEQUENCE,1,list,&Protocol0);


        /************ Construct the rfcomm entry */

        rfcomm_uuid = 0x0003;
        SDP_EncodeAttribute(SDP_UUID,2,&rfcomm_uuid,&RFCOMM_UUID);

        SDP_EncodeAttribute(SDP_UINT,1,&channel,&RFCOMM_CHANNEL);

        /************ Place rfcomm uuid and chanel number in a list */

        list[0] = &RFCOMM_UUID;
        list[1] = &RFCOMM_CHANNEL;
        SDP_CreateList(SDP_SEQUENCE,2,list,&Protocol1);

        /************ place protocol 0 and protocol 1 in a list */

        list[0] = &Protocol0;
        list[1] = &Protocol1;

        SDP_CreateList(SDP_SEQUENCE,2,list,&ProtocolDescriptorList);

        /************ add this as an attribute to the service record */

        SDP_AddAttribute(pRecord,0x0004,&ProtocolDescriptorList);



        /************ language base list:: */


        /************ language identifier english */

        language_id = ('e' << 8) + 'n';

        SDP_EncodeAttribute(SDP_UINT,2,&language_id,&Lang_id);

        /************ language encoding  utf-8 */

        language_encoding = 106; 

        SDP_EncodeAttribute(SDP_UINT,2,&language_encoding,&Lang_Enc);

        /************ language base */

        language_base = 0x0100;

        SDP_EncodeAttribute(SDP_UINT,2,&language_base,&Lang_base);

        /************ place the entries in a list */

        list[0] = &Lang_id;
        list[1] = &Lang_Enc;
        list[2] = &Lang_base;

        SDP_CreateList(SDP_SEQUENCE,3,list,&Lang_list_1);

        /************ add this to the main list */

        list[0] = &Lang_list_1;

        SDP_CreateList(SDP_SEQUENCE,1,list,&Lang_base_list);

        /************ add this as an attribute to the service record */

        SDP_AddAttribute(pRecord,0x0006,&Lang_base_list);

        /************ service name */

        SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name),Service_name,&Serial_Name);

        /************ add this as an attribute to the service record */

        SDP_AddAttribute(pRecord,language_base + 0,&Serial_Name);
      


        printf("Added SDP record for \"%s\" on RFCOMM channel %d\n", Service_name, channel);

        MGR_Shutdown(0);

        
    } else {

        /************  Handle Stack Initialisation Failure */

        printf("Error initialising bluetooth stack - error code 0x%02X\n", status);

        if (status == BT_HCITERROR) {
            printf("Check the connection to the bluetooth device\n");
        }

    }

    printf("\nProgram exiting.\n");

    return 0;
}
