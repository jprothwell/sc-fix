////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/halp_ana_audio.h $
//    $Author: admin $
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//    $Revision: 269 $
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file halp_ana_audio.h                                                    //
/// Private API for HAL Analog Audio Driver
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HALP_ANA_AUDIO_H_
#define _HALP_ANA_AUDIO_H_

#if (CHIP_HAS_STEREO_DAC == 1)
// =============================================================================
// hal_AnaAudioStereoDacPreOpen
// -----------------------------------------------------------------------------
/// Preopen the Stereo DAC. This is a dirty thing needed by some shortcoming 
/// of the stereo DAC. It is needed to preopen the stereo DAC several seconds
/// before being able to use, otherwise, a loud pop noise can be heard.
/// Without the preopening, the output signal sent to the speaker may experience
/// a step down of one Volt, resulting in a unpleasant noise.
/// The bits set in #hal_AnaAudioStereoDacPreOpen must not be cleared, otherwise
/// the output signal will slowly decrease, and lead to a pop noise at the next
/// opening.
///
// =============================================================================
PROTECTED VOID hal_AnaAudioStereoDacPreOpen(VOID);



#endif

#endif // _HALP_ANA_AUDIO_H_


