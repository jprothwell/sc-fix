#ifndef HCI_ERROR_H
#define HCI_ERROR_H

#include "host_config.h"


/*
  These errors might be better used for some sort of debugging api . . 
  as most of these are meaningless to the upper applications.
 */


extern int SDP_Errno; 

enum SDP_ErrorCodes {
  SDP_NoError = 0x00,
  SDP_UnSupported = 0x01,
  SDP_InvalidHandle = 0x02,
  SDP_InvalidSyntax = 0x03,
  SDP_InvalidSize = 0x04,
  SDP_InvalidContinuation = 0x05,
  SDP_InsufficientResources = 0x06
};
  
extern char* SDP_ErrorStrings[];



#endif /* HCI_ERROR_H */
