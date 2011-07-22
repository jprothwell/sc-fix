/* MAP_ADDR.h defines all the variables as memory addresses.
   This file is created by voc_map automatically and needs to
   be included in the c project evironment. */

//#define BINARY_SIMULATION    1
#define INTERRUPT_notDMA    1
/************************
 **  Global Variables  **
 ************************/
#define RAM_X_BEGIN_ADDR               0x0000
#define RAM_Y_BEGIN_ADDR               0x4000 
#define RAM_I_BEGIN_ADDR               0x10000 
//SP
#define SP_16_ADDR								10*1024 -1 + RAM_X_BEGIN_ADDR
#define SP_32_ADDR								10*1024 -2 + RAM_Y_BEGIN_ADDR

/*----------------------------------------------------------------------------------*/
#define GLOBAL_STRUCT_BUFFER_ADDR			0 + RAM_X_BEGIN_ADDR
/*----------------------------------------------------------------------------------*/

//struct INPUT_DATA_BUFFER
#define GLOBAL_INPUT_BUFFER_ADDR			0 + GLOBAL_STRUCT_BUFFER_ADDR 
#define GLOBAL_INPUT_DATA_ADDR				0 + GLOBAL_INPUT_BUFFER_ADDR //2048short
#define GLOBAL_INPUT_OFFSET_ADDR			2048 + GLOBAL_INPUT_BUFFER_ADDR //2short
#define GLOBAL_INPUT_DATALEN_ADDR			2050 + GLOBAL_INPUT_BUFFER_ADDR //1short

//struct tWMAFileStateInternal
#define GLOBAL_tWMAFileStateInternal_ADDR		2052 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_cbPayloadLeft_ADDR			0 + GLOBAL_tWMAFileStateInternal_ADDR  //1short
#define GLOBAL_cbBlockLeft_ADDR				1 + GLOBAL_tWMAFileStateInternal_ADDR  //1short
#define GLOBAL_parse_state_ADDR				2 + GLOBAL_tWMAFileStateInternal_ADDR  //1short
#define GLOBAL_bBlockStart_ADDR				3 + GLOBAL_tWMAFileStateInternal_ADDR  //1short
#define GLOBAL_cbPayloadOffset_ADDR			4 + GLOBAL_tWMAFileStateInternal_ADDR  //2short
#define GLOBAL_pCallBackBuffer_ADDR			6 + GLOBAL_tWMAFileStateInternal_ADDR  //128short
#define GLOBAL_bHasDRM_ADDR				134 + GLOBAL_tWMAFileStateInternal_ADDR//1short
#define GLOBAL_bDecInWaitState_ADDR			135 + GLOBAL_tWMAFileStateInternal_ADDR//1short
#define GLOBAL_iPayload_ADDR				136 + GLOBAL_tWMAFileStateInternal_ADDR//2short
#define GLOBAL_wPayStart_ADDR				138 + GLOBAL_tWMAFileStateInternal_ADDR//2short
#define GLOBAL_bAllFramesDone_ADDR			140 + GLOBAL_tWMAFileStateInternal_ADDR//1short
#define GLOBAL_bFirst_ADDR				141 + GLOBAL_tWMAFileStateInternal_ADDR//1short

//struct tWMAFileHdrStateInternal
#define GLOBAL_tWMAFileHdrStateInternal_ADDR		2194 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_currPacketOffset_ADDR			0 + GLOBAL_tWMAFileHdrStateInternal_ADDR //2short
#define GLOBAL_nextPacketOffset_ADDR			2 + GLOBAL_tWMAFileHdrStateInternal_ADDR //2short
#define GLOBAL_cbPacketSize_ADDR				4 + GLOBAL_tWMAFileHdrStateInternal_ADDR //2short
#define GLOBAL_cbLastPacketOffset_ADDR			6 + GLOBAL_tWMAFileHdrStateInternal_ADDR //2short
#define GLOBAL_nBlockAlign_ADDR				8 + GLOBAL_tWMAFileHdrStateInternal_ADDR //1short
#define GLOBAL_wAudioStreamId_ADDR			10 + GLOBAL_tWMAFileHdrStateInternal_ADDR //2short

//struct PAYLOAD_MAP_ENTRY_EX
#define GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR		2206 + GLOBAL_STRUCT_BUFFER_ADDR
//#define GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR		2328 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_bIsCompressedPayload_ADDR		0 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short
#define GLOBAL_bStreamId_ADDR				1 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short
#define GLOBAL_cbPacketOffset_ADDR			2 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short
#define GLOBAL_cbTotalSize_ADDR				3 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short
#define GLOBAL_bObjectId_ADDR				4 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short 
#define GLOBAL_cbPayloadSize_ADDR			5 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short
#define GLOBAL_cbObjectOffset_ADDR			6 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //2short
#define GLOBAL_cbObjectSize_ADDR			8 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //2short
#define GLOBAL_msObjectPres_ADDR			10 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //2short
#define GLOBAL_cbRepData_ADDR				12 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short
#define GLOBAL_bSubPayloadState_ADDR			13 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //1short
#define GLOBAL_wBytesRead_ADDR				14 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //2short
#define GLOBAL_wTotalDataBytes_ADDR			16 + GLOBAL_PAYLOAD_MAP_ENTRY_EX_ADDR //2short	


//struct PACKET_PARSE_INFO_EX
#define GLOBAL_PACKET_PARSE_INFO_EX_ADDR		2224 + GLOBAL_STRUCT_BUFFER_ADDR
//#define GLOBAL_PACKET_PARSE_INFO_EX_ADDR		2350 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_fParityPacket_ADDR			0 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_fEccPresent_ADDR				1 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_bECLen_ADDR				2 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_bPacketLenType_ADDR			3 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_cbParseOffset_ADDR			4 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_cbPacketLenTypeOffset_ADDR		6 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_bPadLenType_ADDR				8 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_bSequenceLenType_ADDR			9 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_fMultiPayloads_ADDR			10 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_bOffsetBytes_ADDR			11 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_bOffsetLenType_ADDR			12 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short 
#define GLOBAL_wDuration_ADDR				13 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short 
#define GLOBAL_cbPacketLenOffset_ADDR			14 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_cbExplicitPacketLength_ADDR		16 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_cbSequenceOffset_ADDR			18 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_dwSequenceNum_ADDR			20 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_cbPadLenOffset_ADDR			22 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_cbPadding_ADDR				24 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_dwSCR_ADDR				26 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_bPayLenType_ADDR				28 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_bPayBytes_ADDR				29 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //1short
#define GLOBAL_cbPayLenTypeOffset_ADDR			30 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short
#define GLOBAL_cPayloads_ADDR				32 + GLOBAL_PACKET_PARSE_INFO_EX_ADDR //2short

//struct CWMAInputBitStream
#define GLOBAL_CWMAInputBitStream_ADDR			2258 + GLOBAL_STRUCT_BUFFER_ADDR
//#define GLOBAL_CWMAInputBitStream_ADDR			2384 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_m_dwHeaderBuf_ADDR				0 + GLOBAL_CWMAInputBitStream_ADDR//2short
#define GLOBAL_m_pBufferBegin_ADDR				2 + GLOBAL_CWMAInputBitStream_ADDR //1short
#define GLOBAL_m_pBuffer_ADDR					3 + GLOBAL_CWMAInputBitStream_ADDR //1short
#define GLOBAL_m_cbBuflenBegin_ADDR				4 + GLOBAL_CWMAInputBitStream_ADDR //1short 
#define GLOBAL_m_dwBitsLeft_ADDR				5 + GLOBAL_CWMAInputBitStream_ADDR //1short
#define GLOBAL_m_dwDot_ADDR						6 + GLOBAL_CWMAInputBitStream_ADDR //2short
#define GLOBAL_m_iPrevPacketNum_ADDR			8 + GLOBAL_CWMAInputBitStream_ADDR //1short
#define GLOBAL_m_cbBuflen_ADDR					9 + GLOBAL_CWMAInputBitStream_ADDR //1short
#define GLOBAL_m_fAllowPackets_ADDR				10 + GLOBAL_CWMAInputBitStream_ADDR //1short
#define GLOBAL_m_fSuppressPacketLoss_ADDR		11 + GLOBAL_CWMAInputBitStream_ADDR //1short

