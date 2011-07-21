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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * SimDetectionHwMapFile.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for defines of Bootup Module structs
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SimDetectionHwMapFile.h

  	PURPOSE		: defines of Bootup Module structs.

 

	 

	DATE		: 12/03/03

**************************************************************/

#ifndef _SIM_DETECTION_HWMAP_H
#define _SIM_DETECTION_HWMAP_H

#include "mmi_data_types.h"

/* Structures for SIM detection */
#define MMISMUPASSWDREQIND			mmi_smu_password_required_ind_struct
#define MMISMUVERIFYPINRSP			mmi_smu_verify_pin_rsp_struct
#define MMISMUVERIFYPINREQ			mmi_smu_verify_pin_req_struct
#define MMISMUFAILIND				mmi_smu_fail_ind_struct
#define MMISMUSUPPORTPLMNLISTIND    mmi_smu_support_plmn_list_ind_struct
/* End sructures for sim detection */


/* Structure for NETWORK Attached and N/W Signal level Inication and Service Type GSM/GPRS */
#define	MMINWATTACHIND				mmi_nw_attach_ind_struct		
#define MMINWRXLEVELIND				mmi_nw_rx_level_ind_struct
#define MMINWREGSTATEIND			mmi_nw_reg_state_ind_struct
#define MMISMUGETIMSIRSP			mmi_smu_get_imsi_rsp_struct

/* End structure for NEWORK attached and signal events*/


/* Structure for batter status events*/
#define MMIEQBATTERYSTATUSIND		mmi_eq_battery_status_ind_struct

/* End sructures for battery status event*/



/* End Structure for network attached indication */
#endif  //_SIM_DETECTION_HWMAP_H

