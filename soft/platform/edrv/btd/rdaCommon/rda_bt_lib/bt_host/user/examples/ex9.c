
/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 9, v1.0
    - Security-Aware L2CAP Client Application
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bt.h"


/* The PSM number of the service to connect to */
#define EXAMPLE_PSM 0x1001

/* The maximum amount of data the local application can receive */
#define EXAMPLE_MTU 120

/* The maximum and minimum number of integers in a request string */
#define REQUEST_MAXENTRIES 8
#define REQUEST_MINENTRIES 4

/*  The request buffer length - must be large enough  
    to hold REQUEST_MAXENTRIES comma-seperated integers */
#define REQUEST_BUFFERLEN 100

/* The address of the device to connect to */
#define TARGET_ADDRESS "AE:2D:22:00:0B:B3"

/*  Flag to track program completion.
    Volatile because it is modified from the callback functions */
volatile int terminateProgram = 0;

/* Function prototypes */
void recievedDataHandler(u_int16 channel, t_DataBuf *buffer);
void channelEventHandler(u_int8 eventType, t_L2_ControlCallbackArgs *eventArgs);
void sendRequest(u_int16 cid);


int main(int argc, char *argv[]) {
    
    int status;
    t_L2_PsmCallbacks callbackPointers;
    t_bdaddr remoteAddress;
    t_bdaddr deviceAddress;
    char deviceAddressString[MGR_ADDRSTRLEN];
	u_int8 pin[5] = {1, 2, 3, 4, 5};

    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 9 - Security-Aware L2CAP Client Application\n\n");



    /************  Seed the random number generator   */

    /* 
        This uses the command line argument to seed the random number generator.
        If this is not done, (i.e. a numeric argument is not supplied) the random
        numbers generated may be the same on each run of the program
    */

    if (argc>1) {
        srand(atoi(argv[argc-1]));
    }


    /************  Initialise the Host Stack */

	do {
		status = MGR_Init(0);
	} while (status==BT_RETRY);

    if (status == BT_NOERROR) {


        /************  Register the security level */

		MGR_ParseHexBDAddress(TARGET_ADDRESS, &remoteAddress);

        MGR_SetSecurityMode(MGR_SERVICELEVEL,NULL);

        MGR_RegisterAppSecurityLevel("Example 9", MGR_L2CAP, EXAMPLE_PSM, MGR_AUTHENTICATION);

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

        callbackPointers.connectionConfirmation = channelEventHandler;
        callbackPointers.configureIndication = channelEventHandler;
        callbackPointers.configureConfirmation = channelEventHandler;
        callbackPointers.disconnectionConfirmation = channelEventHandler;
        callbackPointers.dataIndication = recievedDataHandler;

        status = L2_RegisterPSM(EXAMPLE_PSM, &callbackPointers);
    
        printf("Connecting to device %s on PSM 0x%04X...\n",TARGET_ADDRESS, EXAMPLE_PSM);

        status = L2_Connect(remoteAddress, EXAMPLE_PSM);
        

        /************  Loop until the disconnection has been completed */

        while (!terminateProgram)
            ;
        

        L2_DeregisterPSM(EXAMPLE_PSM);

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


void channelEventHandler(u_int8 eventType, t_L2_ControlCallbackArgs *eventArgs) {
    /*  
        This function is registered to handle any 
        ConnectionConfirmation (L2CAP_evCONNECTCONF) and 
        DisconnectionIndication (L2CAP_evDISCONNECTIND) 
        events occuring on the PSM.
    */

    char remoteAddressString[MGR_ADDRSTRLEN];

    switch (eventType) {

        case L2CAP_evCONNECTCONF:
            /************  Handle a channel establishment */

            MGR_PrintBDAddress(eventArgs->bdaddr, remoteAddressString); 

            if (eventArgs->result == L2CAP_NOERROR) {
                
                printf("Connected to device %s, cid 0x%04X\n\n",
                    remoteAddressString, eventArgs->cid);

                printf("Sending configuration request\n");
                L2_Configure(eventArgs->cid, EXAMPLE_MTU, L2_FTO_USEEXISTING, 0, NULL);

            } 
			else if (eventArgs->result == BT_PENDING) 
			{
                printf("Received Connection Confirmation with result = pending\n");
			}
			else {

                printf("Connection to device %s failed, L2CAP error code 0x%04X\n",
                    remoteAddressString, eventArgs->result);

                terminateProgram = 1;
            }

            break;

        case L2CAP_evCONFIGUREIND:
            /************  Handle a configuration request */
            printf("Got configuration request - sending response\n");
            L2_ConfigureResponse(eventArgs->cid, EXAMPLE_MTU, 0x00, NULL);
            printf("Sent configuration response - channel configuration complete\n");

            /* The connection is now configured in both directions */
            sendRequest(eventArgs->cid);
            
            break;

        case L2CAP_evCONFIGURECONF:
            /************  Handle a configuration confirmation */
            printf("Got configuration confirm\n");
            break;

        case L2CAP_evDISCONNECTCONF:
            /************  Handle a channel disconnection */
            printf("Channel 0x%04X closed\n", eventArgs->cid);

            terminateProgram = 1;
            break;

    }

}

void sendRequest(u_int16 cid) {
    /*  
        This function is called with a channel identifier.
        It builds a request buffer containing a random list
        of comma-delimited integers and writes the buffer to
        the supplied channel.
    */

    int bufferOffset, numEntries, i;
    t_DataBuf *requestBuffer;

    L2_GetWriteBuffer(cid, REQUEST_BUFFERLEN, &requestBuffer);


    /* Choose a number of entries between REQUEST_MINENTRIES and REQUEST_MAXENTRIES */
    numEntries = rand()%REQUEST_MINENTRIES+REQUEST_MAXENTRIES-REQUEST_MINENTRIES;

    /* Limit the entries to 1000 for readibility */
    bufferOffset = sprintf(requestBuffer->data, "%d", rand()%1000);

    for (i=1; i<numEntries; i++) {
        bufferOffset += sprintf(requestBuffer->data+bufferOffset, ",%d", rand()%1000);
    }

    requestBuffer->dataLen=bufferOffset+1;
    printf("Sending request string \"%s\" on cid 0x%04X\n\n",requestBuffer->data, cid);

    L2_Write(cid, requestBuffer);
}


void recievedDataHandler(u_int16 channel, t_DataBuf *buffer) {
    /*  
        This function is called whenever data is recieved on the channel.
        It expects to recieve a null-terminated string containing an
        integer which is the sum of the numbers sent in the previous request 
    */
    
    /************  Determine the sender device address for information */


    printf("Recieved response on cid 0x%04X: \"%s\"\n",     
        channel, buffer->data);


    /************  Print the response */

    printf("\t- returned result is \"%s\"\n\n", buffer->data);

    L2_FreeWriteBuffer(buffer);

    /************  Close the channel */

    printf("Disconnecting cid 0x%04X...\n", channel);

    L2_Disconnect(channel);
}