//struct CAudioObjectDecoder
#define GLOBAL_CAudioObjectDecoder_ADDR			2270 + GLOBAL_STRUCT_BUFFER_ADDR   
//#define GLOBAL_CAudioObjectDecoder_ADDR			2396 + GLOBAL_STRUCT_BUFFER_ADDR   
#define GLOBAL_m_fPacketLoss_ADDR					0 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_cFrmInPacket_ADDR				1 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_decsts_ADDR						2 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_subfrmdecsts_ADDR				3 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_hdrdecsts_ADDR					4 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_rlsts_ADDR						5 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_iChannel_ADDR					6 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_iBand_ADDR						7 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_fNoMoreData_ADDR				8 + GLOBAL_CAudioObjectDecoder_ADDR //1short
#define GLOBAL_m_fLastSubFrame_ADDR				9 + GLOBAL_CAudioObjectDecoder_ADDR //1short

//struct CAudioObject
#define GLOBAL_CAudioObject_ADDR				2280 + GLOBAL_STRUCT_BUFFER_ADDR 
//#define GLOBAL_CAudioObject_ADDR				2408 + GLOBAL_STRUCT_BUFFER_ADDR 
#define GLOBAL_m_iPacketCurr_ADDR					0 + GLOBAL_CAudioObject_ADDR //2short
#define GLOBAL_m_cBitPackedFrameSize_ADDR		2 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iVersion_ADDR						3 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cRunOfZeros_ADDR					4 + GLOBAL_CAudioObject_ADDR //1short
//													5 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iLevel_ADDR						5 + GLOBAL_CAudioObject_ADDR //1hort
#define GLOBAL_m_fNoiseSub_ADDR					6 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iMaxEscSize_ADDR					7 + GLOBAL_CAudioObject_ADDR //1short

#define GLOBAL_m_iSign_ADDR						8 + GLOBAL_CAudioObject_ADDR //2hort
#define GLOBAL_m_iHighCutOffCurr_ADDR				10 + GLOBAL_CAudioObject_ADDR //2hort
#define GLOBAL_m_iNextBarkIndex_ADDR				12 + GLOBAL_CAudioObject_ADDR //2short

#define GLOBAL_m_codecStatus_ADDR					14 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_fSeekAdjustment_ADDR			15 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_fPacketLossAdj_ADDR				16 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cChannel_ADDR					17 + GLOBAL_CAudioObject_ADDR //1short
//													19 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iSamplingRate_ADDR				18 + GLOBAL_CAudioObject_ADDR //2short
#define GLOBAL_m_cSubband_ADDR					20 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_fAllowSuperFrame_ADDR			21 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_fAllowSubFrame_ADDR				22 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_fV5Lpc_ADDR						23 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iCurrReconCoef_ADDR				24 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iCurrSubFrame_ADDR				25 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_fHeaderReset_ADDR				26 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_stereoMode_ADDR					27 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iSubFrameSizeWithUpdate_ADDR	28 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cPossibleWinSize_ADDR			29 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iMaxSubFrameDiv_ADDR			30 + GLOBAL_CAudioObject_ADDR //1short
//			33 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cMinSubFrameSampleHalf_ADDR	31 + GLOBAL_CAudioObject_ADDR //1short
//	35 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iIncr_ADDR						32 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cSubFrameSample_ADDR			33 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cSubFrameSampleHalf_ADDR		34 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cSubFrameSampleQuad_ADDR		35 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_subfrmconfigPrev_ADDR			36 + GLOBAL_CAudioObject_ADDR //50short
#define GLOBAL_m_subfrmconfigPrev_cSubFrame_ADDR			0 + GLOBAL_m_subfrmconfigPrev_ADDR// 1short 
//									1 + GLOBAL_m_subfrmconfigPrev_ADDR// 1short 
#define GLOBAL_m_subfrmconfigPrev_rgiSubFrameSize_ADDR			2 + GLOBAL_m_subfrmconfigPrev_ADDR// 16short
#define GLOBAL_m_subfrmconfigPrev_rgiSubFrameStart_ADDR		18 + GLOBAL_m_subfrmconfigPrev_ADDR// 16short	//not used by voc
#define GLOBAL_m_subfrmconfigPrev_rgfMaskUpdate_ADDR			34 + GLOBAL_m_subfrmconfigPrev_ADDR// 16short	//not used by voc
#define GLOBAL_m_subfrmconfigCurr_ADDR			86 + GLOBAL_CAudioObject_ADDR //50short
#define GLOBAL_m_subfrmconfigCurr_cSubFrame_ADDR			0 + GLOBAL_m_subfrmconfigCurr_ADDR// 1short
//									1 + GLOBAL_m_subfrmconfigCurr_ADDR// 1short
#define GLOBAL_m_subfrmconfigCurr_rgiSubFrameSize_ADDR			2 + GLOBAL_m_subfrmconfigCurr_ADDR// 16short
#define GLOBAL_m_subfrmconfigCurr_rgiSubFrameStart_ADDR			18 + GLOBAL_m_subfrmconfigCurr_ADDR//16short
#define GLOBAL_m_subfrmconfigCurr_rgfMaskUpdate_ADDR			34 + GLOBAL_m_subfrmconfigCurr_ADDR// 16short
#define GLOBAL_m_subfrmconfigNext_ADDR			136 + GLOBAL_CAudioObject_ADDR //50short
#define GLOBAL_m_subfrmconfigNext_cSubFrame_ADDR			0 + GLOBAL_m_subfrmconfigNext_ADDR// 1short
//									1 + GLOBAL_m_subfrmconfigNext_ADDR// 1short
#define GLOBAL_m_subfrmconfigNext_rgiSubFrameSize_ADDR			2 + GLOBAL_m_subfrmconfigNext_ADDR//16short
#define GLOBAL_m_subfrmconfigNext_rgiSubFrameStart_ADDR			18 + GLOBAL_m_subfrmconfigNext_ADDR//16short
#define GLOBAL_m_subfrmconfigNext_rgfMaskUpdate_ADDR			34 + GLOBAL_m_subfrmconfigNext_ADDR// 16short	//not used by voc
#define GLOBAL_m_cBitsSubbandMax_ADDR			186 + GLOBAL_CAudioObject_ADDR //1short
//			191 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cFrameSample_ADDR				187 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cFrameSampleHalf_ADDR			188 + GLOBAL_CAudioObject_ADDR //1short 
//#define GLOBAL_m_cFrameSampleQuad_ADDR			194 + GLOBAL_CAudioObject_ADDR //2short	//not used by voc
#define GLOBAL_m_cLowCutOff_ADDR					189 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_cHighCutOff_ADDR					190 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_cLowCutOffLong_ADDR				191 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_cHighCutOffLong_ADDR				192 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iWeightingMode_ADDR				193 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iEntropyMode_ADDR				194 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iQuantStepSize_ADDR				195 + GLOBAL_CAudioObject_ADDR //1short 
//#define GLOBAL_FFTcostabExAddr_ADDR				196 + GLOBAL_CAudioObject_ADDR  //2short               DMA
#define GLOBAL_m_cValidBarkBand_ADDR				196 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_rgiBarkIndex_ADDR				197+ GLOBAL_CAudioObject_ADDR //1short 

