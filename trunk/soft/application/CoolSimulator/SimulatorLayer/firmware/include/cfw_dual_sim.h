#ifndef __CFW_DUAL_SIM_H__

#define __CFW_DUAL_SIM_H__

#ifdef CFW_DUAL_SIM
typedef enum {
	CFW_SIM_0 = 0x00,
	CFW_SIM_1 = 0x01,
	CFW_SIM_COUNT = 0x02,
	CFW_SIM_END = 0xFF
}CFW_SIM_ID;
#endif

#endif
