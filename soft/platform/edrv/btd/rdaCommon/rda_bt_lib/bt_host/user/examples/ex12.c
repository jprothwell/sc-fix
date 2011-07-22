
/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 12, v1.0
    - TCS Intercom Call - Terminating Party
*/


#include <stdio.h>
#include "bt.h"

/* Number of bytes to reserve for device name - possible max. length up to 248 characters */
#define NAME_BUFSIZE 80

/*  Flag to track program completion.
    volatile because it is modified from the callback functions */
volatile int terminateProgram = 0;

/* TCS Callbacks */
int CB_tcsSetupIndication(t_bdaddr address, u_int16 callHandle, u_int8 callType, t_TCS_callParams *callParams);
void CB_tcsCallCompleteIndication(t_bdaddr address, u_int16 callHandle);
int CB_tcsDisconnectIndication(u_int16 callHandle, t_TCS_callParams *callParams);
void CB_tcsCallReleaseComplete(t_bdaddr address, u_int16 callHandle, t_TCS_callParams *callParams);

/* Callback structure for TCS */
t_TCS_callbacks TLcallbacks = {
	CB_tcsSetupIndication,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	CB_tcsCallCompleteIndication,
	CB_tcsDisconnectIndication,
	NULL,
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


    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 12 - TCS Intercom Call: Terminating Party\n\n");


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
	
		while(!terminateProgram);
		
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


int CB_tcsSetupIndication(t_bdaddr address, u_int16 callHandle, u_int8 callType, t_TCS_callParams *callParams)
{
	char addrStr[20];
	t_TCS_callParams retCallParams;
	int status;

	if(PRH_TCS_CALLTYPE_TCSBIN == callType)
	{
		MGR_PrintBDAddress(address, addrStr);
		printf("Incoming Intercom call from device %s\n", addrStr);
	
		/* Connect the call */
		retCallParams.bitMask = 0; /* no parameters in this message */
		status = TCS_TLConnect(callHandle, &retCallParams);
		if(BT_NOERROR == status)
			printf("Connecting to Call\n");
		else
			printf("Error in connecting to Call - error code 0x%02X\n", status);
	}

	return 1; /* do not use default handling */
}


void CB_tcsCallCompleteIndication(t_bdaddr address, u_int16 callHandle)
{
	char addrStr[20];

	MGR_PrintBDAddress(address, addrStr);
	printf("Call with device %s is active - call handle %d\n", addrStr, callHandle);
}

int CB_tcsDisconnectIndication(u_int16 callHandle, t_TCS_callParams *callParams)
{
	t_TCS_callParams retCallParams;
	int status;

	printf("Received DISCONNECT message for call - call handle %d\n", callHandle);
	retCallParams.bitMask = 0;
	/* Respond with RELEASE message (could also use default handling) */
	
	status = TCS_TLRelease(callHandle, &retCallParams);
	if(BT_NOERROR == status)
		printf("Responded with RELEASE message\n");
	else
		printf("Error in sending RELEASE message - error code 0x%02X\n", status);
	
	return 1; /* do not use default handling */
}

void CB_tcsCallReleaseComplete(t_bdaddr address, u_int16 callHandle, t_TCS_callParams *callParams)
{
	printf("Call Released - call handle %d\n", callHandle);
	terminateProgram = 1;
}