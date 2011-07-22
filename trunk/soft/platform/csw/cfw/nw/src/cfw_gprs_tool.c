////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2009, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: cfw_gprs_tool.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
//    NAME              DATE                REMAKS
// EP                2009-5-6       Created initial version 1.0
//
////////////////////////////////////////////////////////////////////////////////
  

#ifdef CFW_GPRS_SUPPORT
#include <csw.h>

#include <cfw_config_prv.h>

#if (CFW_NW_DUMP_ENABLE==0) && (CFW_NW_SRV_ENABLE==1)
#include <cfw.h>
#include <cfw_prv.h>
#include "cfw_gprs_tool.h"

// qos to api covert
//[in] Qos
//[out]ApiQos
//[out] pQosLength: qos length for api

void CFW_GprsQos2Api(CFW_GPRS_QOS sQos, UINT8 ApiQos[11], UINT8 *pQosLength)
{
    CFW_GPRS_SSMQOS TempQos;

    TempQos.Length = 11;
		    SUL_MemSet8(&TempQos.QOS, 0x00, (UINT32)TempQos.Length);

    #if 1 //add by wuys for gprs test 2008-09-03 
    //TempQos.QOS.ReliabilityClass = 3;//sQos.nReliability;
    TempQos.QOS.ReliabilityClass = sQos.nReliability;//modify by wuys 2008-01-21
    //TempQos.QOS.DelayClass = 4;//sQos.nDelay;
    TempQos.QOS.DelayClass = sQos.nDelay;//modify by wuys 2008-01-21
    TempQos.QOS.Spare0 = 0;
    //TempQos.QOS.PrecedenceClass = 3;//sQos.nPrecedence;
    TempQos.QOS.PrecedenceClass = sQos.nPrecedence;//modify by wuys 2008-01-21
    TempQos.QOS.Spare1 = 0;
    //TempQos.QOS.PeakThroughput = 4;//sQos.nPeak;
    TempQos.QOS.PeakThroughput = sQos.nPeak;//modify by wuys 2008-01-21
    //TempQos.QOS.MeanThroughput = 16;//sQos.nMean;
      TempQos.QOS.MeanThroughput = 0x1F;//modify by wuys 2010-09-20, refer to Nokia
    TempQos.QOS.MeanThroughput = sQos.nMean;//modify by wuys 2008-01-21
    TempQos.QOS.Spare2 = 0;
    TempQos.QOS.DeliveryOfErroneousSDU = 1;
    TempQos.QOS.DeliveryOrder = 2;
    TempQos.QOS.TrafficClass = 4;
    TempQos.QOS.MaximumSDUSize = 0x96;
    TempQos.QOS.MaximumBitRateForUplink = 0x40;
    TempQos.QOS.MaximumBitRateForDownlink = 0x40;
    TempQos.QOS.SDUErrorRatio = 4;
    TempQos.QOS.ResidualBER = 7;
    TempQos.QOS.TrafficHandlingPriority = 0;
    TempQos.QOS.TransferDelay = 0;
    TempQos.QOS.GuaranteedBitRateForUplink = 0;
    TempQos.QOS.GuaranteedBitRateForDownlink = 0;
    #else

UINT8* PtQoS = &TempQos.QOS;
UINT8 PdpCtxNb = 15;



PtQoS[0]= ((tcpTabQos[PdpCtxNb][0])<<3) | tcpTabQos[PdpCtxNb][1];
                                  /*   Delay class    , Reliability class                          */
                                  /* 1    => Sapi 3   , 1 => RLC ack, LLC ack, protected mode      */
                                  /* 2    => Sapi 5   , 2 => RLC ack, LLC ack, protected mode      */
                                  /* 3    => Sapi 9   , 3 => RLC ack, LLC unack, protected mode    */
                                  /* else => Sapi 11  , 4 => RLC unack, LLC unack, protected mode  */
                                  /*                    5 => RLC unack, LLC unack, unprotected mode*/
  // En mode llc ack, plus le numero de sapi est grand plus la fenetre est petite
  // plus les emmerdes sont grands
  
   PtQoS[1]= ((tcpTabQos[PdpCtxNb][2])<<4) | 3;//tcpTabQos[PdpCtxNb][3];
                                  /* Peak throughput             , Precedence class */
                                  /* 1 => Up to 1 000 octet/s                       */
                                  /* 2 => Up to 2 000 octet/s                       */
                                  /* 3 => Up to 4 000 octet/s                       */
                                  /* 4 => Up to 8 000 octet/s                       */
                                  /* 5 => Up to 16 000 octet/s                      */
                                  /* 6 => Up to 32 000 octet/s                      */
                                  /* 7 => Up to 64 000 octet/s                      */
                                  /* 8 => Up to 128 000 octet/s                     */
                                  /* 9 => Up to 256 000 octet/s                     */
  
   PtQoS[2]= tcpTabQos[PdpCtxNb][4]; /* Mean throughput (1 => 100octets/s) */
   PtQoS[3]= ((tcpTabQos[PdpCtxNb][5])<<5) | ((tcpTabQos[PdpCtxNb][6])<<3) | (tcpTabQos[PdpCtxNb][7]); /* Traffic class,                Delivery order,             Delivery of erroneous SDU               */
                                  /* 0 => Subscribed traffic class 0 Subscribed delivery order 0 Subscribed delivery of erroneous SDUs */
                                  /* 1 => Conversational class     1 With delivery order       1 No detect ('-')                       */
                                  /* 2 => Streaming class          2 Without delivery order    2 Erroneous SDUs are delivered          */
                                  /* 3 => Interactive class                                    3 Erroneous SDUs are not delivered      */
                                  /* 4 => Background class                                                                             */
  
   PtQoS[4]= tcpTabQos[PdpCtxNb][8]; /* Maximum SDU size (0x99 => 1520 bytes)*/
   PtQoS[5]= tcpTabQos[PdpCtxNb][9]; /* Maximum bit rate for uplink (0x3F => 63 kbps) */
   PtQoS[6]= tcpTabQos[PdpCtxNb][10];/* Maximum bit rate for downlink (0x3F => 63 kbps) */
   PtQoS[7]= ((tcpTabQos[PdpCtxNb][11])<<4) | (tcpTabQos[PdpCtxNb][12]);
                                  /* Residual BER (1 => 5*10-2 ), SDU ratio error (1=>1*10-2 ) */
   PtQoS[8]= ((tcpTabQos[PdpCtxNb][13])<<2)| tcpTabQos[PdpCtxNb][14]; /* Transfer delay (0xF => 150ms), Traffic handling priority */
   PtQoS[9] = tcpTabQos[PdpCtxNb][15]; /* Guaranted bit rate for uplink */
   PtQoS[10]= tcpTabQos[PdpCtxNb][16]; /* Guaranted bit rate for downlink */
    
    
    #endif
		    SUL_MemSet8(&TempQos.QOS, 0x00, (UINT32)TempQos.Length);
      TempQos.QOS.MeanThroughput = 0x1F;//modify by wuys 2010-09-20, refer to Nokia

    SUL_MemCopy8(ApiQos, &TempQos.QOS, (UINT32)TempQos.Length);

    *pQosLength = TempQos.Length;
}

