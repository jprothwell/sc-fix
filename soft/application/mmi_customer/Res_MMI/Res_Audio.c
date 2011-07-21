/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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

	FILENAME	: Res_.c

  	PURPOSE		: Populate Resource for 

	REMARKS		: nil

	AUTHOR		: chenqiang

	DATE		: 2-16-2011

**************************************************************/
#include "cs_types.h"
#include "custdatares.h"
#include "stdc.h"
#include "customcfg.h"
#include "custresdef.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "mmi_features.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"
/**************************************************************

	FUNCTION NAME		: PopulateAudioResData

  	PURPOSE				: Populates the resources for Audio

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

	REMARKS				: nil

**************************************************************/

void PopulateAudioResData(void)
{
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs01_mid.mid","../../mmi_customer/CustResource/ringData/include/cs01_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs02_mid.mid","../../mmi_customer/CustResource/ringData/include/cs02_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs03_mid.mid","../../mmi_customer/CustResource/ringData/include/cs03_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs04_mid.mid","../../mmi_customer/CustResource/ringData/include/cs04_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs05_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs05_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs06_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs06_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs07_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs07_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs08_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs08_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs09_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs09_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs10_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs10_mid.ring.txt");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy01_imy.wav","../../mmi_customer/CustResource/ringData/include/s1imy01_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy02_imy.wav","../../mmi_customer/CustResource/ringData/include/s1imy02_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy03_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy03_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy04_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy04_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy05_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy05_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy06_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy06_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy07_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy07_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy08_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy08_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy09_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy09_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy10_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy10_imy.ring.txt");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/game01_mid.amr","../../mmi_customer/CustResource/ringData/include/game01_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/game02_mid.amr","../../mmi_customer/CustResource/ringData/include/game02_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/game03_mid.amr","../../mmi_customer/CustResource/ringData/include/game03_mid.ring.txt");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/camera01_mid_pcm.wav","../../mmi_customer/CustResource/ringData/include/camera01_mid_pcm.tab");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/camera02_mid_pcm.wav","../../mmi_customer/CustResource/ringData/include/camera02_mid_pcm.tab");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/camera03_mid_pcm.wav","../../mmi_customer/CustResource/ringData/include/camera03_mid_pcm.tab");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/daojishi_pcm.wav","../../mmi_customer/CustResource/ringData/include/daojishi_pcm.tab");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power01_mid.mp3","../../mmi_customer/CustResource/ringData/include/power01_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power02_mid.mp3","../../mmi_customer/CustResource/ringData/include/power02_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power03_mid.mp3","../../mmi_customer/CustResource/ringData/include/power03_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power04_mid.mp3","../../mmi_customer/CustResource/ringData/include/power04_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power05_mid.wav","../../mmi_customer/CustResource/ringData/include/power05_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power06_mid.wav","../../mmi_customer/CustResource/ringData/include/power06_mid.ring.txt");
}
#endif


