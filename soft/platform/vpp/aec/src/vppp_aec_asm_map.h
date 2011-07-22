#ifndef MAP_ADDR_H
#define MAP_ADDR_H

/* This file defines all the variables as memory addresses.
   It's created by voc_map automatically.*/

/********************
 **  SECTION_SIZE  **
 ********************/

#define COMMON_GLOBAL_X_SIZE                              		2484
#define COMMON_GLOBAL_Y_SIZE                              		42
#define HI_GLOBAL_X_SIZE                                  		0
#define HI_GLOBAL_Y_SIZE                                  		0
#define BYE_GLOBAL_X_SIZE                                 		2
#define BYE_GLOBAL_Y_SIZE                                 		2
#define HI_LOCAL_X_SIZE                                   		0
#define HI_LOCAL_Y_SIZE                                   		0
#define BYE_LOCAL_X_SIZE                                  		4
#define BYE_LOCAL_Y_SIZE                                  		2
#define COMMON_LOCAL_X_SIZE                               		0
#define COMMON_LOCAL_Y_SIZE                               		0

/**************************
 **  SECTION_BEGIN_ADDR  **
 **************************/

#define COMMON_GLOBAL_X_BEGIN_ADDR                        		(0 + RAM_X_BEGIN_ADDR)
#define COMMON_GLOBAL_Y_BEGIN_ADDR                        		(0 + RAM_Y_BEGIN_ADDR)
#define HI_GLOBAL_X_BEGIN_ADDR                            		(2484 + RAM_X_BEGIN_ADDR)
#define HI_GLOBAL_Y_BEGIN_ADDR                            		(42 + RAM_Y_BEGIN_ADDR)
#define BYE_GLOBAL_X_BEGIN_ADDR                           		(2484 + RAM_X_BEGIN_ADDR)
#define BYE_GLOBAL_Y_BEGIN_ADDR                           		(42 + RAM_Y_BEGIN_ADDR)
#define HI_LOCAL_X_BEGIN_ADDR                             		(2486 + RAM_X_BEGIN_ADDR)
#define HI_LOCAL_Y_BEGIN_ADDR                             		(44 + RAM_Y_BEGIN_ADDR)
#define BYE_LOCAL_X_BEGIN_ADDR                            		(2486 + RAM_X_BEGIN_ADDR)
#define BYE_LOCAL_Y_BEGIN_ADDR                            		(44 + RAM_Y_BEGIN_ADDR)
#define COMMON_LOCAL_X_BEGIN_ADDR                         		(2490 + RAM_X_BEGIN_ADDR)
#define COMMON_LOCAL_Y_BEGIN_ADDR                         		(46 + RAM_Y_BEGIN_ADDR)

/*********************
 **  COMMON_GLOBAL  **
 *********************/

//GLOBAL_VARS
#define VPP_HELLO_MODE_NO                       			(-1) //alias
#define VPP_HELLO_MODE_HI                       			(0) //alias
#define VPP_HELLO_MODE_BYE                      			(1) //alias
#define VPP_HELLO_ERROR_NO                      			(0) //alias
#define VPP_HELLO_ERROR_YES                     			(-1) //alias
#define VPP_HELLO_SAYS_HI                       			(0x0111) //alias
#define VPP_HELLO_SAYS_BYE                      			(0x0B1E) //alias
#define PassThrough                             			(0) //alias
#define AdaptiveFilter                          			(1) //alias
#define AdaptiveFilterPostFilter                			(2) //alias
#define NoiseSuppression                        			(3) //alias
#define LowDelayOpenLoopAF                      			(4) //alias
#define TwoStepNoiseSuppression                 			(5) //alias
#define AdaptiveFilterPostFilterEnhanced        			(6) //alias
#define ECHO_COHF                               			(0) //alias
#define ECHO_TF                                 			(1) //alias
//VPP_HELLO_CONST_STRUCT
#define VPP_HELLO_CONST_STRUCT                  			(0 + COMMON_GLOBAL_X_BEGIN_ADDR) //struct
#define VPP_HELLO_CONST_SAYS_HI                 			(0 + VPP_HELLO_CONST_STRUCT) //1 word
#define VPP_HELLO_CONST_SAYS_BYE                			(2 + VPP_HELLO_CONST_STRUCT) //1 word

