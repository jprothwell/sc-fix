#ifndef __HOMING
#define __HOMING

#include "vpp_hr_typedefs.h"

#define EHF_MASK 0x0008                /* Encoder Homing Frame pattern */

/*_________________________________________________________________________
 |                                                                         |
 |                           Function Prototypes                           |
 |_________________________________________________________________________|
*/

int    decoderHomingFrameTest(INT16 pswSpeechPara[], int iLastPara);

void   decoderReset(void);

int    encoderHomingFrameTest(INT16 pswSpeech[]);

void   encoderReset(void);

void   resetDec(void);

void   resetEnc(void);

void   dtxResetTx(void);

void   dtxResetRx(void);

#endif