// qos to api covert
//[in]ApiQos
//[out] pQos
void CFW_GprsApi2Qos(CFW_GPRS_QOS *pQos, UINT8 ApiQos[11], UINT8 QosLength)
{
    CFW_GPRS_SSMQOS TempQos;
    SUL_MemCopy8(&TempQos.QOS, ApiQos, (UINT32)QosLength);
    TempQos.Length = QosLength;
    pQos->nDelay = TempQos.QOS.DelayClass;
    pQos->nMean = TempQos.QOS.MeanThroughput;
    pQos->nPeak = TempQos.QOS.PeakThroughput;
    pQos->nPrecedence = TempQos.QOS.PrecedenceClass;
    pQos->nReliability = TempQos.QOS.ReliabilityClass;
    
}
//[in]
//[out]
//[out]
void CFW_GprsPdpAddr2Api(CFW_GPRS_PDPCONT_INFO *pPdpCXT, UINT8 *pPdpAddLen, UINT8 *pPdpAdd)
{
    *pPdpAddLen = pPdpCXT->nPdpAddrSize+ 2;
    *pPdpAdd = 0x01;
    *(pPdpAdd + 1) = 0x21;
    SUL_MemCopy8(pPdpAdd + 2, pPdpCXT->pPdpAddr, pPdpCXT->nPdpAddrSize);
}

void CFW_GprsPdpApi2Addr(CFW_GPRS_PDPCONT_INFO *pPdpCXT, UINT8 pPdpAddLen, UINT8 *pPdpAdd)
{
    pPdpCXT->nPdpAddrSize= pPdpAddLen - 2;    
    SUL_MemCopy8(pPdpCXT->pPdpAddr, pPdpAdd + 2, pPdpCXT->nPdpAddrSize);
}


#ifdef CFW_MULTI_SIM
UINT8 CFW_GprsGetFreeCid(CFW_SIM_ID nSimID)
#else
UINT8 CFW_GprsGetFreeCid()
#endif
{
    HAO    hSm;
    GPRS_SM_INFO* GetProc;	
    UINT8 nAllIndex = 0;	
    UINT8 i = 0;
    UINT8 j = 0;

    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM
    while(((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, 0) ) != HNULL) && (i < 7))
#else
    while(((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW) ) != HNULL) && (i < 7))
#endif
    {
        GetProc = (GPRS_SM_INFO*)CFW_GetAoUserData(hSm); 
        if (GetProc )
        {
            if((GetProc->Cid2Index.nCid != 0xff) && (GetProc->Cid2Index.nCid >= 1) && (GetProc->Cid2Index.nCid <= 7))
            {
                nAllIndex |= ( 1 << (GetProc->Cid2Index.nCid - 1));
            }
            i++;
        }
    }
    
    while(j < 7)
    {	
        if(nAllIndex&(1 << j))
            j++;
        else
            return (j+1);
    }
    
    return 0xff;//???????????     
}
#endif
#endif

