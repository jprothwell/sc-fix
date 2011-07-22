/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 2006 Coolsand Technologies, Inc.                 */
/*                                                                       */
/* PROPRIETARY RIGHTS of Coolsand Technology are involved in the         */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*      drv.c functions                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file implements the reset out line functions                */
/* AUTHOR                                                                */
/*      Shawn                                                            */
/* FUNCTIONS                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/* HISTORY                                                               */
/*                                                                       */
/*      DATE  2006/04            REMARKS                                 */
/*************************************************************************/

#if 1 
#include <cswtype.h>
#include <drv.h>
#include "chip_id.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include <hal.h>
#include <hal_config.h>
#else

#include "pmd_m.h"
#include "hal_gpio.h"
// Interrupt handling is of a different type
#define drv_gipo_handler(a) drv_gipo_handler() 

#include "hal_pwm.h"
#include "hal_ebc.h"
#include "hal_sys.h"
#endif

#include "csw_csp.h"

#include <drv_prv.h>
#include <errorcode.h>
#include <cos.h>
#include <base_prv.h>
#include <cos.h>

UINT32 gpio_ind_mask = 0;

extern void _sxr_StartFunctionTimer (u32 Period, void (*Function)(void *), void *Param, u16 Id, u8 I2d);
extern BOOL COS_SendSysEvent(
  COS_EVENT* pEvent,
  UINT16 nOption
);

//jinsong add it in 060807
extern UINT8 uLcdLevel;
extern UINT8 uKeyLevel;
//end by jinsong

UINT32 GPIO_Debonc[32] ;
# if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
// TODO Get initial state from the Target module.
HAL_GPIO_DIRECTION_T g_gpioDirection[HAL_GPIO_QTY];
#endif

void drv_gipo_handler(UINT16 status);

UINT32 DRV_GpioSetMode ( 
    UINT8 nGpio,
    UINT8 nMode
)
{
# if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#if 0
    if( nGpio < 20 )
        return ERR_NOT_SUPPORT;

    if( nGpio < 23 && nGpio != 20)
        return ERR_NOT_SUPPORT;

    if( nGpio > 31 )
        return ERR_NOT_SUPPORT;

    if(nGpio>DRV_GPIO_MAX)
        return ERR_DRV_INVALID_PARAMETER;
#endif
    if(nMode == 1)
    {
        hal_gpio_access->gpio_mode = hal_gpio_access->gpio_mode|(1 <<nGpio);   
    }
    else
    {       
        hal_gpio_access->gpio_mode &=~(1<<nGpio);  
    }
#else
    // Nobody in is right mind would do something like that 
    // at this level. Muxing is dealt with and configure 
    // by the target module. If some muxing to GPIO have to 
    // be done for very special cases (Power saving, or configure
    // a module in SPI by forcing some lines) that have to 
    // handled within the driver, not there
#endif
    return ERR_SUCCESS;
}

UINT32 DRV_GpioSetDirection( UINT8 nGpio, UINT8 nDirection)
{
	if(nGpio>DRV_GPIO_MAX)
		return ERR_DRV_INVALID_PARAMETER;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	if(nDirection == 1)
	{
		hal_gpio_SetOut(1<<nGpio);
	}
	else
	{
		hal_gpio_SetIn(1<<nGpio);
		
	}
#else
    if (nDirection == 1)
    {
        hal_GpioSetOut(HAL_GPIO_0 + nGpio);
        g_gpioDirection[nGpio] = HAL_GPIO_DIRECTION_OUTPUT;
    }
    else
    {
        hal_GpioSetIn(HAL_GPIO_0 + nGpio);
        g_gpioDirection[nGpio] = HAL_GPIO_DIRECTION_INPUT;
    }
#endif
	return ERR_SUCCESS;
}

UINT32 DRV_GpioGetDirection(UINT8 nGpio,UINT8* pDirection)
{
	if(nGpio>DRV_GPIO_MAX)
        
		return ERR_DRV_INVALID_PARAMETER;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	*pDirection = (hal_gpio_GetDirection()>>nGpio)&1;
#else
    if (g_gpioDirection[nGpio] == HAL_GPIO_DIRECTION_OUTPUT)
    {
        *pDirection = 1;
    }
    else
    {
        *pDirection = 0;
    }
#endif
    
	return ERR_SUCCESS;
}

