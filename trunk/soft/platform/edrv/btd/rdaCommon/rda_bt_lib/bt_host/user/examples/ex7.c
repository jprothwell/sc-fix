
/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 7, v1.0
    - Using HCI Functions 
*/


#include <stdlib.h>
#include <stdio.h>

#include "bt.h"


/*  Flag to track program completion.
    Volatile because it is modified from the callback functions */
volatile int terminateProgram = 0;

/* Function prototypes */
void eventCallback(u_int8 *eventPDU);


int main(int argc, char *argv[]) {
    
    int status;

    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 7 - Using HCI Functions\n\n");


    /************  Initialise the Host Stack */

	do {
		status = MGR_Init(0);
	} while (status==BT_RETRY);

    if (status == BT_NOERROR) {


        /************  Register the callback handler */

        HCI_RegisterEventHandler(eventCallback);


        /************  Send the command */

        printf("Sending ReadCountryCode command...\n");
        status = HCI_ReadCountryCode();


        /************  Loop until the events have been received */

        while (!terminateProgram)
            ;
        

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


void eventCallback(u_int8 *eventPDU) {
    /*  
        This function is registered to handle all 
        HCI events received by the host.
    */

    u_int8 code;

    static char *codeLookup[] = {
        "Europe & US",
        "France",
        "Spain",
        "Japan"
    };
    

    if (eventPDU[0] == HCI_evCOMMANDCOMPLETE && 
         (eventPDU[3]|(eventPDU[4]<<8)) == HCI_cmdREADCOUNTRYCODE) {

        code = eventPDU[6];

        printf("Recieved command complete event for ReadCountryCode command");
        printf("\n\t- Device country code is 0x%02X (%s)\n",
            code, codeLookup[code]);

        terminateProgram = 1;

    }   
}
