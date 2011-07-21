////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2006, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: XXX.h
// 
// DESCRIPTION:
//   TODO... 
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   dingjian        20070913      build
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __LED_H__
#define __LED_H__
#if defined (__MMI_RUNNING_LED__)
void showPowerOnLED(void);
void showPowerOffLED(void);
void showPowerOnOffLED(void);
void showMusicRunningLED(void);
void stopMusicRunningLED(void);
void showKeyPadLED(void);
void stopKeyPadLED(void);
#endif		// define __LED__
#endif // __LED_H__ 