UINT32 DRV_GpioSetLevel(UINT8 nGpio,UINT8 nLevel)
{
	if(nGpio>DRV_GPIO_MAX)
		return ERR_DRV_INVALID_PARAMETER;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	if(nLevel == 1)
	{
		hal_gpio_SetBit(1<<nGpio);
	}
	else
	{
		hal_gpio_ClrBit(1<<nGpio);
	}
#else
    HAL_APO_ID_T gpio = {{ 
                            .type = HAL_GPIO_TYPE_IO,
                            .id = nGpio
                        }};
    if (nLevel == 1)
    {
        hal_GpioSet(gpio);
    }
    else
    {
        hal_GpioClr(gpio);
    }
#endif

	return ERR_SUCCESS;
}

UINT32 DRV_GpioGetLevel(UINT8 nGgpio,UINT8* nLevel)
{
	
	if(nGgpio>DRV_GPIO_MAX)
		return ERR_DRV_INVALID_PARAMETER;
	
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	*nLevel=(hal_gpio_GetVal(1<<nGgpio)) >> nGgpio;
#else
    *nLevel = hal_GpioGet(HAL_GPIO_0 + nGgpio);
#endif
	return ERR_SUCCESS;
}
#define HAL_GPIO_RISE_DISABLE(n) (~(((n>>8) & 0xff)))
#define HAL_GPIO_FALL_DISABLE(n) (~(((n) & 0xff00)))

UINT32 DRV_GpioSetEdgeIndication( UINT8 nGpio,UINT8 nEdge,UINT32 nDebonce )
{
	
	if((nGpio<DRV_GPIO_INT_FIRST)||(nGpio>DRV_GPIO_INT_LAST))
		return ERR_DRV_INVALID_PARAMETER;
	GPIO_Debonc[nGpio]=nDebonce;
	if(nDebonce<0)
		GPIO_Debonc[nGpio]=50;
	
	DRV_GpioSetDirection(nGpio,DRV_GPIO_IN);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	hal_gpio_ResetIrq(1<<nGpio);
	if(nEdge==DRV_GPIO_EDGE_F)
	{
		gpio_ind_mask=gpio_ind_mask|HAL_GPIO_FALL(1<<nGpio);
	}
	if(nEdge==DRV_GPIO_EDGE_R)
	{
		gpio_ind_mask=gpio_ind_mask|HAL_GPIO_RISE(1<<nGpio);
	}
	if(nEdge==DRV_GPIO_EDGE_RF)
	{
		
		gpio_ind_mask=gpio_ind_mask|HAL_GPIO_RISE(1<<nGpio)|HAL_GPIO_FALL(1<<nGpio);
	}	
/*the marked code can not pass compiling -- mabo 20070810
	if(nEdge==DRV_GPIO_DISABLE)
	{
		gpio_ind_mask=gpio_ind_mask&HAL_GPIO_RISE_DISABLE(1<<nGpio)&HAL_GPIO_FALL_DISABLE(1<<nGpio);
	}
*/
	hal_SetUsrVectorMsk(HAL_GPIO_IRQ,gpio_ind_mask);
	hal_SetUsrVector(HAL_GPIO_IRQ,drv_gipo_handler);
//	CSW_TRACE(100,"set gpio %x interrupter \n",nGpio);
#else
    HAL_GPIO_CFG_T config;
    config.direction = HAL_GPIO_DIRECTION_INPUT;
    config.value = FALSE;
    config.irqMask.falling = (nEdge==DRV_GPIO_EDGE_F || nEdge==DRV_GPIO_EDGE_RF);
	  config.irqMask.rising = (nEdge==DRV_GPIO_EDGE_R || nEdge==DRV_GPIO_EDGE_RF);
    config.irqMask.debounce = (nDebonce != 0);
    config.irqMask.level = TRUE; // FIXME ?
    config.irqHandler = drv_gipo_handler;

    hal_GpioResetIrq(HAL_GPIO_0 + nGpio);
    hal_GpioOpen(HAL_GPIO_0 + nGpio, &config);


#endif

	return ERR_SUCCESS;
}

#define GPIO_INT_TIMER_ID	0x55555555	//for GPIO debounc