#define GLOBAL_INPUT_MIC_PCM_ADDR               			(4 + COMMON_GLOBAL_X_BEGIN_ADDR) //160 short
#define GLOBAL_INPUT_SPK_PCM_ADDR               			(164 + COMMON_GLOBAL_X_BEGIN_ADDR) //160 short
#define GLOBAL_OUTPUT_MIC_PCM_ADDR              			(324 + COMMON_GLOBAL_X_BEGIN_ADDR) //160 short
#define r_SpkAnaFilterDelayLine                 			(484 + COMMON_GLOBAL_X_BEGIN_ADDR) //128 short
#define r_MicAnaFilterDelayLine                 			(612 + COMMON_GLOBAL_X_BEGIN_ADDR) //128 short
#define r_OutSynFilterDelayLine                 			(740 + COMMON_GLOBAL_X_BEGIN_ADDR) //128 short
#define g_FilterBank_MulBuf                     			(868 + COMMON_GLOBAL_X_BEGIN_ADDR) //128 short
#define g_SpkAnaFilterOutBufI                   			(996 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define g_SpkAnaFilterOutBufQ                   			(1012 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define g_MicAnaFilterOutBufI                   			(1028 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define g_MicAnaFilterOutBufQ                   			(1044 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define g_OutSynFilterInBufI                    			(1060 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define g_OutSynFilterInBufQ                    			(1076 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define r_AF_SpkSigBufI                         			(1092 + COMMON_GLOBAL_X_BEGIN_ADDR) //72 short
#define r_AF_SpkSigBufQ                         			(1164 + COMMON_GLOBAL_X_BEGIN_ADDR) //72 short
#define r_AF_FilterBufI                         			(1236 + COMMON_GLOBAL_X_BEGIN_ADDR) //72 word
#define r_AF_FilterBufQ                         			(1380 + COMMON_GLOBAL_X_BEGIN_ADDR) //72 word
#define g_AF_FilterOutBufI                      			(1524 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve00_addr                          			(1533 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_AF_FilterOutBufQ                      			(1534 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve01_addr                          			(1543 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_AF_OldSpkDataBufI                     			(1544 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve02_addr                          			(1553 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_AF_OldSpkDataBufQ                     			(1554 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve03_addr                          			(1563 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_AF_ErrDataBufI                        			(1564 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve04_addr                          			(1573 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_AF_ErrDataBufQ                        			(1574 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve05_addr                          			(1583 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_AF_ShortFilterBufI                    			(1584 + COMMON_GLOBAL_X_BEGIN_ADDR) //8 short
#define g_AF_ShortFilterBufQ                    			(1592 + COMMON_GLOBAL_X_BEGIN_ADDR) //8 short
#define g_AF_tempCmplxBufI                      			(1600 + COMMON_GLOBAL_X_BEGIN_ADDR) //8 word
#define g_AF_tempCmplxBufQ                      			(1616 + COMMON_GLOBAL_X_BEGIN_ADDR) //8 word
#define r_AF_SpkSigPower                        			(1632 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define r_AF_SpkSigSmoothPower                  			(1650 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define r_AF_RErrSpkI                           			(1668 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define r_AF_RErrSpkQ                           			(1686 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define g_AF_StepSize                           			(1704 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve06_addr                          			(1713 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_SmoothErrPower                     			(1714 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define r_PF_NoisePowerEstBuf                   			(1732 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define r_PF_NoiseFloorDevCnt                   			(1750 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve07_addr                          			(1759 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_PF_PostSToNRatio                      			(1760 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve08_addr                          			(1769 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_PrevSigPowerNBuf                   			(1770 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define r_PF_PrevSigPowerEBuf                   			(1788 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define g_PF_NoiseWeight                        			(1806 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve09_addr                          			(1815 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_PF_NoiseAmpWeight                     			(1816 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve10_addr                          			(1825 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define m_Taps                                  			(1826 + COMMON_GLOBAL_X_BEGIN_ADDR) //4 short
#define m_Registers                             			(1830 + COMMON_GLOBAL_X_BEGIN_ADDR) //32 short
#define TEMP_r_AF_SpkSigBufIQ_addr_addr         			(1862 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define TEMP_r_AF_FilterBufIQ_addr_addr         			(1864 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define TEMP_g_AF_FilterOutBufIQ_addr_addr      			(1866 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define TEMP_r_AF_OldSpkDataBufIQ_addr_addr     			(1868 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define TEMP_g_AF_ErrDataBufIQ_addr_addr        			(1870 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define TEMP_g_MicAnaFilterOutBufIQ_addr_addr   			(1872 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define TEMP_g_SpkAnaFilterOutBufIQ_addr_addr   			(1874 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define TEMP_r_AF_SpkSigPower_addr_addr         			(1876 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define AnaFilterCoef                           			(1878 + COMMON_GLOBAL_X_BEGIN_ADDR) //128 short
#define SynFilterCoef                           			(2006 + COMMON_GLOBAL_X_BEGIN_ADDR) //128 short
#define wnreal                                  			(2134 + COMMON_GLOBAL_X_BEGIN_ADDR) //8 short
#define wnimag                                  			(2142 + COMMON_GLOBAL_X_BEGIN_ADDR) //8 short
#define AdaptTable                              			(2150 + COMMON_GLOBAL_X_BEGIN_ADDR) //32 short
#define g_counter_subframe                      			(2182 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_ch                                    			(2183 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define real_int                                			(2184 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 word
#define imag_int                                			(2216 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 word
#define r_PF_CL_MicSmoothPower                  			(2248 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 word
#define r_PF_CL_MicNoise                        			(2250 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 word
#define r_PF_CL_SpkSmoothPower                  			(2252 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 word
#define r_PF_CL_SpkNoise                        			(2254 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 word
#define r_PF_CL_MicNoiseFloorDevCnt             			(2256 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_CL_SpkNoiseFloorDevCnt             			(2257 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_CL_ifMicSigOn                      			(2258 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_CL_ifSpkSigOn                      			(2259 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_CL_ChnlState                       			(2260 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_CL_State                           			(2261 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_CL_DTCountDown                     			(2262 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define m_AF_FrameCnt                           			(2263 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_NoisePowerSmoothEstBuf             			(2264 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define g_PF_PriorSToNRatio                     			(2282 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define r_PF_SpkPowerBuf                        			(2291 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define r_PF_CCErrSpkVecBufI                    			(2300 + COMMON_GLOBAL_X_BEGIN_ADDR) //72 short
#define r_PF_CCErrSpkVecBufQ                    			(2372 + COMMON_GLOBAL_X_BEGIN_ADDR) //72 short
#define r_PF_EchoPowerEstBuf                    			(2444 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 word
#define g_PF_EchoWeight                         			(2462 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define r_PF_CL_DTCountUp                       			(2471 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define g_PF_PriorSToERatio                     			(2472 + COMMON_GLOBAL_X_BEGIN_ADDR) //9 short
#define reserve12_addr                          			(2481 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define r_PF_CL_ErrSmoothPower                  			(2482 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 word

