#include "mmi_features.h"
#include "hal_gpio.h"
#include "sxr_tim.h"
#include "app_ltlcom.h"
#include "mmi_trace.h"
#include "mmi_msg_struct.h"
#include "oslmemory.h"
#include "queuegprot.h"
#include "csw_csp.h"
#include "hal_sys.h"

#define SECOND_HALL	*HAL_TICK1S
#define MILLI_SECOND_HALL	SECOND_HALL/1000
#define MILLI_SECONDS_HALL	MILLI_SECOND_HALL
#define CSD_HALL_SENSOR_DEBOUNCE_TIME	800 MILLI_SECONDS_HALL

// Please defined HALL_GPIO_USE_GPIO_? in target.def
// GLOBAL_EXPORT_FLAG += HALL_GPIO_USE_GPIO_?
#if defined (HALL_GPIO_USE_GPIO_0)
#define HALL_GPIO HAL_GPIO_0
#elif defined (HALL_GPIO_USE_GPIO_1)
#define HALL_GPIO HAL_GPIO_1
#elif defined (HALL_GPIO_USE_GPIO_2)
#define HALL_GPIO HAL_GPIO_2
#elif defined (HALL_GPIO_USE_GPIO_3)
#define HALL_GPIO HAL_GPIO_3
#elif defined (HALL_GPIO_USE_GPIO_4)
#define HALL_GPIO HAL_GPIO_4
#elif defined (HALL_GPIO_USE_GPIO_5)
#define HALL_GPIO HAL_GPIO_5
#elif defined (HALL_GPIO_USE_GPIO_6)
#define HALL_GPIO HAL_GPIO_6
#elif defined (HALL_GPIO_USE_GPIO_7)
#define HALL_GPIO HAL_GPIO_7
#else
#ifdef HALL_GPIO
#undef HALL_GPIO
#endif
//#define HALL_GPIO HAL_GPIO_NONE
#endif

#if defined(HALL_GPIO)//(HALL_GPIO!=HAL_GPIO_NONE)

extern BOOL dm_SendKeyMessage(COS_EVENT* pEv);
static unsigned char g_HallGpio_Value;

void gpio_hall_sensor_send_message(unsigned char status)//(void)
{
	ilm_struct ilm;
	mmi_eq_gpio_detect_ind_struct *pIndStruct = (mmi_eq_gpio_detect_ind_struct *)OslMalloc(sizeof(mmi_eq_gpio_detect_ind_struct));
	ASSERT(NULL != pIndStruct);
	memset(pIndStruct,0,sizeof(mmi_eq_gpio_detect_ind_struct));
#if 1	
	if (1 == status)
		pIndStruct->gpio_device = EXT_DEV_CLAM_OPEN;
	else
		pIndStruct->gpio_device = EXT_DEV_CLAM_CLOSE;
#else
	pIndStruct->gpio_device = EXT_DEV_HALL_SENSOR;
#endif
	pIndStruct->on_off = 0;
	//pIndStruct->para1= g_HallGpio_Value;
	memset(&ilm,0,sizeof(ilm_struct));
	ilm.src_mod_id = MOD_L4C;
	ilm.dest_mod_id = MOD_MMI;
	//ilm.msg_id = MSG_ID_HALL_SENSOR_IND;
	ilm.msg_id = MSG_ID_MMI_EQ_GPIO_DETECT_IND;
	ilm.local_para_ptr = (local_para_struct *)pIndStruct;
	OslMsgSendExtQueue(&ilm);
}


void csd_hall_Sensor_process_irq(void)
{
//read state infor MMI
    g_HallGpio_Value = hal_GpioGet(HALL_GPIO);
	gpio_hall_sensor_send_message(g_HallGpio_Value);//()
}

void csd_hall_Sensor_irq_reset_on(void)
{
	HAL_GPIO_IRQ_MASK_T mask;
    UINT32 scStatus;
	mask.debounce = TRUE;
	mask.falling = TRUE;
	mask.rising = TRUE;
	mask.level = FALSE;
    scStatus =hal_EnterCriticalSection();            
    hal_GpioIrqSetMask(HALL_GPIO,&mask);
    hal_ExitCriticalSection(scStatus);    
	
    csd_hall_Sensor_process_irq();

}


VOID csd_hall_sensor_IrqHandler(VOID)
{
       UINT32 scStatus;

    HAL_GPIO_IRQ_MASK_T mask;
    //diable irq    
    
    mask.debounce=FALSE;
    mask.falling=FALSE;
    mask.rising=FALSE;
    mask.level=FALSE;
    
    scStatus =hal_EnterCriticalSection();            
    hal_GpioIrqSetMask(HALL_GPIO,&mask);
    hal_ExitCriticalSection(scStatus);            

    //infor MMI    
   // csd_hall_Sensor_process_irq();

    //enable irq after CSD_HALL_SENSOR_DEBOUNCE_TIME
    sxr_StopFunctionTimer(csd_hall_Sensor_irq_reset_on);
    sxr_StartFunctionTimer(CSD_HALL_SENSOR_DEBOUNCE_TIME,csd_hall_Sensor_irq_reset_on,(VOID *)NULL,0x03);
}


void gpio_hall_Sensor_init(void)
{
	HAL_GPIO_CFG_T gpioCfg;

	gpioCfg.direction = HAL_GPIO_DIRECTION_INPUT;
	gpioCfg.irqHandler = csd_hall_sensor_IrqHandler;
	gpioCfg.irqMask.rising=TRUE;
	gpioCfg.irqMask.falling = TRUE;
	gpioCfg.irqMask.debounce = TRUE;
	gpioCfg.irqMask.level=FALSE;
    
	hal_GpioOpen(HALL_GPIO,&gpioCfg);

}

void csd_mmi_app_handle_hall_sensor_message()
{
	mmi_trace(1, "csd_mmi_app_handle_hall_sensor_message,g_HallGpio_Value = %d",g_HallGpio_Value);
}
#else
void gpio_hall_Sensor_init(void)
{
}
#endif