#define GLOBAL_m_qstQuantStep_ADDR				198 + GLOBAL_CAudioObject_ADDR //4short 
#define GLOBAL_m_Quant_iFracBits_ADDR					0 + GLOBAL_m_qstQuantStep_ADDR // 2 short
#define GLOBAL_m_Quant_iFraction_ADDR					2 + GLOBAL_m_qstQuantStep_ADDR // 2 short 
#define GLOBAL_m_iSizePrev_ADDR					202 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iSizeCurr_ADDR					203 + GLOBAL_CAudioObject_ADDR //1short
#define GLOBAL_m_iSizeNext_ADDR					204 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iCoefRecurQ1_ADDR				205 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iCoefRecurQ2_ADDR				206 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iCoefRecurQ3_ADDR				207 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iCoefRecurQ4_ADDR				208 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iDiscardSilence_ADDR				209 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_fltFirstNoiseFreq_ADDR				210 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iFirstNoiseBand_ADDR				211 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iFirstNoiseIndex_ADDR				212 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_iFrameNumber_ADDR				213 + GLOBAL_CAudioObject_ADDR //1short 
#define GLOBAL_m_tRandState_ADDR					214 + GLOBAL_CAudioObject_ADDR //5short
#define GLOBAL_m_piSinForRecon_ADDR				220 + GLOBAL_CAudioObject_ADDR //2short 
//#define GLOBAL_m_piSinForSaveHistory_ADDR			224 + GLOBAL_CAudioObject_ADDR //2short		DMA
//#define GLOBAL_m_piSinForRecon_EXT_ADDR			226 + GLOBAL_CAudioObject_ADDR //2short 		DMA

//struct PerChannelInfo_____left channel
//#define GLOBAL_LeftChannelInfo_ADDR			2718 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_LeftChannelInfo_ADDR			2502 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_Lm_rgiCoefQ_ADDR					0 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_rgiMaskQ_ADDR					1 + GLOBAL_LeftChannelInfo_ADDR//1short	//not used by voc
#define GLOBAL_Lm_rgbBandNotCoded_ADDR			2 + GLOBAL_LeftChannelInfo_ADDR//1short,replaced by buffer,can be del-----
#define GLOBAL_Lm_rgffltSqrtBWRatio_ADDR		3 + GLOBAL_LeftChannelInfo_ADDR//1short,replaced by buffer,can be del-----
#define GLOBAL_Lm_rgiNoisePower_ADDR			4 + GLOBAL_LeftChannelInfo_ADDR//1short,replaced by buffer,can be del-----
#define GLOBAL_Lm_rgiHuffDecTbl_ADDR			5 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_rgiRunEntry_ADDR				6 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_rgiLevelEntry_ADDR			7 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_cSubbandActual_ADDR			8 + GLOBAL_LeftChannelInfo_ADDR//1short 
//							9 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_iPower_ADDR					10 + GLOBAL_LeftChannelInfo_ADDR//1short
//							11 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_iActualPower_ADDR				12 + GLOBAL_LeftChannelInfo_ADDR//1short
//				13 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_rguiWeightFactor_ADDR			14 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_rgiCoefRecon_ADDR				15 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_iMaxMaskQ_ADDR				16 + GLOBAL_LeftChannelInfo_ADDR//1short
//							17 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_iCurrGetPCM_SubFrame_ADDR		18 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_rgiCoefReconCurr_ADDR			19 + GLOBAL_LeftChannelInfo_ADDR//1short
#define GLOBAL_Lm_piPrevOutputCurr_ADDR			20 + GLOBAL_LeftChannelInfo_ADDR//2short
//							22 + GLOBAL_LeftChannelInfo_ADDR//1short
//#define GLOBAL_Lm_rgfltCoefRecon_ADDR			23 + GLOBAL_LeftChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Lm_iSin_ADDR					24 + GLOBAL_LeftChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Lm_iCos_ADDR					26 + GLOBAL_LeftChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Lm_iSin1_ADDR					28 + GLOBAL_LeftChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Lm_iCos1_ADDR					30 + GLOBAL_LeftChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Lm_iStep_ADDR					32 + GLOBAL_LeftChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Lm_rgfltBandWeight_ADDR		34 + GLOBAL_LeftChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Lm_rgfltWeightFactor_ADDR		35 + GLOBAL_LeftChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Lm_wtMaxWeight_ADDR			36 + GLOBAL_LeftChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_L_ppcinfoENC_ADDR				38 + GLOBAL_LeftChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Lm_rgfMaskNeededForBark_ADDR	39 + GLOBAL_LeftChannelInfo_ADDR//1short	//not used by voc
#define GLOBAL_EQtype_ADDR						2540 + GLOBAL_STRUCT_BUFFER_ADDR
//struct PerChannelInfo_____right channel
//#define GLOBAL_RightChannelInfo_ADDR			2758 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_RightChannelInfo_ADDR			2542 + GLOBAL_STRUCT_BUFFER_ADDR
#define GLOBAL_Rm_rgiCoefQ_ADDR					0 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_rgiMaskQ_ADDR					1 + GLOBAL_RightChannelInfo_ADDR//1short	//not used by voc
#define GLOBAL_Rm_rgbBandNotCoded_ADDR			2 + GLOBAL_RightChannelInfo_ADDR//1short,replaced by buffer,can be del-----
#define GLOBAL_Rm_rgffltSqrtBWRatio_ADDR		3 + GLOBAL_RightChannelInfo_ADDR//1short,replaced by buffer,can be del-----
#define GLOBAL_Rm_rgiNoisePower_ADDR			4 + GLOBAL_RightChannelInfo_ADDR//1short,replaced by buffer,can be del-----
#define GLOBAL_Rm_rgiHuffDecTbl_ADDR			5 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_rgiRunEntry_ADDR				6 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_rgiLevelEntry_ADDR			7 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_cSubbandActual_ADDR			8 + GLOBAL_RightChannelInfo_ADDR//1short
//							9 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_iPower_ADDR					10 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_iActualPower_ADDR				12 + GLOBAL_RightChannelInfo_ADDR//1short
//			13 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_rguiWeightFactor_ADDR			14 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_rgiCoefRecon_ADDR				15 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_iMaxMaskQ_ADDR				16 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_iCurrGetPCM_SubFrame_ADDR		18 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_rgiCoefReconCurr_ADDR			19 + GLOBAL_RightChannelInfo_ADDR//1short
#define GLOBAL_Rm_piPrevOutputCurr_ADDR			20 + GLOBAL_RightChannelInfo_ADDR//2short 
//							22 + GLOBAL_RightChannelInfo_ADDR//1short
//#define GLOBAL_Rm_rgfltCoefRecon_ADDR			23 + GLOBAL_RightChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Rm_iSin_ADDR					24 + GLOBAL_RightChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Rm_iCos_ADDR					26 + GLOBAL_RightChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Rm_iSin1_ADDR					28 + GLOBAL_RightChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Rm_iCos1_ADDR					30 + GLOBAL_RightChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Rm_iStep_ADDR					32 + GLOBAL_RightChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_Rm_rgfltBandWeight_ADDR		34 + GLOBAL_RightChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Rm_rgfltWeightFactor_ADDR		35 + GLOBAL_RightChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Rm_wtMaxWeight_ADDR			36 + GLOBAL_RightChannelInfo_ADDR//2short	//not used by voc
//#define GLOBAL_R_ppcinfoENC_ADDR				38 + GLOBAL_RightChannelInfo_ADDR//1short	//not used by voc
//#define GLOBAL_Rm_rgfMaskNeededForBark_ADDR	39 + GLOBAL_RightChannelInfo_ADDR//1short	//not used by voc

