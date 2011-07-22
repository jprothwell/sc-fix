
#include <cswtype.h>
#include <itf_api.h>
#include <cfw.h>
#include <cfw_prv.h>
#include <cos.h>
#include <ts.h>
#include "cfw_emod_tsm.h"

extern VOID* SUL_MemCopy8 (VOID*  dest, CONST VOID* src, UINT32 count);

BOOL ArrayCopare(UINT8 *Src1,UINT8 *Src2,UINT8 nSize)
{
	UINT8 i=0;
	for(i=0;i<nSize;i++)
	{
		if(Src1[i]!=Src2[i])
			return TRUE;
	}
	return FALSE;
}

BOOL CurrCellDataParse(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo,api_CurCellInfoInd_t* pCurrCellInfoInd)
{
	BOOL nPostMark=FALSE;
						
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Arfcn=%d!\n"),pCurrCellInfoInd->Arfcn);
	if(pCurrCellInfo->nTSM_Arfcn!=pCurrCellInfoInd->Arfcn)
	{
		pCurrCellInfo->nTSM_Arfcn = pCurrCellInfoInd->Arfcn;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_Arfcn=%d!\n"),pCurrCellInfo->nTSM_Arfcn);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Rla=%d!\n"),pCurrCellInfoInd->Rla);
	if(pCurrCellInfo->nTSM_AvRxLevel!=pCurrCellInfoInd->Rla)
	{
		pCurrCellInfo->nTSM_AvRxLevel = pCurrCellInfoInd->Rla;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Rla=%d!\n"),pCurrCellInfoInd->Rla);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("BcchDec=%d!\n"),pCurrCellInfoInd->BcchDec);
	if(pCurrCellInfo->nTSM_BCCHDec!=pCurrCellInfoInd->BcchDec)
	{
		pCurrCellInfo->nTSM_BCCHDec = pCurrCellInfoInd->BcchDec;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_BCCHDec=%d!\n"),pCurrCellInfo->nTSM_BCCHDec);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Ber=%d!\n"),pCurrCellInfoInd->Ber);
	if(pCurrCellInfo->nTSM_BER!=pCurrCellInfoInd->Ber)
	{
		pCurrCellInfo->nTSM_BER = pCurrCellInfoInd->Ber;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_BER=%d!\n"),pCurrCellInfo->nTSM_BER);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Bsic=%d!\n"),pCurrCellInfoInd->Bsic);
	if(pCurrCellInfo->nTSM_Bsic!=pCurrCellInfoInd->Bsic)
	{
		pCurrCellInfo->nTSM_Bsic = pCurrCellInfoInd->Bsic;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_Bsic=%d!\n"),pCurrCellInfo->nTSM_Bsic);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("C1=%x!\n"),pCurrCellInfoInd->C1);
	if(pCurrCellInfo->nTSM_C1Value!=pCurrCellInfoInd->C1)
	{
		pCurrCellInfo->nTSM_C1Value = pCurrCellInfoInd->C1;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_C1Value=%x!\n"),pCurrCellInfo->nTSM_C1Value);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CellId=%d!\n"),pCurrCellInfoInd->CellId[0]);
	if(ArrayCopare(pCurrCellInfo->nTSM_CellID,pCurrCellInfoInd->CellId, 2))
	{
		SUL_MemCopy8(pCurrCellInfo->nTSM_CellID,pCurrCellInfoInd->CellId,2);
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_CellID=%d!\n"),pCurrCellInfo->nTSM_CellID[0]);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Lai=%d!\n"),pCurrCellInfoInd->Lai[0]);
	if(ArrayCopare(pCurrCellInfo->nTSM_LAI,pCurrCellInfoInd->Lai, 5))
	{
		SUL_MemCopy8(pCurrCellInfo->nTSM_LAI,pCurrCellInfoInd->Lai,5);
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_LAI=%d!\n"),pCurrCellInfo->nTSM_LAI[0]);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("MTxPower=%d!\n"),pCurrCellInfoInd->MTxPower);
	if(pCurrCellInfo->nTSM_MaxTxPWR!=pCurrCellInfoInd->MTxPower)
	{
		pCurrCellInfo->nTSM_MaxTxPWR = pCurrCellInfoInd->MTxPower;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_MaxTxPWR=%d!\n"),pCurrCellInfo->nTSM_MaxTxPWR);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Nmo=%d!\n"),pCurrCellInfoInd->Nmo);
	if(pCurrCellInfo->nTSM_NetMode!=pCurrCellInfoInd->Nmo)
	{
		pCurrCellInfo->nTSM_NetMode = pCurrCellInfoInd->Nmo;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_NetMode=%d!\n"),pCurrCellInfo->nTSM_NetMode);
		nPostMark=TRUE;
	}				
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Rac=%d!\n"),pCurrCellInfoInd->Rac);
	if(pCurrCellInfo->nTSM_RAC!=pCurrCellInfoInd->Rac)
	{
		pCurrCellInfo->nTSM_RAC = pCurrCellInfoInd->Rac;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_RAC=%d!\n"),pCurrCellInfo->nTSM_RAC);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Ta=%d!\n"),pCurrCellInfoInd->Ta);
	if(pCurrCellInfo->nTSM_TimeADV!=pCurrCellInfoInd->Ta)
	{
		pCurrCellInfo->nTSM_TimeADV = pCurrCellInfoInd->Ta;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_TimeADV=%d!\n"),pCurrCellInfo->nTSM_TimeADV);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("AC=%d!\n"),pCurrCellInfoInd->AC[0]);
	if(ArrayCopare(pCurrCellInfo->nTSM_AC,pCurrCellInfoInd->AC, 2))
	{
		
		SUL_MemCopy8(pCurrCellInfo->nTSM_AC,pCurrCellInfoInd->AC,2);
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_AC=%d!\n"),pCurrCellInfo->nTSM_AC[0]);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("BsAgBlkRes=%d!\n"),pCurrCellInfoInd->BsAgBlkRes);
	if(pCurrCellInfo->nTSM_BsAgBlkRes!=pCurrCellInfoInd->BsAgBlkRes)
	{
		pCurrCellInfo->nTSM_BsAgBlkRes=pCurrCellInfoInd->BsAgBlkRes;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_BsAgBlkRes=%d!\n"),pCurrCellInfo->nTSM_BsAgBlkRes);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("C2=%x!\n"),pCurrCellInfoInd->C2);
	if(pCurrCellInfo->nTSM_C2!=pCurrCellInfoInd->C2)
	{
		pCurrCellInfo->nTSM_C2 = pCurrCellInfoInd->C2;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_C2=%x!\n"),pCurrCellInfo->nTSM_C2);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CCCHconf=%d!\n"),pCurrCellInfoInd->CCCHconf);
	if(pCurrCellInfo->nTSM_CCCHconf!=pCurrCellInfoInd->CCCHconf)
	{
		pCurrCellInfo->nTSM_CCCHconf=pCurrCellInfoInd->CCCHconf;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_CCCHconf=%d!\n"),pCurrCellInfo->nTSM_CCCHconf);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CurrBand=%d!\n"),pCurrCellInfoInd->CurrBand);
	if(pCurrCellInfo->nTSM_CurrBand!=pCurrCellInfoInd->CurrBand)
	{
		pCurrCellInfo->nTSM_CurrBand=pCurrCellInfoInd->CurrBand;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_CurrBand=%d!\n"),pCurrCellInfo->nTSM_CurrBand);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CurrChanMode=%x!\n"),pCurrCellInfoInd->CurrChanMode);
	if(pCurrCellInfo->nTSM_CurrChanMode!=pCurrCellInfoInd->CurrChanMode)
	{
		pCurrCellInfo->nTSM_CurrChanMode=pCurrCellInfoInd->CurrChanMode;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_CurrChanMode=%x!\n"),pCurrCellInfo->nTSM_CurrChanMode);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CurrChanType=%d!\n"),pCurrCellInfoInd->CurrChanType);
	if(pCurrCellInfo->nTSM_CurrChanType!=pCurrCellInfoInd->CurrChanType)
	{
		pCurrCellInfo->nTSM_CurrChanType=pCurrCellInfoInd->CurrChanType;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_CurrChanType=%d!\n"),pCurrCellInfo->nTSM_CurrChanType);
		nPostMark=TRUE;
	}			
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("MaxRetrans=%d!\n"),pCurrCellInfoInd->MaxRetrans);
	if(pCurrCellInfo->nTSM_MaxRetrans!=pCurrCellInfoInd->MaxRetrans)
	{
		pCurrCellInfo->nTSM_MaxRetrans=pCurrCellInfoInd->MaxRetrans;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_MaxRetrans=%d!\n"),pCurrCellInfo->nTSM_MaxRetrans);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("RxLevAccMin=%d!\n"),pCurrCellInfoInd->RxLevAccMin);
	if(pCurrCellInfo->nTSM_RxLevAccMin!=pCurrCellInfoInd->RxLevAccMin)
	{
		pCurrCellInfo->nTSM_RxLevAccMin=pCurrCellInfoInd->RxLevAccMin;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_RxLevAccMin=%d!\n"),pCurrCellInfo->nTSM_RxLevAccMin);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("RxQualFull=%d!\n"),pCurrCellInfoInd->RxQualFull);
	if(pCurrCellInfo->nTSM_RxQualFull!=pCurrCellInfoInd->RxQualFull)
	{
		pCurrCellInfo->nTSM_RxQualFull=pCurrCellInfoInd->RxQualFull;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_RxQualFull=%d!\n"),pCurrCellInfo->nTSM_RxQualFull);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("RxQualSub=%d!\n"),pCurrCellInfoInd->RxQualSub);
	if(pCurrCellInfo->nTSM_RxQualSub!=pCurrCellInfoInd->RxQualSub)
	{
		pCurrCellInfo->nTSM_RxQualSub=pCurrCellInfoInd->RxQualSub;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nTSM_RxQualSub=%d!\n"),pCurrCellInfo->nTSM_RxQualSub);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SI7_8=%d!\n"),pCurrCellInfoInd->SI7_8);
	if(pCurrCellInfo->bTSM_SI7_8!=pCurrCellInfoInd->SI7_8)
	{
		pCurrCellInfo->bTSM_SI7_8=pCurrCellInfoInd->SI7_8;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("bTSM_SI7_8=%d!\n"),pCurrCellInfo->bTSM_SI7_8);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EC=%d!\n"),pCurrCellInfoInd->EC);
	if(pCurrCellInfo->bTSM_EC!=pCurrCellInfoInd->EC)
	{
		pCurrCellInfo->bTSM_EC=pCurrCellInfoInd->EC;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("bTSM_EC=%d!\n"),pCurrCellInfo->bTSM_EC);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ATT=%d!\n"),pCurrCellInfoInd->ATT);
	if(pCurrCellInfo->bTSM_ATT!=pCurrCellInfoInd->ATT)
	{
		pCurrCellInfo->bTSM_ATT=pCurrCellInfoInd->ATT;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("bTSM_ATT=%d!\n"),pCurrCellInfo->bTSM_ATT);
		nPostMark=TRUE;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("API_CURCELLINFO_IND  nPostMark=%d!\r\n") ,nPostMark);
	return nPostMark;
}

