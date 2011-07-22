#ifndef __CFW_MULTI_SIM_H__

#define __CFW_MULTI_SIM_H__

#define CFW_SIM_NUMBER NUMBER_OF_SIM

#if (CFW_SIM_NUMBER!=1)
#define CFW_MULTI_SIM
#endif

#ifdef CFW_MULTI_SIM
typedef enum {
	CFW_SIM_0 = 0x00,
	CFW_SIM_1 = 0x01,
	CFW_SIM_COUNT = CFW_SIM_NUMBER,
	CFW_SIM_END = 0xFF
}CFW_SIM_ID;
#endif

#endif