//global vars&buffers
/*----------------------------------------------------------------------------------*/
//#define GLOBAL_BUFFER_X_ADDR			2798 + RAM_X_BEGIN_ADDR
#define GLOBAL_BUFFER_X_ADDR			2582 + RAM_X_BEGIN_ADDR
/*----------------------------------------------------------------------------------*/
#define GLOBAL_FFTcostabExAddr_ADDR			0 + GLOBAL_BUFFER_X_ADDR  //2short               DMA
#define GLOBAL_m_piSinForSaveHistory_ADDR	2 + GLOBAL_BUFFER_X_ADDR //2short		DMA
#define GLOBAL_m_piSinForRecon_EXT_ADDR		4 + GLOBAL_BUFFER_X_ADDR //2short 		DMA
#define GLOBAL_OutPcmDMAExAddr_ADDR			6 + GLOBAL_BUFFER_X_ADDR//2short		DMA
#define GLOBAL_PrevOutputDMAExAddr_ADDR		8 + GLOBAL_BUFFER_X_ADDR//2short		DMA
#define GLOBAL_HuffDecTblDMAExAddr_ADDR		10 + GLOBAL_BUFFER_X_ADDR  //2short		DMA
#define GLOBAL_FILE_POINTER_ADDR            12 + GLOBAL_BUFFER_X_ADDR//  2short

#define GLOBAL_m_rgiBarkIndexOrig_ADDR		14 + GLOBAL_BUFFER_X_ADDR //130short
#define GLOBAL_m_rgcValidBarkBand_ADDR		144 + GLOBAL_BUFFER_X_ADDR //5short
#define GLOBAL_m_rgiFirstNoiseBand_ADDR		149 + GLOBAL_BUFFER_X_ADDR //5short
#define GLOBAL_m_rgiBarkIndexOrig_BUFFER    	GLOBAL_m_rgiBarkIndexOrig_ADDR//  130shorts(short)//add by zhangl
#define GLOBAL_m_rgcValidBarkBand_BUFFER    	GLOBAL_m_rgcValidBarkBand_ADDR//  5shorts

#define GLOBAL_Lm_rgbBandNotCoded_BUFFER    	154 + GLOBAL_BUFFER_X_ADDR//  25shorts
#define GLOBAL_Rm_rgbBandNotCoded_BUFFER    	179 + GLOBAL_BUFFER_X_ADDR//  25shorts
#define GLOBAL_Lm_rgiNoisePower_BUFFER      	204 + GLOBAL_BUFFER_X_ADDR//  50shorts
#define GLOBAL_Rm_rgiNoisePower_BUFFER      	254 + GLOBAL_BUFFER_X_ADDR//  50shorts
#define GLOBAL_Lm_rgffltSqrtBWRatio_BUFFER  	304 + GLOBAL_BUFFER_X_ADDR//  100shorts
#define GLOBAL_Rm_rgffltSqrtBWRatio_BUFFER  	404 + GLOBAL_BUFFER_X_ADDR//  100shorts
#define GLOBAL_Lm_rgiMaskQ_BUFFER           	504 + GLOBAL_BUFFER_X_ADDR//  25shorts
#define GLOBAL_Rm_rgiMaskQ_BUFFER           	529 + GLOBAL_BUFFER_X_ADDR//  25shorts
#define GLOBAL_L_m_rgiMaskQ_ADDR				GLOBAL_Lm_rgiMaskQ_BUFFER // 25short//add by mohongfee
#define GLOBAL_R_m_rgiMaskQ_ADDR				GLOBAL_Rm_rgiMaskQ_BUFFER // 25short

#define GLOBAL_PHdrParse_BUFFER_ADDR		554 + GLOBAL_BUFFER_X_ADDR//8Short..GLOBAL
#define GLOBAL_PHdrParse_OFFSET_1_ADDR		555 + GLOBAL_BUFFER_X_ADDR//1Short
#define GLOBAL_PHdrParse_OFFSET_2_ADDR		556 + GLOBAL_BUFFER_X_ADDR//1Short
#define GLOBAL_PHdrParse_OFFSET_3_ADDR		557 + GLOBAL_BUFFER_X_ADDR//1Short
#define GLOBAL_PHdrParse_OFFSET_4_ADDR		558 + GLOBAL_BUFFER_X_ADDR//1Short
#define GLOBAL_PHdrParse_OFFSET_5_ADDR		559 + GLOBAL_BUFFER_X_ADDR//1Short
#define GLOBAL_PHdrParse_OFFSET_6_ADDR		560 + GLOBAL_BUFFER_X_ADDR//1Short
#define GLOBAL_PHdrParse_OFFSET_7_ADDR		561 + GLOBAL_BUFFER_X_ADDR//1Short
#define GLOBAL_wmaErr_ADDR					562 + GLOBAL_BUFFER_X_ADDR//1short
#define GLOBAL_wmaResult_ADDR				563 + GLOBAL_BUFFER_X_ADDR//1short
#define GLOBAL_ChannelSelect_ADDR			564 + GLOBAL_BUFFER_X_ADDR//  1short
#define GLOBAL_ChannelInfo_ADDR				565 + GLOBAL_BUFFER_X_ADDR//  1short

#define GLOBAL_VOC_CFG_DMA_LADDR            566 + GLOBAL_BUFFER_X_ADDR//  1short        
#define GLOBAL_VOC_CFG_DMA_SIZE             567 + GLOBAL_BUFFER_X_ADDR//  1short
#define GLOBAL_VOC_CFG_DMA_EADDR            568 + GLOBAL_BUFFER_X_ADDR//  2short
#define GLOBAL_ReLoadLPC_EADDR           566 + GLOBAL_BUFFER_X_ADDR//  2short        
#define GLOBAL_ReLoadIDCT_EADDR            568 + GLOBAL_BUFFER_X_ADDR//  2short


//#define GLOBAL_OutPcmDMAExAddr_ADDR			10 + GLOBAL_BUFFER_X_ADDR//2short		DMA
//#define GLOBAL_PrevOutputDMAExAddr_ADDR		12 + GLOBAL_BUFFER_X_ADDR//2short		DMA
//#define GLOBAL_m_rgiBarkIndexOrig_ADDR		14 + GLOBAL_BUFFER_X_ADDR //130short
//#define GLOBAL_m_rgcValidBarkBand_ADDR		144 + GLOBAL_BUFFER_X_ADDR //5short
//#define GLOBAL_m_rgiFirstNoiseBand_ADDR		149 + GLOBAL_BUFFER_X_ADDR //5short
//#define GLOBAL_m_rgiBarkIndexOrig_BUFFER    	GLOBAL_m_rgiBarkIndexOrig_ADDR//  130shorts(short)//add by zhangl
//#define GLOBAL_m_rgcValidBarkBand_BUFFER    	GLOBAL_m_rgcValidBarkBand_ADDR//  5shorts
//#define GLOBAL_ChannelSelect_ADDR				154 + GLOBAL_BUFFER_X_ADDR//  1short
//#define GLOBAL_ChannelInfo_ADDR				155 + GLOBAL_BUFFER_X_ADDR//  1short

