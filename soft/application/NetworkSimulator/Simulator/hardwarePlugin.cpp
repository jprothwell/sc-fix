/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/**************************************************************

	FILENAME	: HardwarePlugin.cpp

  	PURPOSE		: Hardware Plugins file

	REMARKS		: nil

	AUTHOR		:  

	DATE		: Oct 5,03

**************************************************************/



#include "stdafx.h"
#include "windows.h"
#include "Mmsystem.h"		
#include "windowsx.h"

#include "Messages.h"

#define MAXRINGS    32
#define MAXMIDI		25
unsigned char AudioPlayMelody(unsigned short inRingIndex, unsigned char inRepeatFactor);
void AudioPlayReq(void* Req);
unsigned int AudioStopMelody(void);
void AudioRepeatReq(void);
void AudioStopReq(void);

HWND PWHandle;
#define MIDI_INDEX		0
#define RING_TONE_INDEX	10

// array for sounds to be used along with sound messages 
TCHAR  waveSounds[MAXRINGS][50] = {
							  _T("..\\Melodies\\ring1.wav"),
							  _T("..\\Melodies\\ring2.wav"),
  							  _T("..\\Melodies\\ring3.wav"),
							  _T("..\\Melodies\\ring4.wav"),
							  _T("..\\Melodies\\ring5.wav"),
							  _T("..\\Melodies\\ring6.wav"),
							  _T("..\\Melodies\\ring7.wav"),
							  _T("..\\Melodies\\ring8.wav"),
							  _T("..\\Melodies\\ring9.wav"),
							  _T("..\\Melodies\\ring10.wav"),
							  _T("..\\Melodies\\ring11.wav"),
							  _T("..\\Melodies\\ring12.wav"),
							  _T("..\\Melodies\\ring13.wav"),
							  _T("..\\Melodies\\ring14.wav"),
							  _T("..\\Melodies\\ring15.wav"),
							  _T("..\\Melodies\\ring16.wav"),
  							  _T("..\\Melodies\\ring17.wav"),
							  _T("..\\Melodies\\ring1.wav"),
							  _T("..\\Melodies\\ring2.wav"),
							  _T("..\\Melodies\\ring3.wav"),
							  _T("..\\Melodies\\ring4.wav"),
							  _T("..\\Melodies\\ring5.wav"),
							  _T("..\\Melodies\\ring6.wav"),
							  _T("..\\Melodies\\ring7.wav"),
							  _T("..\\Melodies\\ring8.wav"),
							  _T("..\\Melodies\\ring9.wav"),
							  _T("..\\Melodies\\ring10.wav"),
							  _T("..\\Melodies\\ring11.wav"),
							  _T("..\\Melodies\\ring12.wav"),
							  _T("..\\Melodies\\ring13.wav"),
							  _T("..\\Melodies\\ring14.wav")

};

TCHAR  MidiSounds[MAXMIDI][50] = {
							  _T("..\\Melodies\\demo1.mid"),
							  _T("..\\Melodies\\demo2.mid"),
							  _T("..\\Melodies\\demo3.mid"),//tunes start
							  _T("..\\Melodies\\demo7.mid"),
							  _T("..\\Melodies\\mission.mid"),
							  _T("..\\Melodies\\TESTSND.mid"),
							  _T("..\\Melodies\\demo1.mid"),
							  _T("..\\Melodies\\demo2.mid"),
							  _T("..\\Melodies\\demo3.mid"),//tunes start
							  _T("..\\Melodies\\demo7.mid"),
							  _T("..\\Melodies\\mission.mid"),
							  _T("..\\Melodies\\TESTSND.mid"),
							  _T("..\\Melodies\\demo1.mid"),
							  _T("..\\Melodies\\demo2.mid"),
							  _T("..\\Melodies\\demo3.mid"),//tunes start
							  _T("..\\Melodies\\demo7.mid"),
							  _T("..\\Melodies\\mission.mid"),
							  _T("..\\Melodies\\TESTSND.mid")

};

unsigned char RepeatFactor;
BOOL	bSendAudioFinishInd=FALSE;

/**************************************************************

	FUNCTION NAME		: AudioPlayMelody

  	PURPOSE				: Plugin for playing audio interface.

	INPUT PARAMETERS	: unsigned short inRingIndex, unsigned  char inRepeatFactor
				
	OUTPUT PARAMETERS	: nil

	RETURNS				: unsigned char

	REMARKS				: Plays the audio on windows. It is called by the Simulator
							framework to play mci audio.

**************************************************************/