typedef struct {
UINT8 number;
UINT8 level;
}GPIO_Debounc;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
// Debouncing calback. That is don in hardware for 
// Modem2G chips.
void drv_gpio_callback(void * pgpio)
{
	COS_EVENT ev;
	UINT8 leve=0;
	
	GPIO_Debounc *gpio=(GPIO_Debounc *)pgpio;
    
	DRV_GpioGetLevel(gpio->number,&leve);
	
	if(leve==(gpio->level))
	{
        ev.nEventId = EV_CSW_PRV_GPIO;
        ev.nParam1  = gpio->number;
        ev.nParam2  = gpio->level;
//        CSW_TRACE(0, "GPIO: drv_gpio_callback \n");
        COS_SendSysEvent(&ev, 0);
	}
    
	hal_SetUsrVectorMsk(HAL_GPIO_IRQ,gpio_ind_mask);
	CSW_DRV_FREE(gpio);
}
#endif
   
#if 1 
void drv_gipo_handler(UINT16 status)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	UINT8 number=0,i=0,temp=0,leve;
    
	COS_EVENT ev;

    
	//CSW_TRACE(100, "xxxdrv_gipo_handler");
	UINT32 temp_mask;
	GPIO_Debounc *gpio_de;
	temp_mask = hal_GetUsrVectorMsk(HAL_GPIO_IRQ);
	hal_SetUsrVectorMsk(HAL_GPIO_IRQ,0);//disable interrupter
	
	for(number=DRV_GPIO_INT_FIRST;number<DRV_GPIO_INT_LAST;number++)
	{
		if(status&((GPIO(number))>>8))
		{
			hal_gpio_ResetIrq(number);
			
			temp_mask &=~((HAL_GPIO_RISE(1<<number)|HAL_GPIO_FALL(1<<number)));
			hal_SetUsrVectorMsk(HAL_GPIO_IRQ,temp_mask);
			DRV_GpioGetLevel(number,&leve);
            
			for(i=0;i<5;i++)//avoid glitch 
			{
				DRV_GpioGetLevel(number,&temp);
				if(leve!=temp) 
				{
					hal_SetUsrVectorMsk(HAL_GPIO_IRQ,gpio_ind_mask);
				   return;
				}
			}
			//if(((gpio_ind_mbx[number]!=0))&&((gpio_ind_mask&HAL_GPIO_RISE(1<<number))||(gpio_ind_mask&HAL_GPIO_FALL(1<<number))))
			{
				if(GPIO_Debonc[number]!=0)
				{
				    gpio_de=CSW_DRV_MALLOC(2);
					gpio_de->level=temp;
					gpio_de->number=number;
					_sxr_StartFunctionTimer(GPIO_Debonc[number],drv_gpio_callback, 
                        gpio_de,GPIO_INT_TIMER_ID+number, 
                        0 );
				}
				else 
				{
    			   ev.nEventId = EV_CSW_PRV_GPIO;
    			   ev.nParam1  = number;
    			   ev.nParam2  = temp;
    			   COS_SendSysEvent(&ev, 0);
   			      hal_SetUsrVectorMsk(HAL_GPIO_IRQ,gpio_ind_mask);
				}
				
			}
		}
	}
#else
    // Modem2G API handles Interrupt on a per GPIO basis,
    // and debouncing is done on hardware.
	UINT8 number=0;
    UINT8 leve;
    
	COS_EVENT ev;


    ///@todo FIXME even dirtier than bellow
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GRANITE) // Granite test chip
#define HAL_GPIO_ADDRESS (0xA0B03000)
#else
#define HAL_GPIO_ADDRESS (0xA0B83000)
#endif

    // FIXME Dirtiest thing ever:
    // I don't want to add a get status function in the API, nor
    // give the number of the GPIO to the handler. The HAL GPIO 
    // driver allow higher level things than that...
    UINT32 status = *((UINT32*)HAL_GPIO_ADDRESS + 9);
	for(number=0; number<32; number++)
	{
		if(status&(1<<number))
		{
            // Does this behaves properly ?
			hal_GpioResetIrq(1<<number);
			DRV_GpioGetLevel(number,&leve);
            ev.nEventId = EV_CSW_PRV_GPIO;
            ev.nParam1  = number;
            ev.nParam2  = leve;
            COS_SendSysEvent(&ev, 0);
		}
	}
#endif
}
#endif
 
////////////////exterior interrupter//////////////////////////
// This is not used and exists only for Jade
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

void drv_exti_Handler(UINT16 status);

