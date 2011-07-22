#include <cswtype.h>
#include <base.h>
#include "chip_id.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include <hal.h>
#include <target.h>
#include <target_config.h>
#else
#include "hal_key.h"
#include "tgt_m.h"
#include "hal_pwm.h"
#endif

#include "csw_csp.h"

#include <cos.h>
#include "key_defs.h"
#include <stdkey.h>
#include <base_prv.h>
#include <sul.h>
#include "dm_prv.h"
#include "ts.h"

#define BUTTON(row,col) (((row)<<8) | (col))

extern UINT16 g_PreKeyCode;
extern UINT16 g_PreKeyPressTime;

typedef struct _KEY_SCAN_STATUS 
{
    UINT32     LastButton;
    UINT32     PendingButton;
    UINT8      nSame;          // current number of identical scans
    UINT8      padding;
    UINT16      nPressTime;
    
    // Debouncing parameters
    UINT32     nStable;        // Number of stable scans before returning
} KEY_SCAN_STATUS;


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
void dm_KeyHandler(u8 row, u8 col, hal_KeyState_t status)
{
  UINT16 button = 0; // unused for Jade's code.
  UINT8 key = (row == HAL_KEY_ON_OFF)?HAL_KEY_ON_OFF:(UINT32)Key_Map[row][col];

#else
void dm_KeyHandler(UINT8 button, hal_KeyState_t status)
{
  UINT8 row = (button == HAL_KEY_ON_OFF)?HAL_KEY_ON_OFF:button / 8;
  UINT8 col = (button == HAL_KEY_ON_OFF)?HAL_KEY_ON_OFF:button % 8;
  UINT8 key = tgt_GetKeyCode(button);

#endif

  COS_EVENT ev;
  ev.nParam1 = dm_GetStdKey(key);  // SK_BUTTON
  ev.nParam2 = ((row<<8)|col);    // (row and column)
  ev.nParam3 = key;       // Key value from target
  		//hal_HstSendEvent(0x00000000);
		//hal_HstSendEvent(row);
		//hal_HstSendEvent(col);
		//hal_HstSendEvent(0x00000001);

  if (((row == 255) && (col == 255)) || (button == HAL_KEY_ON_OFF))
  {
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
	  if(status == HAL_KEY_DOWN ){
		ev.nEventId = EV_KEY_DOWN;
		ev.nParam1	= SK_POWER_OFF;
		dm_SendKeyMessage(&ev);
	  }
	  else if(status == HAL_KEY_UP) {
		ev.nEventId = EV_KEY_UP;
		ev.nParam1	= SK_POWER_OFF;
		dm_SendKeyMessage(&ev);
	  }
#endif
  }
  else
  {
    if (status == HAL_KEY_DOWN)
    {
        ev.nEventId = EV_KEY_DOWN;
        dm_SendKeyMessage(&ev);
        CSW_TRACE(1, "dm: dm_KeyHandler ev.nParam1 =%x  2 = %x 3= %c X \n",
                ev.nParam1,ev.nParam2, ev.nParam3);
    }
    else if (status == HAL_KEY_UP)
    {
        ev.nEventId = EV_KEY_UP;
        dm_SendKeyMessage(&ev);
        CSW_TRACE(1, "dm; dm_KeyHandler up dmkey = %x rowcol = %x tgtkey = %c\n",
          ev.nParam1, ev.nParam2, ev.nParam3);

    }
  }

  

}



BOOL dm_KeyInit()
{
    hal_KeyMask_t keyMask;

    hal_key_Open(4, 4, 50);
    hal_key_IrqSetHandler(dm_KeyHandler);
    keyMask.onUp = 1;
    keyMask.onPressed = 0;
    keyMask.onDown = 1;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    hal_key_IrqSetMask(&keyMask);
#else
    hal_key_IrqSetMask(keyMask); // Modem2G's API use the structure directly
#endif


    return TRUE;
}

BOOL DM_KeyExit()
{
   hal_key_Close();
   return TRUE;
}

//CSW_TRACE(CFW_PM_TS_ID, TSTXT("0x%x \n"), *p++);
extern UINT32 DRV_GetRtcClock(VOID);

UINT8 dm_GetStdKey(UINT8 key) 
{
  
	switch (key) 
    {
	case KP_VU:
        return (SK_VOLUME_INC);	 // Volume UP

	case KP_VD:
        return (SK_VOLUME_DEC);	 // Volume Dwn

	case KP_UP:
        return (SK_UP);		     // UP

	case KP_DW:
        return (SK_DOWN);		     // DOWN

	case KP_RT:
        return (SK_RIGHT);	     // RIGHT

	case KP_LT:
        return (SK_LEFT);		     // LEFT

	case KP_1:
        return (SK_NUM1); 	     // 0x31+0x20

	case KP_2:
        return (SK_NUM2);		     // 0x32+0x20

	case KP_3:
        return (SK_NUM3);		     // 0x33+0x20

	case KP_4:
        return (SK_NUM4);		     // 0x34+0x20

	case KP_5:
        return (SK_NUM5);		     // 0x35+0x20

	case KP_6:
        return (SK_NUM6);		     // 0x36+0x20

	case KP_7:
        return (SK_NUM7);		     // 0x37+0x20

	case KP_8:
        return (SK_NUM8);		     // 0x38+0x20

	case KP_9:
        return (SK_NUM9);		     // 0x39+0x20

	case KP_STAR:
        return (SK_STAR);		     // 0x3A+0x20

	case KP_0:
        return (SK_NUM0);		     // 0x30+0x20

	case KP_POUND:
        return (SK_HASH);	         // 0x3B+0x20

	case KP_SR:
        return (SK_CTL_RIGHT);     //	SOFT RIGHT
	
    case KP_SL:
        return (SK_CTL_LEFT);		 // SOFT_LEFT

	case KP_END:
        return (SK_PHONE_END);	 // Phone End

	case KP_SND:
        return (SK_PHONE_SEND);	 // Phone Send

	case KP_OK: 
        return (SK_SELECT);	     // 0x01
        
	case KP_DEL: 
        return (SK_DELETE);	     // C
           
	case KP_BACK:
        return (SK_BACKSPACE);	 // Backspace

	case KP_SC: 
        return (SK_MENU);		     // 0x04

	default:
		break;
	}
    
	return (SK_NONE);
}




