#ifndef VPPP_ANALOGTV_ASM_H
#define VPPP_ANALOGTV_ASM_H



#ifndef CS_PLATFORM_SIMU

 #define RAM_X_BEGIN_ADDR 0x0
 #define RAM_Y_BEGIN_ADDR 0x4000

#endif

#define VPP_ANALOGTV_MAIN_ENTRY           0
#define VPP_ANALOGTV_CRITICAL_SECTION_MIN 0
#define VPP_ANALOGTV_CRITICAL_SECTION_MAX 0

EXPORT  INT32 g_vppAnalogTVMainCodeSize;
EXPORT  INT32 g_vppAnalogTVConstXSize;
EXPORT  INT32 g_vppAnalogTVConstYSize;


EXPORT  UINT16  g_vppAnalogTVConstX[];
EXPORT  UINT16  g_vppAnalogTVConstY[];
EXPORT  INT32 g_vppAnalogTVMainCode[];


#endif  // VPPP_ANALOGTV_ASM_H
