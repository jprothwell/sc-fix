#ifndef __CFW_MULTI_SIM_H__

#define __CFW_MULTI_SIM_H__

#ifdef __MMI_MULTI_SIM__
typedef enum {
	CFW_SIM_0 = 0x00,
	CFW_SIM_COUNT = MMI_SIM_NUMBER,
	CFW_SIM_END = 0xFF
}CFW_SIM_ID;
#endif

#endif