//#define GLOBAL_Lm_rgbBandNotCoded_BUFFER    	156 + GLOBAL_BUFFER_X_ADDR//  25shorts
//#define GLOBAL_Rm_rgbBandNotCoded_BUFFER    	181 + GLOBAL_BUFFER_X_ADDR//  25shorts
//#define GLOBAL_Lm_rgiNoisePower_BUFFER      		206 + GLOBAL_BUFFER_X_ADDR//  50shorts
//#define GLOBAL_Rm_rgiNoisePower_BUFFER      	256 + GLOBAL_BUFFER_X_ADDR//  50shorts
//#define GLOBAL_Lm_rgffltSqrtBWRatio_BUFFER  	306 + GLOBAL_BUFFER_X_ADDR//  100shorts
//#define GLOBAL_Rm_rgffltSqrtBWRatio_BUFFER  	406 + GLOBAL_BUFFER_X_ADDR//  100shorts
//#define GLOBAL_Lm_rgiMaskQ_BUFFER           		506 + GLOBAL_BUFFER_X_ADDR//  25shorts
//#define GLOBAL_Rm_rgiMaskQ_BUFFER           		531 + GLOBAL_BUFFER_X_ADDR//  25shorts
//#define GLOBAL_L_m_rgiMaskQ_ADDR				GLOBAL_Lm_rgiMaskQ_BUFFER // 25short//add by mohongfee
//#define GLOBAL_R_m_rgiMaskQ_ADDR				GLOBAL_Rm_rgiMaskQ_BUFFER // 25short
//#define GLOBAL_HuffDecTblDMAExAddr_ADDR		556 + GLOBAL_BUFFER_X_ADDR  //2short		DMA


/*----------------------------------------------------------------------------------*/
#define GLOBAL_BUFFER_Y_ADDR			0 + RAM_Y_BEGIN_ADDR
/*----------------------------------------------------------------------------------*/
#define GLOBAL_m_rgiCoefReconOrig_ADDR		0 + GLOBAL_BUFFER_Y_ADDR//(8192+32)->4096short
#define GLOBAL_Rm_rgiCoefReconOrig_ADDR		4096 + GLOBAL_BUFFER_Y_ADDR//4096short
#define GLOBAL_Lm_rgiCoefReconOrig_BUFFER 	GLOBAL_m_rgiCoefReconOrig_ADDR//4096shorts
#define GLOBAL_Rm_rgiCoefReconOrig_BUFFER	GLOBAL_Rm_rgiCoefReconOrig_ADDR//4096shorts
#define GLOBAL_Lm_rgiCoefReconOrig_BUFFER 	GLOBAL_m_rgiCoefReconOrig_ADDR
#define GLOBAL_Rm_rgiCoefReconOrig_BUFFER	GLOBAL_Rm_rgiCoefReconOrig_ADDR
#define LOCAL_ChLeft_pWF_ADDR      		1000 + GLOBAL_Rm_rgiCoefReconOrig_ADDR //1kshorts
#define LOCAL_ChRight_pWF_ADDR     		2024 + GLOBAL_Rm_rgiCoefReconOrig_ADDR //1kshorts
#define GLOBAL_Lm_rguiWeightFactor_BUFFER       LOCAL_ChLeft_pWF_ADDR//1024 short
#define GLOBAL_Rm_rguiWeightFactor_BUFFER       LOCAL_ChRight_pWF_ADDR//1024 short


/*********************************
 **  		Constants		    **
 *********************************/
 //address of const stored in RAM_X
#define GLOBAL_ROM_BUFFER_X_ADDR				3156 + RAM_X_BEGIN_ADDR 
 
#define TABLE_g_rgiHuffDecTblNoisePower_ADDR	0 + GLOBAL_ROM_BUFFER_X_ADDR //[76]shorts
#define TABLE_g_rgiLsfReconLevel_ADDR		76 + GLOBAL_ROM_BUFFER_X_ADDR //[160]words
#define TABLE_g_sct64_ADDR					396 + GLOBAL_ROM_BUFFER_X_ADDR //[17]words
#define TABLE_g_sct128_ADDR					430 + GLOBAL_ROM_BUFFER_X_ADDR //[13]words
#define TABLE_g_sct256_ADDR					456 + GLOBAL_ROM_BUFFER_X_ADDR //[17]words
#define TABLE_g_sct512_ADDR					490 + GLOBAL_ROM_BUFFER_X_ADDR //[13]words
#define TABLE_g_sct1024_ADDR				516 + GLOBAL_ROM_BUFFER_X_ADDR //[17]words
#define TABLE_g_sct2048_ADDR				550 + GLOBAL_ROM_BUFFER_X_ADDR //[13]words
#define TABLE_g_InvQuadRootFraction_ADDR	576 + GLOBAL_ROM_BUFFER_X_ADDR //[257]words
#define TABLE_g_InvQuadRootExponent_ADDR	1090 + GLOBAL_ROM_BUFFER_X_ADDR //[65]words

#define CONST_GLOBAL_ADDR						1220 + GLOBAL_ROM_BUFFER_X_ADDR 
#define GLOBAL_g_uWMALCA_BUFFER					0 + CONST_GLOBAL_ADDR //13short
#define GLOBAL_g_uWMALCC_BUFFER					13 + CONST_GLOBAL_ADDR //3short
#define LPC_gLZLTable_ADD					16 + CONST_GLOBAL_ADDR  //128short

#define CONST_VAR_ADDR			1364 + GLOBAL_ROM_BUFFER_X_ADDR //[76]shorts-push 16
#define CONST_SHORT_0_ADDR		0 + CONST_VAR_ADDR
#define CONST_SHORT_1_ADDR		1 + CONST_VAR_ADDR
#define CONST_SHORT_2_ADDR		2 + CONST_VAR_ADDR
#define CONST_SHORT_3_ADDR		3 + CONST_VAR_ADDR
#define CONST_SHORT_4_ADDR		4 + CONST_VAR_ADDR
#define CONST_SHORT_5_ADDR		5 + CONST_VAR_ADDR
#define CONST_SHORT_6_ADDR		6 + CONST_VAR_ADDR
#define CONST_SHORT_7_ADDR		7 + CONST_VAR_ADDR
#define CONST_SHORT_8_ADDR		8 + CONST_VAR_ADDR
#define CONST_SHORT_9_ADDR		9 + CONST_VAR_ADDR
#define CONST_SHORT_10_ADDR		10 + CONST_VAR_ADDR
#define CONST_SHORT_12_ADDR		11 + CONST_VAR_ADDR
#define CONST_SHORT_15_ADDR		12 + CONST_VAR_ADDR
#define CONST_SHORT_16_ADDR		13 + CONST_VAR_ADDR
#define CONST_SHORT_20_ADDR		14 + CONST_VAR_ADDR
#define CONST_SHORT_24_ADDR		15 + CONST_VAR_ADDR
#define CONST_SHORT_26_ADDR		16 + CONST_VAR_ADDR
#define CONST_SHORT_32_ADDR		17 + CONST_VAR_ADDR
#define CONST_SHORT_48_ADDR		18 + CONST_VAR_ADDR
#define CONST_SHORT_64_ADDR		19 + CONST_VAR_ADDR
#define CONST_SHORT_93_ADDR		20 + CONST_VAR_ADDR
#define CONST_SHORT_96_ADDR		21 + CONST_VAR_ADDR
#define CONST_SHORT_128_ADDR	       22 + CONST_VAR_ADDR
#define CONST_SHORT_192_ADDR	       23 + CONST_VAR_ADDR	
#define CONST_SHORT_256_ADDR	       24 + CONST_VAR_ADDR	
#define CONST_SHORT_512_ADDR	       25 + CONST_VAR_ADDR	
#define CONST_SHORT_1024_ADDR	       26 + CONST_VAR_ADDR	
#define CONST_SHORT_2048_ADDR	       27 + CONST_VAR_ADDR	
#define CONST_WORD_1_ADDR			28 + CONST_VAR_ADDR	
#define CONST_WORD_15_ADDR			30 + CONST_VAR_ADDR	
#define CONST_WORD_0X7FFFFFFF_ADDR	32 + CONST_VAR_ADDR	
#define CONST_WORD_0XFFFFFFFF_ADDR	34 + CONST_VAR_ADDR	
#define CONST_WORD_0_ADDR			36 + CONST_VAR_ADDR	
#define CONST_WORD_64_ADDR			38 + CONST_VAR_ADDR	
#define CONST_WORD_0X00007FFF_ADDR	40 + CONST_VAR_ADDR	
#define CONST_0XFF000000_ADDR		42 + CONST_VAR_ADDR

