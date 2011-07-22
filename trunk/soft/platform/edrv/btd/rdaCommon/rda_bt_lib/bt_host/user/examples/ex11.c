/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 11, v1.0
    - Performing an SDP search for an RFCOMM server
*/


#include <stdlib.h>
#include <stdio.h>

#include "bt.h"


/* The service uuid to be searched for - 0x1101 == serial port  */
#define SERVICE_SOUGHT      0x1101 

/* The service attribute to be retrieved - 0x0004 == protocol descriptor list */
#define ATTRIBUTE_SOUGHT    0x0004


u_int16 sample_results(u_int8 *pData);


int main(int argc, char *argv[]) {
    
    int status;
    bdaddr_t addressList[1];
    SDP_Addresses_t devices;
    SDP_Relation_t criteria;
    SDP_SearchPattern_t pattern;
    SDP_AttributeIDs_t attribs;
    SDP_StopRule_t rule;
    SDP_Results_t *pResults;
    SDP_Attribute_t* currentAttrib; 
    char remoteAddressString[BT_ADDRESSSTRING_LEN]; 
    u_int handle; 
    u_int8* recordOffset; 
    u_int numRecords, nameLen;

    portid_t portId;

    u_int8 serverChannel;



    printf("Parthus Technologies Bluetooth Host Software\n");
    printf("Example Program 11 - Performing an SDP search for an RFCOMM server\n\n");


    /************  Initialise the Host Stack */

    status = BT_InitStack();

    if (status == BT_NOERROR) {


        /************  Setup the device list to be searched  */

        BT_ParseBDAddress("50:FF:A8:B4:00:20",&addressList[0]);

        devices.number = 1;
        devices.addresses = addressList


        /************  Setup the device criteria ( all | trusted ) devices  */

        criteria = SDP_ALL_DEVICES; 


        /************  Setup the pattern to search for  */

        pattern.items = 1; 
        patternValue = SERVICE_SOUGHT;
        pattern.pPatterns = &patternValue;


        /************  Setup the attributes to retrieve  */

        attribs.items = 1; 
        attributeValue = ATTRIBUTE_SOUGHT;
        attribs.attributes = &attributeValue;


        /************  Setup the stop rules (10 matches or 60 seconds) */

        rule.items = 10;
        rule.duration = 60;


        /************  Begin the search */

        status = SDP_ServiceSearchEv(devices, criteria, pattern, attribs, 
                        0, rule,&handle, &pResults, NULL);


        /************  Display the results and return the server channel */

        serverChannel = Parse_Results(pResults->element.attribs.pData);


        /************  Connect to the RFCOMM server */

        PE_ConnectPort(addressList[0], serverChannel, "COM7:", &portId);


        BT_ShutdownStack();

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




/*
  This function will print out the contents of the protocol descriptor list
  which is of the following format

  list( list( l2cap_uuid ) , list( rfcomm_uuid , channel ) )

  the channel number the then returned.

 */
u_int8  sample_results(u_int8* pData) {

    u_int16 sdp_u16 = 0x09;
    u_int16 sdp_list = 0x35;
    u_int16 sdp_u8 = 0x08;
    u_int16 sdp_uu16 = 0x19;
    
    u_int16 offset;
    
    u_int8 channel;

    if(*pData++ == sdp_list) { /* outer list of attribute list */
    
        pData++;
        if(*pData++ == sdp_u16) { /* attribute id */
        
            printf("Attribute id");
            if( (pData[0] << 8) + pData[1] == 0x00004) { /* equals protocol descriptor list */
            
                pData +=2;
                printf("Protocol Descriptor List ");
                pData++;

                for(offset = 0 ; offset < pData[0] ; )  { /* for contents of descriptor list */
            
                    offset+=2;
                    printf("{");

                    switch(pData[offset]) { /* switch on protocol entry length */
                    case 3: /* protocol contains only a uuid */
                        if(pData[offset+1] == sdp_uu16) {
                            printf("   UUID : %d", (pData[offset+2] << 8) + pData[offset+3]);
                        }
                        offset += 3;
                        printf("}");
                        break;

                    case 5: /* protocl contains a uuid and a u_int8 parameter */
                        if(pData[offset+1] == sdp_uu16) {
                            printf("   UUID : %d", (pData[offset+2] << 8) + pData[offset+3]);
                        }
                        if(pData[offset+4] == sdp_u8) {
                            printf("   Parameter : %d", pData[offset + 5]);           
                            channel=pData[offset + 5]
                        }
                        offset += 5;
                        printf("}");
                        break;
                    }
                }         
            }
        }
    }

    return channel;
}