UINT32 DRV_EXTISetEdgeIndication(UINT8  nEdge)
{
	UINT32 temp_mask=0;
	switch(nEdge)
	{	
		case  DRV_EXTI_EDGE_R:
			temp_mask =HAL_EXTINT_RISE;
			break;
 		case	DRV_EXTI_EDGE_F:
 			temp_mask =HAL_EXTINT_FALL;
 			break;
		case	DRV_EXTI_EDGE_RF:
			temp_mask =HAL_EXTINT_FALL|HAL_EXTINT_RISE;
			break;
		case DRV_EXTI_EDGE_NUL:
			temp_mask =0;
			break;
		default:
			break;
	}
	hal_extint_ResetIrq();
	
	hal_SetUsrVectorMsk(HAL_EXTERNAL_IRQ,temp_mask);
	hal_SetUsrVector(HAL_EXTERNAL_IRQ, drv_exti_Handler);

	return ERR_SUCCESS;
}

void drv_exti_Handler(UINT16 status)
{	
    COS_EVENT ev;
    
    ev.nEventId = EV_DM_EXTI_IND;
    ev.nParam1  = 0;
    ev.nParam2  = 0;
    ev.nParam3  = 0;
    
    COS_SendSysEvent(&ev, 0);
}
#endif

UINT32 DRV_SetPwl1( UINT8 nLevel)
{
	hal_pwl1_Threshold(nLevel);
    
	uKeyLevel = nLevel;
	return ERR_SUCCESS;
}

//chenhe add for greenstone
UINT32 DRV_SetKeyBacklight( UINT8 nLevel)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    // Was done like that in Jade even though it is bad to harcode the used PWL
    hal_pwl1_Threshold(nLevel);
#else
	pmd_SetLevel(PMD_LEVEL_KEYPAD,nLevel);
#endif
	uKeyLevel = nLevel;
	return ERR_SUCCESS;
}

UINT32 DRV_SetPwl0( UINT8 nLevel)
{
    hal_pwl0_Threshold(nLevel);
    uLcdLevel = nLevel;
   	return ERR_SUCCESS;
}

/* for test backlight
UINT32 Test_SetPwl0( UINT8 nLevel)
{
	hal_pwl0_Threshold(nLevel);
       uLcdLevel = nLevel;
    
	return ERR_SUCCESS;
}
*/



////////////////EBC/////////////////
PVOID DRV_MemorySetupCS( UINT8 nCS, DRV_MEM_CFG *  pMemCfg)
{
    // FIXME This function shouldn't be in CSW. The LCD drivers must
    // not be in MMI, but in EDRV. Using the provided LCDD API, or
    // equivalent for other kind of external chip, there is no need
    // to manually configure the EBC. The CS copening is done in the
    // driver (eg lcdd_Open), and MUST stay at that low level, where
    // the configuration is got from the Target module. HAL must be
    // the only API to propose a Ebc configure function taking 
    // EBC config parameters. This function must be used only in
    // driver code, where those parameters are obtained from target.
	PVOID ebc_cs;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE) 
	//ebc_cs = hal_MemorySetupCS(nCS,(struct hal_MemoryCfg*) pMemCfg);
	ebc_cs = hal_MemoryEnableCS(nCS);
#else
    CONST HAL_EBC_CS_CFG_T csConfig =
        {
        .time = {
                .rdLatHalfMode  =  0,
                .rdLat          = 10,
                .oedlyHalfMode  =  0,
                .oedly          =  3,
                .pageLat        =  0,
                .wrLatHalfMode  =  0,
                .wrLat          = 10,
                .wedlyHalfMode  =  0,
                .wedly          =  3,
                .relax          =  2,
                .forceRelax     =  1
                },
        .mode = {
                .csEn           =  1,
                .polarity       =  0,
                .admuxMode      =  0,
                .writeAllow     =  1,
                .wbeMode        =  0,
                .bedlyMode      =  0,
                .waitMode       =  0,
                .pageSize       =  0,
                .pageMode       =  0,
                .writePageMode  =  0,
                .burstMode      =  0,
                .writeBurstMode =  0,
                .advAsync       =  0,
                .advwePulse     =  0,
                .admuxDly       =  0,
                .writeSingle    =  0,
                .writeHold      =  0
                }
        };
    ebc_cs = hal_EbcCsOpen(nCS, &csConfig);
#endif
    hal_ResetOut(FALSE);
    
	return ebc_cs;
}

UINT32 DRV_ResetOut( BOOL pRest)
{
	hal_ResetOut(pRest);
	
	return ERR_SUCCESS;
}

UINT32 DRV_MemoryDisableCS( UINT8  nCS)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE) 
	hal_MemoryDisableCS(nCS);
#else
    hal_EbcCsClose(nCS);
#endif
	return ERR_SUCCESS;
}

#endif 

