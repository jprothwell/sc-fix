/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__20____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// Õñ¶¯Ä£¿é /////////////////////////////////////////////////////////////
#include "____SMARCHBOX__COOLSAND__V1_0_0__2____.h"
#define FM_VIB_TIMER JMETOC_TIMER_5 + 1

    /**
     * Stop the vibration.
     */
    void stopVibrate(void) {
      PlayPatternReq(VIB_PATTERN_1,PLAY_STOP);
      StopTimer(FM_VIB_TIMER);
    }

    /**
     * Plays the vibration.
     * @param dur the duration in milli seconds
     * @return if it's successful, return 1, othewise return 0
     */
    s32 javax_microedition_lcdui_Display_nVibrate(javax_microedition_lcdui_Display* java_this,s32 dur) {
      PlayPatternReq(VIB_PATTERN_1,PLAY_REPEAT);
      StartTimer(FM_VIB_TIMER,dur,stopVibrate);
      return 1;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
