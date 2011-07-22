/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 5, v1.1
    - Adding an SDP record 
*/



#include "bt.h"
#include <stdio.h>



/* The service uuid to be added - 0x1103 == dial-up networking  */
#define DIALUP_SERVICE_UUID SDP_SCLASS_UUID_DIALUP

/* Some uuids used in the protocol descriptor list (pdl) */
#define L2CAP_UUID  SDP_PROTOCOL_UUID_L2CAP
#define RFCOMM_UUID SDP_PROTOCOL_UUID_RFCOMM


/* The L2CAP PSM on which this RFCOMM instance is available */
#define RFCOMM_PSM  0x1003

/* The name of the service to be added */
#define SERVICE_NAME    "Dial-up networking"

int main(int argc, char *argv[]) {
    
    int status;
    u_int32 recordHandle;
    t_SDP_serviceRecord *pRecord;
    t_SDP_dataElement attrib_dialupUUID, attrib_dialupUUIDlist;
    t_SDP_dataElement attrib_l2capUUID, attrib_l2capList;
    t_SDP_dataElement attrib_rfcommUUID, attrib_rfcommCid, attrib_rfcommList;
    t_SDP_dataElement attrib_protocolDescriptorList;
    t_SDP_dataElement attrib_serviceName;
    t_SDP_dataElement *attributeArray[10];

    u_int32 currentUUID;
    /* The language offset which we are using (this is the default) */
    u_int32 languageOffset = 0x100; 

    /* The l2cap cid which rfcomm has been allocated (for the pdl) */
    u_int16 rfcommPsm = RFCOMM_PSM;



    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 5 - Adding an SDP record\n\n");


    /************  Initialise the Host Stack */

	do {
		status = MGR_Init(0);
	} while (status==BT_RETRY);

	HCI_WriteScanEnable(3);
	
    if (status == BT_NOERROR) {


        /************  Create an empty record */

        status = SDP_CreateRecord(&recordHandle, &pRecord);


        /************  Encode an attribute containing the service UUID (service == dialup) */

        currentUUID = DIALUP_SERVICE_UUID;
        status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&currentUUID, &attrib_dialupUUID);


        /************  Add the attribute to a list */

        attributeArray[0] = &attrib_dialupUUID;
        status = SDP_CreateList(SDP_SEQUENCE, 1, attributeArray, &attrib_dialupUUIDlist);


        /************  Add the list as a record attribute (attribute == service class) */

        status = SDP_AddAttribute(pRecord,SDP_AID_SERVICE_CLASS, &attrib_dialupUUIDlist);



        /************  Encode an attribute containing a protocol UUID (service == l2cap) */

        currentUUID = L2CAP_UUID;
        status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&currentUUID, &attrib_l2capUUID);


        /************  Add the attribute to a list */

        attributeArray[0] = &attrib_l2capUUID;
        status = SDP_CreateList(SDP_SEQUENCE, 1, attributeArray, &attrib_l2capList);


        /************  Encode an attribute containing a protocol UUID (service == rfcomm) */

        currentUUID = RFCOMM_UUID;
        status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&currentUUID, &attrib_rfcommUUID);


        /************  Encode an attribute containing an UINT (int == channelid)*/
        status = SDP_EncodeAttribute(SDP_UINT, 1, (u_int8 *)&rfcommPsm, &attrib_rfcommCid);


        /************  Add the attributes to a list */

        attributeArray[0] = &attrib_rfcommUUID;
        attributeArray[1] = &attrib_rfcommCid;
        status = SDP_CreateList(SDP_SEQUENCE, 2, attributeArray, &attrib_rfcommList);


        /************  Enclose both lists in an outer list */
        attributeArray[0] = &attrib_l2capList;
        attributeArray[1] = &attrib_rfcommList;

        status = SDP_CreateList(SDP_SEQUENCE, 2, attributeArray, 
                    &attrib_protocolDescriptorList);


        /************  Add the outer list as a record attribute (attribute == pdl) */

        status = SDP_AddAttribute(pRecord,SDP_AID_PROTOCOL, &attrib_protocolDescriptorList);



        /************  Encode an attribute containing a string (string == service name)*/

        status = SDP_EncodeAttribute(SDP_TEXT, sizeof(SERVICE_NAME), SERVICE_NAME,
                     &attrib_serviceName);


        /************  Add the list as a record attribute (attribute == service name) */

        status = SDP_AddAttribute(pRecord, (u_int16)(SDP_AID_SERVICE_NAME_OFFSET 
                      + languageOffset), &attrib_serviceName);

        printf("Added SDP record for \"%s\", uuid 0x%08X\n", SERVICE_NAME, DIALUP_SERVICE_UUID);
        printf("\nProtocol descriptor list = \n\t\t ( ( uuid 0x%08X ), ( uuid 0x%08X, psm 0x%04X ) )\n", 
                    L2CAP_UUID, RFCOMM_UUID, RFCOMM_PSM);


        /************ Allow the database to be queried */

        printf("\n\nWaiting forever...");

        while (1)
            ;

        MGR_Shutdown(0);

        
    } else {

        /************  Handle Stack Initialisation Failure */

        printf("Error initialising bluetooth stack - error code 0x%02X", status);

        if (status == BT_HCITERROR) {
            printf("Check the connection to the bluetooth device\n");
        }

    }
        
    printf("\nProgram exiting.\n");

    return 0;
}
