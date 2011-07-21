#ifndef __NEW_EARPHONE_PLAN__
#define __NEW_EARPHONE_PLAN__

/* Modified MINVOLT_EARPHONE_NOT_IN_PHONE and MAXVOLT_EARPHONE_IN_PHONE */

#if defined(__PROJECT_GALLITE_C01__)
#define MINVOLT_EARPHONE_NOT_IN_PHONE  1600  //1500 ,如果耳机没插在手机上，电平值最小值
#define MAXVOLT_EARPHONE_IN_PHONE  1400  //1500 ,如果VMIC打开,耳机插在手机上，没按耳机接听键，电平值的范围
#else
#define MINVOLT_EARPHONE_NOT_IN_PHONE  1500  //如果耳机没插在手机上，电平值最小值

#define MAXVOLT_EARPHONE_IN_PHONE  1500  //如果VMIC打开,耳机插在手机上，没按耳机接听键，电平值的范围

#endif
#define MINVOLT_EARPHONE_IN_PHONE  300

#define MAXVOLT_FOR_EARPHONE_PRESSKEY  300  //按了耳机接听键，电平值的最大值
#endif