unsigned char AudioPlayMelody(unsigned short inRingIndex, unsigned  char inRepeatFactor)
{
    TCHAR buf[256]; 
 	U8 midiflag=0;
	AudioStopMelody();
	if((inRingIndex>=151)&&(inRingIndex<=165))
	{
	//play tunes
		midiflag=1;
		inRingIndex=MIDI_INDEX+inRingIndex-151;
	}
	else if((inRingIndex>=101)&&(inRingIndex<=110))
	//play ring
		inRingIndex=RING_TONE_INDEX+inRingIndex-101;
	else if((inRingIndex>=165)&&(inRingIndex<=180))
	//play tone
		inRingIndex=RING_TONE_INDEX+inRingIndex-165;
	else if(inRingIndex<=12)
	//play key pad tone
		inRingIndex=0;	
	else if((inRingIndex>=48)&&(inRingIndex<=51))
	//play warning_error tone
		inRingIndex=1;
    else
		inRingIndex=0xffff;
		
	
	if(inRingIndex!=0xffff)
    {
	RepeatFactor = inRepeatFactor;
	if(midiflag)
		wsprintf((TCHAR*)buf, _T("open %s type sequencer alias MUSIC"), (TCHAR*)MidiSounds[inRingIndex]);
	else
		wsprintf((TCHAR*)buf, _T("open %s type waveaudio alias MUSIC buffer 5"), (TCHAR*)waveSounds[inRingIndex]);

    if (mciSendString(_T("close all"), NULL, 0, NULL) != 0)
		return(FALSE);
    if (mciSendString(buf, NULL, 0, NULL) != 0)
    	return(FALSE);
		if((inRepeatFactor==0)||(inRepeatFactor==1))//repeat or ascending
		{			
				if (mciSendString(_T("play MUSIC from 0 notify"), NULL, 0, PWHandle) != 0)
    				return(FALSE);
			
		}
		else//single
		{
			if (mciSendString(_T("play MUSIC from 0"), NULL, 0, NULL) != 0)
    			return(FALSE);
		}
	}
    return TRUE;
}

/**************************************************************

	FUNCTION NAME		: AudioPlayReq

  	PURPOSE				: Plays audio after receiving request from MMI

	INPUT PARAMETERS	: void
				
	OUTPUT PARAMETERS	: nil

	RETURNS				: void
	REMARKS				: Calls Audio Play melody to play melody.

**************************************************************/

void  AudioPlayReq(void* Req)
{
	mmi_eq_play_audio_req_struct* audioPlayReq=(mmi_eq_play_audio_req_struct*)Req;
	AudioPlayMelody( audioPlayReq->sound_id, audioPlayReq->style);	
}

/**************************************************************

	FUNCTION NAME		: AudioStopMelody

  	PURPOSE				: Stops audio after receiving request from MMI

	INPUT PARAMETERS	: void
				
	OUTPUT PARAMETERS	: unsigned int

	RETURNS				: void
	REMARKS				: Stops any playing tone.

**************************************************************/


unsigned int AudioStopMelody(void)
{
	RepeatFactor = FALSE;
    if (mciSendString(_T("close all"), NULL, 0, NULL) != 0)
		return(FALSE);
    return TRUE;
}
/**************************************************************

	FUNCTION NAME		: AudioStopReq

  	PURPOSE				: Stops audio after receiving request from MMI

	INPUT PARAMETERS	: void
				
	OUTPUT PARAMETERS	: void

	RETURNS				: void
	REMARKS				: Stops any playing tone.

**************************************************************/


void AudioStopReq(void)
{
	AudioStopMelody();
}

/**************************************************************

	FUNCTION NAME		: AudioReplayMelody

  	PURPOSE				: Replays  audio for handling INFINITE play request

	INPUT PARAMETERS	: void
				
	OUTPUT PARAMETERS	: void

	RETURNS				: BOOL
	REMARKS				: Replays Ausdio

**************************************************************/

BOOL AudioReplayMelody(void)
{
    if((RepeatFactor==0)||(RepeatFactor==1))
	{
		if (mciSendString(_T("play MUSIC from 0 notify"), NULL, 0, PWHandle) != 0)
			return(FALSE);
	}
    return TRUE;
}

/**************************************************************

	FUNCTION NAME		: AudioRepeatReq

  	PURPOSE				: Replays  audio for handling INFINITE play request

	INPUT PARAMETERS	: void
				
	OUTPUT PARAMETERS	: void

	RETURNS				: BOOL
	REMARKS				: Replays Audio

**************************************************************/


void AudioRepeatReq(void)
{
	AudioReplayMelody();
}

/**************************************************************

	FUNCTION NAME		: AudioPlayIMelodyStringReq

  	PURPOSE				: Plays Imelody

	INPUT PARAMETERS	: void*
				
	OUTPUT PARAMETERS	: void

	RETURNS				: BOOL
	REMARKS				: Plays Audio imelody

**************************************************************/

void AudioPlayIMelodyStringReq(void *Req)
{//play demo key pad hit for all note-->BBEP2C.wav--index 0
	AudioPlayMelody(0, 2);//once
}

/**************************************************************

	FUNCTION NAME		: AudioPlayIMelodyFileReq

  	PURPOSE				: Plays Imelody

	INPUT PARAMETERS	: void*
				
	OUTPUT PARAMETERS	: void

	RETURNS				: void

	REMARKS				: Plays Audio Imelody

**************************************************************/

void AudioPlayIMelodyFileReq(void * Req)
{//play demo file-->euroloop.wav--index 8

	static U16 Melodynumber=15;
	mmi_eq_play_audio_by_name_req_struct* audioPlayMelodyFileReq=(mmi_eq_play_audio_by_name_req_struct*)Req;

	TCHAR buf[256]; 
	RepeatFactor=audioPlayMelodyFileReq->style;

	bSendAudioFinishInd=TRUE;
	wsprintf((TCHAR*)buf, _T("open %s type waveaudio alias MUSIC buffer 5"), (TCHAR*)waveSounds[Melodynumber]);
//	wsprintf((TCHAR*)buf, _T("open %s type sequencer alias MUSIC"), (TCHAR*)MidiSounds[Melodynumber]);

    if (mciSendString(_T("close all"), NULL, 0, NULL) != 0)
		return;
    if (mciSendString(buf, NULL, 0, NULL) != 0)
    	return;

		if (mciSendString(_T("play MUSIC from 0 notify"), NULL, 0, PWHandle) != 0)
			return;
		
}

