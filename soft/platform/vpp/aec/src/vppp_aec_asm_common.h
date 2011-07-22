
void vpp_HelloMain(void);
void vpp_HelloRunDma(void);
void vpp_HelloCodeConstReload(void);

void vpp_HelloHiTest(void);
void vpp_HelloBye(void);
void vpp_HelloByeTest(void);

void Coolsand_AnalysisProcessing(void);
void Coolsand_AECReset(void);
void Coolsand_BackCopyVec(void);
void Coolsand_FlipVec(void);
void Coolsand_MulVec(void);
void Coolsand_ClearVec(void);
void Coolsand_AddVec(void);


void Coolsand_IFFT16(void);
void Coolsand_ReverseIndex(void);


void Coolsand_SynthesisProcessing(void);
void Coolsand_CopyVec(void);

void Coolsand_EchoCancelProcessing(void);


void Coolsand_AdaptiveFiltering(void);
void Coolsand_UpdateSpkPower(void);

void Coolsand_MSBPos(void);
void Coolsand_CmplxAppxNorm_int(void);

void Coolsand_FilterUpdateCotrol_Int(void);
void Coolsand_UpdateFilter(void);

void Coolsand_AEC(void);
void Coolsand_NonLinearTransform(void);


void Coolsand_CmplxAppxNorm(void);
void Coolsand_FirstOrderIIRSmooth2(void);

void Coolsand_NoisePSDEst(void);
void Coolsand_ShrinkBit(void);
void Coolsand_ComputeNoiseWeight_DeciDirectCTRL(void);
void Coolsand_Generate(void);
void Coolsand_PostFiltering(void);



#define GLOBAL_CONST_SIZE (64+64+4+4+16)

#define PROTOTYPE_FILTER_LEN 128
#define SUBBAND_CHANNEL_NUM  16
#define SUBSAMPLING_RATE  (SUBBAND_CHANNEL_NUM/2)

#define TOTAL_FILTER_LEN  128
#define FILTER_LEN  (TOTAL_FILTER_LEN/SUBBAND_CHANNEL_NUM)
#define SUBBANDFILTER_NUM  (SUBBAND_CHANNEL_NUM/2+1)


#define DEF_DATA_BLOCK_SIZE (SUBBAND_CHANNEL_NUM/2)

#define CNGTapLen 4
#define CNGRegLen 32


#define g_PE_PPFilterLen 5
void Coolsand_PreProcessFiltering(void);



#define PF_CL_CHNLNUM 1

void Coolsand_SumInstantPower(void);
void Coolsand_InstantPower(void);
void Coolsand_ControlLogicMergeChnlEnhanced(void);
void Coolsand_ControlLogicSingleChnlEnhanced(void);
void Coolsand_ControlLogicChnlDecisionEnhanced(void);
void Coolsand_ControlLogicCalVarSub(void);
void Coolsand_SmoothPeakPower(void);
void Coolsand_InstantSNR(void);
void Coolsand_OverallNoiseEst(void);

void Coolsand_OverallNoiseEstwState(void);
void Cooland_InstantPower(void);

void Coolsand_ComputeEchoWeight_DeciDirect_MFCOHFCTRL(void);

void Coolsand_EchoPSDEst_MFCOHFFix(void);

// VoC_directive: PARSER_ON