//GLOBAL_VARS
//VPP_HELLO_CFG_STRUCT
#define VPP_HELLO_CFG_STRUCT                    			(0 + COMMON_GLOBAL_Y_BEGIN_ADDR) //struct
#define GLOBAL_MODE                             			(0 + VPP_HELLO_CFG_STRUCT) //1 short
#define GLOBAL_RESET_ADDR                       			(1 + VPP_HELLO_CFG_STRUCT) //1 short

//VPP_HELLO_STATUS_STRUCT
#define VPP_HELLO_STATUS_STRUCT                 			(2 + COMMON_GLOBAL_Y_BEGIN_ADDR) //struct
#define GLOBAL_USED_MODE                        			(0 + VPP_HELLO_STATUS_STRUCT) //1 short
#define GLOBAL_ERR_STATUS                       			(1 + VPP_HELLO_STATUS_STRUCT) //1 short
#define GLOBAL_HELLO_STATUS                     			(2 + VPP_HELLO_STATUS_STRUCT) //1 word

//VPP_HELLO_CODE_CFG_STRUCT
#define VPP_HELLO_CODE_CFG_STRUCT               			(6 + COMMON_GLOBAL_Y_BEGIN_ADDR) //struct
#define GLOBAL_HI_CODE_PTR                      			(0 + VPP_HELLO_CODE_CFG_STRUCT) //1 word
#define GLOBAL_BYE_CODE_PTR                     			(2 + VPP_HELLO_CODE_CFG_STRUCT) //1 word
#define GLOBAL_CONST_PTR                        			(4 + VPP_HELLO_CODE_CFG_STRUCT) //1 word

