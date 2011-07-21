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

/*******************************************************************************
 *
 * Filename:
 * ---------
 * tone.c
 *
 * Project:
 * --------
 *   CSD6208
 *
 * Description:
 * ------------
 
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
//#include "media.h"
//#include "afe.h"
#include "media_others.h"

uint32   SaveAndSetIRQMask( void );
void     RestoreIRQMask( uint32 );

#define DSP_TONE_FREQ(f1,f2)  {  DSP_TONE_F1A  = (f1);               \
                                 DSP_TONE_F1B  = (f2);               }
#define DSP_KT_FREQ(f1,f2)    {  DSP_TONE_F2A  = (f1);               \
                                 DSP_TONE_F2B  = (f2);               }

#if IS_EV_BOARD
#define KT_INIT_AMP     0x1000
#else
#define KT_INIT_AMP     0x3FFF
#endif
//delect by licheng for code clean
/*
static struct
{
   const L1SP_Tones  *tonelist;
   const L1SP_Tones  *curr_tone;
   void              (*handler)(void);
   uint16            aud_id;
   uint8             gpt;
} tone;

static struct
{
   uint16            aud_id;
   uint8             gpt;
   int8              lock;
   uint8             volume1;
   uint8             volume2;
} keytone;
*/
/* ------------------------------------------------------------------------------ */
/*  Functions for Playing Comfort Tones                                           */
/* ------------------------------------------------------------------------------ */
#if 0
static void tonePlayOff( void *data );

static void toneStop( void *data )
{

}

static void tonePlayOn( void *data )
{

}

static void toneMute( void *data )
{

}

static void tonePlayOff( void *data )
{

}

static void toneHandler( void *data )     /* This function works in L1Audio Task */
{

}
#endif
void toneInit( uint16 aud_id )
{

}

/* ------------------------------------------------------------------------------ */
/*  Tone Interface                                                                */
/* ------------------------------------------------------------------------------ */
void TONE_SetOutputDevice( uint8 device )
{

}

void TONE_SetOutputVolume( uint8 volume1, uint8 volume2 )
{

}


void TONE_Play( const L1SP_Tones *tonelist )
{

}


void TONE_Stop( void )
{

}

void TONE_SetAmplitude( int16 amp )
{

}

void TONE_SetFIR( kal_bool enable )
{

}

void TONE_SetHandler( void (*handler)(void) )
{

}

/* ------------------------------------------------------------------------------ */
/*  Keytone Interface                                                             */
/* ------------------------------------------------------------------------------ */
/*
static void ktStop( void *data )
{

}
*/
void ktInit( uint16 aud_id )
{

}

void ktLock( void )
{

}

void ktUnlock( void )
{

}

void KT_SetOutputDevice( uint8 device )
{

}

void KT_SetOutputVolume( uint8 volume1, uint8 volume2 )
{

}

/*****************************************************************************
* FUNCTION
*  KT_Play
* DESCRIPTION
*   This function is used to play keytone.
*
* PARAMETERS
*  freq1 - The first frequency
*  freq2 - The second frequency
*  duration - Duration of the tone
* RETURNS
*  None.
* GLOBALS AFFECTED
*  None
*****************************************************************************/
void KT_Play( uint16 freq1, uint16 freq2, uint16 duration )
{

}

/*****************************************************************************
* FUNCTION
*  KT_Stop
* DESCRIPTION
*   This function is used to stop key tone playing.
*****************************************************************************/
void KT_Stop( void )
{

}

void KT_SetAmplitude( int16 amp )
{

}

void KT_SetFIR( kal_bool enable )
{

}