BOOL NeighborCellDataParse(CFW_TSM_ALL_NEBCELL_INFO *pAllNeighborCellInfo,api_AdjCellInfoInd_t* pNeigborCellInfoInd)
{
	BOOL nPostMark=FALSE;
	UINT8 i;

  if(pNeigborCellInfoInd->AdjCellNb > 6)
    pNeigborCellInfoInd->AdjCellNb = 6;
  
	pAllNeighborCellInfo->nTSM_NebCellNUM=pNeigborCellInfoInd->AdjCellNb;
	for(i=0;i<pNeigborCellInfoInd->AdjCellNb;i++)
	{
		if(pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_Arfcn !=pNeigborCellInfoInd->AdjCellElem[i].Arfcn)
		{
			pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_Arfcn =pNeigborCellInfoInd->AdjCellElem[i].Arfcn;
			nPostMark=TRUE;
		}
		if(pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_AvRxLevel!=pNeigborCellInfoInd->AdjCellElem[i].Rla)
		{
			pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_AvRxLevel=pNeigborCellInfoInd->AdjCellElem[i].Rla;
			nPostMark=TRUE;
		}
		if(pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_BCCHDec!=pNeigborCellInfoInd->AdjCellElem[i].BcchDec)
		{
			pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_BCCHDec=pNeigborCellInfoInd->AdjCellElem[i].BcchDec;
			nPostMark=TRUE;
		}
		if(pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_Bsic!=pNeigborCellInfoInd->AdjCellElem[i].Bsic)
		{
			pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_Bsic=pNeigborCellInfoInd->AdjCellElem[i].Bsic;
			nPostMark=TRUE;
		}
		if(pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_C1Value!=pNeigborCellInfoInd->AdjCellElem[i].C1)
		{
			pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_C1Value=pNeigborCellInfoInd->AdjCellElem[i].C1;
			nPostMark=TRUE;
		}
		if(pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_MaxTxPWR!=pNeigborCellInfoInd->AdjCellElem[i].MTxPower)
		{
			pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_MaxTxPWR=pNeigborCellInfoInd->AdjCellElem[i].MTxPower;
			nPostMark=TRUE;
		}
		if(pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_RxLevAM!=pNeigborCellInfoInd->AdjCellElem[i].Ram)
		{
			pAllNeighborCellInfo->nTSM_NebCell[i].nTSM_RxLevAM=pNeigborCellInfoInd->AdjCellElem[i].Ram;
			nPostMark=TRUE;
		}
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("API_ADJCELLSINFO_IND  nPostMark=%d!\r\n") ,nPostMark);
	return nPostMark;
}



