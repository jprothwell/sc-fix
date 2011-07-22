#ifndef __ERR_CONC
#define __ERR_CONC

#include "vpp_hr_typedefs.h"

/*_________________________________________________________________________
 |                                                                         |
 |                            Function Prototypes                          |
 |_________________________________________________________________________|
*/

void   para_conceal_speech_decoder(INT16 pswErrorFlag[],
                       INT16 pswSpeechPara[], INT16 *pswMutePermit);

  INT16 level_calc(INT16 swInd, INT32 *pl_en);

  void   level_estimator(INT16 update, INT16 *pswLevelMean,
                                INT16 *pswLevelMax,
                                INT16 pswDecodedSpeechFrame[]);

  void   signal_conceal_sub(INT16 pswPPFExcit[],
                     INT16 ppswSynthAs[], INT16 pswSynthFiltState[],
                       INT16 pswLtpStateOut[], INT16 pswPPreState[],
                                INT16 swLevelMean, INT16 swLevelMax,
                            INT16 swErrorFlag1, INT16 swMuteFlagOld,
                            INT16 *pswMuteFlag, INT16 swMutePermit);

#endif
