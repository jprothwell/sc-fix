
/* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 1, v1.0
    - Retrieving Device Information
*/


#include <stdio.h>
#include "bt.h"

/* Number of bytes to reserve for device name - possible max. length up to 248 characters */
#define NAME_BUFSIZE 80

int main(int argc, char *argv[]) {
    
    int status;
    t_MGR_HCVersion versionInfo;
    char deviceName[NAME_BUFSIZE];
    u_int8 deviceNameLength;
    t_bdaddr deviceAddress;
    char deviceAddressString[MGR_ADDRSTRLEN];
    u_int8 localFeatures[8]; /* device features are 8 bytes */


    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 1 - Retrieving Device Information\n\n");


    /************  Retrieve Host Version Information */

    printf("Host software stack version %s\n\n",MGR_GetHostVersion());

    /************  Initialise the Host Stack */

    printf("Initialising Host Stack....\n\n");

	do {
		status = MGR_Init(0);
	} while (status==BT_RETRY);

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

		do {
			status = MGR_GetLocalName(deviceName, NAME_BUFSIZE, &deviceNameLength);
		} while (status==BT_RETRY);

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

        /************  Retrieve Device Version Information */

        printf("Host Controller Version Information:\n");

        status = MGR_GetDeviceVersion(&versionInfo);
        
        if (status == BT_NOERROR) {

            printf("\tHCI version 0x%02X, HCI revision 0x%02X\n",
                versionInfo.HCIversion, versionInfo.HCIrevision);

            printf("\tLMP version 0x%02X, LMP subversion 0x%02X\n",
                versionInfo.LMPversion, versionInfo.LMPsubversion);

            printf("\tManufacturer Identification Code 0x%02X\n\n",
                versionInfo.ManufacturerId);

        } else {

            printf("Error retrieving device address - error code 0x%02X\n",status);
        }

        /************  Retrieve Device Supported Features */

        printf("Host Controller Supported Features:\n");

        status = MGR_GetLocalFeatures(localFeatures);
        
        if (status == BT_NOERROR) {

            printf("\tDevice Local Features: 0x%02X%02X%02X%02X%02X%02X%02X%02X\n\n",
                localFeatures[7],localFeatures[6],localFeatures[5],localFeatures[4],
                localFeatures[3],localFeatures[2],localFeatures[1],localFeatures[0]);

        } else {

            printf("Error retrieving device address - error code 0x%02X\n",status);
        }


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
