#ifndef __SP_SFRM
#define __SP_SFRM

#include "vpp_hr_typedefs.h"

/*_________________________________________________________________________
 |                                                                         |
 |                           Function Prototypes                           |
 |_________________________________________________________________________|
*/

INT16 g_corr2(INT16 *pswIn, INT16 *pswIn2,
                         INT32 *pL_out);

int    closedLoopLagSearch(INT16 pswLagList[],
                                  int iNumLags,
                                  INT16 pswLtpState[],
                                  INT16 pswHCoefs[],
                                  INT16 pswPVect[],
                                  INT16 *pswLag,
                                  INT16 *pswLtpShift);

  void   decorr(int iNumVects,
                       INT16 pswGivenVect[],
                       INT16 pswVects[]);

  INT16 g_quant_vl(INT16 swUVCode,
                              INT16 pswWInput[],
                              INT16 swWIShift,
                              INT16 pswWLTPVec[],
                              INT16 pswWVSVec1[],
                              INT16 pswWVSVec2[],
                              struct NormSw snsRs00,
                              struct NormSw snsRs11,
                              struct NormSw snsRs22);

  void   gainTweak(struct NormSw *psErrorTerm);

  void   hnwFilt(INT16 pswInSample[],
                        INT16 pswOutSample[],
                        INT16 pswState[],
                        INT16 pswInCoef[],
                        int iStateOffset,
                        INT16 swZeroState,
                        int iNumSamples);

  void   sfrmAnalysis(INT16 *pswWSpeech,
                             INT16 swVoicingMode,
                             struct NormSw snsSqrtRs,
                             INT16 *pswHCoefs,
                             INT16 *pswLagList,
                             short siNumLags,
                             INT16 swPitch,
                             INT16 swHNWCoef,
                             short *psiLagCode,
                             short *psiVSCode1,
                             short *psiVSCode2,
                             short *psiGsp0Code,
                             INT16 swSP);

  INT16 v_srch(INT16 pswWInput[],
                          INT16 pswWBasisVecs[],
                          short int siNumBasis);

#endif