//#define TABLE_g_rgiHuffDecTbl16ssOb_ADDR	1220 + GLOBAL_ROM_BUFFER_X_ADDR //[848]shorts
#define TABLE_g_rgiHuffDecTbl16ssOb_ADDR	1408 + GLOBAL_ROM_BUFFER_X_ADDR //[848]shorts
#define TABLE_gRun16ssOb_ADDR				2256+ GLOBAL_ROM_BUFFER_X_ADDR //[217]shorts
#define TABLE_gLevel16ssOb_ADDR				2473 + GLOBAL_ROM_BUFFER_X_ADDR //[217]shorts

#define TABLE_g_rgiHuffDecTbl16smOb_ADDR	1408 + GLOBAL_ROM_BUFFER_X_ADDR //[872]shorts
#define TABLE_gRun16smOb_ADDR				2280 + GLOBAL_ROM_BUFFER_X_ADDR //[237]shorts
#define TABLE_gLevel16smOb_ADDR				2517 + GLOBAL_ROM_BUFFER_X_ADDR //[237]shorts

#define TABLE_g_rgiHuffDecTbl44ssQb_ADDR	1408 + GLOBAL_ROM_BUFFER_X_ADDR //[1104]shorts
#define TABLE_gRun44ssQb_ADDR				2512 + GLOBAL_ROM_BUFFER_X_ADDR //[553]shorts
#define TABLE_gLevel44ssQb_ADDR			3065 + GLOBAL_ROM_BUFFER_X_ADDR //[277]shorts

#define TABLE_g_rgiHuffDecTbl44smQb_ADDR	1408 + GLOBAL_ROM_BUFFER_X_ADDR //[1244]shorts
#define TABLE_gRun44smQb_ADDR				2652 + GLOBAL_ROM_BUFFER_X_ADDR //[664]shorts
#define TABLE_gLevel44smQb_ADDR				3316 + GLOBAL_ROM_BUFFER_X_ADDR //[332]shorts

#define TABLE_g_rgiHuffDecTbl44ssOb_ADDR	1408 + GLOBAL_ROM_BUFFER_X_ADDR //[2002]shorts
#define TABLE_gRun44ssOb_ADDR				3410 + GLOBAL_ROM_BUFFER_X_ADDR //[1069]shorts
#define TABLE_gLevel44ssOb_ADDR				4479 + GLOBAL_ROM_BUFFER_X_ADDR //[535]shorts

#define TABLE_g_rgiHuffDecTbl44smOb_ADDR	1408 + GLOBAL_ROM_BUFFER_X_ADDR //[2848]shorts
#define TABLE_gRun44smOb_ADDR				4256 + GLOBAL_ROM_BUFFER_X_ADDR //[667]shorts
#define TABLE_gLevel44smOb_ADDR				4923 + GLOBAL_ROM_BUFFER_X_ADDR //[1333]shorts

#define TABLE_fftCostab_ADDR				1408 + GLOBAL_ROM_BUFFER_X_ADDR //[2048]shorts


 //address of const stored in RAM_Y
#define GLOBAL_ROM_BUFFER_Y_ADDR		8192 + GLOBAL_BUFFER_Y_ADDR//4096short
#define TABLE_getMask_ADDR				0 + GLOBAL_ROM_BUFFER_Y_ADDR //[33]words
#define TABLE_g_rgiHuffDecTblMsk_ADDR	66 + GLOBAL_ROM_BUFFER_Y_ADDR //[226]shorts
#define TABLE_rgSinCosTables_ADDR		292 + GLOBAL_ROM_BUFFER_Y_ADDR //[18]shorts
#define TABLE_rgiMaskMinusPower10_ADDR	310 + GLOBAL_ROM_BUFFER_Y_ADDR //[72]words
#define TABLE_rgiMaskPlusPower10_ADDR	454 + GLOBAL_ROM_BUFFER_Y_ADDR //[50]words
#define TABLE_rgDBPower10_ADDR			554 + GLOBAL_ROM_BUFFER_Y_ADDR //[128]words
#define TABLE_g_InverseFraction_ADDR	810 + GLOBAL_ROM_BUFFER_Y_ADDR //[257]words
#define TABLE_g_SqrtFraction_ADDR		1324 + GLOBAL_ROM_BUFFER_Y_ADDR //[257]words
#define TABLE_BandWeightToEQtype_ADDR			1838 + GLOBAL_ROM_BUFFER_Y_ADDR//70 shorts
#define TABLE_BandFreqToSamplingrate_ADDR		1908 + GLOBAL_ROM_BUFFER_Y_ADDR//70 shorts


/*********************************
 **  		Local Variables     **
 *********************************/
#define LOCAL_VAR_ADDR				9670 + RAM_X_BEGIN_ADDR//+2048-40
//for new fft
#define LOCAL_DMA_BUFFER				0 + LOCAL_VAR_ADDR	//256short
#define LOCAL_pData_ADDR				256 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_n_ADDR				257 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_i_ADDR				258 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_j_ADDR				259 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_CR_ADDR				260 + LOCAL_VAR_ADDR	//2short
#define LOCAL_VAR_SI_ADDR				262 + LOCAL_VAR_ADDR	//2short
#define LOCAL_VAR_t_ADDR				264 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_32divt_ADDR			265 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_ur_ADDR				266 + LOCAL_VAR_ADDR	//2short
#define LOCAL_VAR_ui_ADDR				268 + LOCAL_VAR_ADDR	//2short
#define LOCAL_VAR_ndivt_ADDR			270 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_pxk_ADDR			271 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_K2I_ADDR				272 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_tHalf_ADDR			273 + LOCAL_VAR_ADDR	//1short
#define LOCAL_pData32_ADDR				274 + LOCAL_VAR_ADDR	//1short
#define LOCAL_VAR_Nlogt_ADDR			275 + LOCAL_VAR_ADDR	//1short

//preFFT
#define LOCAL_STEP_ADDR				62 + LOCAL_VAR_ADDR //2short 
#define LOCAL_CR2_ADDR				64 + LOCAL_VAR_ADDR //2short 
#define LOCAL_CI2_ADDR				66 + LOCAL_VAR_ADDR //2short 

#define LOCAL_CI_ADDR				0 + LOCAL_VAR_ADDR //2short
#define LOCAL_CR_ADDR				2 + LOCAL_VAR_ADDR //2short
#define LOCAL_CI1_ADDR				4 + LOCAL_VAR_ADDR //2short
#define LOCAL_CR1_ADDR				6 + LOCAL_VAR_ADDR //2short
#define LOCAL_SimStack32_ADDR			8 + LOCAL_VAR_ADDR //2short
#define LOCAL_iTr_ADDR			    10 + LOCAL_VAR_ADDR //2short
#define LOCAL_iBi_ADDR                     12 + LOCAL_VAR_ADDR //2short