//m_AECProcPara_ADDR
#define m_AECProcPara_ADDR                      			(12 + COMMON_GLOBAL_Y_BEGIN_ADDR) //struct
#define m_AECProcPara_ProcessMode               			(0 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_InitRegularFactor         			(1 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_AFUpdateFactor            			(2 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_EchoEstMethod             			(3 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_NoiseUpdateFactor         			(4 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_SNRUpdateFactor           			(5 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_SNRLimit                  			(6 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_NoiseOverEstFactor        			(7 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_NoiseGainLimit            			(8 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_EchoUpdateFactor          			(9 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_SERUpdateFactor           			(10 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_SERLimit                  			(11 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_EchoOverEstFactor         			(12 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_EchoGainLimit             			(13 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_CNGFactor                 			(14 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_AmpGain                   			(15 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_NoiseMin                  			(16 + m_AECProcPara_ADDR) //1 word
#define m_AECProcPara_NoiseGainLimitStep        			(18 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_AmpThr_ADDR               			(19 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLDataSmoothFactor      			(20 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLNoiseUpdateFactor     			(21 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLThrSigDet             			(22 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLThrDT                 			(23 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_DataSmoothFactor          			(24 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLThrNT                 			(25 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLThrDTDiff             			(26 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLDTHoldTime            			(27 + m_AECProcPara_ADDR) //1 short
#define m_AECProcPara_PFCLDTStartTime           			(28 + m_AECProcPara_ADDR) //1 short
#define reserve13_addr                          			(29 + m_AECProcPara_ADDR) //1 short


/*****************
 **  HI_GLOBAL  **
 *****************/

/******************
 **  BYE_GLOBAL  **
 ******************/

//GLOBAL_VARS
#define GLOBAL_BYE                              			(0 + BYE_GLOBAL_X_BEGIN_ADDR) //1 word

//GLOBAL_VARS
#define GLOBAL_BYE2                             			(0 + BYE_GLOBAL_Y_BEGIN_ADDR) //1 short

/****************
 **  HI_LOCAL  **
 ****************/

/*****************
 **  BYE_LOCAL  **
 *****************/

//vpp_HelloBye
#define vpp_HelloBye_X_BEGIN_ADDR               			(0 + BYE_LOCAL_X_BEGIN_ADDR)
#define LOCAL_BYE                               			(0 + vpp_HelloBye_X_BEGIN_ADDR) //4 short

//vpp_HelloBye
#define vpp_HelloBye_Y_BEGIN_ADDR               			(0 + BYE_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_BYE2                              			(0 + vpp_HelloBye_Y_BEGIN_ADDR) //1 word

/********************
 **  COMMON_LOCAL  **
 ********************/

//RAM_X: size 0x2800, used 0x09ba	RAM_Y: size 0x2800, used 0x002e

#endif
