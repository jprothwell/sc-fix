 /* 
    Parthus Technologies Bluetooth Host Stack
    Programmers Manual Example Program 6, v1.1
    - Performing an SDP search 
*/


#include <stdlib.h>
#include <stdio.h>

#include "bt.h"


/* The service uuid to be searched for - 0x1103 == dial-up networking  */
#define SERVICE_SOUGHT      SDP_SCLASS_UUID_DIALUP

/* The service attribute to be retrieved - 0x0100 == service name, default language */
#define ATTRIBUTE_SOUGHT    0x100

volatile u_int8 searchRunning;

void callback(u_int8 status);

int main(int argc, char *argv[]) {
    
    int status;
    t_bdaddr addressList[1];
    char remoteAddressString[MGR_ADDRSTRLEN];   
    t_SDP_Addresses devices;
    u_int16 criteria;
    t_SDP_SearchPattern pattern;
    t_SDP_AttributeIDs attribs;
    t_SDP_StopRule rule;
    t_SDP_Results *pResults;
    t_SDP_Element *currentResults;  
    u_int8 *recordOffset; 
    u_int32 numRecords, nameLen;
    u_int32 patternValue;
    u_int32 attributeValue;
    
    
    printf("\nParthus Technologies Bluetooth Host Software\n");
    printf("Example Program 6 - Performing an SDP search\n\n");
    
    
    searchRunning = 1;
    
    /************  Initialise the Host Stack */
    
	do {
		status = MGR_Init(0);
	} while (status==BT_RETRY);
    
    if (status == BT_NOERROR) {
        
        
        /************  Setup the device list to be searched - normally several devices */
        
        MGR_ParseHexBDAddress("dbdbdbdb0001",&addressList[0]);
        devices.numItems = 1;
        devices.addresses = addressList;
        
        
        /************  Setup the device criteria ( all | trusted ) devices  */
        
        criteria = SDP_ZERO_PIN; 
        
        
        /************  Setup the pattern to search for  */
        
        pattern.numItems = 1; 
        patternValue = SERVICE_SOUGHT;
        pattern.patternUUIDs = &patternValue;
        
        
        /************  Setup the attributes to retrieve  */
        
        attribs.numItems = 1; 
        attributeValue = ATTRIBUTE_SOUGHT;
        attribs.attributeIDs = &attributeValue;
        
        
        /************  Setup the stop rules (10 matches or 20 seconds) */
        
        rule.maxItems = 10;
        rule.maxDuration = 20;
        rule.maxBytes = 250;
        
        
        /************  Begin the search */
        
        status = SDP_ServiceSearch(&pResults,&devices,&pattern, &attribs, 
            &rule,criteria,callback);
        
        while(searchRunning)
            ;
        
        printf("Search matched %d devices\n",pResults->numElements);
        
        
        if (pResults->numElements) {
            /************  Display the results */
            currentResults = &(pResults->element);
            
            while (currentResults) {
                
                MGR_PrintBDAddress(currentResults->deviceAddress, remoteAddressString);
                
                printf("Retrieved %d records from device %s\n",
                    currentResults->numMatchingRecords, remoteAddressString);
                
                numRecords = currentResults->numMatchingRecords;  
                recordOffset = currentResults->dataBuf;
                recordOffset += 5; /* skip the outer list identifier and length byte */
                
                while(numRecords) {
                    
                    /************  The returned attributes are stored sequentially... */
                    
                    recordOffset += 3; /* skip the attribute identifier type identifier and value */
                    
                    nameLen = recordOffset[1]; /* skip the string type identifier */
                    recordOffset += 2; /* skip the string header (type & length) */
                    
                    printf("\t Service Name : %.*s", nameLen, recordOffset);
                    
                    recordOffset += nameLen;
                    
                    numRecords--;
                }
                
                currentResults = currentResults->pNext;
            }
        }
        
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
 
void callback(u_int8 status) {
    searchRunning = 0;
}