//reconstruct,save history mono
#define LOCAL_ChannelInfo_ADDR			0 + LOCAL_VAR_ADDR //1short
#define LOCAL_pcSampleGet_ADDR			1 + LOCAL_VAR_ADDR //1short
#define LOCAL_iEnd_ADDR				2 + LOCAL_VAR_ADDR //1short
#define LOCAL_iSegEnd_ADDR			3 + LOCAL_VAR_ADDR //1short
#define LOCAL_i_ADDR				4 + LOCAL_VAR_ADDR //1short
#define LOCAL_RecTblBuff_ADDR			6 + LOCAL_VAR_ADDR //32short
#define LOCAL_RecTblBuffEnd_ADDR		36 + LOCAL_VAR_ADDR
//#define LOCAL_OutputPCMBuff_ADDR		38 + LOCAL_VAR_ADDR //16short
#define LOCAL_OutputPCMBuff_ADDR		TABLE_g_rgiHuffDecTbl44smOb_ADDR //1short ,DMA buffer 4096short
#define LOCAL_WORD_0XFFFF8000_ADDR		54 + LOCAL_VAR_ADDR//2short
#define LOCAL_RecTblDMAExAddr_ADDR		56 + LOCAL_VAR_ADDR//2short
#define LOCAL_PrevOutputBuff_ADDR          	98+LOCAL_VAR_ADDR//16short
#define LOCAL_PrevOutputDMAExAddr_ADDR 	114+LOCAL_VAR_ADDR//2short
#define LOCAL_PrevOutputCurrDMAExAddr_ADDR 	116+LOCAL_VAR_ADDR//2short

//GetPCM
#define LOCAL_cSamplesLeft_ADDR             	58+LOCAL_VAR_ADDR//1short
//#define LOCAL_LM_OutPcmDMAExAddr_ADDR       	60+LOCAL_VAR_ADDR//2short
//#define LOCAL_RM_OutPcmDMAExAddr_ADDR       	62+LOCAL_VAR_ADDR//2short
//#define LOCAL_DS_OutPcmDMAExAddr_ADDR       	64+LOCAL_VAR_ADDR//2short
//#define LOCAL_OutPcmDMAExAddr_ADDR          	66+LOCAL_VAR_ADDR//32short ?????????

//added by liyongjian for CII_prvDecodeFrameHeader_VOC
#define LOCAL_cBitsNeed_ADDR					0 + LOCAL_VAR_ADDR// 1short
#define LOCAL_iSizePrev_ADDR					1 + LOCAL_VAR_ADDR// 1short
#define LOCAL_iSizeCurr_ADDR					2 + LOCAL_VAR_ADDR// 1short
#define LOCAL_iSizeNext_ADDR					3 + LOCAL_VAR_ADDR// 1short
#define LOCAL_fSkipAll_ADDR						4+ LOCAL_VAR_ADDR// 1short
#define LOCAL_fUpdateMask_ADDR					0 + LOCAL_VAR_ADDR// 1short
#define LOCAL_rgiMaskQ_ADDR					1 + LOCAL_VAR_ADDR// 1short
#define LOCAL_m_iMaxMaskQ_ADDR				2 + LOCAL_VAR_ADDR// 1short


//add by mohongfee for CII_prvDecodeFrameHeaderLpc_VOC
#define GLOBAL_Temp16_ADDR			0 + LOCAL_VAR_ADDR// 1short
#define GLOBAL_Temp16_2_ADDR			1 + LOCAL_VAR_ADDR// 1short
#define GLOBAL_Temp32_ADDR			2 + LOCAL_VAR_ADDR// 2short
#define GLOBAL_Temp16_3_ADDR			4 + LOCAL_VAR_ADDR// 1short

//add by zhangl--------------------------------------------------------
//used in LPC
#define LOCAL_LEVEL1_Y_BEGIN_ADDR		6 + LOCAL_VAR_ADDR
#define LOCAL_LEVEL2_Y_BEGIN_ADDR		288 + LOCAL_VAR_ADDR
#define LOCAL_LEVEL3_Y_BEGIN_ADDR		6 + LOCAL_VAR_ADDR

#define LOCAL_shortSeq_ADDR                    	0 + LOCAL_LEVEL1_Y_BEGIN_ADDR//6short
#define LOCAL_longSeq_ADDR                     	6 + LOCAL_LEVEL1_Y_BEGIN_ADDR//200short
#define LOCAL_shortlength_ADD                  	206+LOCAL_LEVEL1_Y_BEGIN_ADDR//1short
#define LOCAL_longlength_ADD                   	207+LOCAL_LEVEL1_Y_BEGIN_ADDR//1short
#define LOCAL_order_ADD                        	208+LOCAL_LEVEL1_Y_BEGIN_ADDR//1short
#define LOCAL_lsfQ_ADD                         	210+LOCAL_LEVEL1_Y_BEGIN_ADDR//10short
#define LOCAL_lpc_ADD                          	220+LOCAL_LEVEL1_Y_BEGIN_ADDR//20short
#define LOCAL_p_ADD                          	240+LOCAL_LEVEL1_Y_BEGIN_ADDR//20short
#define LOCAL_q_ADD                          	260+LOCAL_LEVEL1_Y_BEGIN_ADDR//20short
#define LOCAL_ChannelCh_ADDR                 	280+LOCAL_LEVEL1_Y_BEGIN_ADDR//1short  used to judege which channel to be choosed
//#define LOCAL_ChannelInfo_ADDR                281+LOCAL_LEVEL1_Y_BEGIN_ADDR//1short  used to record address of the selected channel 

/* Function CII_prvConvolve */
#define LOCAL_ret_ADDR             		0 + LOCAL_LEVEL2_Y_BEGIN_ADDR//[16]  shorts
#define LOCAL_uiFrac1_ADDR         		16 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_iMSF8_ADDR           		18 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
//used in prvDoLpc4 and prvLpcToSpectrum
#define LOCAL_CmS_ADDR           		20 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts  (1 int)
#define LOCAL_CpS_ADDR           		22 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts  (1 int)
#define LOCAL_T2_ADDR           		24 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_T4_ADDR           		26 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_T6_ADDR           		28 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_T7_ADDR           		30 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_T8_ADDR           		32 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_T9_ADDR           		34 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_TA_ADDR           		36 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_TB_ADDR           		38 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C2_ADDR           		40 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_S2_ADDR           		42 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C3_ADDR           		44 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_S3_ADDR           		46 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C1_ADDR           		48 + LOCAL_LEVEL2_Y_BEGIN_ADDR//2shorts
#define LOCAL_S1_ADDR           		50 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_k_lpc_ADDR            		52 + LOCAL_LEVEL2_Y_BEGIN_ADDR //1shorts
#define LOCAL_pTmp_ADDR         		54 + LOCAL_LEVEL2_Y_BEGIN_ADDR //64shorts
#define LOCAL_SL16_ADDR           		118 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts	
#define LOCAL_CL16_ADDR           		120 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_SL8_ADDR            		122 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CL8_ADDR            		124 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_SL3by16_ADDR        		126 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CL3by16_ADDR        		128 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_SL4_ADDR            			130 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CL4_ADDR            			132 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_t1pO7_ADDR          			134 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tO1pO9_ADDR          		136 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tO0pO8_ADDR          		138 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tO4pO6_ADDR          		140 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tO4mO6_ADDR          		142 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_t1pO7pO3_ADDR        		144 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_t1pO7mO3_ADDR        		146 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tO1pO5pO9_ADDR       		148 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tO0pO2pO4pO6pO8_ADDR	150 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_D_ADDR               			152 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_E_ADDR               			154 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_F_ADDR               			156 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_G_ADDR               			158 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_H_ADDR               			160 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_I_ADDR               			162 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_J_ADDR               			164 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_iSizeBy2_ADDR        		166+ LOCAL_LEVEL2_Y_BEGIN_ADDR //1shorts
#define LOCAL_interval_ADDR        			167+ LOCAL_LEVEL2_Y_BEGIN_ADDR //1shorts
#define LOCAL_S4_ADDR              			168 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C4_ADDR              			170 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_STEP1_ADDR           			172 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_STEP4_ADDR           			174 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_S4p_ADDR             			176 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C4p_ADDR             			178 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_S1p_ADDR             			180 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C1p_ADDR             			182 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CpS1_ADDR            			184 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CmS1_ADDR            			186 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_S8_ADDR              			188 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C8_ADDR              			190 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CmS2_ADDR            			192 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CpS2_ADDR            			194 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tCpS1x_ADDR          		196 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tCpS2x_ADDR          		198 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tCmS2x_ADDR          		200 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_tCmS1x_ADDR          		202 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_S_ADDR               			204 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_C_ADDR               			206 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CLS1_ADDR            			208 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_SLS1_ADDR            			210 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_SLC1_ADDR            			212 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts
#define LOCAL_CLC1_ADDR            			214 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts 
#define LOCAL_wtLpcSpecMax_ADDR    		216 + LOCAL_LEVEL2_Y_BEGIN_ADDR //2shorts 
#define LOCAL_ChannelInfo_Select_ADDR  	218+LOCAL_LEVEL2_Y_BEGIN_ADDR//1short added for zhangl byliyj

