
/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 8, v1.0
    - Security-Aware L2CAP Server Application
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bt.h"

/* The PSM number of the service this program is providing */
#define EXAMPLE_PSM 0x1001

/* The maximum amount of data the local application can receive */
#define EXAMPLE_MTU 120

/* The after we handle EXAMPLE_MAXCONNS clients, we exit */
#define EXAMPLE_MAXCONNS 10


/*  The response buffer length - must be large enough  
    to hold a large integer as a string */
#define RESPONSE_BUFFERLEN 20

/* The address of the client device */
#define TARGET_ADDRESS "AE:2D:22:00:FF:17"

/*  Counters to track connected and total connected clients.
    volatile because they are modified from the callback functions */
volatile int numConnections = 0;
volatile int totalConnections = 0;


/* Function prototypes */
void recievedDataHandler(u_int16 channel, t_DataBuf *buffer);
void channelEventHandler(u_int8 eventType, t_L2_ControlCallbackArgs *eventArgs);



int main(int argc, char *argv[]) {
    
    int status;
    t_L2_PsmCallbacks callbackPointers;
    t_bdaddr deviceAddress;
	t_bdaddr remoteAddress;
    char deviceAddressString[MGR_ADDRSTRLEN];
	u_int8 pin[5] = {1,2,3,4,5};

    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 8 - Security-Aware L2CAP Server Application\n\n");



    /************  Initialise the Host Stack */

	do {
		status = MGR_Init(0);
	} while (status==BT_RETRY);

    if (status == BT_NOERROR) {

        HCI_WriteScanEnable(0x03);

        /************  Register the security level */

        MGR_SetSecurityMode(MGR_SERVICELEVEL,NULL);

        MGR_RegisterAppSecurityLevel("Example 8", MGR_L2CAP, EXAMPLE_PSM, MGR_AUTHENTICATION);
 
		MGR_ParseHexBDAddress(TARGET_ADDRESS, &remoteAddress);

        /* Preset the remote device pin code */
		MGR_SetRemoteDevicePIN(remoteAddress, MGR_PRESETPIN, 5, pin);

 
        /************  Retrieve the Local Device Address */

        status = MGR_GetLocalBDAddress(&deviceAddress);

        if (status == BT_NOERROR) {

            MGR_PrintBDAddress(deviceAddress, deviceAddressString);

            printf("Local device address is %s\n", deviceAddressString);

            printf("\n");

        } else {

            printf("Error retrieving device address - error code 0x%02X\n",status);
        }

        /************  Register the PSM */

        memset(&callbackPointers, 0, sizeof(callbackPointers));

        callbackPointers.connectionIndication = channelEventHandler;
        callbackPointers.configureIndication = channelEventHandler;
        callbackPointers.configureConfirmation = channelEventHandler;
        callbackPointers.disconnectionIndication = channelEventHandler;
        callbackPointers.dataIndication = recievedDataHandler;

        status = L2_RegisterPSM(EXAMPLE_PSM, &callbackPointers);


        printf("Server awaiting connections on PSM 0x%04X...\n\n", EXAMPLE_PSM);


        /************  Loop until EXAMPLE_MAXCONNS clients have been handled */

        while (totalConnections < EXAMPLE_MAXCONNS)
            ;


        /************  Exit when no more clients are connected */

        while (numConnections > 0)
            ;
        

        L2_DeregisterPSM(EXAMPLE_PSM);

        MGR_Shutdown(0);

    } else {

        /************  Handle Stack Initialisation Failure */

        printf("Error initialising bluetooth stack - error code 0x%02X \n", status);

        if (status == BT_HCITERROR) {
            printf("Check the connection to the bluetooth device\n");
        }

    }

        
    printf("\nServer exiting.\n");

    return 0;
}


void channelEventHandler(u_int8 eventType,  t_L2_ControlCallbackArgs *eventArgs) {
    /*  
        This function is registered to handle any 
        ConnectionIndication (L2CAP_evCONNECTIND) and 
        DisconnectionIndication (L2CAP_evDISCONNECTIND) 
        events occuring on the PSM.
    */

    char remoteAddressString[MGR_ADDRSTRLEN];

    MGR_PrintBDAddress(eventArgs->bdaddr, remoteAddressString); 

    switch (eventType) {

        case L2CAP_evCONNECTIND:

            /************  Handle a channel establishment */
            numConnections++;
            totalConnections++;
            printf("Got connection from device %s, cid 0x%04X, %d channels active\n",
                remoteAddressString, eventArgs->cid, numConnections);
            L2_ConnectResponse(eventArgs->bdaddr, eventArgs->cid, 0x00, 0x00); 
            break;

        case L2CAP_evCONFIGUREIND:
            /************  Handle a configuration request */
            printf("Got configuration request - sending response\n");
            L2_ConfigureResponse(eventArgs->cid, EXAMPLE_MTU, 0x00, NULL);

            printf("Sending configuration request\n");
            L2_Configure(eventArgs->cid, EXAMPLE_MTU, L2_FTO_USEEXISTING, 0, NULL);

            break;

        case L2CAP_evCONFIGURECONF:
            /************  Handle a configuration confirmation */
            printf("Got configuration confirm - channel configuration complete\n");
            break;

        case L2CAP_evDISCONNECTIND:

            /************  Handle a channel disconnection */
            numConnections--;
            printf("cid 0x%04X closed, %d channels active\n\n", 
                eventArgs->cid, numConnections);
            break;
    }

}


void recievedDataHandler(u_int16 channel, t_DataBuf *buffer) {
    /*  
        This function is called whenever data is recieved on the channel.
        It expects to recieve a null-terminated string containing a 
        comma-delimited list of integers. It adds the numbers and
        returns the result to the client program.
    */
    
    int total;
    char *parseOffset;
    int status;
    t_DataBuf *responseBuffer;

    printf("Recieved appliation request on cid 0x%04X: \"%s\"\n", channel, buffer->data);

    /************  Total the numbers in the string */

    parseOffset = buffer->data;
    total = atoi(parseOffset);
    while (parseOffset = strchr(parseOffset,',')) {
        total += atoi(++parseOffset);
    }   

    /************  Send the response */

    L2_GetWriteBuffer(channel, RESPONSE_BUFFERLEN, &responseBuffer); 

    responseBuffer->dataLen = 1 + sprintf(responseBuffer->data, "%d", total);
    printf("\t- returning result \"%s\"\n", responseBuffer->data);

    status = L2_Write(channel, responseBuffer);
    
    if (status != BT_NOERROR) {

        /************  Handle write failure */

        printf("Error writing to channel 0x%04X - error code 0x%02X\n", 
            channel, status);

    }

    L2_FreeWriteBuffer(buffer);

}

