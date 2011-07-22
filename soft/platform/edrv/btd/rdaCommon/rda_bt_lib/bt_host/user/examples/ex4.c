
/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 4, v1.0
    - Performing an Inquiry Procedure
*/


#include <stdlib.h>
#include <stdio.h>

#include "bt.h"


/*  Flag to track program completion.
    volatile because it is modified from the callback functions */
volatile int terminateProgram = 0;

/* Function prototypes */
void inquiryCallback(t_MGR_DeviceEntry *deviceInfo, int status);


int main(int argc, char *argv[]) {
    
    int status;
    t_bdaddr deviceAddress;
    char deviceAddressString[MGR_ADDRSTRLEN];

    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 4 - Performing an Inquiry Procedure\n\n");


    /************  Initialise the Host Stack */

	do {
		status = MGR_Init(0);
	} while (status==BT_RETRY);

    if (status == BT_NOERROR) {


        /************  Retrieve the Local Device Address */

        status = MGR_GetLocalBDAddress(&deviceAddress);

        if (status == BT_NOERROR) {

            MGR_PrintBDAddress(deviceAddress, deviceAddressString);

            printf("Local device address is %s\n", deviceAddressString);

            printf("\n");

        } else {

            printf("Error retrieving device address - error code 0x%02X\n",status);
        }


        /************  Perform the Inquiry  */

        printf("Beginning inquiry proecdure for all devices for 10 seconds...\n\n");

        status = MGR_Inquiry(10, BT_GIAC, 0, inquiryCallback);
        

        /************  Loop until the inquiry has been completed */

        while (!terminateProgram)
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


void inquiryCallback(t_MGR_DeviceEntry *deviceInfo, int status) {
    /*  
        This function is registered to handle all 
        inquiryResults received by the host.
    */

    static int devicesDiscovered = 0;
    char remoteAddressString[MGR_ADDRSTRLEN];   

    if (!status) {
        if (deviceInfo) {

            devicesDiscovered++;

            MGR_PrintBDAddress(deviceInfo->address, remoteAddressString);

            printf("Discovered device %d, address %s, class 0x%06X\n", 
                devicesDiscovered, remoteAddressString, deviceInfo->classOfDevice); 

        } else {
            printf("\nInquiry procedure complete, discovered %d devices\n", devicesDiscovered);
            devicesDiscovered = 0;
            terminateProgram = 1;

        }
    } else {
        printf("\nHost Controller error 0x%02X from MGR_Inquiry procedure\n", status);
        devicesDiscovered = 0;
        terminateProgram = 1;

    }
}
