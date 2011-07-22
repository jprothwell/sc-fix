/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      cfw_emod_tsm_in.h                                                     */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   Communication Framework constant definitions and macros.                 */
/*   See CSW developement Manual for more.                                    */
/*                                                                            */
/******************************************************************************/

#include <itf_api.h>

#ifndef _CFW_EMOD_TSM_IN_H_
#define _CFW_EMOD_TSM_IN_H_

#define BOARD_CFG_FACTORY_SIZE SIZEOF(factory_settings)
#define BIND_INFO_SIZE			SIZEOF(bind_info_t)


#define FACTORY_IMEISV			4
#define FACTORY_MB_SN			13
#define FACTORY_MP_SN			77
#define BIND_SIMSIGN			0
#define BIND_SIMNO			1
#define BIND_NWSIGN			91
#define BIND_NWNO			92






typedef struct _SIM_TSM_STATE
{
	UINT8 nPreState;
	UINT8 nCurrState;
	UINT8 nNextState;
	UINT8 pad;
}CFW_TSM_STATE;

typedef struct _CFW_TSM_GET_INFO
{
	CFW_TSM_FUNCTION_SELECT SelecNum;
	CFW_TSM_STATE  	  	 	nState;
	BOOL					bClose;
	UINT8					pad[3];
}CFW_TSM_GET_INFO;

/******************************tsm data prase*************************************************************/
BOOL CurrCellDataParse(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo,api_CurCellInfoInd_t* pCurrCellInfoInd);
BOOL NeighborCellDataParse(CFW_TSM_ALL_NEBCELL_INFO *pAllNeighborCellInfo,api_AdjCellInfoInd_t* pNeigborCellInfoInd);



/*******************************************rsp************************************************************/
UINT32 CFW_EmodOutfieldTestProc (HAO hAo,CFW_EV* pEvent);

/**********************************************************************************************************/


#define CFW_TSM_IDLE						0x01
#define CFW_TSM_REC_IND						0x02

VOID Emod_ASCtoIMSI(UINT8 *InPut,UINT8 *OutPut,UINT8 InLen);
VOID Emod_IMSItoASC(UINT8 *InPut,UINT8 *OutPut, UINT8* OutLen);
#endif // _CFW_EMOD_TSM_IN_H_
