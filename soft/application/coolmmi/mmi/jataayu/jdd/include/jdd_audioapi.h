/***************************************************************************
 * $Id: jdd_audioapi.h,v 1.2 2008/05/15 08:18:43 browserdevjtbase Exp $
 * $Revision: 1.2 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdd_audioapi.h,v $
 * Revision 1.2  2008/05/15 08:18:43  browserdevjtbase
 * Core Update
 *
 * Revision 1.3  2007/06/15 10:49:23  sriramdevhtmlbrow
 * Updated copyright information
 *
 *  
 ***************************************************************************/


#ifndef JDD_AUDIO_API_H

#define JDD_AUDIO_API_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* JDD_AUDIO ;


typedef struct _jc_audio_info_
{
	JC_CHAR * psAudioFileName ;
	JC_INT8  * pAudioBuff ;
	JC_UINT32 uiAudioLen ;
	JC_BOOLEAN bIsFileInput ;
} JC_AUDIO_INFO ;


JC_RETCODE jdd_AudioInitialize (JDD_AUDIO *pAudioHandle);

JC_RETCODE jdd_AudioDeInitialize (JDD_AUDIO  AudioHandle);

JC_RETCODE jdd_AudioPlay (JDD_AUDIO AudioHandle, JC_AUDIO_INFO *pAudioInfo) ;

JC_RETCODE jdd_AudioStop (JDD_AUDIO AudioHandle);

JC_RETCODE jdd_AudioPause (JDD_AUDIO AudioHandle) ;

JC_RETCODE jdd_AudioResume (JDD_AUDIO AudioHandle) ;

void Jmms_audply_play_callback_hdlr();

#ifdef __cplusplus
}
#endif

#endif

