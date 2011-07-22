#include "fmg_m.h"
#include "sxr_ops.h"
#include "sxs_io.h"

#include "key_defs.h"
#include "hal_key.h"
#include "hal_pwm.h"

#include "tgt_m.h"


/* Needs to be defined before we include itf_msg.h */
typedef UINT8 MsgBody_t;
#include "itf_msg.h"


#define MMI_KEY_DOWN    (0xc001 + HAL_KEY_DOWN)
#define MMI_KEY_PRESSED (0xc001 + HAL_KEY_PRESSED)
#define MMI_KEY_UP      (0xc001 + HAL_KEY_UP)
#define MMI_KEYPRESSED  MMI_KEY_DOWN

EXPORT UINT8 testTaskId;
EXPORT UINT8 testMbx;


/*************************************************
*** LCD + Key initialization function for all reference tests
**************************************************/
PUBLIC VOID reftest_KeyCallback(UINT8 id, HAL_KEY_STATE_T status);
PUBLIC VOID reftest_Init(VOID);
PUBLIC VOID reftest_WaitAnyKey(VOID);

