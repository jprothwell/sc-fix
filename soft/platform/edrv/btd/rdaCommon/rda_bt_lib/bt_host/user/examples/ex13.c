
/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 13, v1.0
    - TCS Intercom Call - Originating Party
*/


#include <stdio.h>
#include "bt.h"

/* Number of bytes to reserve for device name - possible max. length up to 248 characters */
#define NAME_BUFSIZE 80


/* The address of the device to set up the call to */
#define TARGET_ADDRESS "DB:DB:DB:DB:00:01"

/*  Flag  
    volatile because they are modified from the callback functions */
volatile int callComplete = 0;
volatile int terminateProgram = 0;

/* TCS Callbacks */
int CB_tcsConnectIndication(t_bdaddr address, u_int16 callHandle, t_TCS_callParams *callParams);
void CB_tcsCallCompleteIndication(t_bdaddr address, u_int16 callHandle);
int CB_tcsReleaseIndication(u_int16 callHandle, t_TCS_callParams *callParams);
void CB_tcsCallReleaseComplete(t_bdaddr address, u_int16 callHandle, t_TCS_callParams *callParams);

/* Callback structure for TCS */
t_TCS_callbacks TLcallbacks = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	CB_tcsConnectIndication,
	NULL,
	CB_tcsCallCompleteIndication,
	NULL,
	CB_tcsReleaseIndication,
	CB_tcsCallReleaseComplete,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL

};




int main(int argc, char *argv[]) {
    
    int status;
    char deviceName[NAME_BUFSIZE];
    u_int8 deviceNameLength;
    t_bdaddr deviceAddress;
    char deviceAddressString[MGR_ADDRSTRLEN];
	t_bdaddr remoteAddress;
	t_TCS_callParams callParams;
	u_int16 newCallHandle;

    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 13 - TCS Intercom Call: Originating Party\n\n");


    /************  Retrieve Host Version Information */

    printf("Host software stack version %s\n\n",MGR_GetHostVersion());

    /************  Initialise the Host Stack */

    printf("Initialising Host Stack....\n\n");

    status = MGR_Init(0);

    if (status == BT_NOERROR) {

        /************  Retrieve Device Address */

        status = MGR_GetLocalBDAddress(&deviceAddress);

        if (status == BT_NOERROR) {

            MGR_PrintBDAddress(deviceAddress, deviceAddressString);

            printf("Local device address is %s\n", deviceAddressString);

            printf("\n");

        } else {

            printf("Error retrieving device address - error code 0x%02X\n",status);
        }


        /************  Retrieve Device Name */

        status = MGR_GetLocalName(deviceName, NAME_BUFSIZE, &deviceNameLength);

        if (status == BT_NOERROR) {

            printf("Local device name is \"%s\"", deviceName);

            if (deviceNameLength > NAME_BUFSIZE-1) {
                printf(" (truncated to %d characters)\n", NAME_BUFSIZE-1);
            } else {
                printf("\n");
            }

            printf("\n");

        } else {

            printf("Error retrieving device name - error code 0x%02X \n",status);
        }   


		 /************  Register the TCS Terminal for Intercom (TCSBIN) only */
		TCS_TLRegister(PRH_TCS_REGISTER_TCSBIN, &TLcallbacks, 0, 0, 0);
		printf("Registered for Intercom calls, waiting for incoming call.....\n\n");
		
		/* Initiate Intercom call with remote device - call class parameter is mandatory */
		MGR_ParseHexBDAddress(TARGET_ADDRESS, &remoteAddress);		
		callParams.bitMask = PRH_TCS_CC_PARAM_CALLCLASS | PRH_TCS_CC_PARAM_SENDINGCOMPLETE;
		callParams.callClass = PRH_TCS_CALL_CLASS_INTERCOM;
		status = TCS_TLIntercomSetup(&callParams, remoteAddress, &newCallHandle);
		if(BT_NOERROR == status)
			printf("Initiating Intercom Call with device %s - call handle %d\n", TARGET_ADDRESS, newCallHandle);
		else
			printf("Error in initiating Intercom Call - error code 0x%02X\n",status);
			
		while(!callComplete);

		/* Disconnect Call */
		callParams.bitMask = 0;
		status = TCS_TLDisconnect(newCallHandle, &callParams);
		if(BT_NOERROR == status)
		{
			printf("Disconnecting Call - call handle %d\n", newCallHandle);
			while(!terminateProgram); /* wait for call to be released */
		}
		else
			printf("Error in disconnecting call - error code 0x%02X\n",status);
	
		
        /************  Shutdown the Host Stack */
		
        printf("Shutting down the Host Stack\n");

        MGR_Shutdown(MGR_FORCED);
		

    } else {

        /************  Handle Stack Initialisation Failure */

        printf("Error initialising bluetooth stack - error code 0x%02X\n",status);

        if (status == BT_HCITERROR || status == BT_HWERROR) {
            printf("Check the connection to the bluetooth device\n");
        }

    }

    printf("\nProgram Exiting.\n");

    return 0;
}



int CB_tcsConnectIndication(t_bdaddr address, u_int16 callHandle, t_TCS_callParams *callParams)
{
	char addrStr[20];

	MGR_PrintBDAddress(address, addrStr);
	/* use default operation, i.e. respond with TCS_ConenctAcknowledge */
	printf("Received CONNECT message from device %s - call handle %d\n", addrStr, callHandle);
	return 0;
}


void CB_tcsCallCompleteIndication(t_bdaddr address, u_int16 callHandle)
{
	char addrStr[20];

	MGR_PrintBDAddress(address, addrStr);
	printf("Call with device %s is active - call handle %d\n", addrStr, callHandle);
	callComplete = 1;
}


int CB_tcsReleaseIndication(u_int16 callHandle, t_TCS_callParams *callParams)
{
	printf("Received RELEASE message for call - call handle %d\n", callHandle);
	// use default operation - return TCS_ReleaseComplete
	return 0;
}

void CB_tcsCallReleaseComplete(t_bdaddr address, u_int16 callHandle, t_TCS_callParams *callParams)
{
	printf("Call Released - call handle %d\n", callHandle);
	terminateProgram = 1;
}