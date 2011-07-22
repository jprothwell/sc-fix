#include <base.h>
#include <cfw.h>
#include <stdkey.h>
#include <sul.h>
#include "api_msg.h"
#include <ts.h>
#include <cfw_prv.h>
#include <event_prv.h>

//		nOption: initialization option.  
//			nOption = 0x1: delete all of the database 
//			nOption = 0x2: rebuild the all the database
//			nOption = 0x4: remap the SIM data to database
#if 0
BOOL SHL_PbkPowerOn(UINT8 nOption) // 0x02|0x04
{
    CFW_PbkPowerOn(nOption, APP_UTI_SHELL);
	return TRUE;
}
#endif