//used in prvInverseQuantizeLowRate  and prvInverseQuantizeMidRate
#define LOCAL_iBark_ADDR           				218 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts 
#define LOCAL_iCoefQ_ADDR          				219 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts 
#define LOCAL_iRecon_ADDR          				220 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts 
#define LOCAL_iUBLimitOniRecon_ADDR    				221+ LOCAL_LEVEL3_Y_BEGIN_ADDR //1short
#define LOCAL_uiInvMaxWeight_ADDR  				222 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_uiQuantStepXInvMaxWeight_ADDR   			224 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define	LOCAL_cQSIMWFracBits_ADDR  				226 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_cMaxWeightFracBits_ADDR  				228 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts 
#define LOCAL_uiMaxWeight_ADDR     				230 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_qrand_ADDR           				232+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_uiWeightFactor_ADDR  				234+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_cWFFracBits_ADDR     				236+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts 
#define LOCAL_QuantStepXMaxWeightXWeightFactor_ADDR  		238+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_cFracBits_ADDR       				240+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_iCoef_ADDR           				242+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_iCoefScaled_ADDR     				244+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_iNoiseQuant_ADDR     				246+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_QuantStepXMaxWeightXWeightFactorXDither_ADDR    	248+ LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_rgfMaskNeededForBark_BUFF  			250+ LOCAL_LEVEL3_Y_BEGIN_ADDR //25shorts
#define LOCAL_MaskResampleInfo_ADDR      			275+ LOCAL_LEVEL3_Y_BEGIN_ADDR //3shorts
#define LOCAL_rgiBarkIndexResampled_ADDR   			278 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts
#define LOCAL_rgiBarkIndex_ADDR            			279 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts
#define LOCAL_iBarkResampled_ADDR          			280 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts
#define LOCAL_iReconTarget_ADDR            			281 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts
#define LOCAL_qfltQuantizer_ADDR           			282 + LOCAL_LEVEL3_Y_BEGIN_ADDR //4shorts
#define LOCAL_iShift_ADDR                  			286 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_uiNoisePowerXinvMaxWeight_ADDR  			288 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_iNoiseRand_ADDR               			290 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shorts
#define LOCAL_iNoiseBand_ADDR              			292 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1short
#define LOCAL_iCurrBand_ADDR               			293 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1short
#define LOCAL_iMaskBand_ADDR               			294 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1short
#define LOCAL_iRsmpBand_ADDR               			295 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1short
#define LOCAL_cMaskHighCutOff_ADDR         			296 + LOCAL_LEVEL3_Y_BEGIN_ADDR //2shortS
#define LOCAL_rgiMaskQ4BandNotCoded_BUFF   			298 + LOCAL_LEVEL3_Y_BEGIN_ADDR //10 shorts
#define LOCAL_cNoiseBand_ADDR              			308 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts
//added by liyongjian for zhangl project inverselowrate & midrate
#define LOCAL_iLoopMax_ADDR                			309 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts
#define LOCAL_fflt_temp_ADDR               			310 + LOCAL_LEVEL3_Y_BEGIN_ADDR //4shorts
#define LOCAL_ffltNoisePower_ADDR				314 + LOCAL_LEVEL3_Y_BEGIN_ADDR //4shorts
#define LOCAL_fAllBandsSynced_ADDR				318 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts//mid
#define LOCAL_iCurrStart_ADDR					319 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts//mid
#define LOCAL_iCurrEnd_ADDR						320 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts//mid
#define LOCAL_iMaskEnd_ADDR					321 + LOCAL_LEVEL3_Y_BEGIN_ADDR //1shorts//mid
#define LOCAL_ffltBandWeight_BUFF   				322 + LOCAL_LEVEL3_Y_BEGIN_ADDR //40 shorts

//added by liyongjian for CII_prvGetBandWeightLowRate_VOC
#define LOCAL_m_rgfltBandWeight_ADDR				0 + LOCAL_VAR_ADDR//100 short
#define LOCAL_temp_ffltiFracBits_ADDR				100 + LOCAL_VAR_ADDR//2short
#define LOCAL_temp_ffltiFraction_ADDR				102 + LOCAL_VAR_ADDR//2short
#define LOCAL_m_fflt_iFracBits_ADDR         		104 + LOCAL_VAR_ADDR//2short
#define LOCAL_m_fflt_iFraction_ADDR         		106 + LOCAL_VAR_ADDR//2short
#define LOCAL_m_cNoiseBand_ADDR						108 + LOCAL_VAR_ADDR//1short redefined
#define LOCAL_m_rgbBandNotCoded_ADDR				109 + LOCAL_VAR_ADDR//1short
#define LOCAL_m_rgxWeightFactor_ADDR				110 + LOCAL_VAR_ADDR//1short
#define LOCAL_m_iBand_ADDR							111 + LOCAL_VAR_ADDR//1short
#define LOCAL_iEnd2Start_ADDR						112 + LOCAL_VAR_ADDR//1short
#define LOCAL_cNoiseBandsub1_ADDR						112 + LOCAL_VAR_ADDR//1short
#define LOCAL_m_rgffltSqrtBWRatio_ADDR				113 + LOCAL_VAR_ADDR//1short

//added by liyongjian for CII_auReconCoefficentsHighRate_VOC
#define LOCAL_m_ctCoefRecon_ADDR				0 + LOCAL_VAR_ADDR//2short
#define LOCAL_temp_qfltQuantizertiFracBits_ADDR			2 + LOCAL_VAR_ADDR//2short
#define LOCAL_temp_qfltQuantizeriFraction_ADDR			4 + LOCAL_VAR_ADDR//2short


//added by liyongjian for function return value WMARESULT
// SUCCESS codes
#define WMA_VOC_OK                0x0000
#define WMA_VOC_S_FALSE           0x0001
#define WMA_VOC_S_BUFUNDERFLOW    0x0002
#define WMA_VOC_S_NEWPACKET       0x0003
#define WMA_VOC_S_NO_MORE_FRAME   0x0004
#define WMA_VOC_S_DEFAULT         0x0005
#define WMA_VOC_S_SWITCHCHMODE    0x0006 
// ERROR codes
#define WMA_VOC_E_FAIL            0x8045
#define WMA_VOC_E_OUTOFMEMORY     0x870E
#define WMA_VOC_E_INVALIDARG      0x8757
#define WMA_VOC_E_NOTSUPPORTED    0x8400
#define WMA_VOC_E_LOSTPACKET      0x8401
#define WMA_VOC_E_BROKEN_FRAME    0x8402
#define WMA_VOC_E_BUFFEROVERFLOW  0x8403 
#define WMA_VOC_E_ONHOLD          0x8404 

